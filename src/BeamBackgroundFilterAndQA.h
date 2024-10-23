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
#include <array>
#include <string>
// f4a libraries
#include <fun4all/SubsysReco.h>
// calo base
#include <calobase/TowerInfo.h>

// forward declarations
class PHCompositeNode;
class QAHistManagerHistDef;
class TowerInfoContainer;



// ============================================================================
//! Filter beam background events and create QA
// ============================================================================
/*! A F4A module to filter out events with significant
 *  beam background and produce some relevant QA
 *  histograms. 
 */
class BeamBackgroundFilterAndQA : public SubsysReco {

  public:

    // ========================================================================
    //! User options for BeamBackgroundFilterAndQAConfig module
    // ========================================================================
    struct Config
    {

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

    // ========================================================================
    //! Convenience struct to hold info from TowerInfo
    // ========================================================================
    struct Tower
    {

      uint8_t status = 0;
      double  energy = -1.;

      // grab info from a TowerInfo object
      void SetInfo(TowerInfo* info)
      {
        status = info->get_status();
        energy = info->get_energy();
      }

      // reset values
      void Reset()
      {
        status = 0;
        energy = -1.;
      }
    };

    // ctor
    BeamBackgroundFilterAndQA(const std::string& name = "BeamBackgroundFilterAndQA");
    ~BeamBackgroundFilterAndQA() override;

    // setters
    void SetConfig(const Config& config) {m_config = config;}

    // getters
    Config GetConfig() {return m_config;}

    // f4a methods
    int Init(PHCompositeNode* topNode)          override;
    int process_event(PHCompositeNode* topNode) override;
    int End(PHCompositeNode* topNode)           override;

  private:

    // private methods
    bool HasStreakInOHCal();
    bool IsTowerNotStreaky(const Tower& tower);
    bool IsNeighborNotStreaky(const Tower& tower);
    void ResetTowerArrays();
    void BuildTowerArray();
    void BuildHistograms();
    void InitHistManager();
    void GrabNodes(PHCompositeNode* topNode);

    // module configuration
    Config m_config;

    // tower info (eta, phi> map
    //   - n.b. reminder that there are 64 hcal towers in phi
    //     and 24 hcal towers in eta
    std::array<std::array<Tower, 64>, 24> m_ohTwrArray;
    std::array<std::size_t, 64>           m_ohNumStreak;

    // f4a members
    Fun4AllHistoManager* m_manager     = NULL;
    TowerInfoContainer*  m_ohcalTowers = NULL;

    /* TODO add histograms */

};  // end BeamBackgroundFilterAndQA

#endif

// end ========================================================================
