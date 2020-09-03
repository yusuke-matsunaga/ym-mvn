#ifndef MVNCASEEQ_H
#define MVNCASEEQ_H

/// @file MvnCaseEq.h
/// @brief MvnCaseEq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"
#include "ym/MvnBvConst.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnCaseEq MvnCaseEq.h "MvnCaseEq.h"
/// @brief Xマスク付きの等価比較ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnCaseEq :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] val 値
  MvnCaseEq(MvnModule* module,
	    const MvnBvConst& val);

  /// @brief デストラクタ
  ~MvnCaseEq();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Xマスクを得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kEqX の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  MvnBvConst
  xmask() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Xマスク
  MvnBvConst mXmask;

};

END_NAMESPACE_YM_MVN

#endif // MVNEQX_H
