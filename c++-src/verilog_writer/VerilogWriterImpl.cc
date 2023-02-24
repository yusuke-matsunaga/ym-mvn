
/// @file VerilogWriterImpl.cc
/// @brief VerilogWriterImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

/*
 * アルゴリズム
 *
 * - MvnNode の出力ピンをネットに対応させる．
 * - ほとんどのノードはただ一つの出力を持つが，セルの場合は複数出力の場合がありうる．
 * - 今は使っていないが module instance も複数出力がありうる．
 * - 組み合わせ回路に対応するノードの場合，assign 文に対応させる．
 * - D-FF とラッチは always 文に対応させる．
 */

#include "VerilogWriterImpl.h"

#include "ym/MvnMgr.h"
#include "ym/MvnModule.h"
#include "ym/MvnPort.h"
#include "ym/MvnNode.h"
#include "ym/MvnInputPin.h"
#include "ym/MvnBvConst.h"
#include "ym/MvnVlMap.h"

#include "ym/ClibCell.h"
#include "ym/ClibPin.h"

#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス VerilogWriterImpl
//////////////////////////////////////////////////////////////////////

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
void
VerilogWriterImpl::dump(
  ostream& s,
  const MvnMgr& mgr
)
{
  mNameArray.clear();
  mNameArray.resize(mgr.max_node_id(), string());
  SizeType n{mgr.max_module_id()};
  for ( SizeType i = 0; i < n; ++ i ) {
    auto module{mgr.module(i)};
    if ( module != nullptr ) {
      dump_module(s, module, mgr);
    }
  }
}

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
// @param[in] node_map ノードと Verilog 名の対応表
void
VerilogWriterImpl::dump(
  ostream& s,
  const MvnMgr& mgr,
  const MvnVlMap& node_map
)
{
  dump(s, mgr);

  SizeType node_num{mgr.max_node_id()};
  for ( SizeType i = 0; i < node_num; ++ i ) {
    auto node = mgr.node(i);
    if ( node == nullptr ) continue;

    s << "// node" << node->id() << " : ";
    if ( node_map.is_single_elem(i) ) {
      auto decl{node_map.get_single_elem(i)};
      ASSERT_COND( decl != nullptr );
      s << decl->full_name();
    }
    else if ( node_map.is_array_elem(i) ) {
      auto declarray{node_map.get_array_elem(i)};
      ASSERT_COND( declarray != nullptr );
      SizeType offset{node_map.get_array_offset(i)};
      SizeType d = declarray->dimension();
      vector<int> index_array(d);
      for ( SizeType i = 0; i < d; ++ i ) {
	auto range = declarray->range(i);
	SizeType n = range->size();
	index_array[i] = offset % n;
	offset /= n;
      }
      s << declarray->full_name();
      for ( SizeType i = 0; i < d; ++ i ) {
	s << "[" << index_array[d - i - 1] << "]";
      }
    }
    s << endl;
  }
}

void
VerilogWriterImpl::dump_module(
  ostream& s,
  const MvnModule* module,
  const MvnMgr& mgr
)
{
  s << "module " << module->name() << "(";

  SizeType np{module->port_num()};
  const char* comma = "";
  for ( SizeType i = 0; i < np; ++ i ) {
    s << comma;
    comma = ", ";
    auto port{module->port(i)};
    dump_port(s, port);
  }

  s << ");" << endl;

  SizeType ni{module->input_num()};
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto node{module->input(i)};
    SizeType bw{node->bit_width()};
    ASSERT_COND( bw > 0 );
    if ( bw == 1 ) {
      s << "  input ";
    }
    else {
      s << "  input [" << bw - 1 << ":0] ";
    }
    s << node_name(node) << ";" << endl;
  }

  SizeType no{module->output_num()};
  for ( SizeType i = 0; i < no; ++ i ) {
    auto node{module->output(i)};
    SizeType bw{node->input(0)->bit_width()};
    ASSERT_COND( bw > 0 );
    if ( bw == 1 ) {
      s << "  output ";
    }
    else {
      s << "  output [" << bw - 1 << ":0] ";
    }
    s << node_name(node) << ";" << endl;
  }

  SizeType nio{module->inout_num()};
  for ( SizeType i = 0; i < nio; ++ i ) {
    auto node = module->inout(i);
    SizeType bw{node->input(0)->bit_width()};
    ASSERT_COND( bw > 0 );
    if ( bw == 1 ) {
      s << "  inout ";
    }
    else {
      s << "  inout [" << bw - 1 << ":0] ";
    }
    s << node_name(node) << ";" << endl;
  }
  s << endl;

  auto& node_list{module->node_list()};
  for ( auto node: node_list ) {
    SizeType bw{node->bit_width()};
    if ( node->type() == MvnNodeType::DFF || node->type() == MvnNodeType::LATCH ) {
      s << "  reg  ";
      if ( bw > 1 ) {
	s << "[" << bw - 1 << ":" << "0]";
      }
      s << " " << node_name(node) << ";" << endl;
    }
    else {
      s << "  wire ";
      if ( bw > 1 ) {
	s << "[" << bw - 1 << ":" << "0]";
      }
      s << " " << node_name(node) << ";" << endl;
    }
  }
  s << endl;

  for ( SizeType i = 0; i < ni; ++ i ) {
    auto node{module->input(i)};
    dump_node(s, node, mgr);
  }
  for ( SizeType i = 0; i < no; ++ i ) {
    auto node{module->output(i)};
    dump_node(s, node, mgr);
  }
  for ( SizeType i = 0; i < nio; ++ i ) {
    auto node{module->inout(i)};
    dump_node(s, node, mgr);
  }
  for ( auto node: node_list ) {
    dump_node(s, node, mgr);
  }

  s << "endmodule" << endl
    << endl;
}

void
VerilogWriterImpl::dump_port(
  ostream& s,
  const MvnPort* port
)
{
  string port_name{port->name()};
  SizeType n{port->port_ref_num()};
  if ( n == 1 ) {
    auto& port_ref{port->port_ref(0)};
    if ( !port_ref.has_bitselect() && !port_ref.has_partselect() ) {
      s << port_name;
      set_node_name(port_ref.node(), port_name);
      return;
    }
  }

  s << "." << port_name << "(";
  if ( n == 1 ) {
    auto& port_ref{port->port_ref(0)};
    dump_port_ref(s, port_ref);
  }
  else if ( n > 1 ) {
    s << "{";
    const char* comma = "";
    for ( SizeType i = 0; i < n; ++ i ) {
      s << comma;
      comma = ", ";
      auto& port_ref{port->port_ref(i)};
      dump_port_ref(s, port_ref);
    }
    s << "}";
  }
  s << ")";
}

void
VerilogWriterImpl::dump_port_ref(
  ostream& s,
  const MvnPortRef& port_ref
)
{
  s << node_name(port_ref.node());
  if ( port_ref.has_bitselect() ) {
    s << "[" << port_ref.bitpos() << "]";
  }
  else if ( port_ref.has_partselect() ) {
    s << "[" << port_ref.msb() << ":" << port_ref.lsb() << "]";
  }
}

void
VerilogWriterImpl::dump_node(
  ostream& s,
  const MvnNode* node,
  const MvnMgr& mgr
)
{
  switch ( node->type() ) {
  case MvnNodeType::INPUT:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 0 );
    }
    break;

  case MvnNodeType::OUTPUT:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 1 );

      auto ipin{node->input(0)};
      auto src_node{ipin->src_node()};
      if ( src_node ) {
	s << "  assign " << node_name(node)
	  << " = " << node_name(src_node)
	  << ";" << endl;
      }
    }
    break;

  case MvnNodeType::INOUT:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 1 );

      auto ipin{node->input(0)};
      auto src_node{ipin->src_node()};
      if ( src_node ) {
	s << "  assign " << node_name(node)
	  << " = " << node_name(src_node)
	  << ";" << endl;
      }
    }
    break;

  case MvnNodeType::DFF:
    { // ピン位置と属性は決め打ち
      SizeType ni{node->input_num()};
      ASSERT_COND( ni >= 2 );

      auto ipin0{node->input(0)};
      auto src_node0{ipin0->src_node()};
      ASSERT_COND( src_node0 != nullptr );
      auto ipin1{node->input(1)};
      auto src_node1{ipin1->src_node()};
      ASSERT_COND( src_node1 != nullptr );

      s << "  always @ ( ";
      if ( node->clock_pol() == MvnPolarity::Positive ) {
	s << "posedge";
      }
      else {
	s << "negedge";
      }
      s << " " << node_name(src_node1);
      SizeType nc{ni - 2};
      for ( SizeType i = 0; i < nc; ++ i ) {
	auto ipin2{node->input(i + 2)};
	auto src_node2{ipin2->src_node()};
	auto polstr{node->control_pol(i) == MvnPolarity::Positive? "posedge" : "negedge"};
	s << " or " << polstr << " " << node_name(src_node2);
      }
      s << " )" << endl;
      auto elif{"if"};
      for ( SizeType i = 0; i < nc; ++ i ) {
	auto not_str{""};
	if ( node->control_pol(i) == MvnPolarity::Negative ) {
	  not_str = "!";
	}
	auto ipin2{node->input(i + 2)};
	auto src_node2{ipin2->src_node()};
	auto src_node3{node->control_val(i)};
	s << "    " << elif << " ( "
	  << not_str << node_name(src_node2) << " )" << endl
	  << "      " << node_name(node) << " <= "
	  << node_name(src_node3) << ";" << endl;
	elif = "else if";
      }
      if ( nc > 0 ) {
	s << "    else" << endl
	  << "  ";
      }
      s << "    " << node_name(node) << " <= "
	<< node_name(src_node0) << ";" << endl
	<< endl;
    }
    break;

  case MvnNodeType::LATCH:
    { // ピン位置と属性は決め打ち
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 2 );

      auto ipin0{node->input(0)};
      auto src_node0{ipin0->src_node()};
      ASSERT_COND( src_node0 != nullptr );
      auto ipin1{node->input(1)};
      auto src_node1{ipin1->src_node()};
      ASSERT_COND( src_node1 != nullptr );

      s << "  always @ ( * )" << endl
	<< "    if ( " << node_name(src_node1) << " )" << endl
	<< "      " << node_name(node) << " <= "
	<< node_name(src_node0) << ";" << endl
	<< endl;
    }
    break;

  case MvnNodeType::THROUGH:
    dump_uop(s, node, "");
    break;

  case MvnNodeType::NOT:
    dump_uop(s, node, "~");
    break;

  case MvnNodeType::AND:
    dump_nop(s, node, "&");
    break;

  case MvnNodeType::OR:
    dump_nop(s, node, "|");
    break;

  case MvnNodeType::XOR:
    dump_nop(s, node, "^");
    break;

  case MvnNodeType::RAND:
    dump_uop(s, node, "&");
    break;

  case MvnNodeType::ROR:
    dump_uop(s, node, "|");
    break;

  case MvnNodeType::RXOR:
    dump_uop(s, node, "^");
    break;

  case MvnNodeType::EQ:
    dump_binop(s, node, "==", true);
    break;

  case MvnNodeType::LT:
    dump_binop(s, node, "<", true);
    break;

  case MvnNodeType::CASEEQ:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 2 );

      auto ipin0{node->input(0)};
      auto src_node0{ipin0->src_node()};

      auto ipin1{node->input(1)};
      auto src_node1{ipin1->src_node()};

      auto xmask{node->xmask()};
      SizeType bw{ipin0->bit_width()};
      string mask_str;
      for ( SizeType i = 0; i < bw; ++ i ) {
	SizeType bitpos = bw - i - 1;
	SizeType blk = bitpos / 32;
	SizeType sft = bitpos % 32;
	if ( xmask[blk] & (1U << sft) ) {
	  mask_str += "?";
	}
	else {
	  mask_str += "0";
	}
      }
      s << "  assign " << node_name(node)
	<< " = (" << node_name(src_node0)
	<< " === " << node_name(src_node1)
	<< " ^ " << bw << "'b" << mask_str
	<< ");" << endl;
    }
    break;

  case MvnNodeType::SLL:
    dump_binop(s, node, "<<");
    break;

  case MvnNodeType::SRL:
    dump_binop(s, node, ">>");
    break;

  case MvnNodeType::SLA:
    dump_binop(s, node, "<<<");
    break;

  case MvnNodeType::SRA:
    dump_binop(s, node, ">>>");
    break;

  case MvnNodeType::ADD:
    dump_binop(s, node, "+");
    break;

  case MvnNodeType::SUB:
    dump_binop(s, node, "-");
    break;

  case MvnNodeType::MUL:
    dump_binop(s, node, "*");
    break;

  case MvnNodeType::DIV:
    dump_binop(s, node, "/");
    break;

  case MvnNodeType::MOD:
    dump_binop(s, node, "%");
    break;

  case MvnNodeType::POW:
    dump_binop(s, node, "**");
    break;

  case MvnNodeType::ITE:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 3 );

      auto ipin0{node->input(0)};
      auto src_node0{ipin0->src_node()};

      auto ipin1{node->input(1)};
      auto src_node1{ipin1->src_node()};

      auto ipin2{node->input(2)};
      auto src_node2{ipin2->src_node()};

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " ? " << node_name(src_node1)
	<< " : " << node_name(src_node2)
	<< ";" << endl;
    }
    break;

  case MvnNodeType::CONCAT:
    {
      s << "  assign " << node_name(node)
	<< " = {";
      const char* comma = "";
      SizeType ni{node->input_num()};
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto ipin{node->input(i)};
	auto src_node{ipin->src_node()};
	s << comma << node_name(src_node);
	comma = ", ";
      }
      s << "};" << endl;
    }
    break;

  case MvnNodeType::CONSTBITSELECT:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 1 );

      auto ipin{node->input(0)};
      auto src_node{ipin->src_node()};
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->bitpos() << "];" << endl;
    }
    break;

  case MvnNodeType::CONSTPARTSELECT:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 1 );

      auto ipin{node->input(0)};
      auto src_node{ipin->src_node()};
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->msb()
	<< ":" << node->lsb()
	<< "];" << endl;
    }
    break;

  case MvnNodeType::BITSELECT:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 2 );

      auto ipin{node->input(0)};
      auto src_node{ipin->src_node()};

      auto ipin1{node->input(1)};
      auto src_node1{ipin1->src_node()};

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1) << "];" << endl;
    }
    break;

  case MvnNodeType::PARTSELECT:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 2 );

      auto ipin{node->input(0)};
      auto src_node{ipin->src_node()};

      auto ipin1{node->input(1)};
      auto src_node1{ipin1->src_node()};

      auto ipin2{node->input(2)};
      auto src_node2{ipin2->src_node()};

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1)
	<< ":" << node_name(src_node2)
	<< "];" << endl;
    }
    break;

  case MvnNodeType::CONSTVALUE:
    {
      SizeType ni{node->input_num()};
      ASSERT_COND( ni == 0 );

      SizeType bw{node->bit_width()};
      s << "  assign " << node_name(node)
	<< " = " << bw << "'b";
      auto cv{node->const_value()};
      for ( SizeType i = 0; i < bw; ++ i ) {
	SizeType idx = bw - i - 1;
	SizeType blk = idx / 32;
	SizeType sft = idx % 32;
	if ( (cv[blk] >> sft) & 1 ) {
	  s << "1";
	}
	else {
	  s << "0";
	}
      }
      s << ";" << endl;
    }
    break;

  case MvnNodeType::CELL:
    {
      auto cell = node->cell();
      SizeType ni{cell.input_num()};
      SizeType no{cell.output_num()};
      SizeType nio{cell.inout_num()};
      ASSERT_COND( no == 1 );
      ASSERT_COND( nio == 0 );

      s << cell.name() << " " << node_name(node)
	<< " (";

      // 出力
      s << "." << cell.output(0).name()
	<< "(" << node_name(node) << ")";

      // 入力
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto pin = cell.input(i);
	auto ipin = node->input(i);
	auto src_node = ipin->src_node();

	s << ", ." << pin.name()
	  << "(" << node_name(src_node) << ")";
      }

      s << ");" << endl;
    }
    break;

  default:
    break;
  }
}

void
VerilogWriterImpl::dump_uop(
  ostream& s,
  const MvnNode* node,
  const char* opr_str
)
{
  SizeType ni{node->input_num()};
  ASSERT_COND( ni == 1 );

  auto ipin{node->input(0)};
  auto src_node{ipin->src_node()};
  s << "  assign " << node_name(node)
    << " = " << opr_str << node_name(src_node)
    << ";" << endl;
}

void
VerilogWriterImpl::dump_binop(
  ostream& s,
  const MvnNode* node,
  const char* opr_str,
  bool need_paren
)
{
  SizeType ni{node->input_num()};
  ASSERT_COND( ni == 2 );

  auto ipin0{node->input(0)};
  auto src_node0{ipin0->src_node()};

  auto ipin1{node->input(1)};
  auto src_node1{ipin1->src_node()};

  const char* lp = "";
  const char* rp = "";
  if ( need_paren ) {
    lp = "(";
    rp = ")";
  }

  s << "  assign " << node_name(node) << " = "
    << lp << node_name(src_node0)
    << " " << opr_str << " " << node_name(src_node1)
    << rp << ";" << endl;
}

void
VerilogWriterImpl::dump_nop(
  ostream& s,
  const MvnNode* node,
  const char* opr_str
)
{
  SizeType ni{node->input_num()};
  ASSERT_COND( ni >= 2 );

  auto ipin0{node->input(0)};
  auto src_node0{ipin0->src_node()};

  s << "  assign " << node_name(node)
    << " = " << node_name(src_node0);
  for ( SizeType i = 1; i < ni; ++ i ) {
    auto ipin1{node->input(i)};
    auto src_node1{ipin1->src_node()};
    s << " " << opr_str << " " << node_name(src_node1);
  }
  s << ";" << endl;
}

string
VerilogWriterImpl::node_name(
  const MvnNode* node
)
{
  auto name{mNameArray[node->id()]};
  if ( name == string() ) {
    ostringstream buf;
    buf << "node" << node->id();
    name = buf.str();
  }
  return name;
}

void
VerilogWriterImpl::set_node_name(
  const MvnNode* node,
  const string& name
)
{
  mNameArray[node->id()] = name;
}

END_NAMESPACE_YM_MVN
