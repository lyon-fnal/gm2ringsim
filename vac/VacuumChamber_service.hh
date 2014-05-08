// Virtualringstation detector service for 

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
#include "gm2ringsim/common/virtualringstation/VirtualRingStationSD.hh"
#include "gm2ringsim/common/virtualringstation/virtualringstation_util.hh"

#include "gm2geom/vac/VacGeometry.hh"

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
    G4String virtualringstationSDName_;
    TurnCounterSD *turnSD_;
    VirtualRingStationSD *virtualringstationSD_;
    
    // We need to hang onto the wallLVs for placement later
    std::vector<G4LogicalVolume*> wallLVs_;
    
    // Private overriden methods
    
    // Create the logical volumes
    virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
    
    // Create the physical volumes
    virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
    
    
    
    // Some internal methods
    G4UnionSolid* buildUnionSolid(const gm2geom::VacGeometry&, gm2geom::VacGeometry::typeToBuild, unsigned int);
    
    void FindScallopPos(int arcNumber, bool &firstpos, bool &secondpos);
  
    void makeWallLVs(const gm2geom::VacGeometry&);
    
    void makeVacuumLVs(std::vector<G4LogicalVolume*>&, const gm2geom::VacGeometry&);
    
    void makeVacuumPVs(std::vector<G4LogicalVolume*>&);
    
    void makeVirtualRingStationPVs(std::vector<G4LogicalVolume*>&, const gm2geom::VacGeometry&);
    
    };
}
using gm2ringsim::VacuumChamber;
DECLARE_ART_SERVICE(VacuumChamber,LEGACY)
#endif
