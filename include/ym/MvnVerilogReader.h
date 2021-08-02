#ifndef YM_MVNVERILOGREADER_H
#define YM_MVNVERILOGREADER_H

/// @file ym/MvnVerilogReader.h
/// @brief MvnVerilogReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"
#include "ym/verilog.h"
#include "ym/vl/VlFwd.h"
#include "ym/File.h"
#include "ym/MsgHandler.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

class ReaderImpl;

END_NAMESPACE_YM_MVN_VERILOG

BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnVerilogReader MvnVerilogReader.h "ym/MvnVerilogReader.h"
/// @brief 合成可能な Verilog 記述を読み込んで MVN に設定するクラス
///
/// Verilog 記述は複数ファイルで構成される場合があるので，
/// ファイルを読み込んだだけでは Mvn を生成しない．
//////////////////////////////////////////////////////////////////////
class MvnVerilogReader
{
public:

  /// @brief コンストラクタ
  MvnVerilogReader();

  /// @brief デストラクタ
  ~MvnVerilogReader();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief verilog 形式のファイルを読み込む．
  /// @retval true 正常に読み込めた．
  /// @retval false 読込中にエラーが起こった．
  bool
  read(
    const string& filename,                   ///< [in] ファイル名
    const SearchPathList& searchpath          ///< [in] サーチパス
    = SearchPathList(),
    const vector<VlLineWatcher*> watcher_list ///< [in] 行番号ウォッチャーのリスト
    = vector<VlLineWatcher*>());

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(
    MvnMgr& mgr,       ///< [in] ネットワーク全体を表すのマネージャ
    MvnVlMap& node_map ///< [out] MvNode と宣言要素の対応付けを保持する配列
  );

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(
    MvnMgr& mgr,                    ///< [in] ネットワーク全体を表すのマネージャ
    const ClibCellLibrary& library, ///< [in] セルライブラリ
    MvnVlMap& node_map              ///< [out] MvNode と宣言要素の対応付けを保持する配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の読み込みを行うオブジェクト
  unique_ptr<nsVerilog::ReaderImpl> mImpl;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVNVERILOGREADER_H
