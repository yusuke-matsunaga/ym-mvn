#ifndef YM_MVNMGR_H
#define YM_MVNMGR_H

/// @file ym/MvnMgr.h
/// @brief MvnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2020, 2021 Yusuke Matsunaga
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
  explicit
  MvnMgr(
    const ClibCellLibrary& library = ClibCellLibrary() ///< [in] セルライブラリ
  );

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
  /// @return 該当するモジュールを返す．
  ///
  /// 該当するモジュールがない場合は nullptr を返す．
  const MvnModule*
  module(
    SizeType id ///< [in] モジュールID ( 0 <= id < max_module_id() )
  ) const;

  /// @brief モジュールIDをキーにしてモジュールにアクセスする．
  /// @return 該当するモジュールを返す．
  ///
  /// 該当するモジュールがない場合は nullptr を返す．
  MvnModule*
  _module(
    SizeType id ///< [in] モジュールID ( 0 <= id < max_module_id() )
  );

  /// @brief ノードの ID番号の最大値 + 1 を返す．
  SizeType
  max_node_id() const;

  /// @brief ノードを得る．
  ///
  /// nullptr が返されることもある．
  const MvnNode*
  node(
    SizeType id ///< [in] ID番号 ( 0 <= id < max_node_id() )
  ) const;

  /// @brief ノードを得る．
  ///
  /// nullptr が返されることもある．
  MvnNode*
  _node(
    SizeType id ///< [in] ID番号 ( 0 <= id < max_node_id() )
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の設定を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールを生成する．
  /// @return 生成したモジュールを返す．
  ///
  /// 入出力ノードのビット幅は1で初期化される．
  MvnModule*
  new_module(
    const string& name, ///< [in] 名前
    SizeType np,        ///< [in] ポート数
    SizeType ni,        ///< [in] 入力ノード数
    SizeType no,        ///< [in] 出力ノード数
    SizeType nio        ///< [in] 入出力ノード数
  );

  /// @brief モジュールを生成する．
  /// @return 生成したモジュールを返す．
  MvnModule*
  new_module(
    const string& name,                      ///< [in] 名前
    SizeType np,                             ///< [in] ポート数
    const vector<SizeType>& ibitwidth_array, ///< [in] 入力ノードのビット幅の配列
    const vector<SizeType>& obitwidth_array, ///< [in] 出力ノードのビット幅の配列
    const vector<SizeType>& iobitwidth_array ///< [in] 入出力ノードのビット幅の配列
  );

  /// @brief モジュールを削除する．
  ///
  /// モジュールインスタンスとして使われているモジュールは削除できない．
  void
  delete_module(
    MvnModule* module ///< [in] 対象のモジュール
  );

  /// @brief モジュールのポートを初期化する．
  void
  init_port(
    MvnModule* module,                     ///< [in] 対象のモジュール
    SizeType pos,                          ///< [in] ポート番号 (0 <= pos < <module->port_num())
    const vector<MvnPortRef>& portref_list ///< [in] ポート参照式のリスト
    = vector<MvnPortRef>(),
    const string& name = string()          ///< [in] ポート名
  );

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
  MvnNode*
  new_input(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] ビット幅
  );

  /// @brief 出力ノードを生成する．
  MvnNode*
  new_output(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] ビット幅
  );

  /// @brief 入出力ノードを生成する．
  MvnNode*
  new_inout(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] ビット幅
  );

  /// @brief フリップフロップノードを生成する．
  ///
  /// pol_array の要素数が非同期セット信号数となる．
  MvnNode*
  new_dff(
    MvnModule* module,                    ///< [in] ノードが属するモジュール
    MvnPolarity clock_pol,                ///< [in] クロックの極性
    const vector<MvnPolarity>& pol_array, ///< [in] 非同期セット信号の極性情報を入れた配列
    const vector<MvnNode*>& val_array,    ///< [in] 非同期セットの値を入れた配列
    SizeType bit_width = 1                ///< [in] ビット幅
  );

  /// @brief ラッチノードを生成する．
  MvnNode*
  new_latch(
    MvnModule* module,     ///< [in] ノードが属するモジュール
    SizeType bit_width = 1 ///< [in] ビット幅
  );

  /// @brief through ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_through(
    MvnModule* module,      ///< [in] ノードが属するモジュール
    SizeType bit_width = 1  ///< [in] ビット幅
  );

  /// @brief not ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_not(
    MvnModule* module,     ///< [in] ノードが属するモジュール
    SizeType bit_width = 1 ///< [in] ビット幅
  );

  /// @brief and ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// すべての入力と出力のビット幅は同一
  MvnNode*
  new_and(MvnModule* module,     ///< [in] ノードが属するモジュール
	  SizeType input_num,	 ///< [in] 入力数
	  SizeType bit_width = 1 ///< [in] ビット幅
  );

  /// @brief or ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// すべての入力と出力のビット幅は同一
  MvnNode*
  new_or(
    MvnModule* module,     ///< [in] ノードが属するモジュール
    SizeType input_num,	   ///< [in] 入力数
    SizeType bit_width = 1 ///< [in] ビット幅
  );

  /// @brief xor ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// すべての入力と出力のビット幅は同一
  MvnNode*
  new_xor(
    MvnModule* module,     ///< [in] ノードが属するモジュール
    SizeType input_num,	   ///< [in] 入力数
    SizeType bit_width = 1 ///< [in] ビット幅
  );

  /// @brief reduction and ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は常に1
  MvnNode*
  new_rand(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] 入力のビット幅
  );

  /// @brief reduction or ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は常に1
  MvnNode*
  new_ror(
    MvnModule* module,  ///< [in] ノードが属するモジュール
    SizeType bit_width  ///< [in] 入力のビット幅
  );

  /// @brief reduction xor ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は常に1
  MvnNode*
  new_rxor(
    MvnModule* module,  ///< [in] ノードが属するモジュール
    SizeType bit_width  ///< [in] 入力のビット幅
  );

  /// @brief equal ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は常に1
  MvnNode*
  new_equal(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] 入力のビット幅
  );

  /// @brief case 文用の equal ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// - 最初の入力が case expression
  /// - 残りの入力は caseitem expression
  MvnNode*
  new_caseeq(
    MvnModule* module,      ///< [in] ノードが属するモジュール
    SizeType bit_width,	    ///< [in] 入力のビット幅
    const MvnBvConst& xmask ///< [in] Xマスク値
  );

  /// @brief less than ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は常に1
  MvnNode*
  new_lt(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] 入力のビット幅
  );

  /// @brief shift left logical ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_sll(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力データのビット幅
    SizeType bit_width2, ///< [in] シフト量のビット幅
    SizeType bit_width3  ///< [in] 出力のビット幅
  );

  /// @brief shift right logical ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_srl(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力データのビット幅
    SizeType bit_width2, ///< [in] シフト量のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief shift left arithmetic ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_sla(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力データのビット幅
    SizeType bit_width2, ///< [in] シフト量のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief shift right arithmetic ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_sra(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力データのビット幅
    SizeType bit_width2, ///< [in] シフト量のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief cmpl ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 2の補数を計算する．
  MvnNode*
  new_cmpl(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] ビット幅
  );

  /// @brief add ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_add(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力1のビット幅
    SizeType bit_width2, ///< [in] 入力2のビット幅
    SizeType bit_width3  ///< [in] 出力のビット幅
  );

  /// @brief sub ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_sub(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力1のビット幅
    SizeType bit_width2, ///< [in] 入力2のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief mult ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_mult(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力1のビット幅
    SizeType bit_width2, ///< [in] 入力2のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief div ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_div(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力1のビット幅
    SizeType bit_width2, ///< [in] 入力2のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief modulo ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_mod(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力1のビット幅
    SizeType bit_width2, ///< [in] 入力2のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief power ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_pow(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力1のビット幅
    SizeType bit_width2, ///< [in] 入力2のビット幅
    SizeType bit_width3	 ///< [in] 出力のビット幅
  );

  /// @brief condition ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_ite(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bit_width ///< [in] ビット幅
  );

  /// @brief concatenate ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は入力のビット幅の和となる．
  MvnNode*
  new_concat(
    MvnModule* module,                      ///< [in] ノードが属するモジュール
    const vector<SizeType>& ibitwidth_array ///< [in] 入力のビット幅の配列
  );

  /// @brief bit-selectノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は常に1
  MvnNode*
  new_constbitselect(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType bitpos,   ///< [in] 選択するビット位置
    SizeType bit_width ///< [in] 入力データのビット幅
  );

  /// @brief part-select ノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は msb - lsb + 1 となる．
  MvnNode*
  new_constpartselect(
    MvnModule* module, ///< [in] ノードが属するモジュール
    SizeType msb,      ///< [in] 範囲指定の MSB
    SizeType lsb,      ///< [in] 範囲指定の LSB
    SizeType bit_width ///< [in] 入力データのビット幅
  );

  /// @brief 可変 bit-selectノードを生成する．
  /// @return 生成したノードを返す．
  ///
  /// 出力のビット幅は常に1
  MvnNode*
  new_bitselect(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力データのビット幅
    SizeType bit_width2  ///< [in] ビット指定入力のビット幅
  );

  /// @brief 可変 part-select ノードを生成する．
  /// @return 生成したノードを返す．
  MvnNode*
  new_partselect(
    MvnModule* module,   ///< [in] ノードが属するモジュール
    SizeType bit_width1, ///< [in] 入力データのビット幅
    SizeType bit_width2, ///< [in] 範囲指定入力のビット幅
    SizeType bit_width3  ///< [in] 範囲のビット幅(出力のビット幅)
  );

  /// @brief constant ノードを生成する．
  ///
  /// ビット幅は値のビット幅で決まる．
  MvnNode*
  new_const(
    MvnModule* module,    ///< [in] ノードが属するモジュール
    const MvnBvConst& val ///< [in] 値
  );

  /// @brief セルノードを生成する．
  MvnNode*
  new_cell(
    MvnModule* module, ///< [in] ノードが属するモジュール
    int cell_id        ///< [in] セル番号
  );

  /// @brief ノードを削除する．
  ///
  /// - 入力ノード, 出力ノードは削除できない
  /// - 接続が残っているノードは削除できない．
  void
  delete_node(
    MvnNode* node ///< [in] 対象のノード
  );

  /// @brief node を alt_node に置き換える．
  ///
  /// node は削除される．
  void
  replace(
    MvnNode* node,    ///< [in] 置換元のノード
    MvnNode* alt_node ///< [in] 置換先のノード
  );

  /// @brief ピンとピンを接続する．
  /// @retval true 接続が成功した．
  /// @retval false 接続が失敗した．
  ///
  /// 接続が失敗するのは，
  ///  - ピンが異なるモジュールに属していた．
  ///  - ピンのビット幅が異なっていた．
  bool
  connect(
    MvnNode* src_node,    ///< [in] 入力元のノード
    SizeType src_pin_pos, ///< [in] 入力元のピン番号
    MvnNode* dst_node,    ///< [in] 出力先のノード
    SizeType dst_pin_pos  ///< [in] 出力先のピン番号
  );

  /// @brief 接続を取り除く
  ///
  /// 現在の実装はあまり効率が良くない．
  void
  disconnect(
    MvnNode* src_node,    ///< [in] 入力元のノード
    SizeType src_pin_pos, ///< [in] 入力元のピン番号
    MvnNode* dst_node,    ///< [in] 出力先のノード
    SizeType dst_pin_pos  ///< [in] 出力先のピン番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 連結演算からビットを抜き出す．
  MvnNode*
  select_from_concat(
    MvnNode* src_node, ///< [in] 連結演算ノード
    SizeType bitpos    ///< [in] 抜き出すビット位置
  );

  /// @brief 部分指定子からビットを抜き出す．
  MvnNode*
  select_from_partselect(
    MvnNode* src_node, ///< [in] 部分指定ノード
    SizeType bitpos    ///< [in] 抜き出すビット位置
  );

  /// @brief 接続を切り替える．
  void
  reconnect(
    MvnNode* old_node,    ///< [in] 元のノード
    SizeType old_pin_pos, ///< [in] 元のピン番号
    MvnNode* new_node,    ///< [in] 新しいノード
    SizeType new_pin_pos  ///< [in] 新しいピン番号
  );

  /// @brief 多入力論理演算ノードを生成する．
  MvnNode*
  new_log_op(
    MvnModule* module,  ///< [in] ノードが属するモジュール
    MvnNodeType type,   ///< [in] 型
    SizeType input_num, ///< [in] 入力数
    SizeType bit_width  ///< [in] ビット幅
  );

  /// @brief 1入力演算ノードを生成する．
  MvnNode*
  new_unary_op(
    MvnModule* module,    ///< [in] ノードが属するモジュール
    MvnNodeType type,     ///< [in] 型
    SizeType ibit_width1, ///< [in] 入力のビット幅
    SizeType obit_width   ///< [in] 出力のビット幅
  );

  /// @brief 2入力演算ノードを生成する．
  MvnNode*
  new_binary_op(
    MvnModule* module,    ///< [in] ノードが属するモジュール
    MvnNodeType type,     ///< [in] 型
    SizeType ibit_width1, ///< [in] 入力1のビット幅
    SizeType ibit_width2, ///< [in] 入力2のビット幅
    SizeType obit_width   ///< [in] 出力のビット幅
  );

  /// @brief 3入力演算ノードを生成する．
  MvnNode*
  new_ternary_op(
    MvnModule* module,    ///< [in] ノードが属するモジュール
    MvnNodeType type,     ///< [in] 型
    SizeType ibit_width1, ///< [in] 入力1のビット幅
    SizeType ibit_width2, ///< [in] 入力2のビット幅
    SizeType ibit_width3, ///< [in] 入力3のビット幅
    SizeType obit_width   ///< [in] 出力のビット幅
  );

  /// @brief 多入力演算ノードを生成する．
  MvnNode*
  new_nary_op(
    MvnModule* module,                        ///< [in] ノードが属するモジュール
    MvnNodeType type,                         ///< [in] 型
    const vector<SizeType>& ibit_width_array, ///< [in] 入力のビット幅の幅
    SizeType obit_width                       ///< [in] 出力のビット幅
  );

  /// @brief ノードを登録する．
  void
  reg_node(
    MvnNode* node ///< [in] 対象のノード
  );

  /// @brief ノードの登録を削除する．
  void
  unreg_node(
    MvnNode* node ///< [in] 対象のノード
  );


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
dump_node_map(
  ostream& s,              ///< [in] 出力ストリーム
  const MvnMgr& mgr,       ///< [in] MVN ネットワーク
  const MvnVlMap& node_map ///< [in] もとの Verilog 記述とのマップ
);

END_NAMESPACE_YM_MVN

#endif // YM_MVNMGR_H
