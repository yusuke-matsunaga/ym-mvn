#ifndef YM_MVN_H
#define YM_MVN_H

/// @file ym/mvn.h
/// @brief ym-mvn の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2017, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


/// @brief mvn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MVN		\
  BEGIN_NAMESPACE_YM			\
  BEGIN_NAMESPACE(nsMvn)

/// @brief mvn 用の名前空間の終了
#define END_NAMESPACE_YM_MVN		\
  END_NAMESPACE(nsMvn)			\
  END_NAMESPACE_YM

/// @brief MvnVerilogReader 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MVN_VERILOG	\
  BEGIN_NAMESPACE_YM_MVN		\
  BEGIN_NAMESPACE(nsVerilog)

/// @brief MvnVerilogReader 用の名前空間の終了
#define END_NAMESPACE_YM_MVN_VERILOG	\
  END_NAMESPACE(nsVerilog)		\
  END_NAMESPACE_YM_MVN


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @brief ノード型定義
/// @note 論理演算とビットワイズ論理演算の区別はない．
//////////////////////////////////////////////////////////////////////
enum class MvnNodeType {
  /// @brief 入力 ( 0入力 )
  INPUT,
  /// @brief 出力 ( 1入力 )
  OUTPUT,
  /// @brief 入出力 ( 1入力 )
  INOUT,

  /// @brief フリップフロップ
  /// (クロック，データ入力，非同期セット入力，データ出力)
  /// 非同期セットは複数ある場合もある．
  DFF,

  /// @brief ラッチ
  /// (データ入力，ラッチイネーブル，データ出力)
  LATCH,

  /// @brief through ( 1入力 )
  THROUGH,

  /// @brief logical not ( 1入力 )
  NOT,
  /// @brief logical and ( n入力 )
  AND,
  /// @brief logical or ( n入力 )
  OR,
  /// @brief logical xor ( n入力 )
  XOR,

  /// @brief reduction and ( 1入力 )
  RAND,
  /// @brief reduction or ( 1入力 )
  ROR,
  /// @brief reduction xor ( 1入力 )
  RXOR,

  /// @brief equal ( 2入力 )
  EQ,
  /// @brief less than ( 2入力 )
  LT,
  /// @brief case-equal ( 2入力 )
  CASEEQ,

  /// @brief shift left logical ( 2入力 )
  SLL,
  /// @brief shift right logical ( 2入力 )
  SRL,
  /// @brief shift left arithmetic ( 2入力 )
  SLA,
  /// @brief shift right arithmetic ( 2入力 )
  SRA,

  /// @brief cmpl ( 1入力 )
  /// 2の補数
  CMPL,
  /// @brief add ( 2入力 )
  ADD,
  /// @brief sub ( 2入力 )
  SUB,
  /// @brief multiply ( 2入力 )
  MUL,
  /// @brief division ( 2入力 )
  DIV,
  /// @brief modulo ( 2入力 )
  MOD,
  /// @brief power ( 2入力 )
  POW,

  /// @brief condition ( 3入力 )
  ITE,

  /// @brief concatenate ( n入力 )
  CONCAT,

  /// @brief constant bit-select ( 1入力 )
  CONSTBITSELECT,

  /// @brief constant part-select ( 1入力 )
  CONSTPARTSELECT,

  /// @brief bit-select ( 2入力 )
  BITSELECT,

  /// @brief part-select ( 3入力 )
  PARTSELECT,

  /// @brief constant ( 0入力 )
  CONST,

  /// @brief cell ( n入力 )
  CELL
};


//////////////////////////////////////////////////////////////////////
/// @brief フリップフロップのクロックの極性
//////////////////////////////////////////////////////////////////////
enum class MvnPolarity {
  /// @brief positive edge
  Positive,
  /// @brief negative edge
  Negative,
};


// クラス名の先行宣言
class MvnMgr;
class MvnModule;
class MvnPort;
class MvnPortRef;
class MvnNode;
class MvnInputPin;
class MvnBvConst;

class MvnVerilogReader;
class MvnVlMap;

class MvnDumper;
class MvnVerilogWriter;

END_NAMESPACE_YM_MVN

BEGIN_NAMESPACE_YM

using nsMvn::MvnNodeType;
using nsMvn::MvnPolarity;

using nsMvn::MvnMgr;
using nsMvn::MvnModule;
using nsMvn::MvnPort;
using nsMvn::MvnPortRef;
using nsMvn::MvnNode;
using nsMvn::MvnInputPin;
using nsMvn::MvnBvConst;

using nsMvn::MvnVerilogReader;
using nsMvn::MvnVlMap;

using nsMvn::MvnDumper;
using nsMvn::MvnVerilogWriter;

END_NAMESPACE_YM

#endif // YM_YM_MVN_H
