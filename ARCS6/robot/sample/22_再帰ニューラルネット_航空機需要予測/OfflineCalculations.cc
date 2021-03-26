//! @file OfflineCalculations.cc
//! @brief ARCS6 オフライン計算用メインコード
//! @date 2021/02/25
//! @author Yokokura, Yuki
//!
//! @par オフライン計算用のメインコード
//! - 「make offline」でコンパイルすると，いつものARCS制御用のコードは走らずに，
//!    このソースコードのみが走るようになる。
//! - ARCSライブラリはもちろんそのままいつも通り使用可能。
//! - 従って，オフラインで何か計算をしたいときに，このソースコードに記述すれば良い。
//!
// Copyright (C) 2011-2021 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

// 基本のインクルードファイル
#include <stdio.h>
#include <cstdlib>
#include <cassert>
#include <array>
#include <complex>

// 追加のARCSライブラリをここに記述
#include "Matrix.hh"
#include "CsvManipulator.hh"
#include "TimeSeriesDatasets.hh"

using namespace ARCS;

//! @brief エントリポイント
//! @return 終了ステータス
int main(void){
	printf("ARCS OFFLINE CALCULATION MODE\n");
	printf("Recurrent Neural Network Test\n");
	
	// 航空機需要データセットを読み込み
	TimeSeriesDatasets<1,144,1> AirlineData("AirlinePassengers.csv");
	AirlineData.DispTimeSeriesData(10);
	AirlineData.WritePngPlot(0, 700, "AirlinePassengers.png");
	
	
	
	return EXIT_SUCCESS;	// 正常終了
}

