
/// @file ReaderImpl.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ReaderImpl.h"
#include "DeclMap.h"
#include "Driver.h"
#include "Env.h"

#include "ym/MvnMgr.h"
#include "ym/MvnModule.h"
#include "ym/MvnNode.h"
#include "ym/MvnBvConst.h"
#include "ym/BitVector.h"
#include "ym/VlValue.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlExpr.h"
#include "ym/vl/VlRange.h"

#include "ym/MsgMgr.h"


#define YM_DEBUG 1

BEGIN_NAMESPACE_YM_MVN_VERILOG

// @brief 式に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_expr(
  MvnModule* parent_module,
  const VlExpr* expr,
  const Env& env
)
{
  MvnBvConst dummy;
  auto node = gen_expr(parent_module, expr, VpiCaseType::Exact, env, dummy);
  return node;
}

// @brief case 文用の式に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] case_type case 文の種類
// @param[in] env 環境
// @param[out] xmask Xマスク
MvnNode*
ReaderImpl::gen_expr(
  MvnModule* parent_module,
  const VlExpr* expr,
  VpiCaseType case_type,
  const Env& env,
  MvnBvConst& xmask
)
{
  xmask = MvnBvConst(expr->bit_size());

  MvnNode* node = nullptr;
  if ( expr->is_const() ) {
    node = gen_const(parent_module, expr, case_type, xmask);
  }
  else if ( expr->is_operation() ) {
    node = gen_opr(parent_module, expr, case_type, env);
  }
  else if ( expr->is_funccall() ) {
    node = gen_funccall(parent_module, expr, case_type, env);
  }
  else {
    node = gen_primary(expr, env);
    if ( expr->is_primary() ) {
      ; // なにもしない．
    }
    else if ( expr->is_bitselect() ) {
      // ビット選択
      if ( expr->is_constant_select() ) {
	// 定数ビット選択
	auto decl{expr->decl_base()};
	SizeType bitpos;
	if ( !decl->calc_bit_offset(expr->index_val(), bitpos) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  expr->file_region(),
			  MsgType::Error,
			  "MVN_VL",
			  "Index is out of range.");
	  return nullptr;
	}
	auto node1 = mMvnMgr->new_constbitselect(parent_module,
						 bitpos,
						 node->bit_width());
	mMvnMgr->connect(node, 0, node1, 0);
	node = node1;
      }
      else {
	// 可変ビット選択
#warning "TODO-2011-07-07-01: [msb:lsb] のオフセット変換をしていない"
	auto node1 = gen_expr(parent_module, expr->index(), env);
	auto node2 = mMvnMgr->new_bitselect(parent_module,
					    node->bit_width(),
					    node1->bit_width());
	mMvnMgr->connect(node, 0, node2, 0);
	mMvnMgr->connect(node1, 0, node2, 1);
	node = node2;
      }
    }
    else if ( expr->is_partselect() ) {
      // 範囲選択
      if ( expr->is_constant_select() ) {
	// 定数範囲選択
	auto decl{expr->decl_base()};

	SizeType msb;
	if ( !decl->calc_bit_offset(expr->left_range_val(), msb) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  expr->left_range()->file_region(),
			  MsgType::Error,
			  "MVN_VL",
			  "Left range is out of range");
	  return nullptr;
	}

	SizeType lsb;
	if ( !decl->calc_bit_offset(expr->right_range_val(), lsb) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  expr->right_range()->file_region(),
			  MsgType::Error,
			  "MVN_VL",
			  "Right range is out of range");
	  return nullptr;
	}
	auto node1 = mMvnMgr->new_constpartselect(parent_module,
						  msb, lsb,
						  node->bit_width());
	mMvnMgr->connect(node, 0, node1, 0);
	node = node1;
      }
      else {
	// 可変範囲選択
#warning "TODO-2011-07-07-02: [msb:lsb] のオフセット変換をしていない"
	// まだできてない．
	// というか可変 part_select は VPI がおかしいと思う．
	ASSERT_NOT_REACHED;
	return nullptr;
      }
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }

  if ( node != nullptr ) {
    // 要求された型に変換する．
    auto node1 = coerce_expr(parent_module, node, expr->req_type());
    node = node1;
  }

  return node;
}

// @brief 定数値に対応したノードを作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] case_type case 文の種類
// @param[out] xmask Xマスク
MvnNode*
ReaderImpl::gen_const(
  MvnModule* parent_module,
  const VlExpr* expr,
  VpiCaseType case_type,
  MvnBvConst& xmask
)
{
  auto value{expr->constant_value()};
  ASSERT_COND( expr->value_type().is_bitvector_type() );

  auto bv{value.bitvector_value()};

  SizeType bit_size{bv.size()};
  MvnBvConst val(bit_size);
  xmask = MvnBvConst(bit_size);
  for ( SizeType i = 0; i < bit_size; ++ i ) {
    auto v{bv.value(i)};
    if ( v.is_one() ) {
      val.set_val(i, true);
    }
    else if ( v.is_zero() ) {
      val.set_val(i, false);
    }
    else if ( v.is_x() ) {
      if ( case_type == VpiCaseType::X ) {
	xmask.set_val(i, true);
      }
      else {
	return nullptr;
      }
    }
    else if ( v.is_z() ) {
      if ( case_type == VpiCaseType::X || case_type == VpiCaseType::Z ) {
	xmask.set_val(i, true);
      }
      else {
	return nullptr;
      }
    }
    else {
      ASSERT_NOT_REACHED;
      return nullptr;
    }
  }
  return mMvnMgr->new_const(parent_module, val);
}

// @brief 演算に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] case_type case 文の種類
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_opr(
  MvnModule* parent_module,
  const VlExpr* expr,
  VpiCaseType case_type,
  const Env& env
)
{
  SizeType out_bw{expr->bit_size()};

  auto op_type{expr->op_type()};

  // オペランドの値を表すノードを作る．
  SizeType n{expr->operand_num()};
  vector<MvnNode*> operand_array(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    MvnBvConst xmask1;
    auto node1 = gen_expr(parent_module, expr->operand(i), case_type,
			  env, xmask1);
    if ( xmask1 != MvnBvConst(xmask1.size()) ) {
      // X を含む値との演算は合成不可
      MsgMgr::put_msg(__FILE__, __LINE__,
		      expr->file_region(),
		      MsgType::Error,
		      "MVN_VLXXX",
		      "'X' or 'Z' value in the operands");
      return nullptr;
    }
    operand_array[i] = node1;
  }

  switch ( op_type ) {
  case VpiOpType::Null:
    return operand_array[0];

  case VpiOpType::Minus:
    {
      SizeType bw{operand_array[0]->bit_width()};
      ASSERT_COND( bw == out_bw );

      auto node = mMvnMgr->new_cmpl(parent_module, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      return node;
    }

  case VpiOpType::Not:
    {
      SizeType bw{operand_array[0]->bit_width()};
      ASSERT_COND( bw == 1 );
      ASSERT_COND( out_bw == bw );

      auto node = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      return node;
    }

  case VpiOpType::BitNeg:
    {
      SizeType bw{operand_array[0]->bit_width()};
      ASSERT_COND( bw == out_bw );

      auto node = mMvnMgr->new_not(parent_module, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      return node;
    }

  case VpiOpType::Plus:
    {
      SizeType bw{operand_array[0]->bit_width()};
      ASSERT_COND( bw == out_bw );
      return operand_array[0];
    }

  case VpiOpType::UnaryAnd:
    {
      ASSERT_COND( out_bw == 1  );
      SizeType bw{operand_array[0]->bit_width()};

      auto node = mMvnMgr->new_rand(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      return node;
    }

  case VpiOpType::UnaryNand:
    {
      ASSERT_COND( out_bw == 1  );
      SizeType bw{operand_array[0]->bit_width()};

      auto node = mMvnMgr->new_rand(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);

      auto node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      return node1;
    }

  case VpiOpType::UnaryOr:
    {
      ASSERT_COND( out_bw == 1  );
      SizeType bw{operand_array[0]->bit_width()};

      auto node = mMvnMgr->new_ror(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      return node;
    }

  case VpiOpType::UnaryNor:
    {
      ASSERT_COND( out_bw == 1  );
      SizeType bw{operand_array[0]->bit_width()};

      auto node = mMvnMgr->new_ror(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);

      auto node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      return node1;
    }

  case VpiOpType::UnaryXor:
    {
      ASSERT_COND( out_bw == 1  );
      SizeType bw{operand_array[0]->bit_width()};

      auto node = mMvnMgr->new_rxor(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      return node;
    }

  case VpiOpType::UnaryXNor:
    {
      ASSERT_COND( out_bw == 1  );
      SizeType bw{operand_array[0]->bit_width()};

      auto node = mMvnMgr->new_rxor(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);

      auto node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      return node1;
    }

  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    ASSERT_NOT_REACHED;
    break;

  case VpiOpType::Add:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_add(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::Sub:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_sub(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::Mult:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_mult(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::Div:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_div(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::Mod:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_mod(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::Power:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      SizeType bw3{out_bw};

      auto node = mMvnMgr->new_pow(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::LShift:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      SizeType bw3{out_bw};

      auto node = mMvnMgr->new_sll(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::RShift:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      SizeType bw3{out_bw};

      auto node = mMvnMgr->new_srl(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::ArithLShift:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      SizeType bw3{out_bw};

      auto node = mMvnMgr->new_sla(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::ArithRShift:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      SizeType bw3{out_bw};

      auto node = mMvnMgr->new_sra(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::BitAnd:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_and(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::BitOr:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_or(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::BitXNor:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_xor(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);

      auto node1 = mMvnMgr->new_not(parent_module, out_bw);
      mMvnMgr->connect(node, 0, node1, 0);
      return node1;
    }

  case VpiOpType::BitXor:
    {
      ASSERT_COND( operand_array[0]->bit_width() == out_bw );
      ASSERT_COND( operand_array[1]->bit_width() == out_bw );

      auto node = mMvnMgr->new_xor(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::LogAnd:
    {
      auto node = mMvnMgr->new_and(parent_module, 2, 1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::LogOr:
    {
      auto node = mMvnMgr->new_or(parent_module, 2, 1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::CaseEq:
  case VpiOpType::CaseNeq:
    ASSERT_NOT_REACHED;
    break;

  case VpiOpType::Eq:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      ASSERT_COND( bw1 == bw2 );

      auto node = mMvnMgr->new_equal(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::Neq:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      ASSERT_COND( bw1 == bw2 );

      auto node = mMvnMgr->new_equal(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);

      auto node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      return node1;
    }

  case VpiOpType::Lt:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      ASSERT_COND( bw1 == bw2 );

      auto node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      return node;
    }

  case VpiOpType::Ge:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      ASSERT_COND( bw1 == bw2 );

      auto node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);

      auto node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      return node1;
    }

  case VpiOpType::Gt:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      ASSERT_COND( bw1 == bw2 );

      auto node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[1], 0, node, 0);
      mMvnMgr->connect(operand_array[0], 0, node, 1);
      return node;
    }

  case VpiOpType::Le:
    {
      SizeType bw1{operand_array[0]->bit_width()};
      SizeType bw2{operand_array[1]->bit_width()};
      ASSERT_COND( bw1 == bw2 );

      auto node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[1], 0, node, 0);
      mMvnMgr->connect(operand_array[0], 0, node, 1);

      auto node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      return node1;
    }

  case VpiOpType::Condition:
    {
      SizeType bw1{operand_array[1]->bit_width()};
      SizeType bw2{operand_array[2]->bit_width()};
      ASSERT_COND( bw1 == out_bw );
      ASSERT_COND( bw2 == out_bw );

      auto node = mMvnMgr->new_ite(parent_module, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      mMvnMgr->connect(operand_array[2], 0, node, 2);
      return node;
    }

  case VpiOpType::MinTypMax:
    ASSERT_NOT_REACHED;
    break;

  case VpiOpType::Concat:
    {
      vector<SizeType> bw_array(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	bw_array[i] = operand_array[i]->bit_width();
      }

      auto node = mMvnMgr->new_concat(parent_module, bw_array);
      for ( SizeType i = 0; i < n; ++ i ) {
	mMvnMgr->connect(operand_array[i], 0, node, i);
      }
      return node;
    }

  case VpiOpType::MultiConcat:
    {
      SizeType r{expr->rep_num()};
      SizeType n1{n - 1};
      vector<SizeType> bw_array(n1 * r);
      for ( SizeType j = 0; j < r; ++ j ) {
	SizeType base{j * n1};
	for ( SizeType i = 0; i < n1; ++ i ) {
	  bw_array[base + i] = operand_array[i + 1]->bit_width();
	}
      }

      auto node = mMvnMgr->new_concat(parent_module, bw_array);
      for ( SizeType j = 0; j < r; ++ j ) {
	SizeType base{j * n1};
	for ( SizeType i = 0; i < n1; ++ i ) {
	  mMvnMgr->connect(operand_array[i + 1], 0, node, base + i);
	}
      }
      return node;
    }

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 宣言要素への参照に対応するノードを作る．
// @param[in] expr 式
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_primary(
  const VlExpr* expr,
  const Env& env
)
{
  auto decl{expr->decl_obj()};
  auto declarray{expr->declarray_obj()};
  if ( decl ) {
    ASSERT_COND(expr->declarray_dimension() == 0 );
    auto node = env.get(decl);
    ASSERT_COND( node != nullptr );
    return node;
  }
  else if ( declarray ) {
    if ( expr->is_constant_select() ) {
      // インデックス固定の配列要素
      SizeType offset{expr->declarray_offset()};
      auto node{env.get(declarray, offset)};
      if ( node == nullptr ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			expr->file_region(),
			MsgType::Error,
			"MVN_VL",
			"Index is out of range.");
	return nullptr;
      }
      return node;
    }
    else {
      // インデックス可変の配列要素
#if 0
      SizeType dim = expr->declarray_dimension();
      ASSERT_COND( declarray->dimension() == dim );
      int offset = 0;
      int mlt = 1;
      for ( int i = 0; i < dim; ++ i ) {
	const VlExpr* index = expr->declarray_index(i);
	int index_val;
	bool stat = index->eval_int(index_val);
	ASSERT_COND( stat );
	offset += index_val * mlt;
	mlt *= declarray->range(i)->size();
      }
      MvnNode* node = env.get(declarray, offset);
      if ( node == nullptr ) {
	cerr << decl->name() << " is not found in mGlobalEnv" << endl;
      }
      ASSERT_COND( node != nullptr );
      return node;
#else
#warning "TODO-2011-07-07-03: 可変インデックスの配列要素"
      ASSERT_NOT_REACHED;
      return nullptr;
#endif
    }
  }
#if defined(YM_DEBUG)
  cerr << "Error in " << expr->decompile() << endl;
  cerr << "  " << expr->file_region() << endl;
#endif
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @bief 右辺式に対応するノードを作る．
// @param[in] parent_module 親のモジュール
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_rhs(
  MvnModule* parent_module,
  const VlExpr* lhs,
  const VlExpr* rhs,
  const Env& env
)
{
  // 左辺で必要とされている型に変換する．
  auto node_orig{gen_expr(parent_module, rhs, env)};
  auto lhs_value_type{lhs->value_type()};
  return coerce_expr(parent_module, node_orig, lhs_value_type);
}

// @brief 関数呼び出しに対応するノードを作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] case_type case 文の種類
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_funccall(
  MvnModule* parent_module,
  const VlExpr* expr,
  VpiCaseType case_type,
  const Env& env
)
{
  // 引数の値を表すノードを作る．
  SizeType n{expr->argument_num()};
  vector<MvnNode*> argument_array(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    MvnBvConst xmask1;
    auto node1 = gen_expr(parent_module, expr->argument(i), case_type,
			  env, xmask1);
    if ( xmask1 != MvnBvConst(xmask1.size()) ) {
      // X を含む値との演算は合成不可
      MsgMgr::put_msg(__FILE__, __LINE__,
		      expr->file_region(),
		      MsgType::Error,
		      "MVN_VLXXX",
		      "'X' or 'Z' value in the operands");
      return nullptr;
    }
    argument_array[i] = node1;
  }


#warning "TODO: 未完"
  return nullptr;
}

// @brief 式の型を補正する．
// @param[in] parent_module 親のモジュール
// @param[in] src_node 元のノード
// @param[in] value_type 要求されるデータ型
MvnNode*
ReaderImpl::coerce_expr(
  MvnModule* parent_module,
  MvnNode* src_node,
  VlValueType value_type
)
{
  SizeType bit_width{value_type.size()};
  SizeType src_bw{src_node->bit_width()};
  auto node = src_node;
  if ( bit_width > src_bw ) {
    // 左辺のビット幅が大きいとき
    // 上位ビットをパディングする．
    SizeType np{bit_width - src_bw};
    if ( value_type.is_signed() ) {
      // 符号付きの場合は再上位ビットをコピーする．
      vector<SizeType> ibw_array(np + 1);
      for ( SizeType i = 0; i < np; ++ i ) {
	ibw_array[i] = 1;
      }
      ibw_array[np] = src_bw;
      node = mMvnMgr->new_concat(parent_module, ibw_array);
      auto msb_node = mMvnMgr->new_constbitselect(parent_module,
						  src_bw - 1, src_bw);
      mMvnMgr->connect(src_node, 0, msb_node, 0);
      for ( SizeType i = 0; i < np; ++ i ) {
	mMvnMgr->connect(msb_node, 0, node, i);
      }
      mMvnMgr->connect(src_node, 0, node, np);
    }
    else {
      // 符号なしの場合は0を入れる．
      vector<SizeType> ibw_array({np, src_bw});
      node = mMvnMgr->new_concat(parent_module, ibw_array);
      MvnBvConst val(np);
      for ( SizeType i = 0; i < np; ++ i ) {
	val.set_val(i, false);
      }
      auto zero = mMvnMgr->new_const(parent_module, val);
      mMvnMgr->connect(zero, 0, node, 0);
      mMvnMgr->connect(src_node, 0, node, 1);
    }
  }
  else if ( bit_width < src_bw ) {
    // 左辺のビット幅が小さいとき
    // ただ単に下位ビットを取り出す．
    node = mMvnMgr->new_constpartselect(parent_module, bit_width - 1, 0, src_bw);
    mMvnMgr->connect(src_node, 0, node, 0);
  }
  ASSERT_COND( node != nullptr );
  return node;
}

// @brief 右辺式から該当する部分を切り出す．
// @param[in] parent_module 親のモジュール
// @param[in] rhs_node 右辺式のノード
// @param[in] offset オフセット
// @param[in] bit_width ビット幅
// @note rhs_node から [offset: offset + bit_width - 1] の選択するノードを返す．
// @note 全範囲を選択する場合には node を返す．
// @note 範囲が合わなかったら nullptr を返す．
MvnNode*
ReaderImpl::splice_rhs(
  MvnModule* parent_module,
  MvnNode* rhs_node,
  SizeType offset,
  SizeType bit_width
)
{
  SizeType src_bw{rhs_node->bit_width()};
  ASSERT_COND( offset + bit_width <= src_bw );

  MvnNode* src_node = nullptr;
  if ( offset == 0 && bit_width == src_bw ) {
    // 全範囲の選択
    src_node = rhs_node;
  }
  else if ( bit_width == 1 ) {
    src_node = mMvnMgr->new_constbitselect(parent_module,
					   offset,
					   src_bw);
    mMvnMgr->connect(rhs_node, 0, src_node, 0);
  }
  else {
    src_node = mMvnMgr->new_constpartselect(parent_module,
					    offset,
					    offset + bit_width - 1,
					    src_bw);
    mMvnMgr->connect(rhs_node, 0, src_node, 0);
  }
  return src_node;
}

END_NAMESPACE_YM_MVN_VERILOG
