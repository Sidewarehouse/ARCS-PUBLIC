//! @file TriangleWave.cc
//! @brief 三角波発振器
//! @date 2017/04/21
//! @author Yuki YOKOKURA
//
// Copyright (C) 2011-2017 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#include "TriangleWave.hh"
#include <math.h>

//! @brief 三角波発振器
double ARCS::TriangleWave(double freq, double time){
	const double Tp = 1.0/freq;
	const double a = 2.0/Tp;
	double t, y;
	
	t = fmod(time,Tp);	// 時刻を0～Tpの時間範囲に収める
	
	// 三角波の生成
	if(0 <= t && t < Tp/2.0){
		y = a*t;		// 正の傾き
	}else{
		y = -a*t + 2.0;	// 負の傾き
	}
	
	return 2.0*y - 1.0;	// ±1になるようにする
}

