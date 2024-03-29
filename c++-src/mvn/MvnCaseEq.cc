﻿
/// @file MvnCaseEq.cc
/// @brief MvnCaseEq の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MvnCaseEq.h"
#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] val 値
MvnCaseEq::MvnCaseEq(
  MvnModule* module,
  const MvnBvConst& x_val
) : MvnNodeBase(module, MvnNodeType::CASEEQ, 2),
    mXmask{x_val}
{
}

// @brief デストラクタ
MvnCaseEq::~MvnCaseEq()
{
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kEqX の時のみ意味を持つ．
MvnBvConst
MvnCaseEq::xmask() const
{
  return mXmask;
}

// @brief eqx ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] xmask Xマスク値
MvnNode*
MvnMgr::new_caseeq(
  MvnModule* module,
  SizeType bit_width,
  const MvnBvConst& xmask
)
{
  ASSERT_COND( xmask.size() == bit_width );
  bool has_x = false;
  for ( SizeType i = 0; i < bit_width; ++ i ) {
    if ( xmask[i] ) {
      has_x = true;
      break;
    }
  }
  if ( has_x ) {
    auto node = new MvnCaseEq(module, xmask);
    reg_node(node);

    node->_input(0)->mBitWidth = bit_width;
    node->_input(1)->mBitWidth = bit_width;
    node->mBitWidth = 1;

    return node;
  }
  else {
    return new_equal(module, bit_width);
  }
}

END_NAMESPACE_YM_MVN
