// Tracker detector service for 

// Include guards
#ifndef COLLIMATOR_HH
#define COLLIMATOR_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"

#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"
#include "gm2geom/collimator/CollimatorGeometry.hh"

#include "gm2ringsim/common/ring/RingSD.hh"
#include "gm2ringsim/common/ring/RingArtRecord.hh"
#include "gm2ringsim/common/ring/RingHit.hh"

// Within a namespace
namespace gm2ringsim {
  // The class
  class Collimator : public artg4::DetectorBase {
    
  public:
    // Constructor
    Collimator(fhicl::ParameterSet const &, art::ActivityRegistry & );
    
    // We always need a virtual destructor
    virtual ~Collimator() {};
    
  private:
    gm2geom::CollimatorGeometry geom_;
    // Private overriden methods
    
    G4String ringSDname_;
    RingSD *ringSD_;

    // Create the logical volumes
    virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
    
    // Create the physical volumes
    virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
  };
}

using gm2ringsim::Collimator;
DECLARE_ART_SERVICE(Collimator,LEGACY)
#endif
