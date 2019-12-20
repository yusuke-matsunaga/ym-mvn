
/// @file MvVlMap.cc
/// @brief MvVlMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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
MvnVlMap::MvnVlMap(const MvnVlMap& src) :
  mArray(src.mArray.size())
{
  int n = src.mArray.size();
  for ( int i = 0; i < n; ++ i ) {
    const MapRec* src_rec = src.mArray[i];
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
const MvnVlMap&
MvnVlMap::operator=(const MvnVlMap& src)
{
  if ( &src != this) {
    clear();
    int n = src.mArray.size();
    mArray.resize(n, nullptr);
    for ( int i = 0; i < n; ++ i ) {
      const MapRec* src_rec = src.mArray[i];
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
  clear();
}

// @brief 内容をクリアする．
void
MvnVlMap::clear()
{
  for ( auto rec: mArray ) {
    delete rec;
  }
  mArray.clear();
}

// @brief 単一の宣言要素を登録する．
// @param[in] id MvNode の ID番号
// @param[in] decl 宣言要素
void
MvnVlMap::reg_node(int id,
		   const VlDecl* decl)
{
  auto rec = new SingleMapRec(decl);
  put(id, rec);
}

// @brief 配列の宣言要素を登録する．
// @param[in] id MvNode の ID番号
// @param[in] declarray 配列宣言要素
// @param[in] offset オフセット
void
MvnVlMap::reg_node(int id,
		   const VlDeclArray* declarray,
		   int offset)
{
  auto rec = new ArrayMapRec(declarray, offset);
  put(id, rec);
}

// @brief src_id の内容を dst_id にコピーする．
void
MvnVlMap::copy(int src_id,
	       int dst_id)
{
  put(dst_id, get(src_id));
}

// @brief id に対応する宣言要素が単一要素の時に true を返す．
// @param[in] id MvNode の ID番号
bool
MvnVlMap::is_single_elem(int id) const
{
  auto rec = get(id);
  if ( rec == nullptr ) {
    return false;
  }
  return rec->is_single_elem();
}

// @brief id に対応する宣言要素が配列要素の時に true を返す．
// @param[in] id MvNode の ID番号
bool
MvnVlMap::is_array_elem(int id) const
{
  auto rec = get(id);
  if ( rec == nullptr ) {
    return false;
  }
  return rec->is_array_elem();
}

// @brief id に対応する宣言要素を返す．(単一要素版)
// @param[in] id MvNode の ID番号
// @note is_single_elem(id) == false の時は nullptr が返される．
const VlDecl*
MvnVlMap::get_single_elem(int id) const
{
  auto rec = get(id);
  if ( rec == nullptr ) {
    return nullptr;
  }
  return rec->get_single_elem();
}

// @brief id に対応する宣言要素を返す．(配列要素版)
// @param[in] id MvNode の ID番号
// @note is_array_elem(id) == false の時は nullptr が返される．
const VlDeclArray*
MvnVlMap::get_array_elem(int id) const
{
  auto rec = get(id);
  if ( rec == nullptr ) {
    return nullptr;
  }
  return rec->get_array_elem();
}

// @brief id に対応する宣言要素のオフセットを返す．(配列要素版)
// @param[in] id MvNode の ID番号
// @note is_array_elem(id) == false の時は 0 が返される．
int
MvnVlMap::get_array_offset(int id) const
{
  auto rec = get(id);
  if ( rec == nullptr ) {
    return 0;
  }
  return rec->get_array_offset();
}

// @brief 要素を設定する．
// @param[in] id ID番号
// @param[in] elem 設定する要素
void
MvnVlMap::put(int id,
	      MapRec* elem)
{
  if ( mArray.size() <= id ) {
    mArray.resize(id + 1, nullptr);
  }
  mArray[id] = elem;
}

// @brief 要素を取り出す．
// @param[in] id ID番号
// @note id が範囲外の時は nullptr が返される．
MapRec*
MvnVlMap::get(int id) const
{
  if ( mArray.size() <= id ) {
    return nullptr;
  }
  return mArray[id];
}


//////////////////////////////////////////////////////////////////////
// クラス SingleMapRec
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] decl 単一の宣言要素
SingleMapRec::SingleMapRec(const VlDecl* decl) :
  mDecl(decl)
{
}

// @brief デストラクタ
SingleMapRec::~SingleMapRec()
{
}

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

// @brief コンストラクタ
// @param[in] declarray 配列要素
// @param[in] offset オフセット
ArrayMapRec::ArrayMapRec(const VlDeclArray* declarray,
			 int offset) :
  mDeclArray(declarray),
  mOffset(offset)
{
}

// @brief デストラクタ
ArrayMapRec::~ArrayMapRec()
{
}

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
