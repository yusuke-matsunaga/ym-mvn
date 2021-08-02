#ifndef YM_MVNINPUTPIN_H
#define YM_MVNINPUTPIN_H

/// @file ym/MvnInputPin.h
/// @brief MvnInputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


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
  node() const
  {
    return mNode;
  }

  /// @brief ビット幅を得る．
  SizeType
  bit_width() const
  {
    return mBitWidth;
  }

  /// @brief ノードの何番目のピンかを返す．
  ///
  /// 入力ピンと出力ピンは区別される．
  SizeType
  pos() const
  {
    return mPos;
  }

  /// @brief 接続しているノードを得る．
  MvnNode*
  src_node() const
  {
    return mSrcNode;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定用の関数
  void
  init(
    MvnNode* node,
    SizeType pos
  )
  {
    mNode = node;
    mPos = pos;
    mBitWidth = 1;
    mSrcNode = nullptr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  MvnNode* mNode;

  // 位置
  SizeType mPos;

  // ビット幅
  SizeType mBitWidth;

  // 接続しているノード
  MvnNode* mSrcNode;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVNINPUTPIN_H
