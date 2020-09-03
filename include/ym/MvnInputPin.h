#ifndef YM_MVNINPUTPIN_H
#define YM_MVNINPUTPIN_H

/// @file ym/MvnInputPin.h
/// @brief MvnInputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
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
  node() const;

  /// @brief ビット幅を得る．
  SizeType
  bit_width() const;

  /// @brief ノードの何番目のピンかを返す．
  ///
  /// 入力ピンと出力ピンは区別される．
  SizeType
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
       SizeType pos);


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
SizeType
MvnInputPin::bit_width() const
{
  return mBitWidth;
}

// @brief ノードの何番目のピンかを返す．
// @note 入力ピンと出力ピンは区別される．
inline
SizeType
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
		  SizeType pos)
{
  mNode = node;
  mPos = pos;
  mBitWidth = 1;
  mSrcNode = nullptr;
}

END_NAMESPACE_YM_MVN

#endif // YM_MVNINPUTPIN_H
