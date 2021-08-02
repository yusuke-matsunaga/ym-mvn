
/// @file MvVlMap.cc
/// @brief MvVlMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MvnVlMap.h"
#include "MapRec.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnVlMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnVlMap::MvnVlMap()
{
}

// @brief コピーコンストラクタ
MvnVlMap::MvnVlMap(
  const MvnVlMap& src
) : mArray(src.mArray.size())
{
  SizeType n{src.mArray.size()};
  for ( SizeType i = 0; i < n; ++ i ) {
    auto src_rec{src.mArray[i].get()};
    if ( src_rec ) {
      if ( src_rec->is_single_elem() ) {
	reg_node(i, src_rec->get_single_elem());
      }
      else {
	reg_node(i, src_rec->get_array_elem(), src_rec->get_array_offset());
      }
    }
  }
}

// @brief 代入演算子
MvnVlMap&
MvnVlMap::operator=(
  const MvnVlMap& src
)
{
  if ( &src != this) {
    clear();
    SizeType n{src.mArray.size()};
    mArray.clear();
    mArray.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto src_rec{src.mArray[i].get()};
      if ( src_rec ) {
	if ( src_rec->is_single_elem() ) {
	  reg_node(i, src_rec->get_single_elem());
	}
	else {
	  reg_node(i, src_rec->get_array_elem(), src_rec->get_array_offset());
	}
      }
    }
  }
  return *this;
}

// @brief デストラクタ
MvnVlMap::~MvnVlMap()
{
}

// @brief 内容をクリアする．
void
MvnVlMap::clear()
{
  mArray.clear();
}

// @brief 単一の宣言要素を登録する．
void
MvnVlMap::reg_node(
  SizeType id,
  const VlDecl* decl
)
{
  auto rec{new SingleMapRec(decl)};
  put(id, unique_ptr<MapRec>{rec});
}

// @brief 配列の宣言要素を登録する．
void
MvnVlMap::reg_node(
  SizeType id,
  const VlDeclArray* declarray,
  SizeType offset
)
{
  auto rec{new ArrayMapRec(declarray, offset)};
  put(id, unique_ptr<MapRec>{rec});
}

// @brief src_id の内容を dst_id にコピーする．
void
MvnVlMap::move(
  SizeType src_id,
  SizeType dst_id)
{
  mArray[dst_id].swap(mArray[src_id]);
}

// @brief id に対応する宣言要素が単一要素の時に true を返す．
bool
MvnVlMap::is_single_elem(
  SizeType id
) const
{
  auto rec{get(id)};
  if ( rec == nullptr ) {
    return false;
  }
  return rec->is_single_elem();
}

// @brief id に対応する宣言要素が配列要素の時に true を返す．
bool
MvnVlMap::is_array_elem(
  SizeType id
) const
{
  auto rec{get(id)};
  if ( rec == nullptr ) {
    return false;
  }
  return rec->is_array_elem();
}

// @brief id に対応する宣言要素を返す．(単一要素版)
const VlDecl*
MvnVlMap::get_single_elem(
  SizeType id
) const
{
  auto rec{get(id)};
  if ( rec == nullptr ) {
    return nullptr;
  }
  return rec->get_single_elem();
}

// @brief id に対応する宣言要素を返す．(配列要素版)
const VlDeclArray*
MvnVlMap::get_array_elem(
  SizeType id
) const
{
  auto rec{get(id)};
  if ( rec == nullptr ) {
    return nullptr;
  }
  return rec->get_array_elem();
}

// @brief id に対応する宣言要素のオフセットを返す．(配列要素版)
SizeType
MvnVlMap::get_array_offset(
  SizeType id
) const
{
  auto rec{get(id)};
  if ( rec == nullptr ) {
    return 0;
  }
  return rec->get_array_offset();
}

// @brief 要素を設定する．
void
MvnVlMap::put(
  SizeType id,
  unique_ptr<MapRec>&& elem
)
{
  while ( mArray.size() < id ) {
    mArray.push_back(nullptr);
  }
  mArray.push_back(std::move(elem));
}

// @brief 要素を取り出す．
MapRec*
MvnVlMap::get(
  SizeType id
) const
{
  if ( mArray.size() <= id ) {
    return nullptr;
  }
  return mArray[id].get();
}


//////////////////////////////////////////////////////////////////////
// クラス SingleMapRec
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素が単一要素の時に true を返す．
bool
SingleMapRec::is_single_elem() const
{
  return true;
}

// @brief 宣言要素が配列要素の時に true を返す．
bool
SingleMapRec::is_array_elem() const
{
  return false;
}

// @brief 宣言要素を返す．(単一要素版)
// @note is_single_elem() == false の時は nullptr が返される．
const VlDecl*
SingleMapRec::get_single_elem() const
{
  return mDecl;
}

// @brief 宣言要素を返す．(配列要素版)
// @note is_array_elem(id) == false の時は nullptr が返される．
const VlDeclArray*
SingleMapRec::get_array_elem() const
{
  return nullptr;
}

// @brief 宣言要素のオフセットを返す．(配列要素版)
// @note is_array_elem(id) == false の時は 0 が返される．
int
SingleMapRec::get_array_offset() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス ArrayMapRec
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素が単一要素の時に true を返す．
bool
ArrayMapRec::is_single_elem() const
{
  return false;
}

// @brief 宣言要素が配列要素の時に true を返す．
bool
ArrayMapRec::is_array_elem() const
{
  return true;
}

// @brief 宣言要素を返す．(単一要素版)
// @note is_single_elem() == false の時は nullptr が返される．
const VlDecl*
ArrayMapRec::get_single_elem() const
{
  return nullptr;
}

// @brief 宣言要素を返す．(配列要素版)
// @note is_array_elem(id) == false の時は nullptr が返される．
const VlDeclArray*
ArrayMapRec::get_array_elem() const
{
  return mDeclArray;
}

// @brief 宣言要素のオフセットを返す．(配列要素版)
// @note is_array_elem(id) == false の時は 0 が返される．
int
ArrayMapRec::get_array_offset() const
{
  return mOffset;
}

END_NAMESPACE_YM_MVN
