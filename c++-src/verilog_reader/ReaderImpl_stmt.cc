
/// @file ReaderImpl_stmt.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ReaderImpl.h"
#include "DeclMap.h"
#include "Env.h"
#include "EnvMerger.h"

#include "ym/MvnMgr.h"
#include "ym/MvnNode.h"
#include "ym/MvnBvConst.h"
#include "ym/BitVector.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlExpr.h"
#include "ym/vl/VlRange.h"

#include "ym/MsgMgr.h"


BEGIN_NONAMESPACE
bool debug = false;
END_NONAMESPACE

BEGIN_NAMESPACE_YM_MVN_VERILOG

// @brief ステートメントの中身を生成する
// @param[in] module 親のモジュール
// @param[in] stmt 本体のステートメント
// @param[in] env 環境
// @param[in] merge 環境をマージするオブジェクト
bool
ReaderImpl::gen_stmt(
  MvnModule* module,
  const VlStmt* stmt,
  ProcEnv& env,
  EnvMerger& merge
)
{
  if ( debug ) {
    cout << "gen_stmt1 " << stmt->file_region() << endl;
  }

  switch ( stmt->type() ) {
  case VpiObjType::Assignment:
    gen_assign(module, stmt, env);
    break;

  case VpiObjType::Begin:
  case VpiObjType::NamedBegin:
    {
      SizeType n{stmt->child_stmt_num()};
      for ( SizeType i = 0; i < n; ++ i ) {
	auto stmt1{stmt->child_stmt(i)};
	bool stat = gen_stmt(module, stmt1, env, merge);
	if ( !stat ) {
	  return false;
	}
      }
    }
    break;

  case VpiObjType::If:
    {
      auto cond{stmt->expr()};
      auto cond_node = gen_expr(module, cond, env);
      ProcEnv then_env(env);
      bool stat = gen_stmt(module, stmt->body_stmt(), then_env, merge);
      if ( !stat ) {
	return false;
      }
      merge(module, env, cond_node, then_env, env);
    }
    break;

  case VpiObjType::IfElse:
    {
      auto cond{stmt->expr()};
      auto cond_node = gen_expr(module, cond, env);

      ProcEnv then_env(env);
      bool stat1 = gen_stmt(module, stmt->body_stmt(), then_env, merge);
      if ( !stat1 ) {
	return false;
      }

      ProcEnv else_env(env);
      bool stat2 = gen_stmt(module, stmt->else_stmt(), else_env, merge);
      if ( !stat2 ) {
	return false;
      }
      merge(module, env, cond_node, then_env, else_env);
    }
    break;

  case VpiObjType::Case:
    {
      auto expr{stmt->expr()};
      MvnBvConst xmask;
      auto expr_node = gen_expr(module, expr, stmt->case_type(), env, xmask);
      if ( expr_node == nullptr ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			expr->file_region(),
			MsgType::Warning,
			"MVN_VL",
			"Case expression contains 'x' or 'z', "
			"which is never match.");
      }
      else {
	bool stat = gen_caseitem(module, stmt, expr_node, xmask, 0, env, merge);
	if ( !stat ) {
	  return false;
	}
      }
    }
    break;

  default:
    MsgMgr::put_msg(__FILE__, __LINE__,
		    stmt->file_region(),
		    MsgType::Error,
		    "MVN_VL",
		    "Unsupported statement for synthesis.");
    return false;
  }

  return true;
}

// @brief case 文の本体を生成する．
// @param[in] module 親のモジュール
// @param[in] stmt case 文のステートメント
// @param[in] expr case 文の式を表すノード
// @param[in] xmask case 文の式の Xマスク
// @param[in] pos caseitem の位置番号
// @param[in] env 環境
// @param[in] merge 環境をマージするオブジェクト
bool
ReaderImpl::gen_caseitem(
  MvnModule* module,
  const VlStmt* stmt,
  MvnNode* expr,
  const MvnBvConst& xmask,
  int pos,
  ProcEnv& env,
  EnvMerger& merge
)
{
  SizeType n{stmt->caseitem_num()};
  if ( pos == n ) {
    return true;
  }

  // pos 番めの条件を作る．
  auto case_type{stmt->case_type()};
  auto caseitem{stmt->caseitem(pos)};
  SizeType ne{caseitem->expr_num()};
  if ( ne == 0 ) {
    // default caseitem
    ASSERT_COND( pos == n - 1 );

    return gen_stmt(module, caseitem->body_stmt(), env, merge);
  }

  vector<MvnNode*> cond_list;
  cond_list.reserve(ne);
  for ( SizeType i = 0; i < ne; ++ i ) {
    auto label_expr{caseitem->expr(i)};
    SizeType bw{label_expr->bit_size()};
    MvnBvConst label_xmask;
    auto label = gen_expr(module, label_expr, case_type,
			  env, label_xmask);
    if ( label == nullptr ) {
      ostringstream buf;
      buf << "Expression '" << label_expr->decompile()
	  << "' contains 'x' or 'z', which is never match.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      label_expr->file_region(),
		      MsgType::Warning,
		      "MVN_VL",
		      buf.str());
    }
    else {
      auto xmask1 = xmask | label_xmask;
      MvnNode* cond = nullptr;
      if ( xmask1.is_all0() ) {
	cond = mMvnMgr->new_equal(module, bw);
      }
      else {
	// xマスク付きの等価比較
	cond = mMvnMgr->new_caseeq(module, bw, xmask1);
      }
      mMvnMgr->connect(expr, 0, cond, 0);
      mMvnMgr->connect(label, 0, cond, 1);
      cond_list.push_back(cond);
    }
  }

  SizeType ni{cond_list.size()};
  if ( ni == 0 ) {
    // この caseitem に合致する条件はない．
    gen_caseitem(module, stmt, expr, xmask, pos + 1, env, merge);
    return true;
  }

  MvnNode* all_cond = nullptr;
  if ( ni == 1 ) {
    all_cond = cond_list[0];
  }
  else {
    all_cond = mMvnMgr->new_or(module, ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      mMvnMgr->connect(cond_list[i], 0, all_cond, i);
    }
  }
  ASSERT_COND( all_cond != nullptr );

  // case を if-else の連鎖に置き換える．

  ProcEnv then_env(env);
  gen_stmt(module, caseitem->body_stmt(), then_env, merge);

  ProcEnv else_env(env);
  gen_caseitem(module, stmt, expr, xmask, pos + 1, else_env, merge);

  merge(module, env, all_cond, then_env, else_env);
  return true;
}

// @brief 代入文を生成する．
// @param[in] module 親のモジュール
// @param[in] stmt 本体のステートメント
// @param[in] env 環境
void
ReaderImpl::gen_assign(
  MvnModule* module,
  const VlStmt* stmt,
  ProcEnv& env
)
{
  auto rhs{stmt->rhs()};
  auto lhs{stmt->lhs()};

  auto rhs_node = gen_rhs(module, lhs, rhs, env);

  SizeType n{lhs->lhs_elem_num()};
  SizeType offset = 0;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto lhs1{lhs->lhs_elem(i)};
    auto lhs_decl{lhs1->decl_obj()};
    auto lhs_declarray{lhs1->declarray_obj()};
    auto lhs_declbase{lhs1->decl_base()};
    SizeType bw{lhs_declbase->bit_size()};
    AssignInfo old_dst;
    SizeType lhs_offset = 0;
    if ( lhs_decl ) {
      old_dst = env.get_info(lhs_decl);
    }
    else if ( lhs_declarray ) {
      if ( lhs1->is_constant_select() ) {
	lhs_offset = lhs1->declarray_offset();
	old_dst = env.get_info(lhs_declarray, lhs_offset);
      }
      else {
	ASSERT_COND( lhs1->declarray_dimension() == lhs_declarray->dimension() );
	// lhs_offset は lhs->declarray_index() から計算される．
#warning "TODO: 配列の可変インデックス時の処理"
      }
    }
    else {
      ASSERT_NOT_REACHED;
    }

    MvnNode* dst_node = nullptr;
    if ( lhs1->is_primary() ) {
      auto src_node{splice_rhs(module, rhs_node, offset, bw)};
      dst_node = mMvnMgr->new_through(module, bw);
      mMvnMgr->connect(src_node, 0, dst_node, 0);
    }
    else if ( lhs1->is_bitselect() ) {
#if 1
#warning "未完"
#else
      if ( lhs1->is_constant_select() ) {
	// 固定インデックス
	auto src_node = splice_rhs(module, rhs_node, offset, 0);
	SizeType offset;
	if ( !lhs_declbase->calc_bit_offset(lhs1->index_val(), offset) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  lhs1->file_region(),
			  MsgType::Error,
			  "MVN_VL",
			  "Index is out of range");
	  return;
	}
	vector<SizeType> bw_array;
	bw_array.reserve(3);
	if ( offset < bw - 1 ) {
	  bw_array.push_back(bw - offset - 1);
	}
	bw_array.push_back(1);
	if ( offset > 0 ) {
	  bw_array.push_back(offset);
	}
	dst_node = mMvnMgr->new_concat(module, bw_array);
	int pos = 0;
	if ( offset < bw - 1 ) {
	  auto tmp_node = mMvnMgr->new_constpartselect(module,
						       bw - 1,
						       offset + 1,
						       bw);
	  mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	  mMvnMgr->connect(tmp_node, 0, dst_node, pos);
	  ++ pos;
	}
	mMvnMgr->connect(src_node, 0, dst_node, pos);
	++ pos;
	if ( offset > 0 ) {
	  auto tmp_node = mMvnMgr->new_constpartselect(module,
						       offset - 1,
						       0,
						       bw);
	  mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	  mMvnMgr->connect(tmp_node, 0, dst_node, pos);
	}
      }
      else {
	// 可変インデックス
      }
#endif
    }
    else if ( lhs1->is_partselect() ) {
#if 1
#warning "未完"
#else
      if ( lhs1->is_constant_select() ) {
	SizeType msb;
	if ( !lhs_declbase->calc_bit_offset(lhs1->left_range_val(), msb) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  lhs1->file_region(),
			  MsgType::Error,
			  "MVN_VL",
			  "Left index is out of range");
	  return;
	}

	SizeType lsb;
	if ( !lhs_declbase->calc_bit_offset(lhs1->right_range_val(), lsb) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  lhs1->file_region(),
			  MsgType::Error,
			  "MVN_VL",
			  "Right index is out of range");
	  return;
	}

	SizeType lbw = msb - lsb + 1;
	auto src_node{splice_rhs(module, rhs_node, offset, lbw)};

	vector<SizeType> bw_array;
	bw_array.reserve(3);
	if ( msb < bw - 1 ) {
	  bw_array.push_back(bw - msb - 1);
	}
	bw_array.push_back(msb - lsb + 1);
	if ( lsb > 0 ) {
	  bw_array.push_back(lsb);
	}
	dst_node = mMvnMgr->new_concat(module, bw_array);
	int pos = 0;
	if ( msb < bw - 1 ) {
	  auto tmp_node = mMvnMgr->new_constpartselect(module,
						       bw - 1,
						       msb + 1,
						       bw);
	  mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	  mMvnMgr->connect(tmp_node, 0, dst_node, pos);
	  ++ pos;
	}
	mMvnMgr->connect(src_node, 0, dst_node, pos);
	++ pos;
	if ( lsb > 0 ) {
	  auto tmp_node = mMvnMgr->new_constpartselect(module,
						       lsb - 1,
						       0,
						       bw);
	  mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	  mMvnMgr->connect(tmp_node, 0, dst_node, pos);
	}
      }
      else {
	// 可変インデックス
      }
#endif
    }
    ASSERT_COND( dst_node );
    if ( lhs_decl ) {
      env.add(lhs_decl, dst_node, stmt->is_blocking());
    }
    else {
      env.add(lhs_declarray, lhs_offset, dst_node, stmt->is_blocking());
    }
    offset += lhs1->bit_size();
  }
}

END_NAMESPACE_YM_MVN_VERILOG
