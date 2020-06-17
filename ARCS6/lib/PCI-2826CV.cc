//! @file PCI-2826.cc
//! @brief PCI-2826入出力クラス
//! Interface社製PCI-2826のための入出力機能を提供します。
//! @date 2020/06/09
//! @author Yuto KOBAYASHI
//
// Copyright (C) 2011-2020 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the BSD License.
// For details, see the License.txt file.

#include <sys/io.h>
#include <unistd.h>
#include <algorithm>
#include <stdint.h>
#include "PCI-2826CV.hh"
#include "ARCSeventlog.hh"

using namespace ARCS;

//! @brief コンストラクタ
//! @param [in] Addr アドレス
//! @param [in] DataMask データマスク
PCI2826CV::PCI2826CV(unsigned int Addr, unsigned int DataMask)
  : ADDR_BASE(Addr),
    ADDR_DIN0(Addr + 0x00),
    ADDR_DIN1(Addr + 0x01),
    ADDR_DIN2(Addr + 0x02),
    ADDR_DIN3(Addr + 0x03),
    ADDR_DOUT0(Addr + 0x00),
    ADDR_DOUT1(Addr + 0x01),
    ADDR_DOUT2(Addr + 0x02),
    ADDR_DOUT3(Addr + 0x03),
    ADDR_CTRL1(Addr + 0x08),
    DATA_MASK(DataMask)
{
    PassedLog();
    iopl(3);        // I/O全アドレス空間にアクセス許可
    outb(0b01100000, ADDR_CTRL1);
}

//! @brief 空コンストラクタ
PCI2826CV::PCI2826CV()
 :  ADDR_BASE(0),
    ADDR_DIN0(0),
    ADDR_DIN1(0),
    ADDR_DIN2(0),
    ADDR_DIN3(0),
    ADDR_DOUT0(0),
    ADDR_DOUT1(0),
    ADDR_DOUT2(0),
    ADDR_DOUT3(0),
    ADDR_CTRL1(0),
    DATA_MASK(0)
{
    // 虚無
}


//! @brief デストラクタ
PCI2826CV::~PCI2826CV(){
    PassedLog();
}

//! @brief　入力の読み取り
//! @param [out] デジタル読み出し値
unsigned int PCI2826CV::GetData(void){
    char buff[4];
    unsigned int data;
    // 各ポートからデータを取得
    buff[0] = inb(ADDR_DIN0);
    buff[1] = inb(ADDR_DIN1);
    buff[2] = inb(ADDR_DIN2);
    buff[3] = inb(ADDR_DIN3);
    // データをまとめてひとつの変数にする
    data = (unsigned int)((buff[3]&0xff)<<24 | (buff[0]&0xff)<<16 | (buff[0]&0xff)<<8 | (buff[0]&0xff));
    // マスク処理
    data = data & DATA_MASK;
    return data;
}

//! @brief 出力の書き込み
//! @param [in] デジタル書き込み値
int PCI2826CV::SetData(unsigned int Data){

    //unsigned char buff[4];
    /*
    unsigned int MaskedData;
    //データのマスク
    MaskedData = 0x0007;//Data & DATA_MASK;
    // データを分割して一時格納
    /*
    buff[0] = (MaskedData>>0)&0xff;
    buff[1] = (MaskedData>>8)&0xff;
    buff[2] = (MaskedData>>16)&0xff;
    buff[3] = (MaskedData>>24)&0xff;
    */
  /*
    buff[0] = 0b00111010;
    buff[1] = 0;
    buff[2] = 0b11111111;
    buff[3] = 0b10101010;
  */
   // マスク処理
    // 各ポートに出力
    outb(0b00111010, ADDR_DOUT0);
    outb(0, ADDR_DOUT1);
    outb(0b11111111, ADDR_DOUT2);
    outb(0b10101010, ADDR_DOUT3);

    return 0;
}


