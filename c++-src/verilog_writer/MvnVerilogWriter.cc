
/// @file MvnVerilogWriter.cc
/// @brief MvnVerilogWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MvnVerilogWriter.h"
#include "VerilogWriterImpl.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnVerilogWriter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnVerilogWriter::MvnVerilogWriter(
) : mImpl{unique_ptr<VerilogWriterImpl>{new VerilogWriterImpl()}}
{
}

// @brief デストラクタ
MvnVerilogWriter::~MvnVerilogWriter()
{
  // VerilogWriteImpl.h を必要とするため
  // ヘッダ中で = default 宣言はできない．
}

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
void
MvnVerilogWriter::operator()(
  ostream& s,
  const MvnMgr& mgr
)
{
  mImpl->dump(s, mgr);
}

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
// @param[in] node_map ノードと Verilog 名の対応表
void
MvnVerilogWriter::operator()(
  ostream& s,
  const MvnMgr& mgr,
  const MvnVlMap& node_map
)
{
  mImpl->dump(s, mgr, node_map);
}

END_NAMESPACE_YM_MVN
