// Tracker detector service for 

// Include guards
#ifndef TRACEBACK_HH
#define TRACEBACK_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4UnionSolid.hh"
#include "gm2ringsim/traceback/StrawSD.hh"

#include "gm2ringsim/traceback/TracebackGeometry.hh"
#include "gm2ringsim/vac/VacGeometry.hh"

#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class Traceback : public artg4::DetectorBase {

    public:

        // Constructor
        Traceback(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~Traceback() {};

    private:
        TracebackGeometry geom_;
        G4String strawSDName_;
        StrawSD *strawSD_;

        // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

        // CHANGE_ME: Delete the next two functions if no hits

        // Tell Art what we'll produce
        //virtual void doCallArtProduces(art::EDProducer * producer) override;

        // Actually add the data to the event
        //virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;
      
      // Convenience functions
        G4LogicalVolume* makeATracebackLV();
        //G4UnionSolid* buildScallopSolid();

        void makeTracebackLVs(std::vector<G4LogicalVolume*>&);
        void makeStrawDetectors(std::vector<G4VPhysicalVolume*>&, std::vector<G4LogicalVolume*>& );

    };
}

#endif
