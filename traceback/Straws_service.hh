// Tracker detector service for 

// Include guards
#ifndef STRAWS_HH
#define STRAWS_HH

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
#include "gm2ringsim/traceback/TracebackGeometry.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class Straws : public artg4::DetectorBase {

    public:

        // Constructor
        Straws(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~Straws() {};

    private:
        TracebackGeometry geom_;

        // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

        int FindValue(char indicator, std::string name);

        // CHANGE_ME: Delete the next two functions if no hits

        // Tell Art what we'll produce
        //virtual void doCallArtProduces(art::EDProducer * producer) override;

        // Actually add the data to the event
        //virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;

    };
}

#endif
