/// ===========================================================================
/*! \file    BeamBackgroundFilterAndQA.h
 *  \authors Hanpu Jiang, Derek Anderson
 *  \date    10.21.2024
 *
 *  A F4A module to filter out events with significant
 *  beam background (the so-called "streaky events")
 *  and produce some relevant QA histograms.
 */
/// ===========================================================================

#define BEAMBACKGROUNDFILTERANDQA_CC

// c++ utiilites
#include <algorithm>
#include <cassert>
#include <iostream>
// calo base
#include <calobase/TowerInfov2.h>
// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
// phool libraries
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/PHCompositeNode.h>
// qa utilities
#include <qautils/QAHistManagerDef.h>
// root libraries
#include <TH1.h>
#include <TH2.h>

// module definition
#include "BeamBackgroundFilterAndQA.h"



// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Module constructor
// ----------------------------------------------------------------------------
BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const std::string &name) : SubsysReco(name) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const std::string &name) Calling ctor" << std::endl;
  }

}  // end ctor



// ----------------------------------------------------------------------------
//! Module destructor
// ----------------------------------------------------------------------------
BeamBackgroundFilterAndQA::~BeamBackgroundFilterAndQA() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "BeamBackgroundFilterAndQA::~BeamBackgroundFilterAndQA() Calling dtor" << std::endl;
  }

  /* nothing to do */

}  // end dtor



// fun4all methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize module
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::Init(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "BeamBackgroundFilterAndQA::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  }

  InitHistManager();
  BuildHistograms();
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Get ready for a new run
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::InitRun(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "BeamBackgroundFilterAndQA::InitRun(PHCompositeNode *topNode) Preparing for new run" << std::endl;
  }

  /* TODO fill in */
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Grab inputs and fills histograms
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::process_event(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "BeamBackgroundFilterAndQA::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  }

  // grab input nodes
  GrabNodes(topNode);

  /* TODO fill in */
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'process_event(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Run final calculations
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::End(PHCompositeNode *topNode) {

  if (m_config.debug) {
    std::cout << "BeamBackgroundFilterAndQA::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  }

  /* nothing to do */
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'End(PHCompositeNode*)'



// private methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize histogram manager
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::InitHistManager() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "BeamBackgroundFilterAndQA::InitHistManager() Initializing histogram manager" << std::endl;
  }

  m_manager = QAHistManagerDef::getHistoManager();
  if (!m_manager) {
    std::cerr << PHWHERE << ": PANIC! Couldn't grab histogram manager!" << std::endl;
    assert(m_manager);
  }
  return;

}  // end 'InitHistManager()'



// ----------------------------------------------------------------------------
//! Build histograms
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::BuildHistograms() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "BeamBackgroundFilterAndQA::BuildHistograms() Creating histograms" << std::endl;
  }

  return;

}  // end 'BuildHistograms()'



// ----------------------------------------------------------------------------
//! Grab input nodes
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::GrabNodes(PHCompositeNode* topNode) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "BeamBackgroundFilterAndQA::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  /* TODO fill in */
  return;

}  // end 'GrabNodes(PHCompositeNode*)'

// end ========================================================================
