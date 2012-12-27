// Implementation of Kicker

#include "gm2ringsim/kicker/Kicker_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4RunManager.hh"

#include "Geant4/G4FieldManager.hh"
#include "fields/g2FieldEqRhs.hh"
#include "Geant4/G4Mag_UsualEqRhs.hh"
#include "Geant4/G4Mag_SpinEqRhs.hh"
#include "Geant4/G4ClassicalRK4.hh"

//#include "kickerMessenger.hh"
#include "kicker/KickerHelpers.hh"
#include "artg4/material/Materials.hh"

#include "Geant4/G4String.hh"

#include "artg4/util/util.hh"
#include "gm2ringsim/common/ring/RingSD.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#include <iomanip>
#include <tr1/functional>


//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Kicker::Kicker(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "kicker"),
                   p.get<std::string>("category", "kicker"),
		 p.get<std::string>("mother_category", "vac")),
    sts_("SpinTracking"),
    spin_tracking_(sts_.spinTrackingEnabled),
    kg_(myName()),
    numKickers(3),
    numKickerObjects(3)
{
  kg_.print();
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Kicker::doBuildLVs() {
  //FIXME: Decide which logical volumes we want to store in ART
  std::vector<G4LogicalVolume *> l_kicker;
  return l_kicker;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Kicker::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  //FIXME: Decide which physical volumes we want to store in ART
  std::vector<G4VPhysicalVolume *> p_kicker;
  return p_kicker;
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Kicker::doCallArtProduces(art::EDProducer * producer) {
//
//}

// Actually add the data to the event
//void gm2ringsim::Kicker::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}

using gm2ringsim::Kicker;
DEFINE_ART_SERVICE(Kicker)
