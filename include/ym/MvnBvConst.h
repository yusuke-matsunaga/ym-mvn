#ifndef MVNBVCONST_H
#define MVNBVCONST_H

/// @file MvnBvConst.h
/// @brief MvnBvConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnBvConst MvnBvConst.h "MvnBvConst.h"
/// @brief ビットベクタの定数を表すクラス
//////////////////////////////////////////////////////////////////////
class MvnBvConst
{
public:

  /// @brief コンストラクタ
  /// @param[in] size サイズ(ビット長)
  ///
  /// 内容は 0 に初期化される．
  MvnBvConst(SizeType size = 0);

  /// @brief 文字列からの変換コンストラクタ
  /// @param[in] str 内容を表す文字列
  ///
  /// * str の長さがビット長になる．
  /// * '0', '1' 以外の文字が含まれていたときの動作は不定
  MvnBvConst(const char* str);

  /// @brief 文字列からの変換コンストラクタ
  /// @param[in] str 内容を表す文字列
  ///
  /// * str の長さがビット長になる．
  /// * '0', '1' 以外の文字が含まれていたときの動作は不定
  MvnBvConst(const string& str);

  MvnBvConst(const MvnBvConst& src) = default;

  MvnBvConst(MvnBvConst&& src) = default;

  MvnBvConst&
  operator=(const MvnBvConst& src) = default;

  MvnBvConst&
  operator=(MvnBvConst&& src) = default;

  /// @brief デストラクタ
  ~MvnBvConst() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを返す．
  SizeType
  size() const;

  /// @brief 内容が全て0の時 true を返す．
  bool
  is_all0() const;

  /// @brief 要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < size() )
  bool
  operator[](int pos) const;

  /// @brief [] の別名
  /// @param[in] pos 位置 ( 0 <= pos < size() )
  bool
  val(int pos) const;

  /// @brief 値をセットする．
  /// @param[in] pos 位置 ( 0 <= pos < size() )
  /// @param[in] val 設定する値
  void
  set_val(int pos,
	  bool val);

  /// @brief ビット反転演算子
  ///
  /// 自身のビットを反転した値を返す．
  MvnBvConst
  operator~() const;

  /// @brief ビットワイズAND演算子
  /// @param[in] right オペランド
  /// @return 演算結果を返す．
  MvnBvConst
  operator&(const MvnBvConst& right) const;

  /// @brief ビットワイズOR算子
  /// @param[in] right オペランド
  /// @return 演算結果を返す．
  MvnBvConst
  operator|(const MvnBvConst& right) const;

  /// @brief ビットワイズXOR演算子
  /// @param[in] right オペランド
  /// @return 演算結果を返す．
  MvnBvConst
  operator^(const MvnBvConst& right) const;

  /// @brief 自身の値をビット反転する．
  /// @return 自身への参照を返す．
  MvnBvConst&
  negate();

  /// @brief intern bitwise AND
  /// @param[in] right オペランド
  /// @return 自身への参照を返す．
  MvnBvConst&
  operator&=(const MvnBvConst& right);

  /// @brief intern bitwise OR
  /// @param[in] right オペランド
  /// @return 自身への参照を返す．
  MvnBvConst&
  operator|=(const MvnBvConst& right);

  /// @brief intern bitwise XOR
  /// @param[in] right オペランド
  /// @return 自身への参照を返す．
  MvnBvConst&
  operator^=(const MvnBvConst& right);

  /// @brief 等価比較演算子
  /// @param[in] right オペランド
  /// @return 等しければ true を返す．
  bool
  operator==(const MvnBvConst& right) const;

  /// @brief 非等価比較演算子
  /// @param[in] right オペランド
  /// @return 等しくなければ true を返す．
  bool
  operator!=(const MvnBvConst& right) const;

  /// @brief 小なり比較演算子
  /// @param[in] right オペランド
  /// @return this < right の時に true を返す．
  ///
  /// 符号なし2進数と見なして大小比較を行う．
  bool
  operator<(const MvnBvConst& right) const;

  /// @brief 大なり比較演算子
  /// @param[in] right オペランド
  /// @return this > right の時に true を返す．
  ///
  /// 符号なし2進数と見なして大小比較を行う．
  bool
  operator>(const MvnBvConst& right) const;

  /// @brief 小なりイコール比較演算子
  /// @param[in] right オペランド
  /// @return this <= right の時に true を返す．
  ///
  /// 符号なし2進数と見なして大小比較を行う．
  bool
  operator<=(const MvnBvConst& right) const;

  /// @brief 大なりイコール比較演算子
  /// @param[in] right オペランド
  /// @return this >= right の時に true を返す．
  ///
  /// 符号なし2進数と見なして大小比較を行う．
  bool
  operator>=(const MvnBvConst& right) const;

  /// @brief 内容を表す文字列を返す．
  /// @return 2進数と見なした時の表現を返す．
  string
  to_string() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置からブロック番号を得る．
  /// @param[in] pos 位置
  static
  SizeType
  block(int pos);

  /// @brief 位置からシフト量を得る．
  /// @param[in] pos 位置
  static
  SizeType
  shift(int pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズ
  SizeType mSize;

  // 本体
  vector<ymuint64> mBody;

};

/// @relates MvnBvconst
/// @brief MvnBvConst のストリーム出力
ostream&
operator<<(ostream& s,
	   const MvnBvConst& obj);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 文字列からの変換コンストラクタ
// @param[in] str 内容を表す文字列
//
// * str の長さがビット長になる．
// * '0', '1' 以外の文字が含まれていたときの動作は不定
inline
MvnBvConst::MvnBvConst(const string& str)
{
  MvnBvConst(str.c_str());
}

// @brief サイズを返す．
inline
SizeType
MvnBvConst::size() const
{
  return mSize;
}

// @brief 要素を返す．
// @param[in] pos 位置 ( 0 <= pos < size() )
inline
bool
MvnBvConst::operator[](int pos) const
{
  ASSERT_COND( 0 <= pos && pos < size() );
  SizeType b = block(pos);
  SizeType s = shift(pos);
  return static_cast<bool>((mBody[b] >> s) & 1U);
}

// @brief [] の別名
// @param[in] pos 位置 ( 0 <= pos < size() )
inline
bool
MvnBvConst::val(int pos) const
{
  return operator[](pos);
}

// @brief 値をセットする．
// @param[in] pos 位置 ( 0 <= pos < size() )
// @param[in] val 設定する値
inline
void
MvnBvConst::set_val(int pos,
		    bool val)
{
  ASSERT_COND( 0 <= pos && pos < size() );
  SizeType b = block(pos);
  SizeType s = shift(pos);
  ymuint64 mask = (1UL << s);
  if ( val ) {
    mBody[b] |= mask;
  }
  else {
    mBody[b] &= ~mask;
  }
}

// @brief ビット反転演算子
//
// 自身のビットを反転した値を返す．
inline
MvnBvConst
MvnBvConst::operator~() const
{
  return MvnBvConst(*this).negate();
}

// @brief ビットワイズAND演算子
// @param[in] right オペランド
// @return 演算結果を返す．
inline
MvnBvConst
MvnBvConst::operator&(const MvnBvConst& right) const
{
  return MvnBvConst(*this) &= right;
}

// @brief ビットワイズOR算子
// @param[in] right オペランド
// @return 演算結果を返す．
inline
MvnBvConst
MvnBvConst::operator|(const MvnBvConst& right) const
{
  return MvnBvConst(*this) |= right;
}

// @brief ビットワイズXOR演算子
// @param[in] right オペランド
// @return 演算結果を返す．
inline
MvnBvConst
MvnBvConst::operator^(const MvnBvConst& right) const
{
  return MvnBvConst(*this) ^= right;
}

// @brief 非等価比較演算子
// @param[in] right オペランド
// @return 等しくなければ true を返す．
inline
bool
MvnBvConst::operator!=(const MvnBvConst& right) const
{
  return !operator==(right);
}

// @brief 大なり比較演算子
// @param[in] right オペランド
// @return this > right の時に true を返す．
//
// 符号なし2進数と見なして大小比較を行う．
inline
bool
MvnBvConst::operator>(const MvnBvConst& right) const
{
  return right.operator<(*this);
}

// @brief 小なりイコール比較演算子
// @param[in] right オペランド
// @return this <= right の時に true を返す．
//
// 符号なし2進数と見なして大小比較を行う．
inline
bool
MvnBvConst::operator<=(const MvnBvConst& right) const
{
  return !right.operator<(*this);
}

// @brief 大なりイコール比較演算子
// @param[in] right オペランド
// @return this >= right の時に true を返す．
//
// 符号なし2進数と見なして大小比較を行う．
inline
bool
MvnBvConst::operator>=(const MvnBvConst& right) const
{
  return !operator<(right);
}

// @brief 位置からブロック番号を得る．
// @param[in] pos 位置
inline
SizeType
MvnBvConst::block(int pos)
{
  return pos / 64;
}

// @brief 位置からシフト量を得る．
// @param[in] pos 位置
inline
SizeType
MvnBvConst::shift(int pos)
{
  return pos % 64;
}

// @relates MvnBvconst
// @brief MvnBvConst のストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const MvnBvConst& obj)
{
  s << obj.to_string();
  return s;
}

END_NAMESPACE_YM_MVN

#endif // MVNBVCONST_H
