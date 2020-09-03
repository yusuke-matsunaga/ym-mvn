#ifndef YM_MVNMODULE_H
#define YM_MVNMODULE_H

/// @file ym/MvnModule.h
/// @brief MvnModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
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
  int
  id() const;

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief 親のノードを得る．
  /// @note トップレベルモジュールの場合には nullptr を返す．
  MvnNode*
  parent() const;

  /// @brief ポート数を得る．
  SizeType
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const MvnPort*
  port(int pos) const;

  /// @brief 入力ノード数を得る．
  SizeType
  input_num() const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  MvnNode*
  input(int pos) const;

  /// @brief 出力ノード数を得る．
  SizeType
  output_num() const;

  /// @brief 出力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  MvnNode*
  output(int pos) const;

  /// @brief 入出力ノード数を得る．
  SizeType
  inout_num() const;

  /// @brief 入出力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < inout_num() )
  MvnNode*
  inout(int pos) const;

  /// @brief 内部ノードの数を得る．
  SizeType
  node_num() const;

  /// @brief 内部ノードのリストを得る．
  const vector<MvnNode*>&
  node_list() const;

  /// @}


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ni 入力ノード数
  /// @param[in] no 出力ノード数
  /// @param[in] nio 入出力ノード数
  MvnModule(const string& name,
	    SizeType np,
	    SizeType ni,
	    SizeType no,
	    SizeType nio);

  /// @brief デストラクタ
  ~MvnModule();


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


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
int
MvnModule::id() const
{
  return mId;
}

// @brief 名前を得る．
inline
string
MvnModule::name() const
{
  return mName;
}

// @brief 親のノードを得る．
// @note トップレベルモジュールの場合には nullptr を返す．
inline
MvnNode*
MvnModule::parent() const
{
  return mParent;
}

// @brief ポート数を得る．
inline
SizeType
MvnModule::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] pos 位置 ( 0 <= pos < port_num() )
inline
const MvnPort*
MvnModule::port(int pos) const
{
  return mPortArray[pos];
}

// @brief 入力ノード数を得る．
inline
SizeType
MvnModule::input_num() const
{
  return mInputArray.size();
}

// @brief 入力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
MvnNode*
MvnModule::input(int pos) const
{
  return mInputArray[pos];
}

// @brief 出力ノード数を得る．
inline
SizeType
MvnModule::output_num() const
{
  return mOutputArray.size();
}

// @brief 出力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
MvnNode*
MvnModule::output(int pos) const
{
  return mOutputArray[pos];
}

// @brief 入出力ノード数を得る．
inline
SizeType
MvnModule::inout_num() const
{
  return mInoutArray.size();
}

// @brief 入出力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < inout_num() )
inline
MvnNode*
MvnModule::inout(int pos) const
{
  return mInoutArray[pos];
}

// @brief 内部ノードの数を得る．
inline
SizeType
MvnModule::node_num() const
{
  return mNodeList.size();
}

// @brief 内部ノードのリストを得る．
inline
const vector<MvnNode*>&
MvnModule::node_list() const
{
  return mNodeList;
}

END_NAMESPACE_YM_MVN

#endif // YM_MVNMODULE_H
