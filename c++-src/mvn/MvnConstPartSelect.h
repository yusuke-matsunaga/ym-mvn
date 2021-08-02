#ifndef MVNCONSTPARTSELECT_H
#define MVNCONSTPARTSELECT_H

/// @file MvnConstPartSelect.h
/// @brief MvnConstPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnConstPartSelect MvnConstPartSelect.h "MvnConstPartSelect.h"
/// @brief 定数ビット指定を表すノード
//////////////////////////////////////////////////////////////////////
class MvnConstPartSelect :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnConstPartSelect(
    MvnModule* module, ///< [in] 親のモジュール
    SizeType msb,      ///< [in] 範囲指定の MSB
    SizeType lsb       ///< [in] 範囲指定の LSB
  );

  /// @brief デストラクタ
  ~MvnConstPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定の MSB を得る．
  SizeType
  msb() const override;

  /// @brief 範囲指定の LSB を得る．
  SizeType
  lsb() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲指定の MSB
  SizeType mMsb;

  // 範囲指定の LSB
  SizeType mLsb;

};

END_NAMESPACE_YM_MVN

#endif // MVNCONSTPARTSELECT_H
