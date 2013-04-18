// Implementation of Straws

#include "gm2ringsim/traceback/Straws_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4Transform3D.hh"
#include "Geant4/G4RotationMatrix.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4ClassicalRK4.hh"
#include "Geant4/G4Mag_UsualEqRhs.hh"
#include "Geant4/G4Mag_SpinEqRhs.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4UImanager.hh"
#include "Geant4/G4UniformMagField.hh"
#include "Geant4/G4SDManager.hh"

#include "gm2ringsim/traceback/TracebackGeometry.hh"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Straws::Straws(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "straws"),
                   p.get<std::string>("category", "straws"),
                   p.get<std::string>("mother_category", "traceback")),
    geom_("traceback")

{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Straws::doBuildLVs() {

  std::vector<G4LogicalVolume*> straws;
  
  G4double innerRadiusOfTheTube = 0.*cm;
  G4double outerRadiusOfTheTube = 6.*mm;
  G4double hightOfTheTube = 10*cm;
  G4double startAngleOfTheTube = 0.*deg;
  G4double spanningAngleOfTheTube = 360.*deg;
  for (unsigned int tb = 0; tb<geom_.whichTracebackLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawLocation.size(); sc++){

      G4Tubs* tracker_tube = new G4Tubs("tracker_tube",
                                    innerRadiusOfTheTube,
                                    outerRadiusOfTheTube,
                                    hightOfTheTube,
                                    startAngleOfTheTube,
                                    spanningAngleOfTheTube
                                    );
      
      std::string strawLVName = artg4::addNumberToName("SingleStrawLV", sc+tb);

      G4LogicalVolume* strawLV = new G4LogicalVolume(
                                                 tracker_tube,
                                                 artg4Materials::Vacuum(),
                                                 strawLVName,
                                                 0,
                                                 0);
      artg4::setVisAtts( strawLV, true, geom_.individualStrawColor,
                    [] (G4VisAttributes* att) {
                      att->SetForceSolid(1);
                      att->SetVisibility(1);
                    }
                    );

      straws.push_back(strawLV);
    }
  }
  return straws;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Straws::doPlaceToPVs( std::vector<G4LogicalVolume*> planes) {
  std::vector<G4VPhysicalVolume*> strawPVs;
  
  int i =0;
  for ( auto aStrawLV : lvs() ) {
    G4Transform3D out_transform(G4RotationMatrix(0, 0, 0),
                                G4ThreeVector(0, 0, 0. ) );
    
    strawPVs.push_back(new G4PVPlacement(out_transform,
                                         aStrawLV,
                                         "myStrawPV",
                                         planes[i],
                                         false,
                                         0, true
                                         )
                       );

    i++;
  }
  return strawPVs;
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Straws::doCallArtProduces(art::EDProducer * producer) {

//}

// Actually add the data to the event
//void gm2ringsim::Straws::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
//}

using gm2ringsim::Straws;
DEFINE_ART_SERVICE(Straws)
