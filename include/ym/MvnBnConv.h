#ifndef YM_MVNBNCONV_H
#define YM_MVNBNCONV_H

/// @file ym/MvnBnConv.h
/// @brief MvnBnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mvn.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

class MvnConv;

//////////////////////////////////////////////////////////////////////
/// @class MvnBdnConv MvnBdnConv.h "MvnBdnConv.h"
/// @brief Mvn から BdnMgr に変換するクラス
//////////////////////////////////////////////////////////////////////
class MvnBnConv
{
public:

  /// @brief コンストラクタ
  MvnBnConv();

  /// @brief デストラクタ
  ~MvnBnConv();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvnMgr の内容を BnNetwork に変換する．
  /// @param[in] mvmgr 対象の MvNetwork
  /// @param[out] bnetwork 変換先の BnNetwork
  /// @param[out] mvnode_map 対応関係を格納するオブジェクト
  void
  operator()(const MvnMgr& mvmgr,
	     BnNetwork& bnetwork,
	     MvnBnMap& mvnode_map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNode の変換関数のリスト
  vector<MvnConv*> mConvList;

};

END_NAMESPACE_YM_NETWORKSBDNCONV

#endif // YM_MVNBDNCONV_H
