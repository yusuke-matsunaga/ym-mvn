
/// @file MvnNodeBase.cc
/// @brief MvnNodeBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"
#include "ym/MvnMgr.h"
#include "ym/MvnBvConst.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnNodeBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] type 型
// @param[in] input_num 入力数
// @param[in] output_num 出力数
MvnNodeBase::MvnNodeBase(MvnModule* parent,
			 MvnNodeType type,
			 SizeType input_num) :
  MvnNode{parent},
  mType{type},
  mInputNum{input_num},
  mInputArray{new MvnInputPin[input_num]}
{
  for ( SizeType i = 0; i < input_num; ++ i ) {
    mInputArray[i].init(this, i);
  }
}

// @brief デストラクタ
MvnNodeBase::~MvnNodeBase()
{
  delete [] mInputArray;
}

// @brief ノードの種類を得る．
MvnNodeType
MvnNodeBase::type() const
{
  return mType;
}

// @brief 入力ピン数を得る．
SizeType
MvnNodeBase::input_num() const
{
  return mInputNum;
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
const MvnInputPin*
MvnNodeBase::input(int pos) const
{
  ASSERT_COND( 0 < pos && pos < mInputNum );
  return mInputArray + pos;
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvnInputPin*
MvnNodeBase::_input(int pos)
{
  ASSERT_COND( pos >= 0 && pos < mInputNum );
  return mInputArray + pos;
}

// @brief クロック信号の極性を得る．
MvnPolarity
MvnNodeBase::clock_pol() const
{
  return MvnPolarity::Positive;
}

// @brief 非同期セット信号の極性を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
MvnPolarity
MvnNodeBase::control_pol(int pos) const
{
  return MvnPolarity::Positive;
}

// @brief 非同期セットの値を表す定数ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @note デフォルトの実装では nullptr を返す．
const MvnNode*
MvnNodeBase::control_val(int pos) const
{
  return nullptr;
}

// @brief ビット位置を得る．
// @note type() が kConstBitSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
SizeType
MvnNodeBase::bitpos() const
{
  return 0;
}

// @brief 範囲指定の MSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
SizeType
MvnNodeBase::msb() const
{
  return 0;
}

// @brief 範囲指定の LSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
SizeType
MvnNodeBase::lsb() const
{
  return 0;
}

// @brief 定数値を得る．
// @return 値を返す．
// @note type() が CONST の時のみ意味を持つ．
// @note デフォルトの実装ではなにもしない．
MvnBvConst
MvnNodeBase::const_value() const
{
  return MvnBvConst();
}

// @brief Xマスクを得る．
// @param[out] val 値を格納するベクタ
// @note type() が kEqX の時のみ意味を持つ．
// @note デフォルトの実装ではなにもしない．
MvnBvConst
MvnNodeBase::xmask() const
{
  return MvnBvConst();
}

// @brief セル番号を得る．
// @note type() が kCell の時のみ意味をモツ．
// @note デフォルトの実装では nullptr を返す．
int
MvnNodeBase::cell_id() const
{
  return -1;
}

// @brief セルの出力ピン番号を返す．
// @note type() が kCell の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
int
MvnNodeBase::cell_opin_pos() const
{
  return 0;
}

// @brief 多出力セルノードの場合の代表ノードを返す．
// @note type() が kCell の時のみ意味を持つ．
// @note 1出力セルノードの時には自分自身を返す．
// @note デフォルトの実装では nullptr を返す．
const MvnNode*
MvnNodeBase::cell_node() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// MvnMgr のノード生成関数
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_input(MvnModule* module,
		  SizeType bit_width)
{
  auto node{new MvnNodeBase(module, MvnNodeType::INPUT, 0)};
  reg_node(node);
  node->mBitWidth = bit_width;
  return node;
}

// @brief 出力ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_output(MvnModule* module,
		   SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::OUTPUT,
			   bit_width, bit_width);
  return node;
}

// @brief 入出力ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_inout(MvnModule* module,
		  SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::INOUT,
			   bit_width, bit_width);
  return node;
}

// @brief ラッチノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_latch(MvnModule* module,
		  SizeType bit_width)
{
  auto node = new_binary_op(module, MvnNodeType::LATCH,
			    bit_width, 1, bit_width);
  return node;
}

// @brief through ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_through(MvnModule* module,
		    SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::THROUGH,
			       bit_width, bit_width);
  return node;
}

// @brief not ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_not(MvnModule* module,
		SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::NOT,
			   bit_width, bit_width);
  return node;
}

// @brief and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_and(MvnModule* module,
		SizeType input_num,
		SizeType bit_width)
{
  auto node = new_log_op(module, MvnNodeType::AND,
			 input_num, bit_width);
  return node;
}

// @brief or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_or(MvnModule* module,
	       SizeType input_num,
	       SizeType bit_width)
{
  auto node = new_log_op(module, MvnNodeType::OR,
			 input_num, bit_width);
  return node;
}

// @brief xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_xor(MvnModule* module,
		SizeType input_num,
		SizeType bit_width)
{
  auto node = new_log_op(module, MvnNodeType::XOR,
			 input_num, bit_width);
  return node;
}

// @brief reduction and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_rand(MvnModule* module,
		 SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::RAND,
			   bit_width, 1);
  return node;
}

// @brief reduction or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_ror(MvnModule* module,
		SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::ROR,
			   bit_width, 1);
  return node;
}

// @brief reduction xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_rxor(MvnModule* module,
		 SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::RXOR,
			   bit_width, 1);
  return node;
}

// @brief cmpl ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
// @note 2の補数を計算する．
MvnNode*
MvnMgr::new_cmpl(MvnModule* module,
		 SizeType bit_width)
{
  auto node = new_unary_op(module, MvnNodeType::CMPL,
			   bit_width, bit_width);
  return node;
}

// @brief add ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_add(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::ADD,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief sub ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sub(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::SUB,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief mult ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_mult(MvnModule* module,
		 SizeType bit_width1,
		 SizeType bit_width2,
		 SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::MUL,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief div ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_div(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::DIV,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief modulo ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_mod(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::MOD,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief power ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_pow(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::POW,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief shift left logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sll(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::SLL,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief shift right logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_srl(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::SRL,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief shift left arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sla(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::SLA,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief shift right arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sra(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2,
		SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::SRA,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief equal ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_equal(MvnModule* module,
		  SizeType bit_width)
{
  auto node = new_binary_op(module, MvnNodeType::EQ,
			    bit_width, bit_width, 1);
  return node;
}

// @brief less than ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_lt(MvnModule* module,
	       SizeType bit_width)
{
  auto node = new_binary_op(module, MvnNodeType::LT,
			    bit_width, bit_width, 1);
  return node;
}

// @brief 可変 bit-selectノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 ビット指定入力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_bitselect(MvnModule* module,
		      SizeType bit_width1,
		      SizeType bit_width2)
{
  auto node = new_binary_op(module, MvnNodeType::BITSELECT,
			    bit_width1, bit_width2, 1);
  return node;
}

// @brief 可変 part-select ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 範囲指定入力のビット幅
// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_partselect(MvnModule* module,
		       SizeType bit_width1,
		       SizeType bit_width2,
		       SizeType bit_width3)
{
  auto node = new_binary_op(module, MvnNodeType::PARTSELECT,
			    bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief condition ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_ite(MvnModule* module,
		SizeType bit_width)
{
  auto node = new_ternary_op(module, MvnNodeType::ITE,
			     1, bit_width, bit_width, bit_width);
  return node;
}

// @brief concatenate ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ibitwidth_array 入力のビット幅の配列
// @return 生成したノードを返す．
// @note 出力のビット幅は入力のビット幅の和となる．
MvnNode*
MvnMgr::new_concat(MvnModule* module,
		   const vector<SizeType>& ibitwidth_array)
{
  SizeType ni{ibitwidth_array.size()};
  SizeType obitwidth = 0;
  for ( SizeType i = 0; i < ni; ++ i ) {
    obitwidth += ibitwidth_array[i];
  }
  return new_nary_op(module, MvnNodeType::CONCAT, ibitwidth_array, obitwidth);
}

// @brief 多入力論理演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_log_op(MvnModule* module,
		   MvnNodeType type,
		   SizeType input_num,
		   SizeType bit_width)
{
  vector<SizeType> ibitwidth_array(input_num);
  for ( SizeType i = 0; i < input_num; ++ i ) {
    ibitwidth_array[i] = bit_width;
  }
  return new_nary_op(module, type, ibitwidth_array, bit_width);
}

// @brief 1入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 出力のビット幅
MvnNode*
MvnMgr::new_unary_op(MvnModule* module,
		     MvnNodeType type,
		     SizeType bit_width1,
		     SizeType bit_width2)
{
  auto node{new MvnNodeBase(module, type, 1)};
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width1;
  node->mBitWidth = bit_width2;

  return node;
}

// @brief 2入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnNode*
MvnMgr::new_binary_op(MvnModule* module,
		      MvnNodeType type,
		      SizeType bit_width1,
		      SizeType bit_width2,
		      SizeType bit_width3)
{
  auto node{new MvnNodeBase(module, type, 2)};
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width1;
  node->_input(1)->mBitWidth = bit_width2;
  node->mBitWidth = bit_width3;

  return node;
}

// @brief 3入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 入力3のビット幅
// @param[in] bit_width4 出力のビット幅
MvnNode*
MvnMgr::new_ternary_op(MvnModule* module,
		       MvnNodeType type,
		       SizeType bit_width1,
		       SizeType bit_width2,
		       SizeType bit_width3,
		       SizeType bit_width4)
{
  auto node{new MvnNodeBase(module, type, 3)};
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width1;
  node->_input(1)->mBitWidth = bit_width2;
  node->_input(2)->mBitWidth = bit_width3;
  node->mBitWidth = bit_width4;

  return node;
}

// @brief 多入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] ibit_width_array 入力のビット幅の幅
// @param[in] obit_width 出力のビット幅
MvnNode*
MvnMgr::new_nary_op(MvnModule* module,
		    MvnNodeType type,
		    const vector<SizeType>& ibit_width_array,
		    SizeType obit_width)
{
  SizeType ni{ibit_width_array.size()};
  auto node{new MvnNodeBase(module, type, ni)};
  reg_node(node);

  for ( SizeType i = 0; i < ni; ++ i ) {
    node->_input(i)->mBitWidth = ibit_width_array[i];
  }
  node->mBitWidth = obit_width;

  return node;
}

END_NAMESPACE_YM_MVN
