// Tracker detector service for Geant N02

// Include guards
#ifndef WORLD_HH
#define WORLD_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"

#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

// The magnetic field
//#include "artg4geantn02/detectors/geantCode/ExN02MagneticField.hh"

// Within a namespace
namespace gm2ringsim {

    class World : public artg4::DetectorBase {

    public:

        // Constructor
        World(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~World() {};
      
        // Initialize
        void initialize() override;

    private:

        // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

        // Hold on to the actual magnetic field
        double magFieldStrength_;
      //        ExN02MagneticField magField_;

    };
}


#endif

