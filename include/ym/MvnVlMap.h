#ifndef YM_MVVLMAP_H
#define YM_MVVLMAP_H

/// @file ym/MvnVlMap.h
/// @brief MvnVlMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2019, 2021 Yusuke Matsunaga
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
  MvnVlMap(
    const MvnVlMap& src ///< [in] コピー元のオブジェクト
  );

  /// @brief 代入演算子
  MvnVlMap&
  operator=(
    const MvnVlMap& src ///< [in] コピー元のオブジェクト
  );

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
  void
  reg_node(
    SizeType id,       ///< [in] MvNode の ID番号
    const VlDecl* decl ///< [in] 宣言要素
  );

  /// @brief 配列の宣言要素を登録する．
  void
  reg_node(
    SizeType id,                  ///< [in] MvNode の ID番号
    const VlDeclArray* declarray, ///< [in] 配列宣言要素
    SizeType offset               ///< [in] オフセット
  );

  /// @brief src_id の内容を dst_id にムーブする．
  void
  move(
    SizeType src_id, ///< [in] 元のID
    SizeType dst_id  ///< [in] 移動先のID
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief id に対応する宣言要素が単一要素の時に true を返す．
  bool
  is_single_elem(
    SizeType id ///< [in] MvNode の ID番号
  ) const;

  /// @brief id に対応する宣言要素が配列要素の時に true を返す．
  bool
  is_array_elem(
    SizeType id ///< [in] MvNode の ID番号
  ) const;

  /// @brief id に対応する宣言要素を返す．(単一要素版)
  ///
  /// is_single_elem(id) == false の時は nullptr が返される．
  const VlDecl*
  get_single_elem(
    SizeType id ///< [in] MvNode の ID番号
  ) const;

  /// @brief id に対応する宣言要素を返す．(配列要素版)
  ///
  /// is_array_elem(id) == false の時は nullptr が返される．
  const VlDeclArray*
  get_array_elem(
    SizeType id ///< [in] MvNode の ID番号
  ) const;

  /// @brief id に対応する宣言要素のオフセットを返す．(配列要素版)
  ///
  /// is_array_elem(id) == false の時は 0 が返される．
  SizeType
  get_array_offset(
    SizeType id ///< [in] MvNode の ID番号
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を設定する．
  void
  put(
    SizeType id,              ///< [in] ID番号
    unique_ptr<MapRec>&& elem ///< [in] 設定する要素
  );

  /// @brief 要素を取り出す．
  ///
  /// id が範囲外の時は nullptr が返される．
  MapRec*
  get(
    SizeType id ///< [in] ID番号
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNode の ID 番号をキーとして VlDecl/VlDeclArray を保持する配列
  vector<unique_ptr<MapRec>> mArray;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVVLMAP_H
