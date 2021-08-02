#ifndef DECLHASH_H
#define DECLHASH_H

/// @file DeclHash.h
/// @brief DeclHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mvn.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class DeclHash DeclHash.h "DeclHash.h"
/// @brief VlDecl とID番号の対応付けを行うハッシュ表
//////////////////////////////////////////////////////////////////////
class DeclHash
{
public:

  /// @brief コンストラクタ
  DeclHash() = default;

  /// @brief デストラクタ
  ~DeclHash() = default;


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief ID番号を得る．
  /// @return ID番号
  ///
  /// 登録されていなかった場合には新しい番号を割り当てる．
  SizeType
  get_id(
    const VlDecl* decl ///< [in] 宣言要素
  );

  /// @brief ID番号を得る．
  /// @return ID番号
  ///
  /// 登録されていなかった場合には新しい番号を割り当てる．
  SizeType
  get_id(
    const VlDeclArray* decl, ///< [in] 配列型宣言要素
    SizeType offset          ///< [in] オフセット
  );

  /// @brief ID番号の最大値 + 1を返す．
  SizeType
  max_id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表の本体
  unordered_map<const VlObj*, SizeType> mHash;

  // 次に割り当て可能な ID 番号
  SizeType mNextId{0};

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // DECLHASH_H
