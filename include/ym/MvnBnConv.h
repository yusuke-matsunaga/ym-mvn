#ifndef YM_MVNBNCONV_H
#define YM_MVNBNCONV_H

/// @file ym/MvnBnConv.h
/// @brief MvnBnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

class MvnConv;

//////////////////////////////////////////////////////////////////////
/// @class MvnBdnConv MvnBdnConv.h "MvnBdnConv.h"
/// @brief Mvn から BnNetwork に変換するクラス
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
  void
  operator()(
    const MvnMgr& mvmgr, ///< [in] 対象の MvNetwork
    BnNetwork& bnetwork, ///< [out] 変換先の BnNetwork
    MvnBnMap& mvnode_map ///< [out] 対応関係を格納するオブジェクト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNode の変換関数のリスト
  vector<unique_ptr<MvnConv>> mConvList;

};

END_NAMESPACE_YM_NETWORKSBDNCONV

#endif // YM_MVNBDNCONV_H
