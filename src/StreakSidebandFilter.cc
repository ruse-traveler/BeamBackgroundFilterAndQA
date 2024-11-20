/// ===========================================================================
/*! \file    StreakSidebandFilter.cc
 *  \authors Hanpu Jiang, Derek Anderson
 *  \date    11.01.2024
 *
 *  Part of the BeamBackgroundFilterAndQA module, this 
 *  filter returns true if event contains a "streaK"
 *  as defined by Hanpu Jiang's streak identification
 *  algorithm.
 */
/// ===========================================================================

#define STREAKSIDEBANDFILTER_cc

// c++ utiilites
#include <algorithm>
#include <iostream>

// calo base
#include <calobase/TowerInfoContainer.h>
// phool libraries
#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
// module components
#include "StreakSidebandFilter.h"



// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
StreakSidebandFilter::StreakSidebandFilter()
{

  //... nothing to do ...//

}  // end ctor()



// ----------------------------------------------------------------------------
//! ctor accepting config struct
// ----------------------------------------------------------------------------
StreakSidebandFilter::StreakSidebandFilter(Config& config)
{

  m_config = config;

}  // end ctor(Config&)



// ----------------------------------------------------------------------------
//! Default dtor
// ----------------------------------------------------------------------------
StreakSidebandFilter::~StreakSidebandFilter()
{

  //... nothing to do ...//

}  // end dtor



// public methods =============================================================

// ----------------------------------------------------------------------------
// Apply filter to check for beam background or not
// ----------------------------------------------------------------------------
bool StreakSidebandFilter::ApplyFilter(PHCompositeNode* topNode)
{

  // print debug message
  if (m_config.debug && (m_config.verbosity > 2))
  {
    std::cout << "StreakSidebandFilter::ApplyFilter() Checking if streak found in OHCal via their sidebands" << std::endl;
  }

  // grab input node
  GrabNodes(topNode);

  // build tower map
  m_ohMap.Reset();
  m_ohMap.Build( m_ohContainer );

  // reset number of streaky towers per eta bin
  std::fill(m_ohNumStreak.begin(), m_ohNumStreak.end(), 0);

  // lambdas to get phi +- 1 neighbors
  auto getAdjacentUp   = [this](const std::size_t phi) {return (phi + 1) % m_ohMap.towers.front().size();};
  auto getAdjacentDown = [this](const std::size_t phi) {return (phi == 0) ? m_ohMap.towers.front().size() : (phi - 1);};

  // loop over tower (eta, phi) map to find streaks
  for (std::size_t iPhi = 0; iPhi < m_ohMap.towers.front().size(); ++iPhi)
  {
    for (std::size_t iEta = 0; iEta < m_ohMap.towers.size(); ++iEta)
    {

      // check if tower is a candidate for being in a streak
      const bool isNotStreak = IsTowerNotStreaky(m_ohMap.towers[iEta][iPhi]);
      if (isNotStreak) continue;

      // grab adjacent towers
      const std::size_t iUp   = getAdjacentUp(iPhi);
      const std::size_t iDown = getAdjacentDown(iPhi);

      // and check if adjacent towers consistent w/ a streak
      const bool isUpNotStreak   = IsNeighborNotStreaky(m_ohMap.towers[iEta][iUp]);
      const bool isDownNotStreak = IsNeighborNotStreaky(m_ohMap.towers[iEta][iDown]);
      if (isUpNotStreak || isDownNotStreak) continue;

      // finally, increment no. of streaky towers for this phi
      // and this phi + 1
      ++m_ohNumStreak[iPhi];
      ++m_ohNumStreak[iUp];

    }  // end eta loop
  }  // end phi loop

  // now find longest streak
  const uint32_t nMaxStreak = *std::max_element(m_ohNumStreak.begin(), m_ohNumStreak.end());

  // return if streak length above threshold
  return (nMaxStreak > m_config.minNumTwrsInStreak);

}  // end 'ApplyFilter()'



// ----------------------------------------------------------------------------
//! Construct histograms
// ----------------------------------------------------------------------------
void StreakSidebandFilter::BuildHistograms(const std::string& tag)
{

  // print debug message
  if (m_config.debug && (m_config.verbosity > 2))
  {
    std::cout << "StreakSidebandFilter::BuildHistograms(std::string) Constructing histograms" << std::endl;
  }

  /* TODO fill in */
  return;

}  // end 'BuildHistograms(std::string&)'



// private methods ============================================================

// ----------------------------------------------------------------------------
//! Grab input nodes
// ----------------------------------------------------------------------------
void StreakSidebandFilter::GrabNodes(PHCompositeNode* topNode)
{

  // print debug message
  if (m_config.debug && (m_config.verbosity > 2))
  {
    std::cout << "StreakSidebandFilter::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  m_ohContainer = findNode::getClass<TowerInfoContainer>(topNode, m_config.inNodeName);
  return;

}  // end 'GrabNodes(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Check if tower not consistent w/ being in a streak
// ----------------------------------------------------------------------------
bool StreakSidebandFilter::IsTowerNotStreaky(const bbfqd::Tower& tower)
{

  // print debug message
  if (m_config.debug && (m_config.verbosity > 2))
  {
    std::cout << "StreakSidebandFilter::IsTowerNotStreaky() Checking if tower not consistent w/ streak" << std::endl;
  }

  const bool isBadStatus   = (tower.status != 1);
  const bool isBelowEneCut = (tower.energy < m_config.minStreakTwrEne);
  return (isBadStatus || isBelowEneCut);

}  // end 'IsTowerNotStreaky(Tower& tower)'



// ----------------------------------------------------------------------------
//! Check if a neighboring tower consistent w/ a streak
// ----------------------------------------------------------------------------
bool StreakSidebandFilter::IsNeighborNotStreaky(const bbfqd::Tower& tower)
{

  // print debug message
  if (m_config.debug && (m_config.verbosity > 2))
  {
    std::cout << "StreakSidebandFilter::IsNeighborNotStreaky() Checking if neighboring tower not consistent w/ streak" << std::endl;
  }

  const bool isBadStatus   = (tower.status != 1);
  const bool isAboveEneCut = (tower.energy > m_config.maxAdjacentTwrEne);
  return (isBadStatus || isAboveEneCut);

}  // end 'IsNeighborNotStreaky(Tower& tower)'

// end ========================================================================
