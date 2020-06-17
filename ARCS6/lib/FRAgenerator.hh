//! @file FRAgenerator.hh
//! @brief FRA用信号生成器
//!
//! Frequency Response Analysis のための信号生成器
//!
//! @date 2019/01/02
//! @author Yuki YOKOKURA & Muto Hirotaka
//
// Copyright (C) 2011-2019 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef FRAGENERATOR
#define FRAGENERATOR

#include <tuple>

namespace ARCS {	// ARCS名前空間
	//! @brief FRA用信号生成器
	class FRAgenerator {
		public:
			FRAgenerator(
				const double FreqMin,		// [Hz]開始周波数
				const double FreqMax,	 	// [Hz]終了周波数
				const double FreqStep,		// [Hz]周波数ステップ
				const double NumIntg,		// [-] 積分周期 (1周波数につき何回sin波を入力するか)
				const double Ampl,			// [-] 振幅
				const double Bias,			// [-] バイアス
				const double TimeSta		// [s] FRA開始時刻
			);				//!< コンストラクタ
			~FRAgenerator();//!< デストラクタ
			std::tuple<double, double> GetSignal(double t);	//!< FRA信号出力関数
			
		private:
			FRAgenerator(const FRAgenerator&) = delete;					//!< コピーコンストラクタ使用禁止
			const FRAgenerator& operator=(const FRAgenerator&) = delete;//!< 代入演算子使用禁止
			const double fmin;	//!< [Hz]開始周波数
			const double fmax;	//!< [Hz]終了周波数
			const double fstep;	//!< [Hz]周波数ステップ
			const double Ni;	//!< [-] 積分周期  (1周波数につき何回sin波を入力するか)
			const double Au;	//!< [-] 振幅
			const double Bu;	//!< [-] バイアス
			const double Tsta;	//!< [s] FRA開始時刻
			bool isEnd;			//!< FRA終了フラグ
			double f = fmin;	//!< [Hz]現在の周波数	
			double tini;		//!< [s] 各周波数ごとの開始時刻
	};
}

#endif

