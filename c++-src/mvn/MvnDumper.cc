
/// @file MvnDumper.cc
/// @brief MvnDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MvnDumper.h"

#include "ym/MvnMgr.h"

#include "ym/MvnModule.h"
#include "ym/MvnPort.h"
#include "ym/MvnNode.h"
#include "ym/MvnInputPin.h"
#include "ym/MvnBvConst.h"

#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

// ノードを表す文字列を返す．
string
node_idstr(
  const MvnNode* node
)
{
  ostringstream buf;
  buf << "Node[" << node->id() << "]";
  return buf.str();
}

// MvnInputPin の内容を出力する．
void
dump_inputpin(
  ostream& s,
  const MvnInputPin* pin,
  const string& pin_name
)
{
  s << "  " << pin_name
    << "(" << pin->bit_width() << ")" << endl;
  auto onode = pin->src_node();
  if ( onode ) {
    s << "    <== Output@" << node_idstr(onode) << endl;
  }
}

// MvnInputPin の内容を出力する．
void
dump_inputpin(
  ostream& s,
  const MvnInputPin* pin
)
{
  ostringstream buf;
  buf << "InputPin#" << pin->pos();
  string pin_name = buf.str();
  dump_inputpin(s, pin, pin_name);
}

// MvnNode の内容を出力する．
void
dump_node(
  ostream& s,
  const MvnNode* node,
  const MvnMgr& mgr
)
{
  s << node_idstr(node) << " : ";
  switch ( node->type() ) {
  case MvnNodeType::INPUT:      s << "Input"; break;
  case MvnNodeType::INOUT:      s << "Inout"; break;
  case MvnNodeType::OUTPUT:     s << "Output"; break;
  case MvnNodeType::DFF:        s << "DFF"; break;
  case MvnNodeType::LATCH:      s << "Latch"; break;
  case MvnNodeType::THROUGH:    s << "Through"; break;
  case MvnNodeType::NOT:        s << "Not"; break;
  case MvnNodeType::AND:        s << "And"; break;
  case MvnNodeType::OR:         s << "Or"; break;
  case MvnNodeType::XOR:        s << "Xor"; break;
  case MvnNodeType::RAND:       s << "Rand"; break;
  case MvnNodeType::ROR:        s << "Ror"; break;
  case MvnNodeType::RXOR:       s << "Rxor"; break;
  case MvnNodeType::EQ:         s << "Eq"; break;
  case MvnNodeType::LT:         s << "Lt"; break;
  case MvnNodeType::CASEEQ:
    {
      s << "CaseEq[";
      auto xmask = node->xmask();
      SizeType bw{node->input(0)->bit_width()};
      for ( SizeType i = 0; i < bw; ++ i ) {
	SizeType bitpos = bw - i - 1;
	if ( xmask[bitpos] ) {
	  s << "-";
	}
	else {
	  s << "1";
	}
      }
      s << "]";
    }
    break;
  case MvnNodeType::SLL:        s << "Sll"; break;
  case MvnNodeType::SRL:        s << "Srl"; break;
  case MvnNodeType::SLA:        s << "Sla"; break;
  case MvnNodeType::SRA:        s << "Sra"; break;
  case MvnNodeType::ADD:        s << "Add"; break;
  case MvnNodeType::SUB:        s << "Sub"; break;
  case MvnNodeType::MUL:        s << "Mult"; break;
  case MvnNodeType::DIV:        s << "Div"; break;
  case MvnNodeType::MOD:        s << "Mod"; break;
  case MvnNodeType::POW:        s << "Pow"; break;
  case MvnNodeType::ITE:        s << "Ite"; break;
  case MvnNodeType::CONCAT:     s << "Concat"; break;
  case MvnNodeType::CONSTBITSELECT:
    s << "ConstBitSelect["
      << node->bitpos()
      << "]";
    break;
  case MvnNodeType::CONSTPARTSELECT:
    s << "ConstPartSelect["
      << node->msb()
      << ":"
      << node->lsb()
      << "]";
    break;
  case MvnNodeType::BITSELECT:  s << "BitSelect"; break;
  case MvnNodeType::PARTSELECT: s << "PartSelect"; break;
  case MvnNodeType::CONSTVALUE:
    {
      auto val = node->const_value();
      s << "Const(" << val << ")";
    }
    break;
  case MvnNodeType::CELL:
    {
      auto cell = node->cell();
      s << "Cell(" << cell.name() << ")";
    }
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  s << endl;

  if ( node->type() == MvnNodeType::DFF ) {
    const MvnInputPin* input = node->input(0);
    dump_inputpin(s, input, "DataInput");
    const MvnInputPin* clock = node->input(1);
    dump_inputpin(s, clock, "Clock");
    s << "    ";
    if ( node->clock_pol() == MvnPolarity::Positive ) {
      s << "posedge";
    }
    else {
      s << "negedge";
    }
    s << endl;
    SizeType ni{node->input_num()};
    SizeType nc{ni - 2};
    for ( SizeType i = 0; i < nc; ++ i ) {
      auto cpin{node->input(i + 2)};
      ostringstream buf;
      buf << "Control#" << i;
      string pin_name = buf.str();
      dump_inputpin(s, cpin, pin_name);
      s << "    ";
      if ( node->control_pol(i) == MvnPolarity::Positive ) {
	s << "posedge";
      }
      else {
	s << "negedge";
      }
      s << endl;
      const MvnNode* dnode = node->control_val(i);
      s << "  Data#" << i << " <== " << node_idstr(dnode) << endl;
    }
  }
  else if ( node->type() == MvnNodeType::LATCH ) {
    #warning "TODO: 未完"
  }
  else {
    SizeType ni{node->input_num()};
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto pin{node->input(i)};
      dump_inputpin(s, pin);
    }
    s << "  Output(" << node->bit_width() << ")" << endl;
    for ( auto ipin: node->dst_pin_list() ) {
      s << "    ==> InputPin#" << ipin->pos()
	<< "@" << node_idstr(ipin->node()) << endl;
    }
  }
  s << endl;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MvnDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnDumper::MvnDumper()
{
}

// @brief デストラクタ
MvnDumper::~MvnDumper()
{
}

// @brief 内容を出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
void
MvnDumper::operator()(
  ostream& s,
  const MvnMgr& mgr
)
{
  SizeType n = mgr.max_module_id();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto module = mgr.module(i);
    if ( module == nullptr ) continue;

    s << "Module#" << module->id() << "(" << module->name() << ")" << endl;
    auto pnode = module->parent();
    if ( pnode ) {
      s << "  parent node: Module#" << pnode->parent()->id()
	<< ":" << node_idstr(pnode) << endl;
    }
    else {
      s << "  toplevel module" << endl;
    }

    SizeType np{module->port_num()};
    for ( SizeType j = 0; j < np; ++ j ) {
      auto port{module->port(j)};
      s << "  Port#" << j << "(" << port->name() << ")" << endl;
      SizeType n{port->port_ref_num()};
      for ( SizeType k = 0; k < n; ++ k ) {
	const auto& port_ref{port->port_ref(k)};
	s << "    " << node_idstr(port_ref.node());
	if ( port_ref.has_bitselect() ) {
	  s << "[" << port_ref.bitpos() << "]";
	}
	else if ( port_ref.has_partselect() ) {
	  s << "[" << port_ref.msb() << ":" << port_ref.lsb() << "]";
	}
	s << endl;
      }
    }

    SizeType ni{module->input_num()};
    for ( SizeType j = 0; j < ni; ++ j ) {
      dump_node(s, module->input(j), mgr);
    }
    SizeType no{module->output_num()};
    for ( SizeType j = 0;j < no; ++ j ) {
      dump_node(s, module->output(j), mgr);
    }
    SizeType nio{module->inout_num()};
    for ( SizeType j = 0; j < nio; ++ j ) {
      dump_node(s, module->inout(j), mgr);
    }
    for ( auto node: module->node_list() ) {
      dump_node(s, node, mgr);
    }

    s << endl;
  }
}

END_NAMESPACE_YM_MVN
