﻿#ifndef MVNCONSTBITSELECT_H
#define MVNCONSTBITSELECT_H

/// @file MvnConstBitSelect.h
/// @brief MvnConstBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnConstBitSelect MvnConstBitSelect.h "MvnConstBitSelect.h"
/// @brief 定数ビット指定を表すノード
//////////////////////////////////////////////////////////////////////
class MvnConstBitSelect :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bitpos ビット位置
  MvnConstBitSelect(MvnModule* module,
		    SizeType bitpos);

  /// @brief デストラクタ
  ~MvnConstBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット位置を得る．
  /// @note type() が kConstBitSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  SizeType
  bitpos() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット位置
  SizeType mBitPos;

};

END_NAMESPACE_YM_MVN

#endif // MVNCONSTBITSELECT_H
