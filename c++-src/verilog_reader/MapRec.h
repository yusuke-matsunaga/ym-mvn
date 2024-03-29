﻿#ifndef MAPREC_H
#define MAPREC_H

/// @file MapRec.h
/// @brief MapRec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MapRec MapRec.h "MapRec.h"
/// @brief VlDecl か VlDeclArray + offset の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class MapRec
{
public:

  /// @brief デストラクタ
  virtual
  ~MapRec() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素が単一要素の時に true を返す．
  virtual
  bool
  is_single_elem() const = 0;

  /// @brief 宣言要素が配列要素の時に true を返す．
  virtual
  bool
  is_array_elem() const = 0;

  /// @brief 宣言要素を返す．(単一要素版)
  /// @note is_single_elem() == false の時は nullptr が返される．
  virtual
  const VlDecl*
  get_single_elem() const = 0;

  /// @brief 宣言要素を返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は nullptr が返される．
  virtual
  const VlDeclArray*
  get_array_elem() const = 0;

  /// @brief 宣言要素のオフセットを返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は 0 が返される．
  virtual
  int
  get_array_offset() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class SingleMapRec MapRec.h "MapRec.h"
/// @brief 単一要素用の MapRec
//////////////////////////////////////////////////////////////////////
class SingleMapRec :
  public MapRec
{
  friend class MvnVlMap;

private:

  /// @brief コンストラクタ
  SingleMapRec(
    const VlDecl* decl ///< [in] 単一の宣言要素
  ) : mDecl{decl}
  {
  }

  /// @brief デストラクタ
  ~SingleMapRec() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MapRec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素が単一要素の時に true を返す．
  bool
  is_single_elem() const override;

  /// @brief 宣言要素が配列要素の時に true を返す．
  bool
  is_array_elem() const override;

  /// @brief 宣言要素を返す．(単一要素版)
  /// @note is_single_elem() == false の時は nullptr が返される．
  const VlDecl*
  get_single_elem() const override;

  /// @brief 宣言要素を返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は nullptr が返される．
  const VlDeclArray*
  get_array_elem() const override;

  /// @brief 宣言要素のオフセットを返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は 0 が返される．
  int
  get_array_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言要素
  const VlDecl* mDecl;

};


//////////////////////////////////////////////////////////////////////
/// @class ArrayMapRec MapRec.h "MapRec.h"
/// @brief 配列要素用の MapRec
//////////////////////////////////////////////////////////////////////
class ArrayMapRec :
  public MapRec
{
  friend class MvnVlMap;

private:

  /// @brief コンストラクタ
  ArrayMapRec(
    const VlDeclArray* declarray, ///< [in] 配列要素
    SizeType offset               ///< [in] オフセット
  ) : mDeclArray(declarray),
      mOffset(offset)
  {
  }

  /// @brief デストラクタ
  ~ArrayMapRec() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MapRec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素が単一要素の時に true を返す．
  bool
  is_single_elem() const override;

  /// @brief 宣言要素が配列要素の時に true を返す．
  bool
  is_array_elem() const override;

  /// @brief 宣言要素を返す．(単一要素版)
  /// @note is_single_elem() == false の時は nullptr が返される．
  const VlDecl*
  get_single_elem() const override;

  /// @brief 宣言要素を返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は nullptr が返される．
  const VlDeclArray*
  get_array_elem() const override;

  /// @brief 宣言要素のオフセットを返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は 0 が返される．
  int
  get_array_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言要素
  const VlDeclArray* mDeclArray;

  // オフセット
  SizeType mOffset;

};

END_NAMESPACE_YM_MVN

#endif // MAPREC_H
