﻿#ifndef YM_MVNMGR_H
#define YM_MVNMGR_H

/// @file ym/MvnMgr.h
/// @brief MvnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mvn.h"
#include "ym/MvnNode.h"
#include "ym/ItvlMgr.h"
#include "ym/ClibCellLibrary.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnMgr MvnMgr.h "ym/MvnMgr.h"
/// @brief 多値ネットワークの生成/設定を行うクラス
/// @sa MvnModule
/// @sa MvnNode
/// @sa MvnPort
///
/// MVN(Multi-Valued-Network)は回路構造を表すDAGである．
/// その名の通り，多ビットの信号線を扱うことができる．
///
/// MvnMgr は構成要素としてノード(MvnNode)を持つ．
/// ノードはノード番号でアクセスできるが，ノード番号は必ずしも
/// 連続しているとは限らない．
//////////////////////////////////////////////////////////////////////
class MvnMgr
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] library セルライブラリ
  MvnMgr(const ClibCellLibrary& library = ClibCellLibrary());

  /// @brief デストラクタ
  ~MvnMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 関連付けられたセルライブラリを返す．
  ClibCellLibrary
  library() const;

  /// @brief トップレベルモジュールのリストを得る．
  /// @return モジュールを格納するリストを返す．
  ///
  /// この関数はトップモジュール数ではなく全モジュール数に比例した
  /// 実行時間を要する．
  vector<const MvnModule*>
  topmodule_list() const;

  /// @brief モジュール番号の最大値+1を得る．
  SizeType
  max_module_id() const;

  /// @brief モジュールIDをキーにしてモジュールにアクセスする．
  /// @param[in] id モジュールID ( 0 <= id < max_module_id() )
  /// @return 該当するモジュールを返す．
  ///
  /// 該当するモジュールがない場合は nullptr を返す．
  const MvnModule*
  module(int id) const;

  /// @brief モジュールIDをキーにしてモジュールにアクセスする．
  /// @param[in] id モジュールID ( 0 <= id < max_module_id() )
  /// @return 該当するモジュールを返す．
  ///
  /// 該当するモジュールがない場合は nullptr を返す．
  MvnModule*
  _module(int id);

  /// @brief ノードの ID番号の最大値 + 1 を返す．
  SizeType
  max_node_id() const;

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_node_id() )
  ///
  /// nullptr が返されることもある．
  const MvnNode*
  node(int id) const;

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_node_id() )
  ///
  /// nullptr が返されることもある．
  MvnNode*
  _node(int id);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の設定を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールを生成する．
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ni 入力ノード数
  /// @param[in] no 出力ノード数
  /// @param[in] nio 入出力ノード数
  /// @return 生成したモジュールを返す．
  ///
  /// 入出力ノードのビット幅は1で初期化される．
  MvnModule*
  new_module(const string& name,
	     SizeType np,
	     SizeType ni,
	     SizeType no,
	     SizeType nio);

  /// @brief モジュールを生成する．
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  /// @param[in] obitwidth_array 出力のビット幅の配列
  /// @param[in] iobitwidth_array 入出力のビット幅の配列
  /// @return 生成したモジュールを返す．
  MvnModule*
  new_module(const string& name,
	     SizeType np,
	     const vector<SizeType>& ibitwidth_array,
	     const vector<SizeType>& obitwidth_array,
	     const vector<SizeType>& iobitwidth_array);

  /// @brief モジュールを削除する．
  /// @param[in] module 対象のモジュール
  ///
  /// モジュールインスタンスとして使われているモジュールは削除できない．
  void
  delete_module(MvnModule* module);

  /// @brief モジュールのポートを初期化する．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] portref_list ポート参照式のリスト
  /// @param[in] name 名前
  void
  init_port(MvnModule* module,
	    int pos,
	    const vector<MvnPortRef>& portref_list = vector<MvnPortRef>(),
	    const string& name = string());

#if 0
  /// @brief ポート参照式の内容を設定する(単純な形式)．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] port_ref_pos ポート参照式の番号
  /// @param[in] node 対応する入出力ノード
  void
  set_port_ref(MvnModule* module,
	       int pos,
	       int port_ref_pos,
	       MvnNode* node);

  /// @brief ポート参照式の内容を設定する(ビット指定形式)．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] port_ref_pos ポート参照式の番号
  /// @param[in] node 対応する入出力ノード
  /// @param[in] index ビット指定位置
  void
  set_port_ref(MvnModule* module,
	       int pos,
	       int port_ref_pos,
	       MvnNode* node,
	       int index);

  /// @brief ポート参照式の内容を設定する(範囲指定形式)．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] port_ref_pos ポート参照式の番号
  /// @param[in] node 対応する入出力ノード
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  void
  set_port_ref(MvnModule* module,
	       int pos,
	       int port_ref_pos,
	       MvnNode* node,
	       int msb,
	       int lsb);
#endif

  /// @brief 冗長な through ノードを取り除く
  void
  sweep();


public:
  //////////////////////////////////////////////////////////////////////
  // ノードの生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  MvnNode*
  new_input(MvnModule* module,
	    SizeType bit_width);

  /// @brief 出力ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  MvnNode*
  new_output(MvnModule* module,
	     SizeType bit_width);

  /// @brief 入出力ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  MvnNode*
  new_inout(MvnModule* module,
	    SizeType bit_width);

  /// @brief フリップフロップノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] clock_pol クロックの極性
  /// @param[in] pol_array 非同期セット信号の極性情報を入れた配列
  /// @param[in] val_array 非同期セットの値を入れた配列
  /// @param[in] bit_width ビット幅
  /// @note pol_array の要素数が非同期セット信号数となる．
  MvnNode*
  new_dff(MvnModule* module,
	  MvnPolarity clock_pol,
	  const vector<MvnPolarity>& pol_array,
	  const vector<MvnNode*>& val_array,
	  SizeType bit_width = 1);

  /// @brief ラッチノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  MvnNode*
  new_latch(MvnModule* module,
	    SizeType bit_width = 1);

  /// @brief through ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_through(MvnModule* module,
	      SizeType bit_width = 1);

  /// @brief not ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_not(MvnModule* module,
	  SizeType bit_width = 1);

  /// @brief and ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_and(MvnModule* module,
	  SizeType input_num,
	  SizeType bit_width = 1);

  /// @brief or ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_or(MvnModule* module,
	 SizeType input_num,
	 SizeType bit_width = 1);

  /// @brief xor ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_xor(MvnModule* module,
	  SizeType input_num,
	  SizeType bit_width = 1);

  /// @brief reduction and ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_rand(MvnModule* module,
	   SizeType bit_width);

  /// @brief reduction or ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_ror(MvnModule* module,
	  SizeType bit_width);

  /// @brief reduction xor ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_rxor(MvnModule* module,
	   SizeType bit_width);

  /// @brief equal ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_equal(MvnModule* module,
	    SizeType bit_width);

  /// @brief case 文用の equal ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @param[in] xmask Xマスク値
  /// @return 生成したノードを返す．
  /// @note 最初の入力が case expression
  /// @note 残りの入力は caseitem expression
  MvnNode*
  new_caseeq(MvnModule* module,
	     SizeType bit_width,
	     const MvnBvConst& xmask);

  /// @brief less than ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_lt(MvnModule* module,
	 SizeType bit_width);

  /// @brief shift left logical ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_sll(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief shift right logical ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_srl(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief shift left arithmetic ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_sla(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief shift right arithmetic ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_sra(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief cmpl ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  /// @note 2の補数を計算する．
  MvnNode*
  new_cmpl(MvnModule* module,
	   SizeType bit_width);

  /// @brief add ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_add(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief sub ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_sub(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief mult ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_mult(MvnModule* module,
	   SizeType bit_width1,
	   SizeType bit_width2,
	   SizeType bit_width3);

  /// @brief div ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_div(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief modulo ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_mod(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief power ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_pow(MvnModule* module,
	  SizeType bit_width1,
	  SizeType bit_width2,
	  SizeType bit_width3);

  /// @brief condition ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_ite(MvnModule* module,
	  SizeType bit_width);

  /// @brief concatenate ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  /// @return 生成したノードを返す．
  /// @note 出力のビット幅は入力のビット幅の和となる．
  MvnNode*
  new_concat(MvnModule* module,
	     const vector<SizeType>& ibitwidth_array);

  /// @brief bit-selectノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @param[in] bitpos ビット位置
  /// @return 生成したノードを返す．
  MvnNode*
  new_constbitselect(MvnModule* module,
		     SizeType bitpos,
		     SizeType bit_width);

  /// @brief part-select ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_constpartselect(MvnModule* module,
		      SizeType msb,
		      SizeType lsb,
		      SizeType bit_width);

  /// @brief 可変 bit-selectノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_width2 ビット指定入力のビット幅
  /// @return 生成したノードを返す．
  MvnNode*
  new_bitselect(MvnModule* module,
		SizeType bit_width1,
		SizeType bit_width2);

  /// @brief 可変 part-select ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_width2 範囲指定入力のビット幅
  /// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
  /// @return 生成したノードを返す．
  MvnNode*
  new_partselect(MvnModule* module,
		 SizeType bit_width1,
		 SizeType bit_width2,
		 SizeType bit_width3);

  /// @brief constant ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @param[in] val 値
  MvnNode*
  new_const(MvnModule* module,
	    SizeType bit_width,
	    const MvnBvConst& val);

  /// @brief セルノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] cell_id セル番号
  MvnNode*
  new_cell(MvnModule* module,
	   int cell_id);

  /// @brief ノードを削除する．
  /// @param[in] node 対象のノード
  /// @note 入力ノード, 出力ノードは削除できない
  /// @note 接続が残っているノードは削除できない．
  void
  delete_node(MvnNode* node);

  /// @brief node を alt_node に置き換える．
  /// @param[in] node 置換元のノード
  /// @param[in] alt_node 置換先のノード
  /// @note node は削除される．
  void
  replace(MvnNode* node,
	  MvnNode* alt_node);

  /// @brief ピンとピンを接続する．
  /// @param[in] src_node 入力元のノード
  /// @param[in] src_pin_pos 入力元のピン番号
  /// @param[in] dst_node 出力先のノード
  /// @param[in] dst_pin_pos 出力先のピン番号
  /// @retval true 接続が成功した．
  /// @retval false 接続が失敗した．
  /// @note 接続が失敗するのは，
  ///  - ピンが異なるモジュールに属していた．
  ///  - ピンのビット幅が異なっていた．
  bool
  connect(MvnNode* src_node,
	  SizeType src_pin_pos,
	  MvnNode* dst_node,
	  SizeType dst_pin_pos);

  /// @brief 接続を取り除く
  /// @param[in] src_node 入力元のノード
  /// @param[in] src_pin_pos 入力元のピン番号
  /// @param[in] dst_node 出力先のノード
  /// @param[in] dst_pin_pos 出力先のピン番号
  /// @note 現在の実装はあまり効率が良くない．
  void
  disconnect(MvnNode* src_node,
	     SizeType src_pin_pos,
	     MvnNode* dst_node,
	     SizeType dst_pin_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 連結演算からビットを抜き出す．
  /// @param[in] src_node 連結演算ノード
  /// @param[in] bitpos 抜き出すビット位置
  MvnNode*
  select_from_concat(MvnNode* src_node,
		     SizeType bitpos);

  /// @brief 部分指定子からビットを抜き出す．
  /// @param[in] src_node 部分指定ノード
  /// @param[in] bitpos 抜き出すビット位置
  MvnNode*
  select_from_partselect(MvnNode* src_node,
			 SizeType bitpos);

  /// @brief 接続を切り替える．
  /// @param[in] old_node 元のノード
  /// @param[in] old_pin_pos 元のピン番号
  /// @param[in] new_node 新しいノード
  /// @param[in] new_pin 新しいピン番号
  void
  reconnect(MvnNode* old_node,
	    SizeType old_pin_pos,
	    MvnNode* new_node,
	    SizeType new_pin_pos);

  /// @brief 多入力論理演算ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] type 型
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  MvnNode*
  new_log_op(MvnModule* module,
	     MvnNodeType type,
	     SizeType input_num,
	     SizeType bit_width);

  /// @brief 1入力演算ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] type 型
  /// @param[in] ibit_width1 入力のビット幅
  /// @param[in] obit_width 出力のビット幅
  MvnNode*
  new_unary_op(MvnModule* module,
	       MvnNodeType type,
	       SizeType ibit_width1,
	       SizeType obit_width);

  /// @brief 2入力演算ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] type 型
  /// @param[in] ibit_width1 入力1のビット幅
  /// @param[in] ibit_width2 入力2のビット幅
  /// @param[in] obit_width 出力のビット幅
  MvnNode*
  new_binary_op(MvnModule* module,
		MvnNodeType type,
		SizeType ibit_width1,
		SizeType ibit_width2,
		SizeType obit_width);

  /// @brief 3入力演算ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] type 型
  /// @param[in] ibit_width1 入力1のビット幅
  /// @param[in] ibit_width2 入力2のビット幅
  /// @param[in] ibit_width3 入力3のビット幅
  /// @param[in] obit_width 出力のビット幅
  MvnNode*
  new_ternary_op(MvnModule* module,
		 MvnNodeType type,
		 SizeType ibit_width1,
		 SizeType ibit_width2,
		 SizeType ibit_width3,
		 SizeType obit_width);

  /// @brief 多入力演算ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] type 型
  /// @param[in] ibit_width_array 入力のビット幅の幅
  /// @param[in] obit_width 出力のビット幅
  MvnNode*
  new_nary_op(MvnModule* module,
	      MvnNodeType type,
	      const vector<SizeType>& ibit_width_array,
	      SizeType obit_width);

  /// @brief ノードを登録する．
  /// @param[in] node 対象のノード
  void
  reg_node(MvnNode* node);

  /// @brief ノードの登録を削除する．
  /// @param[in] node 対象のノード
  void
  unreg_node(MvnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  ClibCellLibrary mLibrary;

  // モジュールを納めた配列
  // ID番号をキーにしている．
  // 場合によっては穴が空いている．
  vector<MvnModule*> mModuleArray;

  // モジュールのID番号を管理するためのオブジェクト
  ItvlMgr mModuleItvlMgr;

  // 全ノードを ID 番号をキーにして格納する配列
  vector<MvnNode*> mNodeArray;

  // ノードのID番号を管理するためのオブジェクト
  ItvlMgr mNodeItvlMgr;

};


/// @brief ノード番号ともとのVerilog名の対応を出力する．
void
dump_node_map(ostream& s,
	      const MvnMgr& mgr,
	      const MvnVlMap& node_map);

END_NAMESPACE_YM_MVN

#endif // YM_MVNMGR_H
