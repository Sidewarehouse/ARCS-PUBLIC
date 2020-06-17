//! @file TransferFunction.hh
//! @brief 伝達関数クラス
//!
//! 任意の伝達関数を保持，入力に対する応答を計算して出力するクラス。
//! (MATLABでいうところの「tf」のようなもの)
//! 注意：厳密にプロパーで且つ分母の最高次数の係数は１のみサポート
//!
//! @date 2020/04/06
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2020 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef TRANSFERFUNCTION
#define TRANSFERFUNCTION

#include <cassert>
#include "Matrix.hh"
#include "StateSpaceSystem.hh"

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

namespace ARCS {	// ARCS名前空間
//! @brief 伝達関数クラス
//! @tparam N	分子次数
//! @tparam	D	分母次数
template <size_t N, size_t D>
class TransferFunction {
	public:
		//! @brief コンストラクタ
		//! @param[in]	Num	分子の係数ベクトル 例：(b1*s + b0) のとき Matrix<1,2> Num = {b1, b0}
		//! @param[in]	Den	分母の係数ベクトル 例：(s^2 + a1*s + a0) のとき Matrix<1,3> Den = {1, a1, a0}
		//! @param[in]	Ts	サンプリング周期 [s]
		TransferFunction(const Matrix<1,N+1>& Num, const Matrix<1,D+1>& Den, const double Ts)
			: A(),
			  b(),
			  c(),
			  Sys()
		{
			static_assert(N < D);		// 厳密にプロパーかどうかのチェック
			arcs_assert(Den[1] == 1);	// 分母の最高次数の係数は１
			
			// 可制御正準系の連続系状態空間モデルの作成
			for(size_t i = 1; i < D; ++i){
				A.SetElement(i + 1, i, 1);
			}
			for(size_t i = 1; i <= D; ++i){
				A.SetElement(i, D, -Den[D - i + 2]);
			}
			b.FillAllZero();
			b[D] = 1;
			for(size_t i = 1; i <= N + 1; ++i){
				c.SetElement(i, 1, Num[N - i + 2]);
			}
			Sys.SetContinuous(A, b, c, Ts);
			
			PassedLog();
		}

		//! @brief ムーブコンストラクタ
		//! @param[in]	r	右辺値
		TransferFunction(TransferFunction&& r)
			: A(),
			  b(),
			  c(),
			  Sys(std::move(r.Sys))
		{
			
		}

		//! @brief デストラクタ
		~TransferFunction(){
			PassedLog();
		}
		
		//! @brief 入力信号に対する伝達関数の応答を返す関数
		//! @param[in]	u	入力信号
		double GetResponse(const double u){
			return Sys.GetNextResponse(u);	// 1サンプル遅れを防ぐために次の応答を即時に返す
		}
		
	private:
		TransferFunction(const TransferFunction&) = delete;					//!< コピーコンストラクタ使用禁止
		const TransferFunction& operator=(const TransferFunction&) = delete;//!< 代入演算子使用禁止
		Matrix<D,D> A;			//!< 連続系A行列
		Matrix<1,D> b;			//!< 連続系bベクトル
		Matrix<D,1> c;			//!< cベクトル
		StateSpaceSystem<D> Sys;//!< システム
};
}

#endif

