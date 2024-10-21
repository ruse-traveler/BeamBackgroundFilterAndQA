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

#ifndef BEAMBACKGROUNDFILTERANDQA_H
#define BEAMBACKGROUNDFILTERANDQA_H

// c++ utilities
#include <string>
#include <vector>
// f4a libraries
#include <fun4all/SubsysReco.h>
/* TODO others will go here... */

// forward declarations
class PHCompositeNode;
class QAHistManagerHistDef;
class TowerInfoContainer;
/* TODO add any others here... */

// aliases for convenience
using TwrVec2D = std::vector<std::vector<TowerInfo*>>;



// ============================================================================
//! User options for BeamBackgroundFilterAndQAConfig module
// ============================================================================
struct BeamBackgroundFilterAndQAConfig {

  // turn debug on/off
  bool debug = true;

  // input node & module name
  std::string inNodeName = "TOWERINFO_CALIB_HCALOUT";
  std::string moduleName = "BeamBackgroundFilterAndQA";

  // ohcal streak algorithm parameters
  float    minStreakTwrEne    = 0.6;
  float    maxAdjacentTwrEne  = 0.06;
  uint32_t minNumTwrsInStreak = 5;

};



// ============================================================================
//! Filter beam background events and create QA
// ============================================================================
/*! A F4A module to filter out events with significant
 *  beam background and produce some relevant QA
 *  histograms. 
 */
class BeamBackgroundFilterAndQA : public SubsysReco {

  public:

    // ctor
    BeamBackgroundFilterAndQA(const std::string& name = "BeamBackgroundFilterAndQA");
    ~BeamBackgroundFilterAndQA() override;

    // setters
    void SetConfig(const BeamBackgroundFilterAndQAConfig& config) {m_config = config;}

    // getters
    BeamBackgroundFilterAndQAConfig GetConfig() {return m_config;}

    // f4a methods
    int Init(PHCompositeNode* topNode)          override;
    int InitRun(PHCompositeNode* topNode)       override;  // FIXME might not need...
    int process_event(PHCompositeNode* topNode) override;
    int End(PHCompositeNode* topNode)           override;

  private:

    // private methods
    bool HasStreakInOHCal();
    void BuildTowerArray();
    void InitHistManager();
    void BuildHistograms();
    void GrabNodes(PHCompositeNode* topNode);

    // module configuration
    BeamBackgroundFilterAndQAConfig m_config;

    // tower info eta, phi map
    TwrVec2D m_ohTwrArray;

    // f4a members
    Fun4AllHistoManager* m_manager     = NULL;
    TowerInfoContainer*  m_ohcalTowers = NULL;

    /* TODO add histograms */

};  // end BeamBackgroundFilterAndQA

#endif

// end ========================================================================
