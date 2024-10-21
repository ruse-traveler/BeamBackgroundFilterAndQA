/// ===========================================================================
/*! \file    Fun4All_TestBeamBackgroundFilterAndQA.C
 *  \authors Hanpu Jiang, Derek Anderson
 *  \date    10.21.2024
 *
 *  A small F4A macro to test the 'BeamBackgroundFilterAndQA' module.
 */
/// ===========================================================================

#define FUN4ALL_TESTBEAMBACKGROUNDFILTERANDQA_C

// c++ utilities
#include <string>
// ffa modules
#include <ffamodules/FlagHandler.h>
#include <ffamodules/CDBInterface.h>
// fun4all libraries
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
// phool utilities
#include <phool/recoConsts.h>
// qa utils
#include <qautils/QAHistManagerDef.h>
// module definitions
#include <calostatusmapper/BeamBackgroundFilterAndQA.h>

R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalostatusmapper.so)



// macro body =================================================================

void Fun4All_TestBeamBackgroundFilterAndQA(
  const int runnumber = 43273,
  const int nEvents = 10,
  const int verbosity = 5,
  const std::string inFile = "/sphenix/lustre01/sphnxpro/commissioning/slurp/caloy2test/run_00042000_00042100/DST_CALO_run2pp_new_2024p001-00042072-0121.root",
  const std::string outFile = "test_bbfaq.root"
) {

  // options ------------------------------------------------------------------

  // trigger cluster maker options
  BeamBackgroundFilterAndQAConfig cfg_filter {
    .debug = true
  };

  // initialize f4a -----------------------------------------------------------

  Fun4AllServer* f4a = Fun4AllServer::instance();
  CDBInterface*  cdb = CDBInterface::instance();
  recoConsts*    rc  = recoConsts::instance();
  f4a -> Verbosity(verbosity);
  cdb -> Verbosity(verbosity);

  // grab lookup tables
  rc -> set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc -> set_uint64Flag("TIMESTAMP", runnumber);

  // register inputs/outputs --------------------------------------------------

  Fun4AllDstInputManager* input = new Fun4AllDstInputManager("InputDstManager");
  input -> fileopen(inFile);
  f4a   -> registerInputManager(input);

  // register subsystem reco modules ------------------------------------------

  // filter beam background events & QA them
  BeamBackgroundFilterAndQA* filter = new BeamBackgroundFilterAndQA("BeamBackgroundFilterAndQA");
  filter -> SetConfig(cfg_filter);
  filter -> Verbosity(verbosity);
  f4a    -> registerSubsystem(filter);

  // run modules and exit -----------------------------------------------------

  // run4all
  f4a -> run(nEvents);
  f4a -> End();

  // save qa output and exit
  QAHistManagerDef::saveQARootFile(outFile);
  delete f4a;

  // close and  exit
  gSystem -> Exit(0);
  return;

}

// end ========================================================================
