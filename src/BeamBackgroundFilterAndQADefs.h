/// ===========================================================================
/*! \file    BeamBackgroundFilterAndQADefs.h
 *  \authors Derek Anderson
 *  \date    11.01.2024
 *
 *  A namespace to hold various definitions
 *  useful across the BeamBackgroundFilterAndQA
 *  module and its various filters.
 */
/// ===========================================================================

#ifndef BEAMBACKGROUNDFILTERANDQADEFS_H
#define BEAMBACKGROUNDFILTERANDQADEFS_H


namespace BeamBackgroundFilterAndQADefs {

  // ==========================================================================
  //! Convenience struct to hold info from TowerInfo
  // ==========================================================================
  /*! This is a lightweight struct to scrape only the info
   *  relevant to various filters from a TowerInfo object.
   *  An example usage is in the StreakSidebandFilter
   *  algorithm, where its used to build a 2D array of
   *  tower energies & status for quick lookup.
   */
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

  };  // end Tower

}  // end BeamBackgroundFilterAndQADefs

#endif

// end ========================================================================
