#ifndef DRIVER_H
#define DRIVER_H

/// @file Driver.h
/// @brief Driver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class Driver Driver.h "Driver.h"
/// @brief ドライバーを表すクラス
//////////////////////////////////////////////////////////////////////
class Driver
{
public:

  /// @brief 単純な形式のコンストラクタ
  explicit
  Driver(
    const FileRegion& loc = FileRegion(), ///< [in] 対応する Verilog 記述のファイル位置
    MvnNode* node = nullptr               ///< [in] 右辺のノード
  ) : mLoc(loc),
      mNode(node),
      mMsb(0U),
      mLsb(0U)
  {
  }

  /// @brief ビット指定形式のコンストラクタ
  Driver(
    const FileRegion& loc, ///< [in] 対応する Verilog 記述のファイル位置
    MvnNode* node,         ///< [in] 右辺のノード
    SizeType index         ///< [in] ビット指定位置
  ) : mLoc(loc),
      mNode(node),
      mMsb((index << 1) | 1U),
      mLsb(0U)
  {
  }

  /// @brief 範囲指定形式のコンストラクタ
  Driver(
    const FileRegion& loc, ///< [in] 対応する Verilog 記述のファイル位置
    MvnNode* node,         ///< [in] 右辺のノード
    SizeType msb,          ///< [in] 範囲指定の MSB
    SizeType lsb           ///< [in] 範囲指定の LSB
  ) : mLoc(loc),
      mNode(node),
      mMsb((msb << 1)),
      mLsb((lsb << 1) | 1U)
  {
  }

  /// @brief コピーコンストラクタ
  Driver(
    const Driver& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief 代入演算子
  Driver&
  operator=(
    const Driver& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~Driver() = default;


public:

  /// @brief ファイル位置を返す．
  const FileRegion&
  loc() const
  {
    return mLoc;
  }

  /// @brief 右辺のノードを返す．
  MvnNode*
  rhs_node() const
  {
    return mNode;
  }

  /// @brief 単純な形式の時 true を返す．
  bool
  is_simple() const
  {
    return mMsb == 0U && mLsb == 0U;
  }

  /// @brief ビット指定形式の時 true を返す．
  bool
  has_bitselect() const
  {
    return static_cast<bool>(mMsb & 1U);
  }

  /// @brief 範囲指定形式の時 true を返す．
  bool
  has_partselect() const
  {
    return static_cast<bool>(mLsb & 1U);
  }

  /// @brief ビット指定位置を返す．
  /// @note has_bitselect() == true の時のみ意味を持つ．
  SizeType
  index() const
  {
    return msb();
  }

  /// @brief 範囲指定の MSB を返す．
  /// @note has_partselect() == true の時のみ意味を持つ．
  SizeType
  msb() const
  {
    return mMsb >> 1;
  }

  /// @brief 範囲指定の LSB を返す．
  /// @note has_partselect() == true の時のみ意味を持つ．
  SizeType
  lsb() const
  {
    return mLsb >> 1;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const Driver& rhs ///< [in] 比較対象のオペランド
  ) const
  {
    return mNode == rhs.mNode && mMsb == rhs.mMsb && mLsb == rhs.mLsb;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mLoc;

  // 右辺のノード
  MvnNode* mNode;

  // ビット指定位置 or 範囲指定の MSB
  std::uint32_t mMsb;

  // 範囲指定の LSB
  std::uint32_t mLsb;

};

/// @brief 非等価比較演算子
inline
bool
operator!=(
  const Driver& lhs, ///< [in] オペランド1
  const Driver& rhs  ///< [in] オペランド2
)
{
  return !(lhs == rhs);
}

END_NAMESPACE_YM_MVN_VERILOG

#endif // DRIVER_H
