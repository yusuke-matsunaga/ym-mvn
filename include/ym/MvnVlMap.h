﻿#ifndef YM_MVVLMAP_H
#define YM_MVVLMAP_H

/// @file ym/MvnVlMap.h
/// @brief MvnVlMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mvn.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_MVN

class MapRec;

//////////////////////////////////////////////////////////////////////
/// @class MvnVlMap MvnVlMap.h "ym/MvnVlMap.h"
/// @brief MvNode と VlDecl/VlDeclArray の対応を記録するクラス
//////////////////////////////////////////////////////////////////////
class MvnVlMap
{
public:

  /// @brief コンストラクタ
  MvnVlMap();

  /// @brief コピーコンストラクタ
  MvnVlMap(const MvnVlMap& src);

  /// @brief 代入演算子
  const MvnVlMap&
  operator=(const MvnVlMap& src);

  /// @brief デストラクタ
  ~MvnVlMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 単一の宣言要素を登録する．
  /// @param[in] id MvNode の ID番号
  /// @param[in] decl 宣言要素
  void
  reg_node(int id,
	   const VlDecl* decl);

  /// @brief 配列の宣言要素を登録する．
  /// @param[in] id MvNode の ID番号
  /// @param[in] declarray 配列宣言要素
  /// @param[in] offset オフセット
  void
  reg_node(int id,
	   const VlDeclArray* declarray,
	   int offset);

  /// @brief src_id の内容を dst_id にコピーする．
  void
  copy(int src_id,
       int dst_id);


public:
  //////////////////////////////////////////////////////////////////////
  // 取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief id に対応する宣言要素が単一要素の時に true を返す．
  /// @param[in] id MvNode の ID番号
  bool
  is_single_elem(int id) const;

  /// @brief id に対応する宣言要素が配列要素の時に true を返す．
  /// @param[in] id MvNode の ID番号
  bool
  is_array_elem(int id) const;

  /// @brief id に対応する宣言要素を返す．(単一要素版)
  /// @param[in] id MvNode の ID番号
  /// @note is_single_elem(id) == false の時は nullptr が返される．
  const VlDecl*
  get_single_elem(int id) const;

  /// @brief id に対応する宣言要素を返す．(配列要素版)
  /// @param[in] id MvNode の ID番号
  /// @note is_array_elem(id) == false の時は nullptr が返される．
  const VlDeclArray*
  get_array_elem(int id) const;

  /// @brief id に対応する宣言要素のオフセットを返す．(配列要素版)
  /// @param[in] id MvNode の ID番号
  /// @note is_array_elem(id) == false の時は 0 が返される．
  int
  get_array_offset(int id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を設定する．
  /// @param[in] id ID番号
  /// @param[in] elem 設定する要素
  void
  put(int id,
      MapRec* elem);

  /// @brief 要素を取り出す．
  /// @param[in] id ID番号
  /// @note id が範囲外の時は nullptr が返される．
  MapRec*
  get(int id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNode の ID 番号をキーとして VlDecl/VlDeclArray を保持する配列
  vector<MapRec*> mArray;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVVLMAP_H
