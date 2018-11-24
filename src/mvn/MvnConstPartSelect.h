﻿#ifndef MVNCONSTPARTSELECT_H
#define MVNCONSTPARTSELECT_H

/// @file MvnConstPartSelect.h
/// @brief MvnConstPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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
  /// @param[in] module 親のモジュール
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  MvnConstPartSelect(MvnModule* module,
		     int msb,
		     int lsb);

  /// @brief デストラクタ
  ~MvnConstPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲指定の MSB
  int mMsb;

  // 範囲指定の LSB
  int mLsb;

};

END_NAMESPACE_YM_MVN

#endif // MVNCONSTPARTSELECT_H
