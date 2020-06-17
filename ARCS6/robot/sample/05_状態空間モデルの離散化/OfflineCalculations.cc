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
#include "Discret.hh"

using namespace ARCS;

//! @brief エントリポイント
//! @return 終了ステータス
int main(void){
	printf("ARCS OFFLINE CALCULATION MODE\n");
	
	// ここにオフライン計算のコードを記述
	
	// 2慣性共振系のパラメータ
	constexpr double Ts = 100e-6; // [s]      サンプリング時間
	constexpr double Jm = 1e-4;   // [kgm^2]  モータ慣性
	constexpr double Jl = 1;      // [kgm^2]  負荷側慣性
	constexpr double Dm = 0.1;    // [Nms/rad]モータ粘性
	constexpr double Dl = 0.3;    // [Nms/rad]負荷側粘性
	constexpr double Ks = 500;    // [Nm/rad] 2慣性間のばね定数
	constexpr double Rg = 50;     //          減速比
	constexpr double Kt = 0.04;   // [Nm/A]   トルク定数
	
	// 2慣性共振系の連続系状態空間モデル
	printf("\n◆ 2慣性共振系の連続系状態空間モデル\n");
	Matrix<3,3> Ac = {
		-Dm/Jm,      0, -Ks/(Rg*Jm),
			 0, -Dl/Jl,       Ks/Jl,
		1.0/Rg,     -1,           0
	};
	Matrix<2,3> Bc = {
		Kt/Jm,       0,
			0, -1.0/Jl,
			0,       0
	};
	PrintMatrix(Ac, "% 16.14e");
	PrintMatrix(Bc, "% 16.14e");
	
	// 連続系状態空間モデルの離散化(普通版)
	printf("\n◆ 連続系状態空間モデルの離散化(普通版)\n");
	Matrix<3,3> Ad;
	Matrix<2,3> Bd;
	Discret::GetDiscSystem(Ac, Bc, Ad, Bd, Ts);	// 離散化
	PrintMatrix(Ad, "% 16.14e");   				// 離散系状態空間モデルのA行列の表示
	PrintMatrix(Bd, "% 16.14e");   				// 離散系状態空間モデルのB行列の表示
	
	// 連続系状態空間モデルの離散化(普通版, パデ近似の次数と定積分の分割数を設定する版)
	printf("\n◆ 連続系状態空間モデルの離散化(普通版, パデ近似の次数と定積分の分割数を設定する版)\n");
	Discret::GetDiscSystem(Ac, Bc, Ad, Bd, Ts, 100, 100000);// 離散化
	PrintMatrix(Ad, "% 16.14e");   				// 離散系状態空間モデルのA行列の表示
	PrintMatrix(Bd, "% 16.14e");   				// 離散系状態空間モデルのB行列の表示
	
	// 連続系状態空間モデルの離散化(構造化束縛版)
	printf("\n◆ 連続系状態空間モデルの離散化(構造化束縛版)\n");
	auto [Ad1, Bd1] = Discret::GetDiscSystem(Ac, Bc, Ts);	// 離散化
	PrintMatrix(Ad1, "% 16.14e");   			// 離散系状態空間モデルのA行列の表示
	PrintMatrix(Bd1, "% 16.14e");   			// 離散系状態空間モデルのB行列の表示
	
	// 連続系状態空間モデルの離散化(構造化束縛版, パデ近似の次数と定積分の分割数を設定する版)
	printf("\n◆ 連続系状態空間モデルの離散化(構造化束縛版, パデ近似の次数と定積分の分割数を設定する版)\n");
	auto [Ad2, Bd2] = Discret::GetDiscSystem(Ac, Bc, Ts, 100, 100000);	// 離散化
	PrintMatrix(Ad2, "% 16.14e");   			// 離散系状態空間モデルのA行列の表示
	PrintMatrix(Bd2, "% 16.14e");   			// 離散系状態空間モデルのB行列の表示
	
	// 2慣性共振系の連続系状態空間モデルのコンパイル時離散化
	printf("\n◆ 2慣性共振系の連続系状態空間モデルのコンパイル時離散化\n");
	constexpr Matrix<3,3> Acx = {
		-Dm/Jm,      0, -Ks/(Rg*Jm),
			 0, -Dl/Jl,       Ks/Jl,
		1.0/Rg,     -1,           0
	};
	constexpr Matrix<2,3> Bcx = {
		Kt/Jm,       0,
			0, -1.0/Jl,
			0,       0
	};
	constexpr Matrix<3,3> Adx = Discret::GetDiscMatA(Acx, Ts);		// A行列の離散化
	constexpr Matrix<2,3> Bdx = Discret::GetDiscMatB(Acx, Bcx, Ts);	// B行列の離散化
	PrintMatrix(Adx, "% 16.14e");   			// 離散系状態空間モデルのA行列の表示
	PrintMatrix(Bdx, "% 16.14e");   			// 離散系状態空間モデルのB行列の表示
	
	return EXIT_SUCCESS;	// 正常終了
}

