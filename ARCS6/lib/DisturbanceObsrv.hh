//! @file DisturbanceObsrv.hh
//! @brief 外乱オブザーバクラス(ベクトル対応版)
//! 
//! q軸電流とモータ側速度/位置からモータ側外乱トルクを推定します。
//! 縦ベクトル変数の入出力も可能です。
//! 
//! @date 2020/04/02
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2020 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef DISTURBANCEOBSRV
#define DISTURBANCEOBSRV

#include <tuple>
#include "Matrix.hh"
#include "Discret.hh"
#include "ARCSeventlog.hh"

namespace ARCS {	// ARCS名前空間
	//! @brief 外乱オブザーバのタイプの定義
	enum class DObType {
		FULL_0TH,	//!< 同一次元0次外乱オブザーバ
		FULL_1ST	//!< 同一次元1次外乱オブザーバ
	};
	
	//! @brief 外乱オブザーバクラス
	//! @tparam	T	外乱オブザーバの形の設定
	//! @tparam N	入出力信号ベクトルの長さ
	template <DObType T, size_t N = 1>
	class DisturbanceObsrv {
		public:
			//! @brief コンストラクタ(スカラー版)
			//! @param [in] TrqConst トルク定数 [Nm/A]
			//! @param [in] Inertia 慣性 [kgm^2]
			//! @param [in] Bandwidth 推定帯域 [rad/s]
			//! @param [in] SmplTime 制御周期 [s]
			DisturbanceObsrv(double TrqConst, double Inertia, double Bandwidth, double SmplTime)
				: Ts(SmplTime),
				  Ktn(TrqConst), Jmn(Inertia), l1(-Bandwidth), l2(-Bandwidth), l3(-Bandwidth),
				  Ac(), Bc(), Ad(), Bd(), c(), u(), x(), x_next(), y(),
				  Ktnv(), Jmnv(), l1v(), l2v(), l3v(),
				  Acv(), Bcv(), Adv(), Bdv(), cv(), uv(), xv(), xv_next(), yv()
			{
				// オブザーバの構成によって状態方程式を変える
				switch(T){
					// 同一次元0次オブザーバの場合
					case DObType::FULL_0TH:
						// 連続系A行列の設定
						Ac.Set(
							l1 + l2,  -1.0/Jmn,
							Jmn*l1*l2,       0
						);
						// 連続系B行列の設定
						Bc.Set(
							Ktn/Jmn,  - l1 - l2,
								  0, -Jmn*l1*l2
						);
						// C行列の設定
						c.Set(
							0, 1
						);
						break;
					
					// 同一次元1次オブザーバの場合
					case DObType::FULL_1ST:
						// 連続系のA行列
						Ac.Set(
							l1 + l2 + l3               , -1.0/Jmn,  0,
							Jmn*(l1*l2 + l2*l3 + l3*l1), 0       ,  1,
							-Jmn*l1*l2*l3              , 0       ,  0
						);
						// 連続系のB行列
						Bc.Set(
							Ktn/Jmn, -( l1 + l2 + l3 ),
							0      , -Jmn*( l1*l2 + l2*l3 + l3*l1 ),
							0      , Jmn*l1*l2*l3
						);
						// C行列
						c.Set(
							0,  1,  0
						);
						break;
					
					// それ以外
					default:
						arcs_assert(false);	// ここには来ない
						break;
				}
				
				// 離散化
				std::tie(Ad, Bd) = Discret::GetDiscSystem(Ac, Bc, Ts);
				PassedLog();
			}
			
			//! @brief コンストラクタ(ベクトル版)
			//! @param [in] TrqConst トルク定数 [Nm/A]
			//! @param [in] Inertia 慣性 [kgm^2]
			//! @param [in] Bandwidth 推定帯域 [rad/s]
			//! @param [in] SmplTime 制御周期 [s]
			DisturbanceObsrv(Matrix<1,N> TrqConst, Matrix<1,N> Inertia, Matrix<1,N> Bandwidth, double SmplTime)
				: Ts(SmplTime),
				  Ktn(), Jmn(), l1(), l2(), l3(),
				  Ac(), Bc(), Ad(), Bd(), c(), u(), x(), x_next(), y(),
				  Ktnv(TrqConst), Jmnv(Inertia), l1v(-Bandwidth), l2v(-Bandwidth), l3v(-Bandwidth),
				  Acv(), Bcv(), Adv(), Bdv(), cv(), uv(), xv(), xv_next(), yv()
			{
				// ベクトルの長さだけ回す
				for(size_t i = 1; i <= N; ++i){
					// オブザーバの構成によって状態方程式を変える
					switch(T){
						// 同一次元0次オブザーバの場合
						case DObType::FULL_0TH:
							// 連続系A行列の設定
							Acv[i-1].Set(
								l1v[i] + l2v[i]      ,  -1.0/Jmnv[i],
								Jmnv[i]*l1v[i]*l2v[i],       0
							);
							// 連続系B行列の設定
							Bcv[i-1].Set(
								Ktnv[i]/Jmnv[i],      - l1v[i] - l2v[i],
											0, -Jmnv[i]*l1v[i]*l2v[i]
							);
							// C行列の設定
							cv[i-1].Set(
								0, 1
							);
							break;
						
						// 同一次元1次オブザーバの場合
						case DObType::FULL_1ST:
							// 連続系のA行列
							Acv[i-1].Set(
								l1v[i] + l2v[i] + l3v[i]                               , -1.0/Jmnv[i],  0,
								Jmnv[i]*(l1v[i]*l2v[i] + l2v[i]*l3v[i] + l3v[i]*l1v[i]), 0           ,  1,
								-Jmnv[i]*l1v[i]*l2v[i]*l3v[i]                          , 0           ,  0
							);
							// 連続系のB行列
							Bcv[i-1].Set(
								Ktnv[i]/Jmnv[i], -( l1v[i] + l2v[i] + l3v[i] ),
								0              , -Jmnv[i]*( l1v[i]*l2v[i] + l2v[i]*l3v[i] + l3v[i]*l1v[i] ),
								0              , Jmnv[i]*l1v[i]*l2v[i]*l3v[i]
							);
							// C行列
							cv[i-1].Set(
								0,  1,  0
							);
							break;
						
						// それ以外
						default:
							arcs_assert(false);	// ここには来ない
							break;
					}
					
					// 離散化
					std::tie(Adv[i-1], Bdv[i-1]) = Discret::GetDiscSystem(Acv[i-1], Bcv[i-1], Ts);
				}
				PassedLog();
			}
			
			//! @brief ムーブコンストラクタ(スカラー版のみ)
			DisturbanceObsrv(DisturbanceObsrv&& right)
				: Ts(right.Ts),
				  Ktn(right.Ktn), Jmn(right.Jmn), l1(right.l1), l2(right.l2), l3(right.l3),
				  Ac(right.Ac), Bc(right.Bc), Ad(right.Ad), Bd(right.Bd), c(right.c),
				  u(right.u), x(right.x), x_next(right.x_next), y(right.y),
				  Ktnv(), Jmnv(), l1v(), l2v(), l3v(),
				  Acv(), Bcv(), Adv(), Bdv(), cv(), uv(), xv(), xv_next(), yv()
			{
				
			}
			
			//! @brief デストラクタ
			~DisturbanceObsrv(){
				PassedLog();
			}
			
			//! @brief 外乱トルクを推定する関数(スカラー版)
			//! @param [in] Current 電流 [A]
			//! @param [in] MotorSpeed モータ側速度 [rad/s]
			double GetDistTorque(double Current, double MotorSpeed){
				// 入力ベクトルの設定
				u.Set(
					Current,
					MotorSpeed
				);
				// 離散系状態方程式の計算
				x_next = Ad*x + Bd*u;
				y = c*x;
				// 状態ベクトルの更新
				x = x_next;
				// 出力ベクトルから抽出して返す
				return y[1];
			}
			
			//! @brief 外乱トルクを推定する関数(ベクトル版)
			//! @param [in] Current 電流 [A]
			//! @param [in] MotorSpeed モータ側速度 [rad/s]
			Matrix<1,N> GetDistTorque(Matrix<1,N> Current, Matrix<1,N> MotorSpeed){
				Matrix<1,N> ret;
				// ベクトルの長さだけ回す
				for(size_t i = 0; i < N; ++i){
					// 入力ベクトルの設定
					uv[i].Set(
						Current[i+1],
						MotorSpeed[i+1]
					);
					// 離散系状態方程式の計算
					xv_next[i] = Adv[i]*xv[i] + Bdv[i]*uv[i];
					yv[i] = cv[i]*xv[i];
					// 状態ベクトルの更新
					xv[i] = xv_next[i];
					// 出力ベクトルから抽出
					ret[i+1] = yv[i][1];
				}
				return ret;
			}
			
			//! @brief 状態ベクトルをクリアする関数
			void ClearStateVector(void){
				if constexpr(N == 1){
					// スカラー版のとき
					x.FillAllZero();		// 状態ベクトルをクリア
					x_next.FillAllZero();	// 次の時刻の状態ベクトルもクリア
				}else{
					// ベクトル版のとき
					// ベクトルの長さだけ回す
					for(size_t i = 0; i < N; ++i){
						xv[i].FillAllZero();		// 状態ベクトルをクリア
						xv_next[i].FillAllZero();	// 次の時刻の状態ベクトルもクリア
					}
				}
			}
			
		private:
			DisturbanceObsrv(const DisturbanceObsrv&) = delete;					//!< コピーコンストラクタ使用禁止
			const DisturbanceObsrv& operator=(const DisturbanceObsrv&) = delete;//!< 代入演算子使用禁止
			
			//!@ brief A行列の幅/高さを返す関数
			static constexpr unsigned int GetNA(void){
				switch(T){
					case DObType::FULL_0TH: return 2;
					case DObType::FULL_1ST: return 3;
				}
				return 0;
			}
			
			//!@ brief B行列の幅を返す関数
			static constexpr unsigned int GetNB(void){
				switch(T){
					case DObType::FULL_0TH: return 2;
					case DObType::FULL_1ST: return 2;
				}
				return 0;
			}
			
			//!@ brief B行列の高さを返す関数
			static constexpr unsigned int GetMB(void){
				switch(T){
					case DObType::FULL_0TH: return 2;
					case DObType::FULL_1ST: return 3;
				}
				return 0;
			}
			
			//!@ brief c行列の幅と状態ベクトルxの高さを返す関数
			static constexpr unsigned int GetCX(void){
				switch(T){
					case DObType::FULL_0TH: return 2;
					case DObType::FULL_1ST: return 3;
				}
				return 0;
			}
			
			// 共通変数
			double Ts;	//!< [s] 制御周期
			
			// スカラー版の変数
			double Ktn;	//!< [Nm/A]  トルク定数
			double Jmn;	//!< [kgm^2] モータ側慣性
			double l1;	//!< [rad/s] オブザーバの極1
			double l2;	//!< [rad/s] オブザーバの極2
			double l3;	//!< [rad/s] オブザーバの極3
			Matrix<GetNA(),GetNA()> Ac;	//!<  連続系 A行列
			Matrix<GetNB(),GetMB()> Bc;	//!<  連続系 B行列
			Matrix<GetNA(),GetNA()> Ad;	//!<  離散系 A行列
			Matrix<GetNB(),GetMB()> Bd;	//!<  離散系 B行列
			Matrix<GetCX(),1> c;		//!<  C行列
			Matrix<1,2> u;				//!<  入力ベクトル
			Matrix<1,GetCX()> x;		//!<  状態ベクトル
			Matrix<1,GetCX()> x_next;	//!<  次の時刻の状態ベクトル
			Matrix<1,1> y;				//!<  出力ベクトル
			
			// ベクトル版の変数
			Matrix<1,N> Ktnv;	//!< [Nm/A]  トルク定数
			Matrix<1,N> Jmnv;	//!< [kgm^2] モータ側慣性
			Matrix<1,N> l1v;	//!< [rad/s] オブザーバの極1
			Matrix<1,N> l2v;	//!< [rad/s] オブザーバの極2
			Matrix<1,N> l3v;	//!< [rad/s] オブザーバの極3
			std::array<Matrix<GetNA(),GetNA()>, N> Acv;	//!<  連続系 A行列
			std::array<Matrix<GetNB(),GetMB()>, N> Bcv;	//!<  連続系 B行列
			std::array<Matrix<GetNA(),GetNA()>, N> Adv;	//!<  離散系 A行列
			std::array<Matrix<GetNB(),GetMB()>, N> Bdv;	//!<  離散系 B行列
			std::array<Matrix<GetCX(),1>, N> cv;		//!<  C行列
			std::array<Matrix<1,2>, N> uv;				//!<  入力ベクトル
			std::array<Matrix<1,GetCX()>, N> xv;		//!<  状態ベクトル
			std::array<Matrix<1,GetCX()>, N> xv_next;	//!<  次の時刻の状態ベクトル
			std::array<Matrix<1,1>, N> yv;				//!<  出力ベクトル
	};
}

#endif

