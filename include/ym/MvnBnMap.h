#ifndef YM_MVNBNMAP_H
#define YM_MVNBNMAP_H

/// @file ym/MvnBnMap.h
/// @brief MvnBnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mvn.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class MvnBnMap MvnBnMap.h "MvnBnMap.h"
/// @brief MvnNode と BnNode の対応を記録するクラス
//////////////////////////////////////////////////////////////////////
class MvnBnMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] n MvnNode の ID の最大値
  MvnBnMap(ymuint n);

  /// @brief デストラクタ
  ~MvnBnMap();


public:

  /// @brief 登録する．(1ビット版)
  /// @param[in] mvnode MvnNode
  /// @param[in] handle BnNodeHandle
  void
  put(const MvnNode* mvnode,
      BnNodeHandle handle);

  /// @brief 登録する．(ベクタ版)
  /// @param[in] mvnode MvnNode
  /// @param[in] index ビット位置
  /// @param[in] handle BnNodeHandle
  void
  put(const MvnNode* mvnode,
      ymuint index,
      BnNodeHandle handle);

  /// @brief 探す．(1ビット版)
  /// @param[in] mvnode MvnNode
  /// @return 対応するハンドルを返す．
  BnNodeHandle
  get(const MvnNode* mvnode) const;

  /// @brief 探す．(ベクタ版)
  /// @param[in] mvnode MvnNode
  /// @param[in] index ビット位置
  /// @return 対応するハンドルを返す．
  BnNodeHandle
  get(const MvnNode* mvnode,
      ymuint index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<BnNodeHandle> > mArray;

};


/// @relates MvnBnMap, MvnMgr
/// @brief MvnBnMap の内容を出力する．
void
dump_mvnode_map(ostream& s,
		const MvnMgr& mvmgr,
		const MvnBnMap& mvnode_map);

END_NAMESPACE_YM_NETWORKSBDNCONV

#endif // YM_MVNBDNMAP_H
