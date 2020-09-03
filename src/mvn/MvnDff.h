#ifndef MVNDFF_H
#define MVNDFF_H

/// @file MvnDff.h
/// @brief MvnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"
#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnDff MvnDff.h "MvnDff.h"
/// @brief D-FF を表すクラス
//////////////////////////////////////////////////////////////////////
class MvnDff :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] clock_pol クロックの極性
  /// @param[in] pol_array 非同期セット信号の極性情報を入れた配列
  /// @param[in] val_array 非同期セットの値を入れた配列
  MvnDff(MvnModule* module,
	 MvnPolarity clock_pol,
	 const vector<MvnPolarity>& pol_array,
	 const vector<MvnNode*>& val_array);

  /// @brief デストラクタ
  ~MvnDff();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クロック信号の極性を得る．
  MvnPolarity
  clock_pol() const override;

  /// @brief 非同期セット信号の極性を得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  MvnPolarity
  control_pol(int pos) const override;

  /// @brief 非同期セットの値を表す定数ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @note デフォルトの実装では nullptr を返す．
  const MvnNode*
  control_val(int pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // クロックと非同期セット入力の極性の配列
  ymuint32* mPolArray;

  // 非同期セットの値を表すノードの配列
  MvnNode** mValArray;

};

END_NAMESPACE_YM_MVN

#endif // MVNDFF_H
