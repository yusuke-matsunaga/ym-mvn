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
  ${PROJECT_BINARY_DIR}/include
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

add_subdirectory ( tests/programs )
#add_subdirectory ( cython-src )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( mvn_SOURCES
  c++-src/mvn/MvnBvConst.cc
  c++-src/mvn/MvnCaseEq.cc
  c++-src/mvn/MvnCellNode.cc
  c++-src/mvn/MvnConst.cc
  c++-src/mvn/MvnConstBitSelect.cc
  c++-src/mvn/MvnConstPartSelect.cc
  c++-src/mvn/MvnDff.cc
  c++-src/mvn/MvnDumper.cc
  c++-src/mvn/MvnMgr.cc
  c++-src/mvn/MvnNodeBase.cc
  c++-src/mvn/MvnPort.cc
  )

set ( verilog_reader_SOURCES
  c++-src/verilog_reader/DeclHash.cc
  c++-src/verilog_reader/DeclMap.cc
  c++-src/verilog_reader/Env.cc
  c++-src/verilog_reader/EnvMerger.cc
  c++-src/verilog_reader/MvnVerilogReader.cc
  c++-src/verilog_reader/MvnVlMap.cc
  c++-src/verilog_reader/ReaderImpl.cc
  c++-src/verilog_reader/ReaderImpl_expr.cc
  c++-src/verilog_reader/ReaderImpl_item.cc
  c++-src/verilog_reader/ReaderImpl_stmt.cc
  )

set ( verilog_writer_SOURCES
  c++-src/verilog_writer/MvnVerilogWriter.cc
  c++-src/verilog_writer/VerilogWriterImpl.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_object_library ( ym_mvn
  ${mvn_SOURCES}
  ${verilog_reader_SOURCES}
  ${verilog_writer_SOURCES}
  )
