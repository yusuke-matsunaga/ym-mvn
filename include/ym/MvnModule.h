#ifndef YM_MVNMODULE_H
#define YM_MVNMODULE_H

/// @file ym/MvnModule.h
/// @brief MvnModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnModule MvnModule.h "ym/MvnModule.h"
/// @brief 多値ネットワークを表すクラス
///
/// 基本的には MvnNode のネットワークだが，MvnPort というオブジェクト
/// も持つ．これはモジュールのIOを名前で参照するためのクラス
//////////////////////////////////////////////////////////////////////
class MvnModule
{
  friend class MvnMgr;

public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部情報を得るためのメンバ関数
  /// @{

  /// @brief ID番号を得る．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 名前を得る．
  string
  name() const
  {
    return mName;
  }

  /// @brief 親のノードを得る．
  ///
  /// トップレベルモジュールの場合には nullptr を返す．
  MvnNode*
  parent() const
  {
    return mParent;
  }

  /// @brief ポート数を得る．
  SizeType
  port_num() const
  {
    return mPortArray.size();
  }

  /// @brief ポートを得る．
  const MvnPort*
  port(
    SizeType pos ///< [in] 位置 ( 0 <= pos < port_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < port_num() );
    return mPortArray[pos];
  }

  /// @brief 入力ノード数を得る．
  SizeType
  input_num() const
  {
    return mInputArray.size();
  }

  /// @brief 入力ノードを得る．
  MvnNode*
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < input_num() );
    return mInputArray[pos];
  }

  /// @brief 出力ノード数を得る．
  SizeType
  output_num() const
  {
    return mOutputArray.size();
  }

  /// @brief 出力ノードを得る．
  MvnNode*
  output(
    SizeType pos ///< [in] 位置 ( 0 <= pos < output_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < output_num() );
    return mOutputArray[pos];
  }

  /// @brief 入出力ノード数を得る．
  SizeType
  inout_num() const
  {
    return mInoutArray.size();
  }

  /// @brief 入出力ノードを得る．
  MvnNode*
  inout(
    SizeType pos ///< [in] 位置 ( 0 <= pos < inout_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < input_num() );
    return mInoutArray[pos];
  }

  /// @brief 内部ノードの数を得る．
  SizeType
  node_num() const
  {
    return mNodeList.size();
  }

  /// @brief 内部ノードのリストを得る．
  const vector<MvnNode*>&
  node_list() const
  {
    return mNodeList;
  }

  /// @}


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnModule(
    const string& name, ///< [in] 名前
    SizeType np,        ///< [in] ポート数
    SizeType ni,        ///< [in] 入力ノード数
    SizeType no,        ///< [in] 出力ノード数
    SizeType nio        ///< [in] 入出力ノード数
  );

  /// @brief デストラクタ
  ~MvnModule() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  int mId;

  // 名前
  string mName;

  // 親のノード
  MvnNode* mParent;

  // ポートの配列
  vector<MvnPort*> mPortArray;

  // 入力ノードの配列
  vector<MvnNode*> mInputArray;

  // 出力ノードの配列
  vector<MvnNode*> mOutputArray;

  // 入出力ノードの配列
  vector<MvnNode*> mInoutArray;

  // 内部ノードのリスト
  vector<MvnNode*> mNodeList;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVNMODULE_H
