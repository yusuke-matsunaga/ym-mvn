
/// @file DeclMap.cc
/// @brief DeclMap の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "DeclMap.h"
#include "ym/MvnNode.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

BEGIN_NONAMESPACE

const
bool debug_declmap = false;

END_NONAMESPACE


// @brief 内部のデータをクリアする．
void
DeclMap::clear()
{
  mHash.clear();
  mNum = 0;
}

// @brief 登録する(単一要素の場合)
void
DeclMap::add(
  const VlDecl* decl,
  MvnNode* node
)
{
  if ( debug_declmap ) {
    cerr << "DeclMap::add(" << decl->full_name()
	 << ", Node#" << node->id() << ")"
	 << endl;
  }
  mHash.emplace(make_pair(decl, 0), node);
}

// @brief 登録する(配列の場合)
void
DeclMap::add(
  const VlDeclArray* decl,
  SizeType offset,
  MvnNode* node
)
{
  if ( debug_declmap ) {
    cerr << "DeclMap::add(" << decl->full_name()
	 << "[" << offset << "]"
	 << ", Node#" << node->id() << ")"
	 << endl;
  }
  mHash.emplace(make_pair(decl, offset), node);
}

// @brief 対応するノードを取り出す．
MvnNode*
DeclMap::get(
  const VlDecl* decl
) const
{
  if ( mHash.count(make_pair(decl, 0)) > 0 ) {
    return mHash.at(make_pair(decl, 0));
  }
  return nullptr;
}

// @brief 対応するノードを取り出す(配列型)．
MvnNode*
DeclMap::get(
  const VlDeclArray* decl,
  SizeType offset
) const
{
  if ( mHash.count(make_pair(decl, offset)) > 0 ) {
    return mHash.at(make_pair(decl, offset));
  }
  return nullptr;
}

END_NAMESPACE_YM_MVN_VERILOG
