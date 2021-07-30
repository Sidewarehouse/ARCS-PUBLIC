//! @file ZFA01.cc
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

#include <cassert>
#include "ZFA01.hh"

// ARCS組込み用マクロ
#ifdef ARCS_IN
	// ARCSに組み込まれる場合
	#include "ARCSassert.hh"
	#include "ARCSeventlog.hh"
#else
	// ARCSに組み込まれない場合
	#define arcs_assert(a) (assert(a))
	#define PassedLog()
	#define EventLog(a)
	#define EventLogVar(a)
#endif

using namespace ARCS;

//! @brief コンストラクタ
ZFA01::ZFA01()
	// :
{
	
}

//! @brief ムーブコンストラクタ
//! @param[in]	r	右辺値
ZFA01::ZFA01(ZFA01&& r)
	// :
{
	
}

//! @brief デストラクタ
ZFA01::~ZFA01(){
	
}

