//! @file ZFA01.hh
//! @brief Zero Force Arm 01 - ZFA01クラス
//!
//! ZFA01ロボット制御のためのクラス
//!
//! @date 2021/07/30
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2021 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef ZFA01_INCGUARD
#define ZFA01_INCGUARD

#include <cmath>
#include "Matrix.hh"

namespace ARCS {	// ARCS名前空間
//! @brief Zero Force Arm 01 - ZFA01クラス
class ZFA01 {
	public:
		// ZFA01基本パラメータの定義
		static constexpr size_t N_AX = 3;				//!< [-] 軸数
		static constexpr double TORQUE_CONST1S = 0.31;	//!< [Nm] 1S軸 トルク定数 (ユニサーボ SVM-750-100)
		static constexpr double TORQUE_CONST2L = 0.31;	//!< [Nm] 2L軸 トルク定数 (ユニサーボ SVM-750-100)
		static constexpr double TORQUE_CONST3U = 0.28;	//!< [Nm] 3U軸 トルク定数 (ユニサーボ SVM-200-100)
		static constexpr double CURRENT_RATED1S = 7.62;	//!< [A]  1S軸 定格電流 (ユニサーボ SVM-750-100)
		static constexpr double CURRENT_RATED2L = 7.62;	//!< [A]  2L軸 定格電流 (ユニサーボ SVM-750-100)
		static constexpr double CURRENT_RATED3U = 2.25;	//!< [A]  3U軸 定格電流 (ユニサーボ SVM-200-100)
		static constexpr double CURRENT_MAX1S = 25.5;	//!< [A]  1S軸 瞬時最大電流 (ユニサーボ SVM-750-100)
		static constexpr double CURRENT_MAX2L = 25.5;	//!< [A]  2L軸 瞬時最大電流 (ユニサーボ SVM-750-100)
		static constexpr double CURRENT_MAX3U =  7.25;	//!< [A]  3U軸 瞬時最大電流 (ユニサーボ SVM-200-100)
		static constexpr double TORQUE_RATED1S = 174;	//!< [Nm]  1S軸 定格トルク (ユニサーボ SVM-750-100)
		static constexpr double TORQUE_RATED2L = 174;	//!< [Nm]  2L軸 定格トルク (ユニサーボ SVM-750-100)
		static constexpr double TORQUE_RATED3U =  45;	//!< [Nm]  3U軸 定格トルク (ユニサーボ SVM-200-100)
		static constexpr double TORQUE_MAX1S = 433;		//!< [Nm]  1S軸 瞬時最大トルク (ユニサーボ SVM-750-100)
		static constexpr double TORQUE_MAX2L = 433;		//!< [Nm]  2L軸 瞬時最大トルク (ユニサーボ SVM-750-100)
		static constexpr double TORQUE_MAX3U = 107;		//!< [Nm]  3U軸 瞬時最大トルク (ユニサーボ SVM-200-100)
		static constexpr double GEAR_RATIO1S = 100;		//!< [-] 1S軸 減速比 (ユニサーボ SVM-750-100)
		static constexpr double GEAR_RATIO2L = 100;		//!< [-] 2L軸 減速比 (ユニサーボ SVM-750-100)
		static constexpr double GEAR_RATIO3U = 100;		//!< [-] 3U軸 減速比 (ユニサーボ SVM-200-100)
		static constexpr double POSITION_INIT1S = 0;	//!< [rad] 1S軸 初期位置
		static constexpr double POSITION_INIT2L = 0;	//!< [rad] 2L軸 初期位置
		static constexpr double POSITION_INIT3U = 0;	//!< [rad] 3U軸 初期位置
		static constexpr double INERTIA_ALL1S = 1e-3;	//!< [kgm^2] 1S軸 全慣性
		static constexpr double INERTIA_ALL2L = 1e-3;	//!< [kgm^2] 2L軸 全慣性
		static constexpr double INERTIA_ALL3U = 1e-4;	//!< [kgm^2] 3U軸 全慣性
		
		//! @brief トルク定数 [Nm/A]
		static constexpr Matrix<1,N_AX> Kt = {TORQUE_CONST1S, TORQUE_CONST2L, TORQUE_CONST3U};
		
		//! @brief 瞬時最大電流 [A]
		static constexpr Matrix<1,N_AX> iq_max = {CURRENT_MAX1S, CURRENT_MAX2L, CURRENT_MAX3U};
		
		//! @brief 減速比 [-]
		static constexpr Matrix<1,N_AX> Rg = {GEAR_RATIO1S, GEAR_RATIO2L, GEAR_RATIO3U};
		
		//! @brief 全慣性 [kgm^2]
		static constexpr Matrix<1,N_AX> Ja = {INERTIA_ALL1S, INERTIA_ALL2L, INERTIA_ALL3U};
		
		ZFA01();			//!< コンストラクタ
		ZFA01(ZFA01&& r);	//!< ムーブコンストラクタ
		~ZFA01();			//!< デストラクタ
		
	private:
		ZFA01(const ZFA01&) = delete;					//!< コピーコンストラクタ使用禁止
		const ZFA01& operator=(const ZFA01&) = delete;	//!< 代入演算子使用禁止
};
}

#endif

