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
#include <calobase/TowerInfo.h>
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
// root libraries
#include <TH1.h>
#include <TH2.h>

// module definition
#include "BeamBackgroundFilterAndQA.h"



// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Module constructor
// ----------------------------------------------------------------------------
BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const std::string &name) : SubsysReco(name)
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::BeamBackgroundFilterAndQA(const std::string &name) Calling ctor" << std::endl;
  }

}  // end ctor



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

}  // end dtor



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

  InitHistManager();
  BuildHistograms();
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Get ready for a new run
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::InitRun(PHCompositeNode* topNode)
{

  if (m_config.debug)
  {
    std::cout << "BeamBackgroundFilterAndQA::InitRun(PHCompositeNode *topNode) Preparing for new run" << std::endl;
  }

  /* TODO fill in */
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Grab inputs, check for beam background, and fill histograms
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::process_event(PHCompositeNode* topNode)
{

  if (m_config.debug)
  {
    std::cout << "BeamBackgroundFilterAndQA::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  }

  // grab input nodes & build array of ohcal towers
  GrabNodes(topNode);
  BuildTowerArray();

  // check for beam background
  const bool hasBeamBkgd = HasStreakInOHCal;

  if (hasBeamBkgd)
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
    std::cout << "BeamBackgroundFilterAndQA::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  }

  /* nothing to do */
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'End(PHCompositeNode*)'



// private methods ============================================================

// ----------------------------------------------------------------------------
//! Check if streak found in OHCal
// ----------------------------------------------------------------------------
bool BeamBackgroundFilterAndQA::HasStreakInOHCal()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::HasStreakInOHCal() Checking if streak found in OHCal" << std::endl;
  }

  // loop over OHCal towers
  for (uint32_t iTwr = 0; iTwr < m_ohcalTowers -> size(); ++iTwr)
  {


    /* MORE REFERENCE
    // grab eta, phi indices
    const int32_t key  = m_ohcalTowers -> encode_key(iTwr);
    const int32_t iEta = m_ohcalTowers -> getTowerEtaBin(key);
    const int32_t iPhi = m_ohcalTowers -> getTowerPhiBin(key);

    // HANDY REFERENCEs
    const double energy = m_ohcalTowers    
        TowerInfo* tower = towers -> get_tower_at_channel(channel);

    */

  }  // end OCHal tower loop

  /* reference
  float cut_abrupt_target_check = 0.6;
  float cut_abrupt_surround_check = 0.06;
  int cut_threshold = 5;
  int doublephi_ntower_isabrupt[n_hcal_phibin] = {0};
  int up, bottom;
  for (int iphi = 0; iphi < n_hcal_phibin; ++iphi) {
    for (int ieta = 0; ieta < n_hcal_etabin; ++ieta) {
      if (ohcal_tower_status[ieta][iphi] != 0 || ohcal_tower_e[ieta][iphi] < cut_abrupt_target_check) continue;
      up = get_hcal_up(iphi);
      bottom = get_hcal_bottom(iphi);
      if (ohcal_tower_status[ieta][up] != 0 || ohcal_tower_status[ieta][bottom] != 0) continue;
      if (ohcal_tower_e[ieta][up] > cut_abrupt_surround_check || ohcal_tower_e[ieta][bottom] > cut_abrupt_surround_check) continue;
      doublephi_ntower_isabrupt[iphi]++;
      doublephi_ntower_isabrupt[up]++;
    }
  }
  int max_abrupt = 0;
  for (int iphi = 0; iphi < n_hcal_phibin; ++iphi) {
    if (doublephi_ntower_isabrupt[iphi] > max_abrupt) {
      max_abrupt = doublephi_ntower_isabrupt[iphi];
    }
  }
  h_record->Fill(max_abrupt);
  return max_abrupt >= cut_threshold;
*/

  return true;

}  // end 'HasStreakInOHCal()'



// ----------------------------------------------------------------------------
//! Build array in eta, phi of tower info objects
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::BuildTowerArray()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::BuildTowerArray() Building array of OHCal towers" << std::endl;
  }

  // make sure array is empty
  m_ohTwrArray.clear();

  // loop over OHCal towers
  for (uint32_t iTwr = 0; iTwr < m_ohcalTowers -> size(); ++iTwr)
  {

    // TODO fill in
    TowerInfo* tower = towers -> get_tower_at_channel(channel);

  }  // end tower loop

}  // end 'BuildTowerArray()'



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

  /* TODO fill in */
  return;

}  // end 'BuildHistograms()'



// ----------------------------------------------------------------------------
//! Grab input nodes
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::GrabNodes(PHCompositeNode* topNode)
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  m_ohcalTowers = findNode::getClass<TowerInfoContainer>(topNode, m_config.inNodeName);
  return;

}  // end 'GrabNodes(PHCompositeNode*)'

// end ========================================================================
