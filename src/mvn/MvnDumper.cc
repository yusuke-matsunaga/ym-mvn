
/// @file MvnDumper.cc
/// @brief MvnDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MvnDumper.h"

#include "ym/MvnMgr.h"

#include "ym/MvnModule.h"
#include "ym/MvnPort.h"
#include "ym/MvnNode.h"
#include "ym/MvnInputPin.h"

#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

// ノードを表す文字列を返す．
string
node_idstr(const MvnNode* node)
{
  ostringstream buf;
  buf << "Node[" << node->id() << "]";
  return buf.str();
}

// MvnInputPin の内容を出力する．
void
dump_inputpin(ostream& s,
	      const MvnInputPin* pin,
	      const string& pin_name)
{
  s << "  " << pin_name
    << "(" << pin->bit_width() << ")" << endl;
  const MvnNode* onode = pin->src_node();
  if ( onode ) {
    s << "    <== Output@" << node_idstr(onode) << endl;
  }
}

// MvnInputPin の内容を出力する．
void
dump_inputpin(ostream& s,
	      const MvnInputPin* pin)
{
  ostringstream buf;
  buf << "InputPin#" << pin->pos();
  string pin_name = buf.str();
  dump_inputpin(s, pin, pin_name);
}

// MvnNode の内容を出力する．
void
dump_node(ostream& s,
	  const MvnNode* node,
	  const MvnMgr& mgr)
{
  s << node_idstr(node) << " : ";
  switch ( node->type() ) {
  case MvnNode::kInput:      s << "Input"; break;
  case MvnNode::kInout:      s << "Inout"; break;
  case MvnNode::kOutput:     s << "Output"; break;
  case MvnNode::kDff:        s << "DFF"; break;
  case MvnNode::kLatch:      s << "Latch"; break;
  case MvnNode::kThrough:    s << "Through"; break;
  case MvnNode::kNot:        s << "Not"; break;
  case MvnNode::kAnd:        s << "And"; break;
  case MvnNode::kOr:         s << "Or"; break;
  case MvnNode::kXor:        s << "Xor"; break;
  case MvnNode::kRand:       s << "Rand"; break;
  case MvnNode::kRor:        s << "Ror"; break;
  case MvnNode::kRxor:       s << "Rxor"; break;
  case MvnNode::kEq:         s << "Eq"; break;
  case MvnNode::kLt:         s << "Lt"; break;
  case MvnNode::kCaseEq:
    {
      s << "CaseEq[";
      vector<ymuint32> xmask;
      node->xmask(xmask);
      int bw = node->input(0)->bit_width();
      for ( int i = 0; i < bw; ++ i ) {
	int bitpos = bw - i - 1;
	int blk = bitpos / 32;
	int sft = bitpos % 32;
	if ( xmask[blk] & (1U << sft) ) {
	  s << "-";
	}
	else {
	  s << "1";
	}
      }
      s << "]";
    }
    break;
  case MvnNode::kSll:        s << "Sll"; break;
  case MvnNode::kSrl:        s << "Srl"; break;
  case MvnNode::kSla:        s << "Sla"; break;
  case MvnNode::kSra:        s << "Sra"; break;
  case MvnNode::kAdd:        s << "Add"; break;
  case MvnNode::kSub:        s << "Sub"; break;
  case MvnNode::kMult:       s << "Mult"; break;
  case MvnNode::kDiv:        s << "Div"; break;
  case MvnNode::kMod:        s << "Mod"; break;
  case MvnNode::kPow:        s << "Pow"; break;
  case MvnNode::kIte:        s << "Ite"; break;
  case MvnNode::kConcat:     s << "Concat"; break;
  case MvnNode::kConstBitSelect:
    s << "ConstBitSelect["
      << node->bitpos()
      << "]";
    break;
  case MvnNode::kConstPartSelect:
    s << "ConstPartSelect["
      << node->msb()
      << ":"
      << node->lsb()
      << "]";
    break;
  case MvnNode::kBitSelect:  s << "BitSelect"; break;
  case MvnNode::kPartSelect: s << "PartSelect"; break;
  case MvnNode::kConst:
    {
      s << "Const(";
      vector<ymuint32> val;
      node->const_value(val);
      int n = val.size();
      s << hex;
      for ( int i = 0; i < n; ++ i ) {
	s << " " << val[n - i - 1];
      }
      s << dec << ")";
    }
    break;
  case MvnNode::kCell:
    {
      const ClibCell& cell = mgr.library().cell(node->cell_id());
      s << "Cell(" << cell.name() << ")";
    }
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  s << endl;

  if ( node->type() == MvnNode::kDff ) {
    const MvnInputPin* input = node->input(0);
    dump_inputpin(s, input, "DataInput");
    const MvnInputPin* clock = node->input(1);
    dump_inputpin(s, clock, "Clock");
    s << "    ";
    if ( node->clock_pol() ) {
      s << "posedge";
    }
    else {
      s << "negedge";
    }
    s << endl;
    int ni = node->input_num();
    int nc = ni - 2;
    for ( int i = 0; i < nc; ++ i ) {
      const MvnInputPin* cpin = node->input(i + 2);
      ostringstream buf;
      buf << "Control#" << i;
      string pin_name = buf.str();
      dump_inputpin(s, cpin, pin_name);
      s << "    ";
      if ( node->control_pol(i) ) {
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
  else if ( node->type() == MvnNode::kLatch ) {
    #warning "TODO: 未完"
  }
  else {
    int ni = node->input_num();
    for ( int i = 0; i < ni; ++ i ) {
      const MvnInputPin* pin = node->input(i);
      dump_inputpin(s, pin);
    }
    s << "  Output(" << node->bit_width() << ")" << endl;
    const list<MvnInputPin*>& fo_list = node->dst_pin_list();
    for (list<MvnInputPin*>::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      const MvnInputPin* ipin = *p;
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
MvnDumper::operator()(ostream& s,
		      const MvnMgr& mgr)
{
  int n = mgr.max_module_id();
  for ( int i = 0; i < n; ++ i ) {
    const MvnModule* module = mgr.module(i);
    if ( module == nullptr ) continue;

    s << "Module#" << module->id() << "(" << module->name() << ")" << endl;
    const MvnNode* pnode = module->parent();
    if ( pnode ) {
      s << "  parent node: Module#" << pnode->parent()->id()
	<< ":" << node_idstr(pnode) << endl;
    }
    else {
      s << "  toplevel module" << endl;
    }

    int np = module->port_num();
    for ( int j = 0; j < np; ++ j ) {
      const MvnPort* port = module->port(j);
      s << "  Port#" << j << "(" << port->name() << ")" << endl;
      int n = port->port_ref_num();
      for ( int k = 0; k < n; ++ k ) {
	const MvnPortRef& port_ref = port->port_ref(k);
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

    int ni = module->input_num();
    for ( int j = 0; j < ni; ++ j ) {
      dump_node(s, module->input(j), mgr);
    }
    int no = module->output_num();
    for ( int j = 0;j < no; ++ j ) {
      dump_node(s, module->output(j), mgr);
    }
    int nio = module->inout_num();
    for ( int j = 0; j < nio; ++ j ) {
      dump_node(s, module->inout(j), mgr);
    }
    for ( MvnNode* node: module->node_list() ) {
      dump_node(s, node, mgr);
    }

    s << endl;
  }
}

END_NAMESPACE_YM_MVN
