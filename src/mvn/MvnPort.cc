
/// @file MvnPort.cc
/// @brief MvnPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MvnPort.h"
#include "ym/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnPortRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnPortRef::MvnPortRef() :
  mNode{nullptr},
  mMsb{0UL},
  mLsb{0UL}
{
}

// @brief ノード単体のポート要素用のコンストラクタ
// @param[in] node ノード
MvnPortRef::MvnPortRef(const MvnNode* node) :
  mNode{node},
  mMsb{0UL},
  mLsb{0UL}
{
}

// @brief ビット指定のポート要素用のコンストラクタ
// @param[in] node ノード
// @param[in] bitpos ビット位置
MvnPortRef::MvnPortRef(const MvnNode* node,
		       SizeType bitpos) :
  mNode{node},
  mMsb{(bitpos << 1) | 1U},
  mLsb{0UL}
{
}

// @brief 範囲指定のポート要素用のコンストラクタ
// @param[in] node ノード
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
MvnPortRef::MvnPortRef(const MvnNode* node,
		       SizeType msb,
		       SizeType lsb) :
  mNode{node},
  mMsb{(msb << 1)},
  mLsb{(lsb << 1) | 1U}
{
}

// @brief デストラクタ
MvnPortRef::~MvnPortRef()
{
}

// @brief この実体のビット幅を返す．
SizeType
MvnPortRef::bit_width() const
{
  if ( has_bitselect() ) {
    return 1;
  }
  else if ( has_partselect() ) {
    return msb() - lsb() + 1;
  }
  else {
    auto n{node()};
    if ( n->type() == MvnNodeType::INPUT ) {
      return n->bit_width();
    }
    else if ( n->type() == MvnNodeType::OUTPUT ) {
      return n->input(0)->bit_width();
    }
    else if ( n->type() == MvnNodeType::INOUT ) {
      return n->bit_width();
    }
  }
  ASSERT_NOT_REACHED;
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] portref_list ポート参照式のリスト
// @param[in] name 名前
//
// 名前がない場合(name == nullptr)もありうる．
// name == "" の場合も考慮する．
MvnPort::MvnPort(const vector<MvnPortRef>& portref_list,
		 const string& name) :
  mName{name},
  mPortRefNum{portref_list.size()},
  mPortRefArray{new MvnPortRef[mPortRefNum]}
{
  for ( SizeType i = 0; i < mPortRefNum; ++ i ) {
    mPortRefArray[i] = portref_list[i];
  }
}

// @brief デストラクタ
MvnPort::~MvnPort()
{
  delete [] mPortRefArray;
}

// @brief ビット幅を得る．
SizeType
MvnPort::bit_width() const
{
  SizeType ans = 0;
  for ( SizeType i = 0; i < mPortRefNum; ++ i ) {
    auto& pr{mPortRefArray[i]};
    ans += pr.bit_width();
  }
  return ans;
}

END_NAMESPACE_YM_MVN
