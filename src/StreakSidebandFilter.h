/// ===========================================================================
/*! \file    StreakSidebandFilter.h
 *  \authors Hanpu Jiang, Derek Anderson
 *  \date    11.01.2024
 *
 *  Part of the BeamBackgroundFilterAndQA module, this 
 *  filter returns true if event contains a "streaK"
 *  as defined by Hanpu Jiang's streak identification
 *  algorithm.
 */
/// ===========================================================================

#ifndef STREAKSIDEBANDFILTER_H
#define STREAKSIDEBANDFILTER_H

// c++ utilities
#include <array>
#include <string>
// module components
#include "BaseBeamBackgroundFilter.h"
#include "BeamBackgroundFilterAndQADefs.h"

// forward declarations
class PHCompositeNode;
class TowerInfoContainer;

// alias for convenience
namespace bbfqd = BeamBackgroundFilterAndQADefs;



// ============================================================================
//! User options for filter
// ============================================================================
struct StreakSidebandFilterConfig
{

  bool        debug              = true;
  float       minStreakTwrEne    = 0.6;
  float       maxAdjacentTwrEne  = 0.06;
  uint32_t    minNumTwrsInStreak = 5;
  uint32_t    verbosity          = 0;
  std::string inNodeName         = "TOWERINFO_CALIB_HCALOUT";

};  // end StreakSidebandFilterConfig



// ============================================================================
//! Identify streaks via sidebands 
// ============================================================================
/*! A beam background filter which identifies streaks
 *  in the OHCal by comparing streak candidates vs.
 *  their sidebands, i.e. adjacent phi slices.
 */
class StreakSidebandFilter : public BaseBeamBackgroundFilter<StreakSidebandFilterConfig>
{

  public:

    StreakSidebandFilter();
    ~StreakSidebandFilter();

    // inherited methods
    void BuildHistograms(const std::string& tag = "") override;
    bool ApplyFilter(PHCompositeNode* topNode) override;

  private:

    // inherited methods
    void GrabNodes(PHCompositeNode* topNode) override;

    // filter-specific methods
    bool IsTowerNotStreaky(const bbfqd::Tower& tower);
    bool IsNeighborNotStreaky(const bbfqd::Tower& tower);

    // input nodes
    TowerInfoContainer* m_ohContainer;

    // tower info (eta, phi) map
    bbfqd::OHCalMap m_ohMap;

    // no. of streaks per event in ohcal
    std::array<std::size_t, 64> m_ohNumStreak;

};  // end StreakSidebandFilter

#endif

// end ========================================================================
