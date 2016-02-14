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
ym_add_library ( ym_mvn
  SHARED
  ${mvn_SOURCES}
  ${verilog_reader_SOURCES}
  ${verilog_writer_SOURCES}
  DESTINATION ${YM_INSTALL_LIBDIR}
  )

target_link_libraries ( ym_mvn
  ym_verilog
  ym_cell
  ym_logic
  ym_common
  )

target_link_libraries ( ym_mvn_d
  ym_verilog_d
  ym_cell_d
  ym_logic_d
  ym_common_d
  )

target_link_libraries ( ym_mvn_p
  ym_verilog_p
  ym_cell_p
  ym_logic_p
  ym_common_p
  )
