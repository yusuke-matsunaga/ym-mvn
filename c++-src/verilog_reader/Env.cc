
/// @file Env.cc
/// @brief Env の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "Env.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

#if 0
// @brief 登録する(単一要素の場合)
// @param[in] decl 宣言要素
// @param[in] node 対応するノード
void
Env::add(
  const VlDecl* decl,
  MvnNode* node
)
{
  SizeType id = mDeclHash.get_id(decl);
  add_by_id(id, node);
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 対応するノード
void
Env::add(
  const VlDeclArray* decl,
  SizeType offset,
  MvnNode* node
)
{
  SizeType id = mDeclHash.get_id(decl, offset);
  add_by_id(id, node);
}
#endif

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には nullptr を返す．
MvnNode*
Env::get(
  const VlDecl* decl
) const
{
  SizeType id = mDeclHash.get_id(decl);
  return get_from_id(id);
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には nullptr を返す．
MvnNode*
Env::get(
  const VlDeclArray* decl,
  SizeType offset
) const
{
  SizeType id = mDeclHash.get_id(decl, offset);
  return get_from_id(id);
}


//////////////////////////////////////////////////////////////////////
// クラス ProcEnv
//////////////////////////////////////////////////////////////////////

// @brief 登録する(単一要素の場合)
void
ProcEnv::add(
  const VlDecl* decl,
  MvnNode* node,
  bool block
)
{
  SizeType id = decl_hash().get_id(decl);
  add_by_id(id, node, nullptr, block);
}

// @brief 登録する(配列の場合)
void
ProcEnv::add(
  const VlDeclArray* decl,
  SizeType offset,
  MvnNode* node,
  bool block
)
{
  SizeType id = decl_hash().get_id(decl, offset);
  add_by_id(id, node, nullptr, block);
}

// @brief 対応するノードを取り出す．
MvnNode*
ProcEnv::get(
  const VlDecl* decl
) const
{
  SizeType id = decl_hash().get_id(decl);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs != nullptr &&
       ans.mBlock == true &&
       ans.mCond == nullptr ) {
    return ans.mRhs;
  }
  return mGlobalEnv.get_from_id(id);
}

// @brief 対応するノードを取り出す(配列型)．
MvnNode*
ProcEnv::get(
  const VlDeclArray* decl,
  SizeType offset
) const
{
  SizeType id = decl_hash().get_id(decl, offset);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs != nullptr &&
       ans.mBlock == true &&
       ans.mCond == nullptr ) {
    return ans.mRhs;
  }
  return mGlobalEnv.get_from_id(id);
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には nullptr を返す．
AssignInfo
ProcEnv::get_info(
  const VlDecl* decl
) const
{
  SizeType id = decl_hash().get_id(decl);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs == nullptr ) {
    MvnNode* node = mGlobalEnv.get(decl);
    return AssignInfo{node};
  }
  return ans;
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には nullptr を返す．
AssignInfo
ProcEnv::get_info(
  const VlDeclArray* decl,
  SizeType offset
) const
{
  SizeType id = decl_hash().get_id(decl, offset);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs == nullptr ) {
    MvnNode* node = mGlobalEnv.get(decl, offset);
    return AssignInfo{node};
  }
  return ans;
}

// @brief ID番号に対応するノードを登録する．
// @param[in] id ID番号
// @param[in] node 代入式の右辺
// @param[in] cond 代入条件
// @param[in] block blocking 代入の時に true とするフラグ
void
ProcEnv::add_by_id(
  SizeType id,
  MvnNode* node,
  MvnNode* cond,
  bool block
)
{
  while ( mAIArray.size() <= id ) {
    mAIArray.push_back(AssignInfo());
  }
  mAIArray[id] = AssignInfo{node, cond, block};
}

// @brief ID番号に対応するノードを取り出す．
AssignInfo
ProcEnv::get_from_id(
  SizeType id
) const
{
  if ( mAIArray.size() <= id ) {
    return AssignInfo();
  }
  return mAIArray[id];
}

END_NAMESPACE_YM_MVN_VERILOG
