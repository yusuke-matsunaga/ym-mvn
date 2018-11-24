#ifndef MVNNODEBASE_H
#define MVNNODEBASE_H

/// @file MvnNodeBase.h
/// @brief MvnNodeBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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
  /// @param[in] module 親のモジュール
  /// @param[in] type 型
  /// @param[in] input_num 入力数
  MvnNodeBase(MvnModule* parent,
	      tType type,
	      int input_num);

  /// @brief デストラクタ
  ~MvnNodeBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を得る．
  tType
  type() const override;

  /// @brief 入力ピン数を得る．
  int
  input_num() const override;

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  const MvnInputPin*
  input(int pos) const override;

  /// @brief クロック信号の極性を得る．
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  int
  clock_pol() const override;

  /// @brief 非同期セット信号の極性を得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  int
  control_pol(int pos) const override;

  /// @brief 非同期セットの値を表す定数ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @note デフォルトの実装では nullptr を返す．
  const MvnNode*
  control_val(int pos) const override;

  /// @brief ビット位置を得る．
  /// @note type() が kConstBitSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  int
  bitpos() const override;

  /// @brief 範囲指定の MSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  int
  msb() const override;

  /// @brief 範囲指定の LSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  int
  lsb() const override;

  /// @brief 定数値を得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kConst の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  void
  const_value(vector<ymuint32>& val) const override;

  /// @brief Xマスクを得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kCaseEq の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  void
  xmask(vector<ymuint32>& val) const override;

  /// @brief セル番号を得る．
  /// @note type() が kCell の時のみ意味を持つ．
  ///
  /// デフォルトの実装では -1 を返す．
  int
  cell_id() const override;

  /// @brief セルの出力ピン番号を返す．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
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
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  MvnInputPin*
  _input(int pos) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  tType mType;

  // 入力数
  int mInputNum;

  // 入力ピンの配列
  MvnInputPin* mInputArray;

};

END_NAMESPACE_YM_MVN

#endif // MVNNODEBASE_H
