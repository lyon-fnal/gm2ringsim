// Tracker detector service for 

// Include guards
#ifndef RING_SERVICE_HH
#define RING_SERVICE_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Core/EDProducer.h"

#include "gm2ringsim/common/ring/RingSD.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"

#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

#include "gm2ringsim/common/ring/arthits/RingArtRecord.hh"
#include "gm2ringsim/common/ring/ring_util.hh"

// Within a namespace
namespace gm2ringsim {
  
  // The class
  class Ring : public artg4::DetectorBase {
    
  public:
    // Constructor
    Ring(fhicl::ParameterSet const &, art::ActivityRegistry & );
    
    // We always need a virtual destructor
    virtual ~Ring() {};
    
  private:
    G4String ringSDname_;
    RingSD *ringSD_;

    // Private overriden methods
      
    // Create the logical volumes
    virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
    
    // Create the physical volumes
    virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
    
    // Tell Art what we'll produce
        virtual void doCallArtProduces(art::EDProducer * producer) override;
    
    // Actually add the data to the event
     virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;

  };

}//namespace gm2ringsim

#endif //RING_SERVICE_HH
