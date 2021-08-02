#ifndef YM_MVNVERILOGWRITER_H
#define YM_MVNVERILOGWRITER_H

/// @file ym/MvnVerilogWriter.h
/// @brief MvnVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


BEGIN_NAMESPACE_YM_MVN

class VerilogWriterImpl;

//////////////////////////////////////////////////////////////////////
/// @class MvnVerilogWriter MvnVerilogWriter.h "ym/MvnVerilogWriter.h"
/// @brief Mvn の内容を Verilog-HDL 形式で出力するためのクラス
///
/// 実際には VerilogWriteImpl に丸投げする facade パタン
//////////////////////////////////////////////////////////////////////
class MvnVerilogWriter
{
public:

  /// @brief コンストラクタ
  MvnVerilogWriter();

  /// @brief デストラクタ
  ~MvnVerilogWriter();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を Verilog-HDL 形式で出力する
  void
  operator()(
    ostream& s,       ///< [in] 出力先のストリーム
    const MvnMgr& mgr ///< [in] Mvn ネットワーク
  );

  /// @brief 内容を Verilog-HDL 形式で出力する
  void
  operator()(
    ostream& s,              ///< [in] 出力先のストリーム
    const MvnMgr& mgr,	     ///< [in] Mvn ネットワーク
    const MvnVlMap& node_map ///< [in] ノードと Verilog 名の対応表
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際に処理を行う実装クラス
  unique_ptr<VerilogWriterImpl> mImpl;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVNVERILOGWRITER_H
