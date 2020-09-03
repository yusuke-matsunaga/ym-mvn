
/// @file DeclHash.cc
/// @brief DeclHash の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "DeclHash.h"
#include "ym/mvn.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

// @brief 内部のデータをクリアする．
void
DeclHash::clear()
{
  mHash.clear();
  mNextId = 0;
}

// @brief ID番号を得る．
// @param[in] decl 宣言要素
// @return ID番号
// @note 登録されていなかった場合には新しい番号を割り当てる．
SizeType
DeclHash::get_id(const VlDecl* decl)
{
  if ( mHash.count(decl) > 0 )  {
    return mHash.at(decl);
  }
  else {
    SizeType id = mNextId;
    ++ mNextId;
    mHash.emplace(decl, id);
    return id;
  }
}

// @brief ID番号を得る．
// @param[in] decl 配列型宣言要素
// @param[in] offset オフセット
// @return ID番号
// @note 登録されていなかった場合には新しい番号を割り当てる．
SizeType
DeclHash::get_id(const VlDeclArray* decl,
		 SizeType offset)
{
  SizeType base = 0;
  if ( mHash.count(decl) > 0 ) {
    base = mHash.at(decl);
  }
  else {
    base = mNextId;
    mNextId += decl->array_size();
    mHash.emplace(decl, base);
  }
  return base + offset;
}

// @brief ID番号の最大値 + 1を返す．
SizeType
DeclHash::max_id() const
{
  return mNextId;
}

END_NAMESPACE_YM_MVN_VERILOG
