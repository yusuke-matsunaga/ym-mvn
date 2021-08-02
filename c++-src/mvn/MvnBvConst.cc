
/// @file MvnBvConst.cc
/// @brief MvnBvConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MvnBvConst.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

inline
SizeType
block_size(
  SizeType size
)
{
  return (size + 63) / 64;
}

inline
SizeType
str_size(
  const char* str
)
{
  if ( str == nullptr ) {
    return 0;
  }
  else {
    return strlen(str);
  }
}

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] size サイズ(ビット長)
//
// 内容は 0 に初期化される．
MvnBvConst::MvnBvConst(
  SizeType size
) : mSize{size},
    mBody(block_size(mSize), 0UL)
{
}

// @brief 文字列からの変換コンストラクタ
// @param[in] str 内容を表す文字列
//
// * str の長さがビット長になる．
// * '0', '1' 以外の文字が含まれていたときの動作は不定
MvnBvConst::MvnBvConst(
  const char* str
) : mSize{str_size(str)},
    mBody(block_size(mSize), 0UL)
{
  for ( SizeType i = 0; i < mSize; ++ i ) {
    if ( str[i] == '1' ) {
      set_val(i, true);
    }
    else if ( str[i] == '0' ) {
      set_val(i, false);
    }
    else {
      // エラー
      cerr << "error in MvnBvConst: illegal charactor '"
	   << str[i] << "'" << endl;
      set_val(i, false);
    }
  }
}

// @brief 内容が全て0の時 true を返す．
bool
MvnBvConst::is_all0() const
{
  SizeType n{mBody.size()};
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( mBody[i] != 0UL ) {
      return false;
    }
  }
  return true;
}

// @brief 自身の値をビット反転する．
// @return 自身への参照を返す．
MvnBvConst&
MvnBvConst::negate()
{
  for ( auto& v: mBody ) {
    v = ~v;
  }
  return *this;
}

// @brief intern bitwise AND
// @param[in] right オペランド
// @return 自身への参照を返す．
MvnBvConst&
MvnBvConst::operator&=(
  const MvnBvConst& right
)
{
  ASSERT_COND( size() == right.size() );

  SizeType n{mBody.size()};
  for ( SizeType i = 0; i < n; ++ i ) {
    mBody[i] &= right.mBody[i];
  }
  return *this;
}

// @brief intern bitwise OR
// @param[in] right オペランド
// @return 自身への参照を返す．
MvnBvConst&
MvnBvConst::operator|=(
  const MvnBvConst& right
)
{
  ASSERT_COND( size() == right.size() );

  SizeType n{mBody.size()};
  for ( SizeType i = 0; i < n; ++ i ) {
    mBody[i] |= right.mBody[i];
  }
  return *this;
}

// @brief intern bitwise XOR
// @param[in] right オペランド
// @return 自身への参照を返す．
MvnBvConst&
MvnBvConst::operator^=(
  const MvnBvConst& right
)
{
  ASSERT_COND( size() == right.size() );

  SizeType n{mBody.size()};
  for ( SizeType i = 0; i < n; ++ i ) {
    mBody[i] ^= right.mBody[i];
  }
  return *this;
}

// @brief 等価比較演算子
// @param[in] right オペランド
// @return 等しければ true を返す．
bool
MvnBvConst::operator==(
  const MvnBvConst& right
) const
{
  ASSERT_COND( size() == right.size() );

  SizeType n{mBody.size()};
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( mBody[i] != right.mBody[i] ) {
      return false;
    }
  }
  return true;
}

// @brief 小なり比較演算子
// @param[in] right オペランド
// @return this < right の時に true を返す．
//
// 符号なし2進数と見なして大小比較を行う．
bool
MvnBvConst::operator<(
  const MvnBvConst& right
) const
{
  ASSERT_COND( size() == right.size() );

  SizeType n{mBody.size()};
  for ( int i = n - 1; i >= 0; -- i ) {
    if ( mBody[i] < right.mBody[i] ) {
      return true;
    }
  }
  return false;
}

// @brief 内容を表す文字列を返す．
// @return 2進数と見なした時の表現を返す．
string
MvnBvConst::to_string() const
{
  if ( size() == 0 ) {
    return string();
  }

  string ans;
  for ( int i = size() - 1; i >= 0; -- i ) {
    if ( val(i) ) {
      ans += '1';
    }
    else {
      ans += '0';
    }
  }

  return ans;
}

END_NAMESPACE_YM_MVN
