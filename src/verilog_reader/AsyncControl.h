#ifndef ASYNCCONTROL_H
#define ASYNCCONTROL_H

/// @file AsyncControl.h
/// @brief AsyncControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mvn.h"
#include "ym/vl/VlFwd.h"
#include "Env.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AsyncControl AsyncControl.h "AsyncControl.h"
/// @brief 非同期 set/reset 用コントロール
//////////////////////////////////////////////////////////////////////
struct AsyncControl
{

  // 環境
  ProcEnv mEnv;

  // コントロール信号
  MvnNode* mNode{nullptr};

  // 極性
  MvnPolarity mPol;

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // ASYNCCONTROL_H
