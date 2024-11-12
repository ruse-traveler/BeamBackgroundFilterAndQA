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
#include <memory>
#include <set>
#include <string>
// f4a libraries
#include <fun4all/SubsysReco.h>
// module components
#include "StreakSidebandFilter.h"

// forward declarations
class Fun4AllHistoManager;
class PHCompositeNode;
class QAHistManagerHistDef;
class TowerInfoContainer;



// ============================================================================
//! User options for BeamBackgroundFilterAndQAConfig module
// ============================================================================
struct BeamBackgroundFilterAndQAConfig
{

  // turn modes on/off
  bool debug = true;
  bool doQA  = true;

  // module name
  std::string moduleName = "BeamBackgroundFilterAndQA";

  // which filters to apply
  std::set<std::string> filters = {"StreakSideband"};

  // filter configurations
  StreakSidebandFilterConfig sideband;

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

    // ctor/dtor
    BeamBackgroundFilterAndQA(const std::string& name = "BeamBackgroundFilterAndQA", const bool debug = false);
    BeamBackgroundFilterAndQA(const BeamBackgroundFilterAndQAConfig& config); 
    ~BeamBackgroundFilterAndQA() override;

    // setters
    void SetConfig(const BeamBackgroundFilterAndQAConfig& config) {m_config = config;}

    // getters
    BeamBackgroundFilterAndQAConfig GetConfig() const {return m_config;}

    // f4a methods
    int Init(PHCompositeNode* topNode) override;
    int process_event(PHCompositeNode* topNode) override;
    int End(PHCompositeNode* topNode) override;

  private:

    // private methods
    void InitFilters();
    void InitHistManager();
    void BuildHistograms();
    bool ApplyFilters(PHCompositeNode* topNode);

    // module configuration
    BeamBackgroundFilterAndQAConfig m_config;

    // filters
    std::unique_ptr<StreakSidebandFilter> m_sideband;

    // f4a members
    Fun4AllHistoManager* m_manager = NULL;

};  // end BeamBackgroundFilterAndQA

#endif

// end ========================================================================
