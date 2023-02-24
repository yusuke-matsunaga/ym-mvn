#ifndef MVNCELLNODE_H
#define MVNCELLNODE_H

/// @file MvnCellNode.h
/// @brief MvnCellNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MvnNodeBase.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @calss MvnCellNode MvnCellNode.h "MvnCellNode.h"
/// @brief セルインスタンスを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnCellNode :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnCellNode(
    MvnModule* module,   ///< [in] 親のモジュール
    const ClibCell& cell ///< [in] セル
  );

  /// @brief デストラクタ
  ~MvnCellNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル番号を得る．
  /// @note type() が kCell の時のみ意味を持つ．
  ClibCell
  cell() const override;

  /// @brief セルの出力ピン番号を返す．
  /// @note type() が kCell の時のみ意味を持つ．
  int
  cell_opin_pos() const override;

  /// @brief 多出力セルノードの場合の代表ノードを返す．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note 1出力セルノードの時には自分自身を返す．
  const MvnNode*
  cell_node() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  ClibCell mCell;

};


//////////////////////////////////////////////////////////////////////
/// @class MvnExtCellNode MvnCellNode.h "MvnCellNode.h"
/// @brief 2つめ以降の出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnExtCellNode :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnExtCellNode(
    MvnModule* module,  ///< [in] 親のモジュール
    MvnNode* cell_node, ///< [in] 代表ノード
    int opos            ///< [in] 出力ピン番号
  );

  /// @brief デストラクタ
  ~MvnExtCellNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル番号を得る．
  /// @note type() が kCell の時のみ意味を持つ．
  ClibCell
  cell() const override;

  /// @brief セルの出力ピン番号を返す．
  /// @note type() が kCell の時のみ意味を持つ．
  int
  cell_opin_pos() const override;

  /// @brief 多出力セルノードの場合の代表ノードを返す．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note 1出力セルノードの時には自分自身を返す．
  const MvnNode*
  cell_node() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表ノード
  MvnNode* mCellNode;

  // 出力ピン番号
  ymuint32 mOpos;

};

END_NAMESPACE_YM_MVN

#endif // MVNCELLNODE_H
