//! @file RecurrentNeuralLayer.hh
//! @brief 再帰ニューラルレイヤクラス
//!
//! 再帰ニューラルネットのレイヤ
//!
//! @date 2021/06/22
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2021 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef RECURRENTNEURALLAYER
#define RECURRENTNEURALLAYER

#include <cassert>
#include <array>
#include <cmath>
#include <string>
#include "Matrix.hh"
#include "SingleLayerPerceptron.hh"

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
//! @brief 再帰ニューラルレイヤクラス
//! @tparam N	入力の数
//! @tparam P	パーセプトロンの数
//! @tparam M	ミニバッチ数
//! @tparam	AF	活性化関数の種類
//! @tparam	IT	初期化のタイプ
//! @tparam	GD	勾配降下法のタイプ
//! @tparam	DD	ドロップアウト動作フラグ
template <
	size_t N,
	size_t P,
	size_t M,
	ActvFunc AF,
	NnInitTypes IT = NnInitTypes::XAVIER,
	NnDescentTypes GD = NnDescentTypes::MOMENTUM,
	NnDropout DD = NnDropout::DISABLE
>
class RecurrentNeuralLayer : public SingleLayerPerceptron<N, P, M, AF, IT, GD, DD> {
	// 単層パーセプトロンクラスを継承
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::u;		//!< 状態ベクトル
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::U;		//!< 状態行列
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::W;		//!< 重み行列
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::b;		//!< バイアスベクトル
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::l;		//!< 1ベクトル
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::lT;		//!< 1ベクトルの転置
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::Delta;	//!< 誤差行列
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::fpU;		//!< 活性化関数の微分を通した後の値
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::dW;		//!< 重み更新差分値
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::db;		//!< バイアス更新差分値
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::DW;		//!< 更新ゲイン乗算後の重み更新差分値
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::Db;		//!< 更新ゲイン乗算後のバイアス更新差分値
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::HW;		//!< AdaGrad, RMSprop, Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::Hb;		//!< AdaGrad, RMSprop, Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::GW;		//!< AdaDelta, Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::Gb;		//!< AdaDelta, Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::HWhat;	//!< Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::GWhat;	//!< Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::Hbhat;	//!< Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::Gbhat;	//!< Adam用
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::SGDcount;	//!< Adam勾配降下法用のカウンタ
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::DropRand;	//!< ドロップアウト用メルセンヌ・ツイスタ
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::DropMask;	//!< ドロップアウト用マスクベクトル
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::xbar;		//!< データセット正規化(標準化)用の平均値
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::sigma;	//!< データセット正規化(標準化)用の標準偏差
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::eps;		//!< 更新ゲイン(SGD, Momentum, AdaGrad, RMSprop)
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::alph;		//!< 更新ゲイン(Momentum, RMSprop, AdaDelta)
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::bet;		//!< 更新ゲイン(Adam)
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::NearZero;	//!< ゼロ割回避用のゼロに近い値
	using SingleLayerPerceptron<N, P, M, AF, IT, GD, DD>::DropRate;	//!< ドロップアウト率
	
	public:
		//! @brief コンストラクタ
		RecurrentNeuralLayer()
		{
			
		}
		
		//! @brief ムーブコンストラクタ
		//! @param[in]	r	右辺値
		RecurrentNeuralLayer(RecurrentNeuralLayer&& r)
		{
			
		}
		
		//! @brief デストラクタ
		~RecurrentNeuralLayer(){
			
		}
		
		//! @brief 再帰ニューラルネットの順伝播計算(ベクトル入出力訓練版)
		//! @param[in]	zprev	前の層からの入力ベクトル
		//! @param[in]	z		出力ベクトル
		void CalcRecurrentForwardForTraining(const Matrix<1,N>& zprev, Matrix<1,P>& z){
			u = W*zprev + Wh*hprev + b;				// 重み乗算加算とバイアス加算
			ActivationFunctions::f<AF,1,P>(u, z);	// 活性化関数
			
			if constexpr(DD == NnDropout::ENABLE){
				// ドロップアウトする場合
				z = z & DropMask;	// 該当ノードをドロップアウト
			}
		}
	
	private:
		Matrix<1,P> hprev;	//!< 前回の状態ベクトル
		Matrix<N,P> Wh;		//!< 前回の状態ベクトルに対する重み行列

};
}

#endif

