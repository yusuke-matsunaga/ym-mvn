﻿#ifndef YM_MVNINPUTPIN_H
#define YM_MVNINPUTPIN_H

/// @file ym/MvnInputPin.h
/// @brief MvnInputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ym_mvn.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnInputPin MvnInputPin.h "ym/MvnInputPin.h"
/// @brief ノードの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnInputPin
{
  friend class MvnMgr;
  friend class MvnNodeBase;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnInputPin();

  /// @brief デストラクタ
  ~MvnInputPin();


public:

  /// @brief 属しているノードを得る．
  MvnNode*
  node() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief ノードの何番目のピンかを返す．
  /// @note 入力ピンと出力ピンは区別される．
  ymuint
  pos() const;

  /// @brief 接続しているノードを得る．
  MvnNode*
  src_node() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定用の関数
  void
  init(MvnNode* node,
       ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  MvnNode* mNode;

  // 位置
  ymuint32 mPos;

  // ビット幅
  ymuint32 mBitWidth;

  // 接続しているノード
  MvnNode* mSrcNode;

  // mSrcNode の mDstPinList 上の反復子
  list<MvnInputPin*>::iterator mListIter;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 属しているノードを得る．
inline
MvnNode*
MvnInputPin::node() const
{
  return mNode;
}

// @brief ビット幅を得る．
inline
ymuint
MvnInputPin::bit_width() const
{
  return mBitWidth;
}

// @brief ノードの何番目のピンかを返す．
// @note 入力ピンと出力ピンは区別される．
inline
ymuint
MvnInputPin::pos() const
{
  return mPos;
}

// @brief 接続しているノードを得る．
inline
MvnNode*
MvnInputPin::src_node() const
{
  return mSrcNode;
}

// @brief 設定用の関数
inline
void
MvnInputPin::init(MvnNode* node,
		  ymuint pos)
{
  mNode = node;
  mPos = pos;
  mBitWidth = 1;
  mSrcNode = nullptr;
}

END_NAMESPACE_YM_MVN

#endif // YM_MVNINPUTPIN_H
