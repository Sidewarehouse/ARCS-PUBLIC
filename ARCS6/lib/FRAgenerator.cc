//! @file FRAgenerator.cc
//! @brief FRA用信号生成器
//!
//! Frequency Response Analysis のための信号生成器
//!
//! @date 2019/01/17
//! @author Yuki YOKOKURA & Muto Hirotaka
//
// Copyright (C) 2011-2019 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#include <cmath>
#include "FRAgenerator.hh"
#include "ARCSeventlog.hh"

using namespace ARCS;

//! @brief コンストラクタ
FRAgenerator::FRAgenerator(
	const double FreqMin,		// [Hz]開始周波数
	const double FreqMax,	 	// [Hz]終了周波数
	const double FreqStep,		// [Hz]周波数ステップ
	const double NumIntg,		// [-] 積分周期 (1周波数につき何回sin波を入力するか)
	const double Ampl,			// [-] 振幅
	const double Bias,			// [-] バイアス
	const double TimeSta		// [s] FRA開始時刻
)
	: fmin(FreqMin), fmax(FreqMax), fstep(FreqStep), Ni(NumIntg), Au(Ampl), Bu(Bias), Tsta(TimeSta),
	  isEnd(false), f(fmin), tini(0)
{
	PassedLog();
}

//! @brief デストラクタ
FRAgenerator::~FRAgenerator(){
	PassedLog();
}

//! @brief FRA信号出力関数
//! @param[in] t [s] 時刻
//! @return outsig FRA信号, f [Hz] 周波数
std::tuple<double, double> FRAgenerator::GetSignal(double t){
	double outsig = Bu;	// 初期値はバイアス値に設定
	if(Tsta <= t && isEnd == false){	// FRA開始時刻を過ぎて，且つ最大周波数以下であれば，
		outsig = Au*cos(2.0*M_PI*f*(t - tini - Tsta)) + Bu;	 // [A] FRA信号の生成
		if (Ni/f <= t - tini - Tsta){ 	// Ni回必要な時間になるまでf[Hz]を加える。Ni回終わったら....
			if(f <= fmax){				// 最大周波数以下なら次の周波数へ移行
				tini = t - Tsta;		// [s] 次の周波数の開始時刻
				f = f + fstep;			// [Hz]次の周波数 (double型の累積加算は…まあご愛嬌)
			}else{				
				isEnd = true;	// 最大周波数に達したら終了
			}
		}
	}else{
		outsig = Bu;	// FRA非動作時はバイアス値を出力
	}
	return std::forward_as_tuple(f, outsig);	// 周波数とFRA信号を返す
}

