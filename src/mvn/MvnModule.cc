
/// @file MvnModule.cc
/// @brief MvnModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MvnModule.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
MvnModule::MvnModule(const string& name,
		     SizeType np,
		     SizeType ni,
		     SizeType no,
		     SizeType nio) :
  mName(name),
  mParent(nullptr),
  mPortArray(np),
  mInputArray(ni),
  mOutputArray(no),
  mInoutArray(nio)
{
}

// @brief デストラクタ
MvnModule::~MvnModule()
{
}

END_NAMESPACE_YM_MVN
