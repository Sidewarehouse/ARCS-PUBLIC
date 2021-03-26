//! @file TimeSeriesDatasets.hh
//! @brief 機械学習用 時系列データセットクラス
//!
//! 機械学習のための時系列データを扱うためのデータセットクラス
//! データ自体は保持していないので，外部のCSVデータを必要とする
//! 注意：CSVデータの改行コードは「LF」とし、「CRLF」は不可
//!
//! @date 2021/02/25
//! @author Yokokura, Yuki
//
// Copyright (C) 2011-2021 Yokokura, Yuki
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#ifndef TIMESERIESDATASETS
#define TIMESERIESDATASETS

#include <cassert>
#include <array>
#include <string>
#include <ctime>
#include "Matrix.hh"
#include "BatchProcessor.hh"
#include "CsvManipulator.hh"
#include "FrameGraphics.hh"
#include "CuiPlot.hh"

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
//! @brief 機械学習用 時系列データセットクラス
//! @tparam N	計測データチャネル数
//! @tparam	D	時間方向のデータ数
//! @tparam	M	ミニバッチ数
template <size_t N, size_t D, size_t M>
class TimeSeriesDatasets {
	public:
		// データセット情報の定義
		static constexpr size_t C = 1;	//!< 訓練データチャネル数
		static constexpr size_t FinalMinbatNum = D/M - 1;	//!< 最後のミニバッチ番号
		
		// 時系列データ
		Matrix<1,D> TimeStamp;			//!< タイムスタンプ
		Matrix<N,D> TimeSeriesData;		//!< 時系列データ
		
		//! @brief コンストラクタ
		//! @param[in]	FileName	時系列データのCSVファイル名
		TimeSeriesDatasets(const std::string& FileName)
			: TimeStamp(), TimeSeriesData()
		{
			PassedLog();
			TimeStamp = Matrix<1,D>::ramp();					// タイムスタンプを生成
			CsvManipulator::LoadFile(TimeSeriesData, FileName);	// 時系列データを行列として読み込み
		}
		
		//! @brief ムーブコンストラクタ
		//! @param[in]	r	右辺値
		TimeSeriesDatasets(TimeSeriesDatasets&& r)
			: TimeStamp(r.TimeStamp), TimeSeriesData(r.TimeSeriesData)
		{
			
		}
		
		//! @brief デストラクタ
		~TimeSeriesDatasets(){
			PassedLog();
		}
		
		//! @brief 時系列データを表示する関数
		//! @param[in]	DispNum	表示間引き数
		void DispTimeSeriesData(const size_t DispNum){
			printf("\nTime Series Data:\n");
			for(size_t i = 0; i < D; ++i){
				if((i % DispNum) == 0) printf("%10.0f : %7.3f\n", TimeStamp[i+1], TimeSeriesData[i+1]);
			}
		}
		
		//! @brief 時系列データの時間波形をPNGファイルに書き出す関数
		void WritePngPlot(const double Min, const double Max, const std::string& FileName){
			FrameGraphics FG(GRAPH_WIDTH, GRAPH_HEIGHT);
			CuiPlot Plot(FG, 0, 0, GRAPH_WIDTH, GRAPH_HEIGHT);
			Plot.SetAxisLabels("Time Index", FileName);
			Plot.SetRanges(0, D, Min, Max);
			Plot.SetGridLabelFormat("%5.0f", "%3.0f");
			Plot.DrawAxis();
			Plot.DrawLegend(1, "Variable1", FGcolors::CYAN);
			Plot.Plot(TimeStamp, TimeSeriesData, CuiPlotTypes::PLOT_LINE, FGcolors::CYAN);
			FG.SavePngImageFile(FileName);
		}
		
		/*
		//! @brief i番目のミニバッチ為替データを取得する関数
		//! @param[in]	i	ミニバッチ番号
		//! @param[out]	Y	為替データ
		void GetCurrectBatchData(const size_t i, Matrix<M,N>& Y){
			arcs_assert(i <= FinalMinbatNum);	// ミニバッチ番号の範囲チェック
			Y = BatchProcessor::GetMiniBatchData<N,D,M>(MeasuredData, i);	// データセットを切り出してミニバッチにする
		}
		*/
		
	private:
		TimeSeriesDatasets(const TimeSeriesDatasets&) = delete;					//!< コピーコンストラクタ使用禁止
		const TimeSeriesDatasets& operator=(const TimeSeriesDatasets&) = delete;//!< 代入演算子使用禁止
		
		// 学習曲線グラフの設定
		static constexpr int GRAPH_WIDTH = 1000;	//!< [px] グラフの横幅
		static constexpr int GRAPH_HEIGHT = 500;	//!< [px] グラフの高さ
};
}

#endif

