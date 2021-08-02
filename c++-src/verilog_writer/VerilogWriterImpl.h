#ifndef VERILOGWRITERIMPL_H
#define VERILOGWRITERIMPL_H

/// @file VerilogWriterImpl.h
/// @brief VerilogWriterImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class VerilogWriterImpl VerilogWriterImpl.h
/// @brief MvnVerilogWriter の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class VerilogWriterImpl
{
public:

  /// @brief コンストラクタ
  VerilogWriterImpl() = default;

  /// @brief デストラクタ
  ~VerilogWriterImpl() = default;


public:

  /// @brief 内容を出力する．
  void
  dump(
    ostream& s,       ///< [in] 出力先のストリーム
    const MvnMgr& mgr ///< [in] MvnMgr
  );

  /// @brief 内容を出力する．
  void
  dump(
    ostream& s,              ///< [in] 出力先のストリーム
    const MvnMgr& mgr,       ///< [in] MvnMgr
    const MvnVlMap& node_map ///< [in] ノードと Verilog 名の対応表
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  void
  dump_module(
    ostream& s,
    const MvnModule* module,
    const MvnMgr& mgr
  );

  void
  dump_port(
    ostream& s,
    const MvnPort* port
  );

  void
  dump_port_ref(
    ostream& s,
    const MvnPortRef& port_ref
  );

  void
  dump_node(
    ostream& s,
    const MvnNode* node,
    const MvnMgr& mgr
  );

  void
  dump_uop(
    ostream& s,
    const MvnNode* node,
    const char* opr_str
  );

  void
  dump_binop(
    ostream& s,
    const MvnNode* node,
    const char* opr_str,
    bool need_paren = false
  );

  void
  dump_nop(
    ostream& s,
    const MvnNode* node,
    const char* opr_str
  );

  string
  node_name(
    const MvnNode* node
  );

  void
  set_node_name(
    const MvnNode* node,
    const string& name
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのID をキーにして名前を格納する配列
  vector<string> mNameArray;

};

END_NAMESPACE_YM_MVN

#endif // VERILOGWRITERIMPL_H
