// Tracker detector service for 

// Include guards
#ifndef ARC_HH
#define ARC_HH

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

// Within a namespace
namespace gm2ringsim {

    // The class
    class Arc : public artg4::DetectorBase {

    public:

        // Constructor
        Arc(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~Arc() {};

    private:

        // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
      
        // Convenience functions
        G4LogicalVolume* gm2ringsim::Arc::makeAnArcLV(unsigned int);


    };
}

#endif
