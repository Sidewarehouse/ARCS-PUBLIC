//! @file PCI-2826.hh
//! @brief PCI-2826入出力クラス
//! Interface社製PCI-2826のための入出力機能を提供します。
//! @date 2020/06/09
//! @author Yuto KOBAYASHI
//
// Copyright (C) 2011-2020 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the BSD License.
// For details, see the License.txt file.

#ifndef PCI_2826CV
#define PCI_2826CV

namespace ARCS {
    //! @brief PCI-2826入出力クラス
    //! Interface社製PCI-2826のための入出力機能を提供します。
    class PCI2826CV {
        public:
            PCI2826CV(unsigned int Addr, unsigned int DataMask); //!< コンストラクタ
            PCI2826CV();                  //!< 空コンストラクタ
            ~PCI2826CV();                 //!< デストラクタ
            
            
            unsigned int GetData(void);    //!< 入力の読み取り            
            int SetData(unsigned int Data);   //!< 出力の書き込み

        private:
            PCI2826CV(const PCI2826CV&) = delete;       //!< コピーコンストラクタ使用禁止
            const PCI2826CV& operator=(const PCI2826CV&) = delete;  //!< 代入演算子使用禁止

            const unsigned int ADDR_BASE;   //!< ベースアドレス
            const unsigned int ADDR_DIN0;   //!< デジタル入力 ポート0
            const unsigned int ADDR_DIN1;   //!< デジタル入力 ポート1
            const unsigned int ADDR_DIN2;   //!< デジタル入力 ポート2
            const unsigned int ADDR_DIN3;   //!< デジタル入力 ポート3
            const unsigned int ADDR_DOUT0;   //!< デジタル出力 ポート0
            const unsigned int ADDR_DOUT1;   //!< デジタル出力 ポート1
            const unsigned int ADDR_DOUT2;   //!< デジタル出力 ポート2
            const unsigned int ADDR_DOUT3;   //!< デジタル出力 ポート3
            const unsigned int ADDR_CTRL1;  //!< 制御ポート1
            
            const unsigned int DATA_MASK;         //!< データマスク


    };
}

# endif
