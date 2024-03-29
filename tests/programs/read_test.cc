﻿
/// @file read_test.cc
/// @brief MvnNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MvnMgr.h"
#include "ym/MvnVerilogReader.h"
#include "ym/MvnVlMap.h"
#include "ym/MvnDumper.h"
#include "ym/MvnVerilogWriter.h"

#include "ym/ClibCellLibrary.h"

#include "ym/PoptMainApp.h"
#include "ym/MsgMgr.h"
#include "ym/MsgHandler.h"
#include "ym/StreamMsgHandler.h"


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;

  PoptMainApp popt;

  PoptStr popt_dotlib("liberty", 0, "specify liverty(dotlib) library", "\"file name\"");
  PoptStr popt_mislib("mislib", 0, "specify mislib library", "\"file name\"");
  PoptNone popt_dump("dump", 'd', "dump network");
  PoptNone popt_verilog("verilog", 'V', "dump verilog");

  popt.add_option(&popt_dotlib);
  popt.add_option(&popt_mislib);
  popt.add_option(&popt_dump);
  popt.add_option(&popt_verilog);

  popt.set_other_option_help("<file-name> ...");

  PoptStat stat = popt.parse_options(argc, argv, 0);
  if ( stat == PoptStat::Abort ) {
    return -1;
  }

  // 残りの引数はすべてファイル名と見なす
  vector<string> filename_list;
  auto n_files = popt.get_args(filename_list);

  ClibCellLibrary cell_library;
  if ( popt_dotlib.is_specified() ) {
    cell_library = ClibCellLibrary::read_liberty(popt_dotlib.val());
    if ( !cell_library.is_valid() ) {
      cerr << "Error: could not read " << popt_dotlib.val() << endl;
      return -1;
    }
  }
  else if ( popt_mislib.is_specified() ) {
    cell_library = ClibCellLibrary::read_mislib(popt_mislib.val());
    if ( !cell_library.is_valid() ) {
      cerr << "Error: could not read " << popt_mislib.val() << endl;
      return -1;
    }
  }

  int mode = 0;
  if ( popt_dump.is_specified() ) {
    mode = 1;
  }
  else if ( popt_verilog.is_specified() ) {
    mode = 2;
  }

#if 0
  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "liberty", '\0', POPT_ARG_STRING, &liberty_name, 0,
      "specify liberty library", "\"file name\"" },

    { "mislib", '\0', POPT_ARG_STRING, &mislib_name, 0,
      "specify mislib library", "\"file name\"" },

    { "dump", 'd', POPT_ARG_NONE, nullptr, 1,
      "dump network", nullptr},

    { "verilog", 'V', POPT_ARG_NONE, nullptr, 2,
      "dump verilog", nullptr},

    POPT_AUTOHELP

    { nullptr, '\0', 0, nullptr, 0, nullptr, nullptr }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(nullptr, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <file-name> ...");

  // オプション解析を行う．
  ymuint mode = 1;
  for ( ; ; ) {
    int rc = poptGetNextOpt(popt_context);
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      fprintf(stderr, "%s: %s\n",
	      poptBadOption(popt_context, POPT_BADOPTION_NOALIAS),
	      poptStrerror(rc));
      return 1;
    }
    mode = rc;
  }

  // 残りの引数はすべてファイル名と見なす
  list<string> filename_list;
  for ( ; ; ) {
    const char* str = poptGetArg(popt_context);
    if ( str == nullptr ) break;
    filename_list.push_back(str);
  }

  const CellLibrary* cell_library = nullptr;
  if ( liberty_name != nullptr ) {
    CellDotlibReader read;
    cell_library = read(liberty_name);
  }
  else if ( mislib_name != nullptr ) {
    CellMislibReader read;
    cell_library = read(mislib_name);
  }
#endif

#if !defined(YM_DEBUG)
  try {
#endif
    MsgHandler* mh = new StreamMsgHandler(cerr);
    mh->set_mask(kMsgMaskAll);
    mh->delete_mask(MsgType::Info);
    mh->delete_mask(MsgType::Debug);
    MsgMgr::attach_handler(mh);

    MvnVerilogReader reader;

    for (auto& name: filename_list ) {
      cerr << "Reading " << name;
      cerr.flush();
      bool stat = reader.read(name);
      cerr << " end" << endl;
      if ( !stat ) {
	return 1;
      }
    }
    cerr << "Generating MvnNetwork" << endl;
    MvnMgr mgr;
    MvnVlMap node_map;
    bool stat = reader.gen_network(mgr, cell_library, node_map);
    cerr << " End" << endl;
    if ( !stat ) {
      cerr << "error occured" << endl;
      return 2;
    }

    if ( mode == 1 ) {
      MvnDumper dump;
      dump(cout, mgr);
    }
    else if ( mode == 2 ) {
      MvnVerilogWriter vl_writer;

      vl_writer(cout, mgr, node_map);
    }

#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}
