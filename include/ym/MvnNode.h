#ifndef YM_MVNNODE_H
#define YM_MVNNODE_H

/// @file ym/MvnNode.h
/// @brief MvnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"
#include "ym/clib.h"
#include "ym/MvnInputPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnNode MvnNode.h "ym/MvnNode.h"
/// @brief MvNetwork のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnNode
{
  friend class MvnMgr;

protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnNode(
    MvnModule* parent ///< [in] 親のモジュール
  ) : mParent{parent}
  {
  }

  /// @brief デストラクタ
  virtual
  ~MvnNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  int
  id() const
  {
    return mId;
  }

  /// @brief 親のモジュールを得る．
  const MvnModule*
  parent() const
  {
    return mParent;
  }

  /// @brief ノードの種類を得る．
  virtual
  MvnNodeType
  type() const = 0;

  /// @brief 入力ピン数を得る．
  virtual
  SizeType
  input_num() const = 0;

  /// @brief 入力ピンを得る．
  virtual
  const MvnInputPin*
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const = 0;

  /// @brief 出力のビット幅を得る．
  SizeType
  bit_width() const
  {
    return mBitWidth;
  }

  /// @brief 出力に接続している入力ピンのリストを得る．
  const vector<MvnInputPin*>&
  dst_pin_list() const
  {
    return mDstPinList;
  }

  /// @brief クロック信号の極性を得る．
  /// @retval MvnPolarity::Positive 正極性(posedge)
  /// @retval MvnPolarity::Negative 負極性(negedge)
  ///
  /// type() が DFF の時のみ意味を持つ．
  virtual
  MvnPolarity
  clock_pol() const = 0;

  /// @brief 非同期セット信号の極性を得る．
  /// @retval MvnPolarity::Positive 正極性(posedge)
  /// @retval MvnPolarity::Negative 負極性(negedge)
  ///
  /// type() が DFF の時のみ意味を持つ．
  virtual
  MvnPolarity
  control_pol(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() - 2 )
  ) const = 0;

  /// @brief 非同期セットの値を表す定数ノードを得る．
  virtual
  const MvnNode*
  control_val(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() - 2 )
  ) const = 0;

  /// @brief ビット位置を得る．
  ///
  /// type() が CONSTBITSELECT の時のみ意味を持つ．
  virtual
  SizeType
  bitpos() const = 0;

  /// @brief 範囲指定の MSB を得る．
  ///
  /// type() が CONSTPARTSELECT の時のみ意味を持つ．
  virtual
  SizeType
  msb() const = 0;

  /// @brief 範囲指定の LSB を得る．
  ///
  /// type() が CONSTPARTSELECT の時のみ意味を持つ．
  virtual
  SizeType
  lsb() const = 0;

  /// @brief 定数値を得る．
  /// @return 定数値を返す．
  ///
  /// type() が CONST の時のみ意味を持つ．
  virtual
  MvnBvConst
  const_value() const = 0;

  /// @brief Xマスクを得る．
  /// @return Xマスクを表す低数値を返す．
  ///
  /// type() が CASEEQ の時のみ意味を持つ．
  virtual
  MvnBvConst
  xmask() const = 0;

  /// @brief セルを得る．
  ///
  /// type() が CELL の時のみ意味を持つ．
  /// デフォルトの実装では不正値を返す．
  virtual
  ClibCell
  cell() const = 0;

  /// @brief セルの出力ピン番号を返す．
  ///
  /// type() が CELL の時のみ意味を持つ．
  /// デフォルトの実装では 0 を返す．
  virtual
  int
  cell_opin_pos() const = 0;

  /// @brief 多出力セルノードの場合の代表ノードを返す．
  ///
  /// type() が CELL の時のみ意味を持つ．
  /// 1出力セルノードの時には自分自身を返す．
  /// デフォルトの実装では nullptr を返す．
  virtual
  const MvnNode*
  cell_node() const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  virtual
  MvnInputPin*
  _input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  int mId;

  // 親のモジュール
  MvnModule* mParent;

  // 出力のビット幅
  SizeType mBitWidth;

  // ファンアウト先の入力ピンのリスト
  vector<MvnInputPin*> mDstPinList;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVNNODE_H
