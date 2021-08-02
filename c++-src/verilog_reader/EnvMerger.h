#ifndef ENVMERGER_H
#define ENVMERGER_H

/// @file EnvMerger.h
/// @brief EnvMerger のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"
#include "Env.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EnvMerger EnvMerger.h "EnvMerger.h"
/// @brief 条件分岐で Env をマージするための基底クラス
//////////////////////////////////////////////////////////////////////
class EnvMerger
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr MvnMgr
  EnvMerger(
    MvnMgr* mgr
  ) : mMvnMgr{mgr}
  {
  }

  /// @brief デストラクタ
  virtual
  ~EnvMerger() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Env をマージする．
  virtual
  void
  operator()(
    MvnModule* parent_module, ///< [in] 親のモジュール
    ProcEnv& env,             ///< [in] 対象の環境
    MvnNode* cond,            ///< [in] 条件を表すノード
    const ProcEnv& then_env,  ///< [in] 条件が成り立ったときに通るパスの環境
    const ProcEnv& else_env   ///< [in] 条件が成り立たなかったときに通るパスの環境
  ) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入条件をマージする．
  ///
  /// 基本的には ITE(cond, then_cond, else_cond) だが，nullptr の場合がある．
  MvnNode*
  merge_cond(
    MvnModule* parent_module, ///< [in] 親のモジュール
    MvnNode* cond,            ///< [in] 切り替え条件
    MvnNode* then_cond,       ///< [in] cond が成り立ったときの代入条件
    MvnNode* else_cond        ///< [in] cond が成り立たなかったときの代入条件
  );

  /// @brief MvMgr を返す．
  MvnMgr*
  mgr() const
  {
    return mMvnMgr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNetwork を扱うマネージャ
  MvnMgr* mMvnMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class EnvMerger1 EnvMerger.h "EnvMerger.h"
/// @brief combinational always 用の merger
//////////////////////////////////////////////////////////////////////
class EnvMerger1 :
  public EnvMerger
{
public:

  /// @brief コンストラクタ
  EnvMerger1(
    MvnMgr* mgr ///< [in] Mvn ネットワーク
  ) : EnvMerger(mgr)
  {
  }

  /// @brief デストラクタ
  ~EnvMerger1() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Env をマージする．
  void
  operator()(
    MvnModule* parent_module, ///< [in] 親のモジュール
    ProcEnv& env,	      ///< [in] 対象の環境
    MvnNode* cond,	      ///< [in] 条件を表すノード
    const ProcEnv& then_env,  ///< [in] 条件が成り立ったときに通るパスの環境
    const ProcEnv& else_env   ///< [in] 条件が成り立たなかったときに通るパスの環境
  ) override;

};


//////////////////////////////////////////////////////////////////////
/// @class EnvMerger2 EnvMerger.h "EnvMerger.h"
/// @brief sequential always 用の merger
//////////////////////////////////////////////////////////////////////
class EnvMerger2 :
  public EnvMerger
{
public:

  /// @brief コンストラクタ
  EnvMerger2(
    MvnMgr* mgr,    ///< [in] Mvn ネットワーク
    Env& global_env ///< [in] グローバル環境
  ) : EnvMerger(mgr),
      mGlobalEnv{global_env}
  {
  }

  /// @brief デストラクタ
  ~EnvMerger2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Env をマージする．
  void
  operator()(
    MvnModule* parent_module, ///< [in] 親のモジュール
    ProcEnv& env,	      ///< [in] 対象の環境
    MvnNode* cond,	      ///< [in] 条件を表すノード
    const ProcEnv& then_env,  ///< [in] 条件が成り立ったときに通るパスの環境
    const ProcEnv& else_env   ///< [in] 条件が成り立たなかったときに通るパスの環境
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グローバル環境
  Env& mGlobalEnv;

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // ENVMERGER_H
