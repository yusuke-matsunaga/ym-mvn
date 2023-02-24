
/// @file MvnCellNode.cc
/// @brief MvnCellNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MvnCellNode.h"
#include "ym/MvnMgr.h"
#include "ym/ClibCell.h"
#include "ym/ClibPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnCellNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] cell セル
MvnCellNode::MvnCellNode(
  MvnModule* module,
  const ClibCell& cell
) : MvnNodeBase(module, MvnNodeType::CELL, 0),
    mCell{cell}
{
}

// @brief デストラクタ
MvnCellNode::~MvnCellNode()
{
}

// @brief セル番号を得る．
// @note type() が kCell の時のみ意味を持つ．
ClibCell
MvnCellNode::cell() const
{
  return mCell;
}

// @brief セルの出力ピン番号を返す．
// @note type() が kCell の時のみ意味を持つ．
int
MvnCellNode::cell_opin_pos() const
{
  return 0;
}

// @brief 多出力セルノードの場合の代表ノードを返す．
// @note type() が kCell の時のみ意味を持つ．
// @note 1出力セルノードの時には自分自身を返す．
const MvnNode*
MvnCellNode::cell_node() const
{
  return this;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnExtCellNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] cell_node 代表ノード
// @param[in] opos 出力ピン番号
MvnExtCellNode::MvnExtCellNode(
  MvnModule* module,
  MvnNode* cell_node,
  int opos
) : MvnNodeBase(module, MvnNodeType::CELL, 0),
    mCellNode(cell_node),
    mOpos(opos)
{
}

// @brief デストラクタ
MvnExtCellNode::~MvnExtCellNode()
{
}

// @brief セル番号を得る．
// @note type() が kCell の時のみ意味を持つ．
ClibCell
MvnExtCellNode::cell() const
{
  return mCellNode->cell();
}

// @brief セルの出力ピン番号を返す．
// @note type() が kCell の時のみ意味を持つ．
int
MvnExtCellNode::cell_opin_pos() const
{
  return mOpos;
}

// @brief 多出力セルノードの場合の代表ノードを返す．
// @note type() が kCell の時のみ意味を持つ．
// @note 1出力セルノードの時には自分自身を返す．
const MvnNode*
MvnExtCellNode::cell_node() const
{
  return mCellNode;
}


// @brief セルノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] cell_id セル番号
MvnNode*
MvnMgr::new_cell(
  MvnModule* module,
  const ClibCell& cell
)
{
#if 0
  int ni = 0;
  int no = 0;
  int np = cell->pin_num();
#endif
#if 0 // 2018/03/01 わけわからいのでコメントアウトした
  vector<ymuint32> pin_pos(np);
  for ( int i = 0; i < np; ++ i ) {
    const ClibPin* pin = cell->pin(i);
    if ( pin->is_input() ) {
      pin_pos[i] = (ni << 1);
      ++ ni;
    }
    else if ( pin->is_output() ) {
      pin_pos[i] = (no << 1) + 1;
      ++ no;
    }
    else if ( pin->is_inout() ) {
      cout << cell->name() << " has inout pins." << endl;
      ASSERT_NOT_REACHED;
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
#endif

  MvnCellNode* node = new MvnCellNode(module, cell);
  reg_node(node);

#if 0
  for ( int i = 0; i < ni; ++ i ) {
    node->_input(i)->mBitWidth = 1;
  }
#endif
  node->mBitWidth = 1;

  return node;
}

END_NAMESPACE_YM_MVN
