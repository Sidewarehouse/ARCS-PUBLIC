//! @file TwoInertiaParamDef.hh
//! @brief 2慣性共振系のパラメータ定義ファイル
//!
//! 2慣性共振系の慣性，粘性，剛性などのパラメータを構造体として定義する。
//! 軸数が増えてくるとパラメータを渡すだけでも大変になってくるので。
//!
//! @date 2020/04/03
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2020 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef TWOINERTIAPARAMDEF
#define TWOINERTIAPARAMDEF

namespace ARCS {	// ARCS名前空間
	//! @brief 2慣性共振系のパラメータ構造体
	struct TwoInertiaParamDef {
		double Kt;	//!< [Nm/A]		トルク定数
		double Jm;	//!< [kgm^2]	モータ側慣性
		double Dm;	//!< [Nm s/rad]	モータ側粘性
		double Jl;	//!< [kgm^2]	負荷側慣性
		double Dl;	//!< [Nm s/rad]	負荷側粘性
		double Ks;  //!< [Nm/rad]	2慣性間の剛性
		double Rg;	//!< [-]		減速比
	};
}

#endif

