#ifndef physicsListQGSP_BERT_hh
#define physicsListQGSP_BERT_hh

/** @file physicsListQGSP_BERT.hh

    This is a derived class from physicsList.hh in order to preserve the 
    possibility to switch on/off decays and to enable decays with spin.
    The only difference is to use the QGSP_BERT physics list as a basis 
    for the process registration

    - Peter Winter Mar/2013
 */

#include "Geant4/globals.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/G4VPhysicsConstructor.hh"

#include "gm2ringsim/actions/physicsList.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

namespace gm2ringsim {
  class physicsListQGSP_BERT : public physicsList {
    
  public:
    physicsListQGSP_BERT(fhicl::ParameterSet const & p);
    ~physicsListQGSP_BERT();
    
  private:
    
  };//class physicsListQGSP_BERT
} //namespace gm2ringsim

#endif
