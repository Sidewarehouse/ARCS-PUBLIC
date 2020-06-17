//! @file Statistics.cc
//! @brief 統計処理クラス(テンプレート行列＆std::array版)
//! 平均，分散，標準偏差，共分散，相関係数を計算する
//!
//! @date 2020/05/10
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2020 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef STATISTICS
#define STATISTICS

#include <cassert>
#include <cmath>
#include "Matrix.hh"

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
//! @brief 統計処理クラス(テンプレート行列＆std::array版)
class Statistics {
	public:
		//! @brief 行列U全体の平均を計算する
		template <size_t N, size_t M>
		static double Mean(const Matrix<N,M>& U){
			double Ubar = 0;
			for(size_t i = 1; i <= M; ++i){
				for(size_t j = 1; j <= N; ++j) Ubar += U.GetElement(j,i);
			}
			return Ubar/(double)(N*M);	// 平均値を計算して返す
		}
		
		//! @brief 行列Uの横方向の平均を計算して縦ベクトルを出力する関数
		template <size_t N, size_t M>
		static void MeanRow(const Matrix<N,M>& U, Matrix<1,M>& y){
			y = sumrow(U)/(double)N;	// 横方向に加算して行列の幅で割って平均を算出
		}
		
		//! @brief 配列uの平均を計算する
		template <size_t N>
		static double Mean(const std::array<double, N>& u){
			double ubar = 0;
			for(size_t i = 0; i < N; ++i){
				ubar += u[i];
			}
			return ubar/(double)N;	// 平均値を計算して返す
		}
		
		//! @brief 行列U全体の標本分散を求める
		template <size_t N, size_t M>
		static double SampledVariance(const Matrix<N,M>& U){
			// Uの平均を計算
			double Ubar = Mean(U);
			
			// Uから平均値を減算して2乗して累積加算
			double xi_xbar = 0, s2 = 0;
			for(size_t i = 1; i <= M; ++i){
				for(size_t j = 1; j <= N; ++j){
					xi_xbar = U.GetElement(j,i) - Ubar;
					s2 += xi_xbar*xi_xbar;
				}
			}
			return s2/(double)(N*M);
		}
		
		//! @brief 配列uの標本分散を求める
		template <size_t N>
		static double SampledVariance(const std::array<double, N>& u){
			// Uの平均を計算
			double ubar = Mean(u);
			
			// Uから平均値を減算して2乗して累積加算
			double xi_xbar = 0, s2 = 0;
			for(size_t i = 0; i < N; ++i){
				xi_xbar = u[i] - ubar;
				s2 += xi_xbar*xi_xbar;
			}
			return s2/(double)N;
		}
		
		//! @brief 行列U全体の不偏分散を求める
		template <size_t N, size_t M>
		static double UnbiasedVariance(const Matrix<N,M>& U){
			// Uの平均を計算
			double Ubar = Mean(U);
			
			// Uから平均値を減算して2乗して累積加算
			double xi_xbar = 0, s2 = 0;
			for(size_t i = 1; i <= M; ++i){
				for(size_t j = 1; j <= N; ++j){
					xi_xbar = U.GetElement(j,i) - Ubar;
					s2 += xi_xbar*xi_xbar;
				}
			}
			return s2/(double)(N*M - 1);
		}
		
		//! @brief 配列uの不偏分散を求める
		template <size_t N>
		static double UnbiasedVariance(const std::array<double, N>& u){
			// Uの平均を計算
			double ubar = Mean(u);
			
			// Uから平均値を減算して2乗して累積加算
			double xi_xbar = 0, s2 = 0;
			for(size_t i = 0; i <= N; ++i){
				xi_xbar = u[i] - ubar;
				s2 += xi_xbar*xi_xbar;
			}
			return s2/(double)(N - 1);
		}
		
		//! @brief 行列U全体の分散を求める(エイリアス)
		template <size_t N, size_t M>
		static double Variance(const Matrix<N,M>& U){
			return UnbiasedVariance(U);	// 不偏分散を返す
		}
		
		//! @brief 配列uの分散を求める(エイリアス)
		template <size_t N>
		static double Variance(const std::array<double, N>& u){
			return UnbiasedVariance(u);	// 不偏分散を返す
		}
		
		//! @brief 行列U全体の標準偏差を求める
		template <size_t N, size_t M>
		static double StandardDeviation(const Matrix<N,M>& U){
			return sqrt(Variance(U));
		}
		
		//! @brief 配列uの標準偏差を求める
		template <size_t N>
		static double StandardDeviation(const std::array<double, N>& u){
			return sqrt(Variance(u));
		}
		
		//! @brief 行列U1と行列U2の間の共分散を計算する
		template <size_t N, size_t M>
		static double Covariance(const Matrix<N,M>& U1, const Matrix<N,M>& U2){
			// U1とU2の平均を計算
			double Ubar1 = Mean(U1);
			double Ubar2 = Mean(U2);
			
			// U1とU2から平均値を減算して共分散を計算して返す
			double U1_Ubar1 = 0, U2_Ubar2 = 0, v = 0;
			for(size_t i = 1; i <= M; ++i){
				for(size_t j = 1; j <= N; ++j){
					U1_Ubar1 = U1.GetElement(j,i) - Ubar1;
					U2_Ubar2 = U2.GetElement(j,i) - Ubar2;
					v += U1_Ubar1*U2_Ubar2;
				}
			}
			return v/(double)(N*M);
		}
		
		//! @brief 配列u1と配列u2の間の共分散を計算する
		template <size_t N>
		static double Covariance(const std::array<double, N>& u1, const std::array<double, N>& u2){
			// u1とu2の平均を計算
			double ubar1 = Mean(u1);
			double ubar2 = Mean(u2);
			
			// u1とu2から平均値を減算して共分散を計算して返す
			double u1_ubar1 = 0, u2_ubar2 = 0, v = 0;
			for(size_t i = 0; i < N; ++i){
				u1_ubar1 = u1[i] - ubar1;
				u2_ubar2 = u2[i] - ubar2;
				v += u1_ubar1*u2_ubar2;
			}
			return v/(double)N;
		}
		
		//! @brief 行列U1と行列U2の間のピアソンの相関係数を求める
		template <size_t N, size_t M>
		static double Correlation(const Matrix<N,M>& U1, const Matrix<N,M>& U2){
			double Var1 = SampledVariance(U1);	// U1の標本分散
			double Var2 = SampledVariance(U2);	// U2の標本分散
			double Cov = Covariance(U1, U2);	// U1とU2の共分散
			return Cov/(sqrt(Var1)*sqrt(Var2));	// ピアソンの相関係数
		}
		
		//! @brief 配列u1と配列u2の間のピアソンの相関係数を求める
		template <size_t N>
		static double Correlation(const std::array<double, N>& u1, const std::array<double, N>& u2){
			double Var1 = SampledVariance(u1);	// u1の標本分散
			double Var2 = SampledVariance(u2);	// u2の標本分散
			double Cov = Covariance(u1, u2);	// u1とu2の共分散
			return Cov/(sqrt(Var1)*sqrt(Var2));	// ピアソンの相関係数
		}
		
	private:
		Statistics() = delete;									//!< コンストラクタ使用禁止
		Statistics(Statistics&& r) = delete;					//!< ムーブコンストラクタ使用禁止
		~Statistics() = delete;									//!< デストラクタ使用禁止
		Statistics(const Statistics&) = delete;					//!< コピーコンストラクタ使用禁止
		const Statistics& operator=(const Statistics&) = delete;//!< 代入演算子使用禁止
};
}

#endif

