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
//! Grab inputs, check for beam background, and fill histograms
// ----------------------------------------------------------------------------
int BeamBackgroundFilterAndQA::process_event(PHCompositeNode* topNode)
{

  if (m_config.debug)
  {
    std::cout << "BeamBackgroundFilterAndQA::process_event(PHCompositeNode *topNode) Processing event" << std::endl;
  }

  // grab input nodes & build array of ohcal towers
  GrabNodes(topNode);
  ResetTowerArrays();
  BuildTowerArray();

  // check for beam background
  const bool hasBeamBkgd = HasStreakInOHCal();

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
    std::cout << "BeamBackgroundFilterAndQA::End(PHCompositeNode *topNode) This is the end..." << std::endl;
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

  // lambdas to get phi +- 1 neighbors
  auto getAdjacentUp   = [this](const std::size_t phi) {return (phi + 1) % m_ohTwrArray.front().size();};
  auto getAdjacentDown = [this](const std::size_t phi) {return (phi == 0) ? m_ohTwrArray.front().size() : (phi - 1);};

  // loop over tower (eta, phi) map to find streaks
  for (std::size_t iPhi = 0; iPhi < m_ohTwrArray.front().size(); ++iPhi)
  {
    for (std::size_t iEta = 0; iEta < m_ohTwrArray.size(); ++iEta)
    {

      // check if tower is a candidate for being in a streak
      const bool isNotStreak = IsTowerNotStreaky(m_ohTwrArray[iEta][iPhi]);
      if (isNotStreak) continue;

      // grab adjacent towers
      const std::size_t iUp   = getAdjacentUp(iPhi);
      const std::size_t iDown = getAdjacentDown(iPhi);

      // and check if adjacent towers consistent w/ a streak
      const bool isUpNotStreak   = IsNeighborNotStreaky(m_ohTwrArray[iEta][iUp]);
      const bool isDownNotStreak = IsNeighborNotStreaky(m_ohTwrArray[iEta][iDown]);
      if (isUpNotStreak || isDownNotStreak) continue;

      // finally, increment no. of streaky towers for this phi
      ++m_ohNumStreak[iPhi];

    }  // end eta loop
  }  // end phi loop

  // now find longest streak
  const uint32_t nMaxStreak = *std::max_element(m_ohNumStreak.begin(), m_ohNumStreak.end());

  // return if streak length above threshold
  return (nMaxStreak > m_config.minNumTwrsInStreak);

}  // end 'HasStreakInOHCal()'



// ----------------------------------------------------------------------------
//! Check if tower not consistent w/ being in a streak
// ----------------------------------------------------------------------------
bool BeamBackgroundFilterAndQA::IsTowerNotStreaky(const Tower& tower)
{

  // print debug message
  if (m_config.debug && (Verbosity() > 1))
  {
    std::cout << "BeamBackgroundFilterAndQA::IsTowerNotStreaky() Checking if tower not consistent w/ streak" << std::endl;
  }

  const bool isBadStatus   = (tower.status != 0);
  const bool isBelowEneCut = (tower.energy < m_config.minStreakTwrEne);
  return (isBadStatus || isBelowEneCut);

}  // end 'IsTowerNotStreaky(Tower& tower)'



// ----------------------------------------------------------------------------
//! Check if a neighboring tower consistent w/ a streak
// ----------------------------------------------------------------------------
bool BeamBackgroundFilterAndQA::IsNeighborNotStreaky(const Tower& tower)
{

  // print debug message
  if (m_config.debug && (Verbosity() > 1))
  {
    std::cout << "BeamBackgroundFilterAndQA::IsNeighborNotStreaky() Checking if neighboring tower not consistent w/ streak" << std::endl;
  }

  const bool isBadStatus   = (tower.status != 0);
  const bool isAboveEneCut = (tower.energy > m_config.maxAdjacentTwrEne);
  return (isBadStatus || isAboveEneCut);

}  // end 'IsNeighborNotStreaky(Tower& tower)'



// ----------------------------------------------------------------------------
//! Reset tower array
// ----------------------------------------------------------------------------
void BeamBackgroundFilterAndQA::ResetTowerArrays()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "BeamBackgroundFilterAndQA::ResetArrays() Resestting OHCal tower arrays" << std::endl;
  }

  // reset (eta, phi) map
  for (auto row : m_ohTwrArray)
  {
    for (auto tower : row)
    {
      tower.Reset();
    }
  }

  // reset number of streaky towers array
  std::fill(m_ohNumStreak.begin(), m_ohNumStreak.end(), 0);
  return;

}  // end 'ResetTowerArrays()'



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

  // loop over OHCal towers
  for (std::size_t iTower = 0; iTower < m_ohcalTowers -> size(); ++iTower)
  {

    // get indices
    const int32_t key  = m_ohcalTowers->encode_key(iTower);
    const int32_t iEta = m_ohcalTowers->getTowerEtaBin(key);
    const int32_t iPhi = m_ohcalTowers->getTowerPhiBin(key);

    // grab tower & set info
    TowerInfo* info = m_ohcalTowers->get_tower_at_channel(iTower);
    m_ohTwrArray[iEta][iPhi].SetInfo(info);

  }  // end tower loop
  return;

}  // end 'BuildTowerArray()'



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
