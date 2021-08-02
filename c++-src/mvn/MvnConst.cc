
/// @file MvnConst.cc
/// @brief MvnConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "MvnConst.h"
#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] val 値
MvnConst::MvnConst(
  MvnModule* module,
  const MvnBvConst& val
) : MvnNodeBase(module, MvnNodeType::CONSTVALUE, 0),
    mVal{val}
{
}

// @brief デストラクタ
MvnConst::~MvnConst()
{
}

// @brief 定数値を得る．
// @note type() が kConst の時のみ意味を持つ．
MvnBvConst
MvnConst::const_value() const
{
  return mVal;
}

// @brief constant ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] val 値
MvnNode*
MvnMgr::new_const(
  MvnModule* module,
  const MvnBvConst& val
)
{
  auto node = new MvnConst(module, val);
  reg_node(node);

  node->mBitWidth = val.size();

  return node;
}

END_NAMESPACE_YM_MVN
