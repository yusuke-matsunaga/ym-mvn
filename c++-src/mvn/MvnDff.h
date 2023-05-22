#ifndef MVNDFF_H
#define MVNDFF_H

/// @file MvnDff.h
/// @brief MvnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
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
  MvnDff(
    MvnModule* module,                    ///< [in] 親のモジュール
    MvnPolarity clock_pol,                ///< [in] クロックの極性
    const vector<MvnPolarity>& pol_array, ///< [in] 非同期セット信号の極性情報を入れた配列
    const vector<MvnNode*>& val_array     ///< [in] 非同期セットの値を入れた配列
  );

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
  MvnPolarity
  control_pol(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() - 2 )
  ) const override;

  /// @brief 非同期セットの値を表す定数ノードを得る．
  const MvnNode*
  control_val(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() - 2 )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // クロックと非同期セット入力の極性の配列
  std::uint32_t* mPolArray;

  // 非同期セットの値を表すノードの配列
  MvnNode** mValArray;

};

END_NAMESPACE_YM_MVN

#endif // MVNDFF_H
