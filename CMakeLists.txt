# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${PROJECT_SOURCE_DIR}/ym-logic/include
  ${PROJECT_SOURCE_DIR}/ym-cell/include
  ${PROJECT_SOURCE_DIR}/ym-verilog/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

#add_subdirectory ( cython-src )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( mvn_SOURCES
  src/mvn/MvnCaseEq.cc
  src/mvn/MvnCellNode.cc
  src/mvn/MvnConst.cc
  src/mvn/MvnConstBitSelect.cc
  src/mvn/MvnConstPartSelect.cc
  src/mvn/MvnDff.cc
  src/mvn/MvnDumper.cc
  src/mvn/MvnMgr.cc
  src/mvn/MvnModule.cc
  src/mvn/MvnNode.cc
  src/mvn/MvnNodeBase.cc
  src/mvn/MvnPort.cc
  )

set ( verilog_reader_SOURCES
  src/verilog_reader/DeclHash.cc
  src/verilog_reader/DeclMap.cc
  src/verilog_reader/Env.cc
  src/verilog_reader/EnvMerger.cc
  src/verilog_reader/MvnVerilogReader.cc
  src/verilog_reader/MvnVlMap.cc
  src/verilog_reader/ReaderImpl.cc
  src/verilog_reader/ReaderImpl_expr.cc
  src/verilog_reader/ReaderImpl_item.cc
  src/verilog_reader/ReaderImpl_stmt.cc
  src/verilog_reader/Xmask.cc
  )

set ( verilog_writer_SOURCES
  src/verilog_writer/MvnVerilogWriter.cc
  src/verilog_writer/VerilogWriterImpl.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_object_library ( ym_mvn
  ${mvn_SOURCES}
  ${verilog_reader_SOURCES}
  ${verilog_writer_SOURCES}
  )
