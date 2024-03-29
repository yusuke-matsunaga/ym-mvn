﻿
/// @file NodeDumper.cc
/// @brief NodeDumper とその継承クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "NodeDumper.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス UnaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
UnaryOpDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  ymuint ni = node->input_num();
  ASSERT_COND( ni == 1 );
  ymuint no = node->output_num();
  ASSERT_COND( no == 1 );

  const MvnInputPin* ipin = node->input(0);
  const MvnOutputPin* src_pin = ipin->src_pin();
  ASSERT_COND( src_pin != nullptr );

  s << mOprStr << net_name(src_pin);
}


//////////////////////////////////////////////////////////////////////
// クラス BinaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
BinaryOpDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  ymuint ni = node->input_num();
  ASSERT_COND( ni == 2 );
  ymuint no = node->output_num();
  ASSERT_COND( no == 1 );

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  ASSERT_COND( src_pin0 != nullptr );

  const MvnInputPin* ipin1 = node->input(1);
  const MvnOutputPin* src_pin1 = ipin1->src_pin();
  ASSERT_COND( src_pin1 != nullptr );

  const char* lp = "";
  const char* rp = "";
  if ( mNeedParen ) {
    lp = "(";
    rp = ")";
  }

  s << lp << net_name(src_pin0)
    << " " << mOprStr << " "
    << net_name(src_pin1) << rp;
}


//////////////////////////////////////////////////////////////////////
// クラス TernaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
TernaryOpDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  ymuint ni = node->input_num();
  ASSERT_COND( ni == 3 );
  ymuint no = node->output_num();
  ASSERT_COND( no == 1 );

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  ASSERT_COND( src_pin0 != nullptr );

  const MvnInputPin* ipin1 = node->input(1);
  const MvnOutputPin* src_pin1 = ipin1->src_pin();
  ASSERT_COND( src_pin1 != nullptr );

  const MvnInputPin* ipin2 = node->input(2);
  const MvnOutputPin* src_pin2 = ipin2->src_pin();
  ASSERT_COND( src_pin2 != nullptr );

  s << net_name(src_pin0) << " " << mOprStr1 << " "
    << net_name(src_pin1) << " " << mOprStr2 << " "
    << net_name(src_pin2);
}


//////////////////////////////////////////////////////////////////////
// クラス NaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
NaryOpDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  ymuint ni = node->input_num();
  ASSERT_COND( ni >= 2 );
  ymuint no = node->output_num();
  ASSERT_COND( no == 1 );

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  ASSERT_COND( src_pin0 != nullptr );

  s << net_name(src_pin0);
  for (ymuint i = 1; i < ni; ++ i) {
    const MvnInputPin* ipin1 = node->input(i);
    const MvnOutputPin* src_pin1 = ipin1->src_pin();
    ASSERT_COND( src_pin1 != nullptr );
    s << " " << mOprStr << " " << net_name(src_pin1);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ConcatDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
ConcatDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  ymuint no = node->output_num();
  ASSERT_COND( no == 1 );

  s << "{";
  const char* comma = "";
  ymuint ni = node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvnInputPin* ipin = node->input(i);
    const MvnOutputPin* opin = ipin->src_pin();
    ASSERT_COND( opin != nullptr );

    s << comma << net_name(opin);
    comma = ", ";
  }
  s << "};" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス CaseEqDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
CaseEqDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  #warning "TODO: 未完"
}


//////////////////////////////////////////////////////////////////////
// クラス ConstBitSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
ConstBitSelectDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  #warning "TODO: 未完"
}


//////////////////////////////////////////////////////////////////////
// クラス ConstPartSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
ConstPartSelectDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  #warning "TODO: 未完"
}


//////////////////////////////////////////////////////////////////////
// クラス BitSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
BitSelectDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  #warning "TODO: 未完"
}


//////////////////////////////////////////////////////////////////////
// クラス PartSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
PartSelectDumper::operator()(
  ostream& s,
  const MvnNode* node
)
{
  #warning "TODO: 未完"
}


END_NAMESPACE_YM_MVN
