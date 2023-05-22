#ifndef MVNBVCONST_H
#define MVNBVCONST_H

/// @file MvnBvConst.h
/// @brief MvnBvConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnBvConst MvnBvConst.h "MvnBvConst.h"
/// @brief ビットベクタの定数を表すクラス
///
/// 各ビットの値は純粋な bool であり，ドントケア(不定値)はない．
//////////////////////////////////////////////////////////////////////
class MvnBvConst
{
public:

  /// @brief コンストラクタ
  ///
  /// 内容は 0 に初期化される．
  explicit
  MvnBvConst(
    SizeType size = 0 ///< [in] サイズ(ビット長)
  );

  /// @brief 文字列からの変換コンストラクタ
  explicit
  MvnBvConst(
    const char* str ///< [in] 内容を表す文字列
                    ///< - この文字列の長さがビット長になる．
                    ///< - '0', '1' 以外の文字が含まれていたときの動作は不定
  );

  /// @brief 文字列からの変換コンストラクタ
  explicit
  MvnBvConst(
    const string& str ///< [in] 内容を表す文字列
		      ///< - この文字列の長さがビット長になる．
		      ///< - '0', '1' 以外の文字が含まれていたときの動作は不定
  )
  {
    MvnBvConst(str.c_str());
  }

  // コピーコンストラクタ，ムーブコンストラクタ
  // コピー代入，ムーブ代入はデフォルト

  /// @brief デストラクタ
  ~MvnBvConst() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを返す．
  SizeType
  size() const
  {
    return mSize;
  }

  /// @brief 内容が全て0の時 true を返す．
  bool
  is_all0() const;

  /// @brief 要素を返す．
  bool
  operator[](
    int pos ///< [in] 位置 ( 0 <= pos < size() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < size() );
    SizeType b = block(pos);
    SizeType s = shift(pos);
    return static_cast<bool>((mBody[b] >> s) & 1U);
  }

  /// @brief [] の別名
  bool
  val(
    int pos ///< [in] 位置 ( 0 <= pos < size() )
  ) const
  {
    return operator[](pos);
  }

  /// @brief 値をセットする．
  void
  set_val(
    int pos, ///< [in] 位置 ( 0 <= pos < size() )
    bool val ///< [in] 設定する値
  )
  {
    ASSERT_COND( 0 <= pos && pos < size() );
    SizeType b = block(pos);
    SizeType s = shift(pos);
    std::uint64_t mask = (1UL << s);
    if ( val ) {
      mBody[b] |= mask;
    }
    else {
      mBody[b] &= ~mask;
    }
  }

  /// @brief ビット反転演算子
  ///
  /// 自身のビットを反転した値を返す．
  MvnBvConst
  operator~() const
  {
    return MvnBvConst(*this).negate();
  }

  /// @brief 自身の値をビット反転する．
  /// @return 自身への参照を返す．
  MvnBvConst&
  negate();

  /// @brief intern bitwise AND
  /// @return 自身への参照を返す．
  MvnBvConst&
  operator&=(
    const MvnBvConst& right ///< [in] オペランド
  );

  /// @brief intern bitwise OR
  /// @return 自身への参照を返す．
  MvnBvConst&
  operator|=(
    const MvnBvConst& right ///< [in] オペランド
  );

  /// @brief intern bitwise XOR
  /// @return 自身への参照を返す．
  MvnBvConst&
  operator^=(
    const MvnBvConst& right ///< [in] オペランド
  );

  /// @brief 等価比較演算子
  /// @return 等しければ true を返す．
  bool
  operator==(
    const MvnBvConst& right ///< [in] オペランド
  ) const;

  /// @brief 小なり比較演算子
  /// @return this < right の時に true を返す．
  ///
  /// 符号なし2進数と見なして大小比較を行う．
  bool
  operator<(
    const MvnBvConst& right ///< [in] オペランド
  ) const;

  /// @brief 内容を表す文字列を返す．
  /// @return 2進数と見なした時の表現を返す．
  string
  to_string() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置からブロック番号を得る．
  static
  SizeType
  block(
    int pos ///< [in] pos 位置
  )
  {
    return pos / 64;
  }

  /// @brief 位置からシフト量を得る．
  static
  SizeType
  shift(
    int pos ///< [in] 位置
  )
  {
    return pos % 64;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズ
  SizeType mSize;

  // 本体
  vector<std::uint64_t> mBody;

};


/// @relates MvnBvConst
/// @brief ビットワイズAND演算子
/// @return 演算結果を返す．
inline
MvnBvConst
operator&(
  const MvnBvConst& left, ///< [in] 左のオペランド
  const MvnBvConst& right ///< [in] 右のオペランド
)
{
  return MvnBvConst(left) &= right;
}

/// @relates MvnBvConst
/// @brief ビットワイズOR算子
/// @return 演算結果を返す．
inline
MvnBvConst
operator|(
  const MvnBvConst& left, ///< [in] 左のオペランド
  const MvnBvConst& right ///< [in] 右のオペランド
)
{
  return MvnBvConst(left) |= right;
}

/// @relates MvnBvConst
/// @brief ビットワイズXOR演算子
/// @return 演算結果を返す．
inline
MvnBvConst
operator^(
  const MvnBvConst& left, ///< [in] 左のオペランド
  const MvnBvConst& right ///< [in] 右のオペランド
)
{
  return MvnBvConst(left) ^= right;
}

/// @relates MvnBvConst
/// @brief 非等価比較演算子
/// @return 等しくなければ true を返す．
inline
bool
operator!=(
  const MvnBvConst& left, ///< [in] 左のオペランド
  const MvnBvConst& right ///< [in] 右のオペランド
)
{
  return !left.operator==(right);
}

/// @relates MvnBvConst
/// @brief 大なり比較演算子
/// @return this > right の時に true を返す．
///
/// 符号なし2進数と見なして大小比較を行う．
inline
bool
operator>(
  const MvnBvConst& left, ///< [in] 左のオペランド
  const MvnBvConst& right ///< [in] 右のオペランド
)
{
  return right.operator<(left);
}

/// @relates MvnBvConst
/// @brief 小なりイコール比較演算子
/// @return this <= right の時に true を返す．
///
/// 符号なし2進数と見なして大小比較を行う．
inline
bool
operator<=(
  const MvnBvConst& left, ///< [in] 左のオペランド
  const MvnBvConst& right ///< [in] 右のオペランド
)
{
  return !right.operator<(left);
}

/// @relates MvnBvConst
/// @brief 大なりイコール比較演算子
/// @return this >= right の時に true を返す．
///
/// 符号なし2進数と見なして大小比較を行う．
inline
bool
operator>=(
  const MvnBvConst& left, ///< [in] 左のオペランド
  const MvnBvConst& right ///< [in] 右のオペランド
)
{
  return !left.operator<(right);
}

/// @relates MvnBvconst
/// @brief MvnBvConst のストリーム出力
/// @return 出力ストリームを返す．
inline
ostream&
operator<<(
  ostream& s,           ///< [in] 出力ストリーム
  const MvnBvConst& obj ///< [in] ビットベクタ
)
{
  s << obj.to_string();
  return s;
}

END_NAMESPACE_YM_MVN

#endif // MVNBVCONST_H
