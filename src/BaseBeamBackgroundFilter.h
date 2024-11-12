/// ===========================================================================
/*! \file    BeamBackgroundFilter.h
 *  \authors Derek Anderson
 *  \date    11.01.2024
 *
 *  Part of the BeamBackgroundFilterAndQA module, this 
 *  is an abstract template for any filters run as
 *  part of the module.
 */
/// ===========================================================================

#ifndef BEAMBACKGROUNDFILTER_H
#define BEAMBACKGROUNDFILTER_H

// c++ utilities
#include <map>
#include <string>

// forward declarations
class PHCompositeNode;



// ============================================================================
//! Abstract base filter 
// ============================================================================
/*! An abstract template for filters to be applied in the
 *  BeamBackgroundFilterAndQA module. Defines all the
 *  machinery common between filters.
 *
 *  Derived filters need to define the following by
 *  overwriting the appropriate method:
 *    - <METHOD 1>
 */
template <typename Config> class BaseBeamBackgroundFilter
{

  protected:

    // ------------------------------------------------------------------------
    //! Grab relevant input nodes
    // ------------------------------------------------------------------------
    /*! This <GRABS THE INPUT>
     */
    virtual void GrabNodes(PHCompositeNode* topNode) = 0;

    ///! filter configuration
    Config m_config;

    /* TODO add histogram map(s) */

  public:

    // ------------------------------------------------------------------------
    //! Build associated histograms
    // ------------------------------------------------------------------------
    /*! This <MAKES THE HISTOGRAMS>
     */
    virtual void BuildHistograms(const std::string& tag) = 0;

    // ------------------------------------------------------------------------
    //! Apply filter
    // ------------------------------------------------------------------------
    /*! This <DOES THE THING>
     */ 
    virtual bool ApplyFilter(PHCompositeNode* topNode) = 0;

    /* TODO add method to register histograms */

    ///! Set filter configuration
    inline void SetConfig(const Config& config) {m_config = config;}

    ///! Get filter configuration
    inline Config GetConfig() const {return m_config;}

    ///! default ctor/dtor
    BaseBeamBackgroundFilter()  {};
    ~BaseBeamBackgroundFilter() {};

};  // end BaseBeamBackgroundFilter

#endif

// end ========================================================================
