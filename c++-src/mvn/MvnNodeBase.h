﻿#ifndef MVNNODEBASE_H
#define MVNNODEBASE_H

/// @file MvnNodeBase.h
/// @brief MvnNodeBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnNodeBase MvnNodeBase.h "MvnNodeBase.h"
/// @brief MvnNode の実装用基底クラス
//////////////////////////////////////////////////////////////////////
class MvnNodeBase :
  public MvnNode
{
  friend class MvnMgr;

protected:

  /// @brief コンストラクタ
  MvnNodeBase(
    MvnModule* parent, ///< [in] 親のモジュール
    MvnNodeType type,  ///< [in] 型
    SizeType input_num ///< [in] 入力数
  );

  /// @brief デストラクタ
  ~MvnNodeBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を得る．
  MvnNodeType
  type() const override;

  /// @brief 入力ピン数を得る．
  SizeType
  input_num() const override;

  /// @brief 入力ピンを得る．
  const MvnInputPin*
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const override;

  /// @brief クロック信号の極性を得る．
  /// @retval MvnPolarity::Positive 正極性(posedge)
  /// @retval MvnPolarity::Negative 負極性(negedge)
  ///
  /// デフォルトの実装では 0 を返す．
  MvnPolarity
  clock_pol() const override;

  /// @brief 非同期セット信号の極性を得る．
  /// @retval MvnPolarity::Positive 正極性(posedge)
  /// @retval MvnPolarity::Negative 負極性(negedge)
  ///
  /// デフォルトの実装では 0 を返す．
  MvnPolarity
  control_pol(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() - 2 )
  ) const override;

  /// @brief 非同期セットの値を表す定数ノードを得る．
  ///
  /// デフォルトの実装では nullptr を返す．
  const MvnNode*
  control_val(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() - 2 )
  ) const override;

  /// @brief ビット位置を得る．
  ///
  /// @note デフォルトの実装では 0 を返す．
  SizeType
  bitpos() const override;

  /// @brief 範囲指定の MSB を得る．
  ///
  /// デフォルトの実装では 0 を返す．
  SizeType
  msb() const override;

  /// @brief 範囲指定の LSB を得る．
  ///
  /// デフォルトの実装では 0 を返す．
  SizeType
  lsb() const override;

  /// @brief 定数値を得る．
  /// @return 値を返す．
  ///
  /// type() が kConst の時のみ意味を持つ．
  /// デフォルトの実装ではなにもしない．
  MvnBvConst
  const_value() const override;

  /// @brief Xマスクを得る．
  /// @return Xマスクを表す定数を返す．
  ///
  /// type() が kCaseEq の時のみ意味を持つ．
  /// デフォルトの実装ではなにもしない．
  MvnBvConst
  xmask() const override;

  /// @brief セル番号を得る．
  ///
  /// type() が kCell の時のみ意味を持つ．
  /// デフォルトの実装では不正値を返す．
  ClibCell
  cell() const override;

  /// @brief セルの出力ピン番号を返す．
  ///
  /// type() が kCell の時のみ意味を持つ．
  /// デフォルトの実装では 0 を返す．
  int
  cell_opin_pos() const override;

  /// @brief 多出力セルノードの場合の代表ノードを返す．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note 1出力セルノードの時には自分自身を返す．
  /// @note デフォルトの実装では nullptr を返す．
  const MvnNode*
  cell_node() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  MvnInputPin*
  _input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  MvnNodeType mType;

  // 入力数
  SizeType mInputNum;

  // 入力ピンの配列
  MvnInputPin* mInputArray;

};

END_NAMESPACE_YM_MVN

#endif // MVNNODEBASE_H
