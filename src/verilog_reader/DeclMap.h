#ifndef DECLMAP_H
#define DECLMAP_H

/// @file DeclMap.h
/// @brief DeclMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mvn.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_STD

template<>
struct hash<pair<const nsYm::VlObj*, int>>
{
  SizeType
  operator()(const pair<const nsYm::VlObj*, int>& src) const
  {
    auto obj = src.first;
    auto offset = src.second;
    return reinterpret_cast<ympuint>(obj) * (offset + 1) + offset;
  }
};

END_NAMESPACE_STD


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class DeclMap DeclMap.h "DeclMap.h"
/// @brief VlDecl と MvnNode の対応付けを行うハッシュ表
//////////////////////////////////////////////////////////////////////
class DeclMap
{
public:

  /// @brief コンストラクタ
  DeclMap() = default;

  /// @brief デストラクタ
  ~DeclMap() = default;


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief 登録する(単一要素の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] node 対応するノード
  void
  add(const VlDecl* decl,
      MvnNode* node);

  /// @brief 登録する(配列の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] offset
  /// @param[in] node 対応するノード
  void
  add(const VlDeclArray* decl,
      int offset,
      MvnNode* node);

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には nullptr を返す．
  MvnNode*
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には nullptr を返す．
  MvnNode*
  get(const VlDeclArray* decl,
      int offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表の本体
  unordered_map<pair<const VlObj*, int>, MvnNode*> mHash;

  // 要素数
  int mNum{0};

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // DECLMAP_H
