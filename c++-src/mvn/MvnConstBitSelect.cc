﻿
/// @file MvnConstBitSelect.cc
/// @brief MvnConstBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MvnConstBitSelect.h"
#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bitpos ビット位置
MvnConstBitSelect::MvnConstBitSelect(
  MvnModule* module,
  SizeType bitpos
) : MvnNodeBase(module, MvnNodeType::CONSTBITSELECT, 1),
    mBitPos{bitpos}
{
}

// @brief デストラクタ
MvnConstBitSelect::~MvnConstBitSelect()
{
}

// @brief ビット位置を得る．
SizeType
MvnConstBitSelect::bitpos() const
{
  return mBitPos;
}

// @brief bit-selectノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bitpos ビット位置
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_constbitselect(
  MvnModule* module,
  SizeType bitpos,
  SizeType bit_width
)
{
  auto node{new MvnConstBitSelect(module, bitpos)};
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width;
  node->mBitWidth = 1;

  return node;
}

END_NAMESPACE_YM_MVN
