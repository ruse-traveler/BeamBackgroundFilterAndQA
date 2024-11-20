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
#include <cassert>
#include <iostream>

// calo base
#include <calobase/TowerInfoContainer.h>

// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>

// phool libraries
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/PHCompositeNode.h>

// qa utilities
#include <qautils/QAHistManagerDef.h>

// module definition
#include "BeamBackgroundFilterAndQA.h"



// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default module constructor
// ----------------------------------------------------------------------------
BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const std::string& name, const bool debug) : SubsysReco(name)
{

  // print debug message
  if (debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const std::string &name) Calling ctor" << std::endl;
  }

}  // end ctor(std::string&, bool)'



// ----------------------------------------------------------------------------
//! Module constructor accepting a configuration
// ----------------------------------------------------------------------------
BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const Config& config) : SubsysReco(config.moduleName)
{

  // set configuration
  m_config = config;

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const std::string &name) Calling ctor" << std::endl;
  }

}  // end ctor(BeamBackgroundFilterAndQAConfig&)'



// ----------------------------------------------------------------------------
//! Module destructor
// ----------------------------------------------------------------------------
BeamBackgroundFilterAndQA::~BeamBackgroundFilterAndQA()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::~BeamBackgroundFilterAndQA() Calling dtor" << std::endl;
  }

  /* nothing to do */

}  // end dtor()



// fun4all methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize module
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::Init(PHCompositeNode* topNode)
{

  if (m_config.debug)
  {
    std::cout << "BeamBackgroundFilterAndQA::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  }

  // initialize relevant filters
  InitFilters();
  BuildHistograms();

  // if needed, initialize histograms + manager
  if (m_config.doQA)
  {
    InitHistManager();
    RegisterHistograms();
  }
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Grab inputs, check for beam background, and fill histograms
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::process_event(PHCompositeNode* topNode)
{

  if (m_config.debug)
  {
    std::cout << "BeamBackgroundFilterAndQA::process_event(PHCompositeNode *topNode) Processing event" << std::endl;
  }

  // check for beam background
  const bool hasBeamBkgd = ApplyFilters(topNode);

  // if it does, abort event
  if (hasBeamBkgd && m_config.doEvtAbort)
  {
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  else
  {
    return Fun4AllReturnCodes::EVENT_OK;
  }

}  // end 'process_event(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Run final calculations
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::End(PHCompositeNode *topNode)
{

  if (m_config.debug)
  {
    std::cout << "BeamBackgroundFilterAndQA::End(PHCompositeNode *topNode) This is the end..." << std::endl;
  }

  //... nothing to do ...//
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'End(PHCompositeNode*)'



// private methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize filters
// ---------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::InitFilters()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 1))
  {
    std::cout << "BeamBackgroundFilterAndQA::InitFilters() Initializing background filters" << std::endl;
  }


  m_filters["StreakSideband"] = std::make_unique<StreakSidebandFilter>( m_config.sideband );
  //... other filters added here ...//

  return;

}  // end 'InitFilters()'



// ----------------------------------------------------------------------------
//! Initialize histogram manager
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::InitHistManager()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::InitHistManager() Initializing histogram manager" << std::endl;
  }

  m_manager = QAHistManagerDef::getHistoManager();
  if (!m_manager)
  {
    std::cerr << PHWHERE << ": PANIC! Couldn't grab histogram manager!" << std::endl;
    assert(m_manager);
  }
  return;

}  // end 'InitHistManager()'



// ----------------------------------------------------------------------------
//! Build histograms
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::BuildHistograms()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::BuildHistograms() Creating histograms" << std::endl;
  }


  for (const std::string& filterToApply : m_config.filtersToApply)
  {
    m_filters.at(filterToApply)->BuildHistograms();
  }
  return;

}  // end 'BuildHistograms()'



// ----------------------------------------------------------------------------
//! Register histograms
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::RegisterHistograms()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::RegisterHistograms() Registering histograms w/ manager" << std::endl;
  }


  for (const std::string& filterToApply : m_config.filtersToApply)
  {
    m_filters.at(filterToApply)->RegisterHistograms(m_manager);
  }
  return;

}  // end 'RegisterHistograms()'



// ----------------------------------------------------------------------------
//! Apply relevant filters
// ----------------------------------------------------------------------------
bool BeamBackgroundFilterAndQA::ApplyFilters(PHCompositeNode* topNode)
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::ApplyFilters(PHCompositeNode*) Creating histograms" << std::endl;
  }

  bool hasBkgd = false;
  for (const std::string& filterToApply : m_config.filtersToApply)
  {
    hasBkgd += m_filters.at(filterToApply)->ApplyFilter(topNode);
  }
  return hasBkgd;

}  // end 'ApplyFilters(PHCompositeNode*)'

// end ========================================================================
