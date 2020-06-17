//! @file SquareWave.cc
//! @brief 方形波発生器
//!
//! @date 2019/02/22
//! @author Yuki YOKOKURA
//
// Copyright (C) 2011-2019 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#include <cmath>
#include "SquareWave.hh"

//! @brief 方形波を出力する関数
//! @param[in]	freq	[Hz] 周波数
//! @param[in]	phase	[rad]位相
//! @param[in]	time	[s]  時刻
//! @return	方形波
double ARCS::SquareWave(const double freq, const double phase, const double time){
	double y;
	double r = sin(2.0*M_PI*freq*time + phase);
	if(0 < r){
		y =  1;
	}else{
		y = -1;
	}
	return y;
}

