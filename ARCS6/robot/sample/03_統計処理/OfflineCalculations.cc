//! @file OfflineCalculations.cc
//! @brief ARCS6 オフライン計算用メインコード
//! @date 2020/05/03
//! @author Yokokura, Yuki
//!
//! @par オフライン計算用のメインコード
//! - 「make offline」でコンパイルすると，いつものARCS制御用のコードは走らずに，
//!    このソースコードのみが走るようになる。
//! - ARCSライブラリはもちろんそのままいつも通り使用可能。
//! - 従って，オフラインで何か計算をしたいときに，このソースコードに記述すれば良い。
//!
// Copyright (C) 2011-2020 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

// 基本のインクルードファイル
#include <stdio.h>
#include <cstdlib>
#include <cassert>
#include <array>

// 追加のARCSライブラリをここに記述
#include "Matrix.hh"
#include "CsvManipulator.hh"
#include "Statistics.hh"

using namespace ARCS;

//! @brief エントリポイント
//! @return 終了ステータス
int main(void){
	printf("ARCS OFFLINE CALCULATION MODE\n");
	
	// ここにオフライン計算のコードを記述
	
	// 行列の平均のテスト
	printf("◆ 行列の平均のテスト\n");
	Matrix<3,3> A = {
		1,  1,  1,
		2,  3, -2,
		3, -1,  2
	};
	PrintMat(A);
	printf("Mean = % g\n", Statistics::Mean(A));
	
	// 行列の横方向の平均のテスト
	printf("◆ 行列の横方向の平均のテスト\n");
	Matrix<1,3> a;
	Statistics::MeanRow(A, a);
	PrintMat(a);
	
	return EXIT_SUCCESS;	// 正常終了
}

