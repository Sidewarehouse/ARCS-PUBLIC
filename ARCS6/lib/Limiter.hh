//! @file Limiter.hh
//! @brief リミッタ
//! 任意の数値で入力を制限して出力
//! @date 2019/08/30
//! @author Yuki YOKOKURA
//
// Copyright (C) 2011-2019 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef LIMITER
#define LIMITER

namespace ARCS {	// ARCS名前空間
	double Limiter(const double input, const double limit);	//!< リミッタ
	double Limiter(const double input, const double pos_limit, const double neg_limit);	//!< リミッタ
}

#endif

