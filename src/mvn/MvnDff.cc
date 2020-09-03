
/// @file MvnDff.cc
/// @brief MvnDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnDff.h"
#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] clock_pol クロックの極性
// @param[in] pol_array 非同期セット信号の極性情報を入れた配列
// @param[in] val_array 非同期セットの値を入れた配列
MvnDff::MvnDff(MvnModule* module,
	       MvnPolarity clock_pol,
	       const vector<MvnPolarity>& pol_array,
	       const vector<MvnNode*>& val_array) :
  MvnNodeBase(module, MvnNodeType::DFF, pol_array.size() + 2)
{
  SizeType np{pol_array.size()};

  SizeType n1{(np + 32) / 32};
  mPolArray = new ymuint32[n1];
  for ( SizeType i = 0; i < n1; ++ i ) {
    mPolArray[i] = 0UL;
  }
  if ( clock_pol == MvnPolarity::Positive ) {
    mPolArray[0] |= 1U;
  }
  mValArray = new MvnNode*[np];
  for ( SizeType i = 0; i < np; ++ i ) {
    SizeType blk{(i + 1) / 32};
    SizeType sft{(i + 1) % 32};
    if ( pol_array[i] == MvnPolarity::Positive ) {
      mPolArray[blk] |= (1UL << sft);
    }
    mValArray[i] = val_array[i];
  }
}

// @brief デストラクタ
MvnDff::~MvnDff()
{
  delete [] mPolArray;
  delete [] mValArray;
}

// @brief クロック信号の極性を得る．
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
MvnPolarity
MvnDff::clock_pol() const
{
  return (mPolArray[0] & 1U) ? MvnPolarity::Positive : MvnPolarity::Negative;
}

// @brief 非同期セット信号の極性を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
MvnPolarity
MvnDff::control_pol(int pos) const
{
  SizeType blk = (pos + 1) / 32;
  SizeType sft = (pos + 1) % 32;
  return ((mPolArray[blk] >> sft) & 1U) ? MvnPolarity::Positive : MvnPolarity::Negative;
}

// @brief 非同期セットの値を表す定数ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @note デフォルトの実装では nullptr を返す．
const MvnNode*
MvnDff::control_val(int pos) const
{
  return mValArray[pos];
}

// @brief 非同期セット/リセットタイプの FF ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] pol_array 非同期セット信号の極性情報を入れた配列
// @param[in] val_array 非同期セットの値を入れた配列
// @param[in] bit_width ビット幅
// @note control_array の要素数が非同期セット信号数となる．
MvnNode*
MvnMgr::new_dff(MvnModule* module,
		MvnPolarity clock_pol,
		const vector<MvnPolarity>& pol_array,
		const vector<MvnNode*>& val_array,
		SizeType bit_width)
{
  auto node{new MvnDff(module, clock_pol, pol_array, val_array)};
  reg_node(node);

  SizeType np{pol_array.size()};

  // データ入力
  node->_input(0)->mBitWidth = bit_width;
  // クロック入力
  node->_input(1)->mBitWidth = 1;
  // 非同期セット入力
  for ( SizeType i = 0; i < np; ++ i ) {
    node->_input(i + 2)->mBitWidth = 1;
  }
  // データ出力
  node->mBitWidth = bit_width;

  return node;
}

END_NAMESPACE_YM_MVN
