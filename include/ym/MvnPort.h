#ifndef YM_MVNPORT_H
#define YM_MVNPORT_H

/// @file ym/MvnPort.h
/// @brief MvnPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2016, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnPortRef MvnPort.h "ym/MvnPort.h"
/// @brief ポートが参照している実体を表すクラス
///
/// 具体的には以下の3通りのタイプがある．
/// - ノードのみ
/// - ノード＋ビット指定
/// - ノード＋範囲指定
/// 実装としてはメンバ mMsb, mLsb の下位1ビットを使って，
/// - ノードのみ -> どちらも 0
/// - ノード＋ビット指定 -> mMsb のみ 1 mLsb は 0
/// - ノード＋範囲指定   -> mLsb のみ 1 mMsb は 0
/// で区別する．
//////////////////////////////////////////////////////////////////////
class MvnPortRef
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 内容は不定
  MvnPortRef() = default;

  /// @brief ノード単体のポート要素用のコンストラクタ
  MvnPortRef(
    const MvnNode* node ///< [in] ノード
  ) : mNode{node},
      mMsb{0UL},
      mLsb{0UL}
  {
  }

  /// @brief ビット指定のポート要素用のコンストラクタ
  MvnPortRef(
    const MvnNode* node, ///< [in] ノード
    SizeType bitpos      ///< [in] ビット位置
  ) : mNode{node},
      mMsb{(bitpos << 1) | 1U},
      mLsb{0UL}
  {
  }

  /// @brief 範囲指定のポート要素用のコンストラクタ
  MvnPortRef(
    const MvnNode* node, ///< [in] ノード
    SizeType msb,        ///< [in] 範囲指定の MSB
    SizeType lsb         ///< [in] 範囲指定の LSB
  ) : mNode{node},
      mMsb{(msb << 1)},
      mLsb{(lsb << 1) | 1U}
  {
  }

  /// @brief デストラクタ
  ~MvnPortRef() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部情報を得るためのメンバ関数
  /// @{

  /// @brief ノードを返す．
  /// @note ノードのタイプは kInput か kOutput
  const MvnNode*
  node() const
  {
    return mNode;
  }

  /// @brief 単純な形式の場合 true を返す．
  bool
  is_simple() const
  {
    return mMsb == 0U && mLsb == 0U;
  }

  /// @brief ビット指定ありの場合 true を返す．
  bool
  has_bitselect() const
  {
    return static_cast<bool>(mMsb & 1U);
  }

  /// @brief 範囲指定ありの場合 true を返す．
  /// @note ビット指定は含まない．
  bool
  has_partselect() const
  {
    return static_cast<bool>(mLsb & 1U);
  }

  /// @brief この実体のビット幅を返す．
  SizeType
  bit_width() const;

  /// @brief ビット指定位置を返す．
  /// @note has_bitselect() == true の時のみ意味を持つ．
  SizeType
  bitpos() const
  {
    // 実は msb() のエイリアス
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

  /// @}


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  const MvnNode* mNode{nullptr};

  // 範囲指定の MSB
  // ただし下位1ビットは範囲指定/ビット指定ありのフラグ
  SizeType mMsb{0};

  // 範囲指定の LSB
  // ただし下位1ビットは範囲指定ありのフラグ
  SizeType mLsb{0};

};


//////////////////////////////////////////////////////////////////////
/// @class MvnPort MvnPort.h "ym/MvnPort.h"
/// @brief MvnModule のポートを表すクラス
///
/// 単純には1つのポートは1つの入力ノードもしくは出力ノードに対応するが，
/// 場合によっては対応するノードがない場合，複数ある場合，ノードの一部の
/// ビット線のみに対応する場合などがある．
/// そのために MvnPortRef のベクタを用いる．
//////////////////////////////////////////////////////////////////////
class MvnPort
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  ///
  /// 名前がない場合(name == nullptr)もありうる．
  /// name == "" の場合も考慮する．
  MvnPort(
    const vector<MvnPortRef>& portref_list ///< [in] ポート参照式のリスト
    = vector<MvnPortRef>{},
    const string& name = string()          ///< [in] 名前
  );

  /// @brief デストラクタ
  ~MvnPort();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部情報を得るためのメンバ関数
  /// @{

  /// @brief 名前を得る．
  /// @note 空("")の場合もある．
  string
  name() const
  {
    return mName;
  }

  /// @brief ビット幅を得る．
  SizeType
  bit_width() const;

  /// @brief port_ref 数を得る．
  SizeType
  port_ref_num() const
  {
    return mPortRefNum;
  }

  /// @brief port_ref を得る．
  const MvnPortRef&
  port_ref(
    SizeType pos ///< [in] 位置 ( 0 <= pos < port_ref_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < port_ref_num() );

    return mPortRefArray[pos];
  }

  /// @}


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // portref の数
  SizeType mPortRefNum;

  // portref の配列
  MvnPortRef* mPortRefArray;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVNPORT_H
