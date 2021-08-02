#ifndef YM_MVNBNMAP_H
#define YM_MVNBNMAP_H

/// @file ym/MvnBnMap.h
/// @brief MvnBnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2021 Yusuke Matsunaga
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
  MvnBnMap(
    int n ///< [in] MvnNode の ID の最大値
  );

  /// @brief デストラクタ
  ~MvnBnMap();


public:

  /// @brief 登録する．(1ビット版)
  void
  put(
    const MvnNode* mvnode, ///< [in] 元のノード
    BnNodeHandle handle    ///< [in] 対応するBnNodeハンドル
  );

  /// @brief 登録する．(ベクタ版)
  void
  put(
    const MvnNode* mvnode, ///< [in] 元のノードMvnNode
    int index,             ///< [in] ビット位置
    BnNodeHandle handle    ///< [in] 対応するBnNodeハンドル
  );

  /// @brief 探す．(1ビット版)
  /// @return 対応するハンドルを返す．
  BnNodeHandle
  get(
    const MvnNode* mvnode ///< [in] 元のノード
  ) const;

  /// @brief 探す．(ベクタ版)
  /// @return 対応するハンドルを返す．
  BnNodeHandle
  get(
    const MvnNode* mvnode, ///< [in] 元のノード
    int index              ///< [in] ビット位置
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<BnNodeHandle> > mArray;

};


/// @relates MvnBnMap, MvnMgr
/// @brief MvnBnMap の内容を出力する．
void
dump_mvnode_map(
  ostream& s,                 ///< [in] 出力ストリーム
  const MvnMgr& mvmgr,        ///< [in] Mvn ネットワーク
  const MvnBnMap& mvnode_map  ///< [in] 対応関係のマップ
);

END_NAMESPACE_YM_NETWORKSBDNCONV

#endif // YM_MVNBDNMAP_H
