//! @file ControlFunctions.cc
//! @brief 制御用周期実行関数群クラス
//! @date 2021/07/30
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2021 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the BSD License.
// For details, see the License.txt file.

// 基本のインクルードファイル
#include <cmath>
#include "ControlFunctions.hh"
#include "ARCSprint.hh"
#include "ARCSassert.hh"
#include "ScreenParams.hh"
#include "InterfaceFunctions.hh"
#include "GraphPlot.hh"
#include "DataMemory.hh"

// 追加のARCSライブラリをここに記述
#include "Matrix.hh"
#include "Limiter.hh"
#include "../robot/ZFA01/addon/ZFA01.hh"
#include "DisturbanceObsrv.hh"
#include "SquareWave.hh"

using namespace ARCS;

//! @brief スレッド間通信用グローバル変数の無名名前空間
namespace {
	// スレッド間で共有したい変数をここに記述
	std::array<double, ConstParams::ACTUATOR_NUM> PositionRes = {0};	//!< [rad] 位置応答
	std::array<double, ConstParams::ACTUATOR_NUM> VelocityRes = {0};	//!< [rad/s] 速度応答
	std::array<double, ConstParams::ACTUATOR_NUM> TorqueRes = {0};		//!< [Nm] トルク応答
	std::array<double, ConstParams::ACTUATOR_NUM> CurrentRef = {0};		//!< [Nm]  電流指令
}

//! @brief 制御用周期実行関数1
//! @param[in]	t		時刻 [s]
//! @param[in]	Tact	計測周期 [s]
//! @param[in]	Tcmp	消費時間 [s]
//! @return		クロックオーバーライドフラグ (true = リアルタイムループ, false = 非リアルタイムループ)
bool ControlFunctions::ControlFunction1(double t, double Tact, double Tcmp){
	// 制御用定数設定
	[[maybe_unused]] constexpr double Ts = ConstParams::SAMPLING_TIME[0]*1e-9;	// [s]	制御周期
	static constexpr Matrix<1,ZFA01::N_AX> Kt = ZFA01::Kt;	// [Nm/A] トルク定数
	static constexpr Matrix<1,ZFA01::N_AX> Ja = ZFA01::Ja;	// [kgm^2] 全慣性
	static constexpr Matrix<1,ZFA01::N_AX> Rg = ZFA01::Rg;	// [-] 減速比
	static constexpr Matrix<1,ZFA01::N_AX> gdis = {170, 170, 170};	// [rad/s] 外乱オブザーバの推定帯域
	static constexpr Matrix<1,ZFA01::N_AX> Kpv = {1, 1, 1};	// [-] 速度制御比例ゲイン
	
	// 制御用変数宣言
	static Matrix<1,ZFA01::N_AX> thetam, thetal;		// [rad] モータ側, 負荷側位置
	static Matrix<1,ZFA01::N_AX> omegam, omegal;		// [rad/s] モータ側, 負荷側速度
	static Matrix<1,ZFA01::N_AX> taus;					// [Nm] ねじれトルク
	static Matrix<1,ZFA01::N_AX> iq_ref;				// [A] q軸電流指令
	static Matrix<1,ZFA01::N_AX> tau_dis;				// [Nm] 推定外乱トルク
	static Matrix<1,ZFA01::N_AX> alpham_ref, alphal_ref;// [rad/s^2] モータ側, 負荷側加速度指令
	static Matrix<1,ZFA01::N_AX> omegal_ref;			// [rad/s] 負荷側速度指令
	
	// 制御器宣言
	static DisturbanceObsrv<DObType::FULL_1ST, ZFA01::N_AX> DObs(Kt, Ja, gdis, Ts);	// 外乱オブザーバ
	
	if(CmdFlag == CTRL_INIT){
		// 初期化モード (ここは制御開始時/再開時に1度だけ呼び出される(非リアルタイム空間なので重い処理もOK))
		Initializing = true;		// 初期化中ランプ点灯
		Screen.InitOnlineSetVar();	// オンライン設定変数の初期値の設定
		Interface.ServoON();		// サーボON指令の送出
		Initializing = false;		// 初期化中ランプ消灯
	}
	if(CmdFlag == CTRL_LOOP){
		// 周期モード (ここは制御周期 SAMPLING_TIME[0] 毎に呼び出される(リアルタイム空間なので処理は制御周期内に収めること))
		// リアルタイム制御ここから
		Interface.GetPositionAndVelocity(PositionRes, VelocityRes);	// [rad,rad/s] 位置と速度応答の取得
		Interface.GetTorque(TorqueRes);	// [Nm] ねじれトルク応答の取得
		Screen.GetOnlineSetVar();		// オンライン設定変数の読み込み
		
		// センサ系データ取得＆縦ベクトルとして読み込み
		thetam.LoadArray(PositionRes);	// [rad] モータ側位置
		omegam.LoadArray(VelocityRes);	// [rad/s] モータ側速度
		taus.LoadArray(TorqueRes);		// [Nm] ねじれトルク
		thetal = thetam % Rg;			// [rad] 負荷側位置(簡易的換算)
		omegal = omegam % Rg;			// [rad/s] 負荷側速度(簡易的換算)
		
		// 速度指令生成
		omegal_ref[1] = SquareWave(0.25, 0, t)*M_PI_4;	// [rad/s] 方形波速度指令
		
		// 3軸単慣性系の速度制御系
		alphal_ref = (omegal_ref - omegal) & Kpv;			// [rad/s^2] 速度制御器
		alpham_ref = alphal_ref & Rg;						// [rad/s^2] モータ側加速度指令に換算
		tau_dis = DObs.GetDistTorque(iq_ref, omegam);		// [Nm] 外乱オブザーバの推定演算
		iq_ref = (alpham_ref & (Ja % Kt)) + (tau_dis % Kt);	// [A] 加速度制御器
		
		// 安全系と電流指令設定
		Limiter<ZFA01::N_AX>(iq_ref, ZFA01::iq_max);// [A] q軸電流リミッタ
		iq_ref.StoreArray(CurrentRef);				// [A] 最終的な全軸の電流指令
		
		Interface.SetCurrent(CurrentRef);	// [A] 電流指令の出力
		Screen.SetVarIndicator(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// 任意変数インジケータ(変数0, ..., 変数9)
		Graph.SetTime(Tact, t);									// [s] グラフ描画用の周期と時刻のセット
		Graph.SetVars(0, iq_ref[1], iq_ref[2], iq_ref[3], 0, 0, 0, 0, 0);	// グラフプロット0 (グラフ番号, 変数0, ..., 変数7)
		Graph.SetVars(1, thetam[1], thetam[2], thetam[3], 0, 0, 0, 0, 0);	// グラフプロット1 (グラフ番号, 変数0, ..., 変数7)
		Graph.SetVars(2, omegam[1], omegam[2], omegam[3], 0, 0, 0, 0, 0);	// グラフプロット2 (グラフ番号, 変数0, ..., 変数7)
		Graph.SetVars(3, taus[1], taus[2], taus[3], 0, 0, 0, 0, 0);			// グラフプロット3 (グラフ番号, 変数0, ..., 変数7)
		Memory.SetData(Tact, t, 0, 0, 0, 0, 0, 0, 0, 0, 0);		// CSVデータ保存変数 (周期, A列, B列, ..., J列)
		// リアルタイム制御ここまで
	}
	if(CmdFlag == CTRL_EXIT){
		// 終了処理モード (ここは制御終了時に1度だけ呼び出される(非リアルタイム空間なので重い処理もOK))
		Interface.SetZeroCurrent();	// 電流指令を零に設定
		Interface.ServoOFF();		// サーボOFF信号の送出
	}
	return true;	// クロックオーバーライドフラグ(falseにすると次の周期時刻を待たずにスレッドが即刻動作する)
}

//! @brief 制御用周期実行関数2
//! @param[in]	t		時刻 [s]
//! @param[in]	Tact	計測周期 [s]
//! @param[in]	Tcmp	消費時間 [s]
//! @return		クロックオーバーライドフラグ (true = リアルタイムループ, false = 非リアルタイムループ)
bool ControlFunctions::ControlFunction2(double t, double Tact, double Tcmp){
	// 制御用定数宣言
	[[maybe_unused]] constexpr double Ts = ConstParams::SAMPLING_TIME[1]*1e-9;	// [s]	制御周期
	
	// 制御用変数宣言
	
	// 制御器等々の宣言
	
	if(CmdFlag == CTRL_INIT){
		// 初期化モード (ここは制御開始時/再開時に1度だけ呼び出される(非リアルタイム空間なので重い処理もOK))
	}
	if(CmdFlag == CTRL_LOOP){
		// 周期モード (ここは制御周期 SAMPLING_TIME[1] 毎に呼び出される(リアルタイム空間なので処理は制御周期内に収めること))
		// リアルタイム制御ここから
		
		// リアルタイム制御ここまで
	}
	if(CmdFlag == CTRL_EXIT){
		// 終了処理モード (ここは制御終了時に1度だけ呼び出される(非リアルタイム空間なので重い処理もOK))
	}
	return true;	// クロックオーバーライドフラグ(falseにすると次の周期時刻を待たずにスレッドが即刻動作する)
}

//! @brief 制御用周期実行関数3
//! @param[in]	t		時刻 [s]
//! @param[in]	Tact	計測周期 [s]
//! @param[in]	Tcmp	消費時間 [s]
//! @return		クロックオーバーライドフラグ (true = リアルタイムループ, false = 非リアルタイムループ)
bool ControlFunctions::ControlFunction3(double t, double Tact, double Tcmp){
	// 制御用定数宣言
	[[maybe_unused]] constexpr double Ts = ConstParams::SAMPLING_TIME[2]*1e-9;	// [s]	制御周期
	
	// 制御用変数宣言
	
	if(CmdFlag == CTRL_INIT){
		// 初期化モード (ここは制御開始時/再開時に1度だけ呼び出される(非リアルタイム空間なので重い処理もOK))
	}
	if(CmdFlag == CTRL_LOOP){
		// 周期モード (ここは制御周期 SAMPLING_TIME[2] 毎に呼び出される(リアルタイム空間なので処理は制御周期内に収めること))
		// リアルタイム制御ここから
		
		// リアルタイム制御ここまで
	}
	if(CmdFlag == CTRL_EXIT){
		// 終了処理モード (ここは制御終了時に1度だけ呼び出される(非リアルタイム空間なので重い処理もOK))
	}
	return true;	// クロックオーバーライドフラグ(falseにすると次の周期時刻を待たずにスレッドが即刻動作する)
}

//! @brief 制御用変数値を更新する関数
void ControlFunctions::UpdateControlValue(void){
	// ARCS画面パラメータに値を書き込む
	Screen.SetNetworkLink(NetworkLink);						// ネットワークリンクフラグを書き込む
	Screen.SetInitializing(Initializing);					// ロボット初期化フラグを書き込む
	Screen.SetCurrentAndPosition(CurrentRef, PositionRes);	// 電流指令と位置応答を書き込む
}

