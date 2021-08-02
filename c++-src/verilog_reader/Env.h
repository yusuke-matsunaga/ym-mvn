#ifndef ENV_H
#define ENV_H

/// @file Env.h
/// @brief Env のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"
#include "ym/vl/VlFwd.h"
#include "DeclHash.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AssignInfo Env.h "Env.h"
/// @brief 代入情報を表すクラス
///
/// 具体的には代入の右辺と代入条件を表すノードの組
//////////////////////////////////////////////////////////////////////
struct AssignInfo
{

  MvnNode* mRhs{nullptr}; ///< 代入の右辺式を表すノード

  MvnNode* mCond{nullptr}; ///< 代入条件を表すノード
                           ///< ただし常に代入する時は nullptr

  bool mBlock{false};      ///< blocking 代入を表すフラグ

  bool mRefFlag{false};    ///< 参照されたことを表すフラグ

};


//////////////////////////////////////////////////////////////////////
/// @class Env Env.h "Env.h"
/// @brief 基本的には const VlDecl と MvnNode の対応をとる連想配列
//////////////////////////////////////////////////////////////////////
class Env
{
public:

  /// @brief コンストラクタ
  explicit
  Env(
    DeclHash& decl_hash ///< [in] 宣言要素のハッシュ表
  ) : mDeclHash{decl_hash},
      mNodeArray(decl_hash.max_id())
  {
  }

  /// @brief コピーコンストラクタ
  Env(
    const Env& src ///< [in] もとのオブジェクト
  ) = default;

  /// @brief デストラクタ
  virtual
  ~Env() = default;


public:

  /// @brief 内容をクリアする．
  void
  clear()
  {
    mNodeArray.clear();
    mNodeArray.resize(max_id(), 0);
  }

  /// @brief ID番号の最大値+1を返す．
  SizeType
  max_id() const
  {
    return mDeclHash.max_id();
  }

  /// @brief 登録する(単一要素の場合)
  void
  add(
    const VlDecl* decl, ///< [in] 宣言要素
    MvnNode* node       ///< [in] 対応するノード
  )
  {
    SizeType id = mDeclHash.get_id(decl);
    add_by_id(id, node);
  }

  /// @brief 登録する(配列の場合)
  void
  add(
    const VlDeclArray* decl, ///< [in] 宣言要素
    SizeType offset,         ///< [in] オフセット
    MvnNode* node            ///< [in] 対応するノード
  )
  {
    SizeType id = mDeclHash.get_id(decl, offset);
    add_by_id(id, node);
  }

  /// @brief 対応するノードを取り出す．
  /// @return 対応するノードを返す．
  ///
  /// 登録されていない場合と配列型の場合には nullptr を返す．
  virtual
  MvnNode*
  get(
    const VlDecl* decl ///< [in] 宣言要素
  ) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @return 対応するノードを返す．
  ///
  /// 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には nullptr を返す．
  virtual
  MvnNode*
  get(
    const VlDeclArray* decl, ///< [in] 宣言要素
    SizeType offset          ///< [in] オフセット
  ) const;

  /// @brief ID番号に対応するノードを登録する．
  void
  add_by_id(
    SizeType id,  ///< [in] ID番号
    MvnNode* node ///< [in] 対応するノード
  )
  {
    while ( mNodeArray.size() <= id ) {
      mNodeArray.push_back(nullptr);
    }
    mNodeArray[id] = node;
  }

  /// @brief ID番号に対応するノードを取り出す．
  MvnNode*
  get_from_id(
    SizeType id ///< [in] ID番号
  ) const
  {
    if ( mNodeArray.size() <= id ) {
      return nullptr;
    }
    return mNodeArray[id];
  }

  /// @brief DeclHash を得る．
  DeclHash&
  decl_hash() const
  {
    return mDeclHash;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // VlDecl 用のハッシュ表
  DeclHash& mDeclHash;

  // VlDecl の ID をキーに MvnNode の配列を格納する配列
  vector<MvnNode*> mNodeArray;

};


//////////////////////////////////////////////////////////////////////
/// @class ProcEnv Env.h "Env.h"
/// @brief プロセス内部の Env
//////////////////////////////////////////////////////////////////////
class ProcEnv :
  public Env
{
public:

  /// @brief コンストラクタ
  explicit
  ProcEnv(
    const Env& global_env ///< [in] プロセスの外側の Env
  ) : Env(global_env.decl_hash()),
      mGlobalEnv{global_env},
      mAIArray(global_env.max_id())
  {
  }

  /// @brief コピーコンストラクタ
  ProcEnv(
    const ProcEnv& tmp_env ///< [in] 元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~ProcEnv() = default;


public:

  /// @brief 内容をクリアする．
  void
  clear()
  {
    Env::clear();
    mAIArray.clear();
    mAIArray.resize(max_id());
  }

  /// @brief 登録する(単一要素の場合)
  void
  add(
    const VlDecl* decl, ///< [in] 宣言要素
    MvnNode* node,      ///< [in] 代入式の右辺
    bool block          ///< [in] 代入の時に true とするフラグ
  );

  /// @brief 登録する(配列の場合)
  void
  add(
    const VlDeclArray* decl, ///< [in] 宣言要素
    SizeType offset,         ///< [in] オフセット
    MvnNode* node,           ///< [in] 代入式の右辺
    bool block               ///< [in] 代入の時に true とするフラグ
  );

  /// @brief 対応するノードを取り出す．
  /// @return 対応するノードを返す．
  ///
  /// 登録されていない場合と配列型の場合には nullptr を返す．
  MvnNode*
  get(
    const VlDecl* decl ///< [in] 宣言要素
  ) const override;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @return 対応するノードを返す．
  ///
  /// 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には nullptr を返す．
  MvnNode*
  get(
    const VlDeclArray* decl, ///< [in] 宣言要素
    SizeType offset          ///< [in] オフセット
  ) const override;

  /// @brief 対応するノードを取り出す．
  /// @return 対応するノードを返す．
  ///
  /// 登録されていない場合と配列型の場合には nullptr を返す．
  AssignInfo
  get_info(
    const VlDecl* decl ///< [in] 宣言要素
  ) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @return 対応するノードを返す．
  ///
  /// 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には nullptr を返す．
  AssignInfo
  get_info(
    const VlDeclArray* decl, ///< [in] 宣言要素
    SizeType offset          ///< [in] オフセット
  ) const;

  /// @brief ID番号に対応するノードを登録する．
  void
  add_by_id(
    SizeType id,   ///< [in] ID番号
    MvnNode* node, ///< [in] 代入式の右辺
    MvnNode* cond, ///< [in] 代入条件
    bool block     ///< [in] blocking 代入の時に true とするフラグ
  );

  /// @brief ID番号に対応するノードを取り出す．
  AssignInfo
  get_from_id(
    SizeType id ///< [in] ID番号
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 外側の Env
  const Env& mGlobalEnv;

  // VlDecl の ID をキーに AssignInfo の配列を格納する配列
  vector<AssignInfo> mAIArray;

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // ENV_H
