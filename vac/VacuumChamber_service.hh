// Tracker detector service for 

// Include guards
#ifndef VACUUMCHAMBER_HH
#define VACUUMCHAMBER_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4UnionSolid.hh"

#include "gm2ringsim/actions/muonStorageStatus/TurnCounterSD.hh"
#include "gm2ringsim/common/tracker/TrackerSD.hh"
#include "gm2ringsim/common/tracker/tracker_util.hh"

#include "gm2ringsim/vac/VacGeometry.hh"

#include <vector>


// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

// Within a namespace
namespace gm2ringsim {

  // The class
  class VacuumChamber : public artg4::DetectorBase {
    
    public:
    
      // Constructor
      VacuumChamber(fhicl::ParameterSet const &, art::ActivityRegistry & );
     
    // We always need a virtual destructor
    virtual ~VacuumChamber() {};
     
  private:
    G4String turnCounterSDName_;
    G4String trackerSDName_;
    TurnCounterSD *turnSD_;
    TrackerSD *trackerSD_;
    
    // We need to hang onto the wallLVs for placement later
    std::vector<G4LogicalVolume*> wallLVs_;
    
    // Private overriden methods
    
    // Create the logical volumes
    virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
    
    // Create the physical volumes
    virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

 // Tell Art what we'll produce
        virtual void doCallArtProduces(art::EDProducer * producer) override;
    
    // Actually add the data to the event
     virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;
    
    
      
        // Some internal methods
        G4UnionSolid* buildUnionSolid(const VacGeometry&, VacGeometry::typeToBuild, unsigned int);
      
        void makeWallLVs(const VacGeometry&);
    
        void makeVacuumLVs(std::vector<G4LogicalVolume*>&, const VacGeometry&);
      
        void makeVacuumPVs(std::vector<G4LogicalVolume*>&);
      
        void makeTrackerPVs(std::vector<G4LogicalVolume*>&, const VacGeometry&);

    };
}

#endif
