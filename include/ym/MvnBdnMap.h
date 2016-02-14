﻿#ifndef YM_MVNBDNMAP_H
#define YM_MVNBDNMAP_H

/// @file ym/MvnBdnMap.h
/// @brief MvnBdnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/mvnbdnconv_nsdef.h"
#include "ym/ym_mvn.h"
#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class MvnBdnMap MvnBdnMap.h "MvnBdnMap.h"
/// @brief MvnNode と BdNode の対応を記録するクラス
//////////////////////////////////////////////////////////////////////
class MvnBdnMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] n MvnNode の ID の最大値
  MvnBdnMap(ymuint n);

  /// @brief デストラクタ
  ~MvnBdnMap();


public:

  /// @brief 登録する．(1ビット版)
  /// @param[in] mvnode MvnNode
  /// @param[in] handle BdnNodeHandle
  void
  put(const MvnNode* mvnode,
      BdnNodeHandle handle);

  /// @brief 登録する．(ベクタ版)
  /// @param[in] mvnode MvnNode
  /// @param[in] index ビット位置
  /// @param[in] handle BdnNodeHandle
  void
  put(const MvnNode* mvnode,
      ymuint index,
      BdnNodeHandle handle);

  /// @brief 探す．(1ビット版)
  /// @param[in] mvnode MvnNode
  /// @return 対応するハンドルを返す．
  BdnNodeHandle
  get(const MvnNode* mvnode) const;

  /// @brief 探す．(ベクタ版)
  /// @param[in] mvnode MvnNode
  /// @param[in] index ビット位置
  /// @return 対応するハンドルを返す．
  BdnNodeHandle
  get(const MvnNode* mvnode,
      ymuint index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<BdnNodeHandle> > mArray;

};


/// @relates MvnBdnMap, MvnMgr
/// @brief MvnBdnMap の内容を出力する．
void
dump_mvnode_map(ostream& s,
		const MvnMgr& mvmgr,
		const MvnBdnMap& mvnode_map);

END_NAMESPACE_YM_NETWORKSBDNCONV

#endif // YM_MVNBDNMAP_H
