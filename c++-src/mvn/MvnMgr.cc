
/// @file MvnMgr.cc
/// @brief MvnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MvnMgr.h"

#include "ym/MvnModule.h"
#include "ym/MvnPort.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] library セルライブラリ
MvnMgr::MvnMgr(
  const ClibCellLibrary& library
) : mLibrary{library}
{
}

// @brief デストラクタ
MvnMgr::~MvnMgr()
{
}

// @brief 関連付けられたセルライブラリを返す．
ClibCellLibrary
MvnMgr::library() const
{
  return mLibrary;
}

// @brief トップレベルモジュールのリストを得る．
// @return モジュールを格納するリストを返す．
// @note この関数はトップモジュール数ではなく全モジュール数に比例した
// 実行時間を要する．
vector<const MvnModule*>
MvnMgr::topmodule_list() const
{
  vector<const MvnModule*> module_list;
  for ( auto module: mModuleArray ) {
    if ( module && module->parent() == nullptr ) {
      module_list.push_back(module);
    }
  }
  return module_list;
}

// @brief モジュール番号の最大値+1を得る．
SizeType
MvnMgr::max_module_id() const
{
  return mModuleArray.size();
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は nullptr を返す．
const MvnModule*
MvnMgr::module(
  SizeType id
) const
{
  ASSERT_COND( id >= 0 && id < max_module_id() );
  return mModuleArray[id];
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は nullptr を返す．
MvnModule*
MvnMgr::_module(
  SizeType id
)
{
  ASSERT_COND( id >= 0 && id < max_module_id() );
  return mModuleArray[id];
}

// @brief ノードの ID番号の最大値 + 1 を返す．
SizeType
MvnMgr::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note nullptr が還されることもある．
const MvnNode*
MvnMgr::node(
  SizeType id
) const
{
  ASSERT_COND( 0 <= id && id < max_node_id() );
  return mNodeArray[id];
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note nullptr が還されることもある．
MvnNode*
MvnMgr::_node(
  SizeType id
)
{
  ASSERT_COND( 0 <= id && id < max_node_id() );
  return mNodeArray[id];
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
// @return 生成したモジュールを返す．
// @note 入出力ノードのビット幅は1で初期化される．
MvnModule*
MvnMgr::new_module(
  const string& name,
  SizeType np,
  SizeType ni,
  SizeType no,
  SizeType nio
)
{
  vector<SizeType> ibitwidth_array(ni, 1);
  vector<SizeType> obitwidth_array(no, 1);
  vector<SizeType> iobitwidth_array(nio, 1);
  return new_module(name, np,
		    ibitwidth_array, obitwidth_array, iobitwidth_array);
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ibitwidth_array 入力のビット幅の配列
// @param[in] obitwidth_array 出力のビット幅の配列
// @param[in] iobitwidth_array 入出力のビット幅の配列
// @return 生成したモジュールを返す．
MvnModule*
MvnMgr::new_module(
  const string& name,
  SizeType np,
  const vector<SizeType>& ibitwidth_array,
  const vector<SizeType>& obitwidth_array,
  const vector<SizeType>& iobitwidth_array
)
{
  int tmp = mModuleItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // IDが枯渇？
    return nullptr;
  }
  mModuleItvlMgr.erase(tmp);
  int id = tmp;

  SizeType ni = ibitwidth_array.size();
  SizeType no = obitwidth_array.size();
  SizeType nio = iobitwidth_array.size();
  auto module{new MvnModule(name, np, ni, no, nio)};
  module->mId = id;
  while ( mModuleArray.size() <= id ) {
    mModuleArray.push_back(nullptr);
  }
  mModuleArray[id] = module;

  for ( SizeType i = 0; i < ni; ++ i ) {
    auto node{new_input(module, ibitwidth_array[i])};
    module->mInputArray[i] = node;
  }
  for ( SizeType i = 0; i < no; ++ i ) {
    auto node{new_output(module, obitwidth_array[i])};
    module->mOutputArray[i] = node;
  }
  for ( SizeType i = 0; i < nio; ++ i ) {
    auto node{new_inout(module, iobitwidth_array[i])};
    module->mInoutArray[i] = node;
  }
  return module;
}

// @brief モジュールを削除する．
// @param[in] module 対象のモジュール
// @note モジュールインスタンスとして使われているモジュールは削除できない．
void
MvnMgr::delete_module(
  MvnModule* module
)
{
  if ( module->parent() ) {
    // モジュールインスタンスは削除できない．
    return;
  }
  // module の要素を削除

  // module の下位モジュールを再帰的に削除

  mModuleItvlMgr.add(module->mId);
  mModuleArray[module->mId] = nullptr;
  delete module;
}

// @brief モジュールのポートを初期化する．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] portref_list ポート参照式のリスト
// @param[in] name 名前
void
MvnMgr::init_port(
  MvnModule* module,
  SizeType pos,
  const vector<MvnPortRef>& portref_list,
  const string& name
)
{
  module->mPortArray[pos] = new MvnPort(portref_list, name);
}

#if 0
// @brief ポート参照式の内容を設定する(単純な形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
void
MvnMgr::set_port_ref(MvnModule* module,
		     int pos,
		     int port_ref_pos,
		     MvnNode* node)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node);
}

// @brief ポート参照式の内容を設定する(ビット指定形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
// @param[in] index ビット指定位置
void
MvnMgr::set_port_ref(MvnModule* module,
		     int pos,
		     int port_ref_pos,
		     MvnNode* node,
		     int index)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node, index);
}

// @brief ポート参照式の内容を設定する(範囲指定形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
void
MvnMgr::set_port_ref(MvnModule* module,
		     int pos,
		     int port_ref_pos,
		     MvnNode* node,
		     int msb,
		     int lsb)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node, msb, lsb);
}
#endif

// @brief ノードを削除する．
// @param[in] node 対象のノード
// @note 入力ノード, 出力ノードは削除できない
// @note 接続が残っているノードは削除できない．
void
MvnMgr::delete_node(
  MvnNode* node
)
{
  if ( node->type() == MvnNodeType::INPUT || node->type() == MvnNodeType::OUTPUT ) {
    return;
  }
  for ( SizeType i = 0; i < node->input_num(); ++ i ) {
    if ( node->input(i)->src_node() ) {
      cerr << "node" << node->id() << " has fanin" << endl;
      return;
    }
  }
  if ( !node->dst_pin_list().empty() ) {
    cerr << "node" << node->id() << " has fanout" << endl;
    return;
  }
  unreg_node(node);
  delete node;
}

bool
no_fanouts(
  MvnNode* node
)
{
  if ( !node->dst_pin_list().empty() ) {
    return false;
  }
  return true;
}

// @brief 冗長な through ノードを取り除く
void
MvnMgr::sweep()
{
  SizeType n{max_node_id()};
  // ビット/部分選択と接続している連結演算の削除を行う．
  vector<MvnNode*> node_list;
  node_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto node{_node(i)};
    if ( node != nullptr ) {
      node_list.push_back(node);
    }
  }
  for ( auto node: node_list ) {
    MvnNode* alt_node = nullptr;
    if ( node->type() == MvnNodeType::THROUGH ) {
      auto src_node = node->input(0)->src_node();
      if ( src_node != nullptr ) {
	alt_node = src_node;
      }
    }
    else if ( node->type() == MvnNodeType::CONSTBITSELECT ) {
      auto src_node = node->input(0)->src_node();
      if ( src_node->type() == MvnNodeType::CONCAT ) {
	alt_node = select_from_concat(src_node, node->bitpos());
      }
      else if ( src_node->type() == MvnNodeType::CONSTPARTSELECT ) {
	alt_node = select_from_partselect(src_node, node->bitpos());
      }
    }
    if ( alt_node != nullptr ) {
      // node を alt_node に置き換える．
      replace(node, alt_node);
    }
  }

  // どこにも出力していないノードを削除する．
  vector<bool> marks(n, false);
  for ( int i = 0; i < n; ++ i ) {
    auto node = _node(i);
    if ( node->type() == MvnNodeType::DFF ) {
      SizeType nc{node->input_num() - 2};
      for ( SizeType j = 0; j < nc; ++ j ) {
	auto node1 = node->control_val(j);
	marks[node1->id()] = true;
      }
    }
  }
  list<MvnNode*> node_queue;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto node = _node(i);
    if ( node == nullptr ) continue;
    if ( node->type() == MvnNodeType::INPUT ||
	 node->type() == MvnNodeType::OUTPUT ||
	 node->type() == MvnNodeType::INOUT ) continue;
    if ( marks[node->id()] ) continue;
    if ( no_fanouts(node) ) {
      node_queue.push_back(node);
    }
  }
  while ( !node_queue.empty() ) {
    auto node = node_queue.front();
    node_queue.pop_front();
    SizeType ni{node->input_num()};
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto src_node = node->input(i)->src_node();
      if ( src_node ) {
	disconnect(src_node, 0, node, i);
	if ( no_fanouts(src_node) ) {
	  node_queue.push_back(src_node);
	}
      }
    }
    delete_node(node);
  }
}

// @brief 連結演算からビットを抜き出す．
// @param[in] src_node 連結演算ノード
// @param[in] bitpos 抜き出すビット位置
MvnNode*
MvnMgr::select_from_concat(
  MvnNode* src_node,
  SizeType bitpos
)
{
  ASSERT_COND( src_node->type() == MvnNodeType::CONCAT );
  SizeType ni{src_node->input_num()};
  for ( SizeType i = 0; i < ni; ++ i ) {
    SizeType idx{ni - i - 1};
    auto ipin = src_node->input(idx);
    SizeType bw{ipin->bit_width()};
    if ( bitpos < bw ) {
      auto inode = ipin->src_node();
      if ( inode->type() == MvnNodeType::CONCAT ) {
	return select_from_concat(inode, bitpos);
      }
      else if ( inode->type() == MvnNodeType::CONSTPARTSELECT ) {
	return select_from_partselect(inode, bitpos);
      }
      else if ( bw == 1 ) {
	return inode;
      }
      else {
	auto bitsel = new_constbitselect(src_node->mParent, bitpos, bw);
	connect(inode, 0, bitsel, 0);
	return bitsel;
      }
    }
    bitpos -= bw;
  }
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 部分指定子からビットを抜き出す．
// @param[in] src_node 部分指定ノード
// @param[in] bitpos 抜き出すビット位置
MvnNode*
MvnMgr::select_from_partselect(
  MvnNode* src_node,
  SizeType bitpos
)
{
  ASSERT_COND( src_node->type() == MvnNodeType::CONSTPARTSELECT );

  auto ipin = src_node->input(0);
  SizeType bw{ipin->bit_width()};
  ASSERT_COND( bitpos < bw );
  SizeType msb = src_node->msb();
  SizeType lsb = src_node->lsb();
  if ( msb > lsb ) {
    bitpos = bitpos + lsb;
  }
  else {
    bitpos = lsb - bitpos;
  }

  auto inode = ipin->src_node();
  if ( inode->type() == MvnNodeType::CONCAT ) {
    return select_from_concat(inode, bitpos);
  }
  else if ( inode->type() == MvnNodeType::CONSTPARTSELECT ) {
    return select_from_partselect(inode, bitpos);
  }
  else {
    auto bitsel = new_constbitselect(src_node->mParent, bitpos, bw);
    connect(inode, 0, bitsel, 0);
    return bitsel;
  }
}

// @brief node を alt_node に置き換える．
// @param[in] node 置換元のノード
// @param[in] alt_node 置換先のノード
// @note node は削除される．
void
MvnMgr::replace(
  MvnNode* node,
  MvnNode* alt_node
)
{
  reconnect(node, 0, alt_node, 0);
}

// @brief ピンとピンを接続する．
// @param[in] src_node 入力元のノード
// @param[in] src_pin_pos 入力元のピン番号
// @param[in] dst_node 出力先のノード
// @param[in] dst_pin 出力先のピン番号
// @return 接続を表すネットを返す．
// @note 接続が失敗したら nullptrを返す．
// @note 接続が失敗するのは，
//  - ピンが異なるモジュールに属していた．
//  - ピンのビット幅が異なっていた．
bool
MvnMgr::connect(
  MvnNode* src_node,
  SizeType src_pin_pos,
  MvnNode* dst_node,
  SizeType dst_pin_pos
)
{
  if ( src_node->parent() != dst_node->parent() ) {
    cerr << "connecting between differnt module" << endl;
    abort();
    return false;
  }

  auto dst_pin = dst_node->_input(dst_pin_pos);
  if ( src_node->bit_width() != dst_pin->bit_width() ) {
    cerr << "src_node->bit_width() = " << src_node->bit_width()
	 << ", dst_pin->bit_width() = " << dst_pin->bit_width() << endl;
    cerr << "bit_width mismatch" << endl;
    abort();
    return false;;
  }
  src_node->mDstPinList.push_back(dst_pin);
  dst_pin->mSrcNode = src_node;
  return true;
}

// @brief 接続を取り除く
// @param[in] src_node 入力元のノード
// @param[in] src_pin_pos 入力元のピン番号
// @param[in] dst_node 出力先のノード
// @param[in] dst_pin 出力先のピン番号
void
MvnMgr::disconnect(
  MvnNode* src_node,
  SizeType src_pin_pos,
  MvnNode* dst_node,
  SizeType dst_pin_pos
)
{
  auto dst_pin = dst_node->_input(dst_pin_pos);
  ASSERT_COND( dst_pin->mSrcNode == src_node );
  dst_pin->mSrcNode = nullptr;
  #warning "src_node の mDstPinList から dst_pin を取り除く"
}

// @brief 接続を切り替える．
// @param[in] old_node 元のノード
// @param[in] old_pin_pos 元のピン番号
// @param[in] new_node 新しいノード
// @param[in] new_pin 新しいピン番号
void
MvnMgr::reconnect(
  MvnNode* old_node,
  SizeType old_pin_pos,
  MvnNode* new_node,
  SizeType new_pin_pos
)
{
  auto& fo_list{old_node->dst_pin_list()};

  // リンクトリストをたどっている途中でリンクの変更はできないので
  // 配列にコピーする．
  vector<MvnInputPin*> tmp_list;
  tmp_list.reserve(fo_list.size());
  for ( auto ipin: fo_list ) {
    tmp_list.push_back(ipin);
  }
  for ( auto ipin: tmp_list ) {
#warning "old_node->mDstPinList から ipin を取り除く"
    ipin->mSrcNode = new_node;
    new_node->mDstPinList.push_back(ipin);
  }
}

// @brief ノードを登録する．
// @param[in] node 対象のノード
void
MvnMgr::reg_node(
  MvnNode* node
)
{
  int tmp = mNodeItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // ID番号が枯渇？
    return;
  }
  mNodeItvlMgr.erase(tmp);
  int id = tmp;
  node->mId = id;
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(nullptr);
  }
  mNodeArray[id] = node;

  if ( node->type() != MvnNodeType::INPUT &&
       node->type() != MvnNodeType::OUTPUT &&
       node->type() != MvnNodeType::INOUT ) {
    MvnModule* module = node->mParent;
    module->mNodeList.push_back(node);
  }
}

// @brief ノードの登録を削除する．
// @param[in] node 対象のノード
void
MvnMgr::unreg_node(
  MvnNode* node
)
{
  mNodeItvlMgr.add(node->id());
  mNodeArray[node->id()] = nullptr;

  if ( node->type() != MvnNodeType::INPUT &&
       node->type() != MvnNodeType::OUTPUT &&
       node->type() != MvnNodeType::INOUT ) {
    MvnModule* module = node->mParent;
    #warning "module->mNodeList から node を取り除く"
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MvnModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
MvnModule::MvnModule(
  const string& name,
  SizeType np,
  SizeType ni,
  SizeType no,
  SizeType nio
) : mName(name),
    mParent(nullptr),
    mPortArray(np),
    mInputArray(ni),
    mOutputArray(no),
    mInoutArray(nio)
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvnInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnInputPin::MvnInputPin() :
  mNode{nullptr},
  mPos{0},
  mBitWidth{0},
  mSrcNode{nullptr}
{
}

// @brief デストラクタ
MvnInputPin::~MvnInputPin()
{
}

END_NAMESPACE_YM_MVN
