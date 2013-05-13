// Implementation of Traceback


#include "gm2ringsim/traceback/StrawTracker_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

//Geant4
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

#include "Geant4/G4Box.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4UnionSolid.hh"
#include "Geant4/G4SubtractionSolid.hh"
#include "Geant4/G4Tubs.hh"
#include "Geant4/G4Trap.hh"
#include "Geant4/G4TwoVector.hh"

#include "gm2ringsim/traceback/TracebackGeometry.hh"
#include "gm2ringsim/traceback/StrawSD.hh"
#include "gm2ringsim/traceback/StrawArtRecord.hh"
#include "gm2ringsim/traceback/StrawHit.hh"

#include "gm2ringsim/vac/VacGeometry.hh"

#include "boost/format.hpp"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::StrawTracker::StrawTracker(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
                   p.get<std::string>("name", "traceback"),
                   p.get<std::string>("category", "traceback"),
                   p.get<std::string>("mother_category", "vac")),
  geom_(myName())
{}


void gm2ringsim::StrawTracker::makePlaneLVs(std::vector<G4LogicalVolume*>& planes){

  for (unsigned int tb = 0; tb<geom_.whichTracebackLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawStationLocation.size(); sc++){
      
      G4VSolid *strawSystem = new G4Box("strawSystem", geom_.strawStationSizeHalf[sc], geom_.strawStationWidthHalf, geom_.strawStationHeightHalf-10);
      
      std::string strawLVName = artg4::addNumberToName("StrawChamberLV", sc+tb);
      
      G4LogicalVolume* strawLV = new G4LogicalVolume(
                                                     strawSystem,
                                                     artg4Materials::Vacuum(),
                                                     strawLVName,
                                                     0,
                                                     0);
      
      artg4::setVisAtts( strawLV, geom_.displayStation, geom_.stationColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(0);
                          att->SetVisibility(1);
                        }
                        );
      
      
      // We can make the physical volumes here
      //strawLV->SetSensitiveDetector( strawSD_ );
      
      planes.push_back(strawLV);
      
    }
  }


}


//Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::StrawTracker::doBuildLVs() {

  std::vector<G4LogicalVolume*> planes;
  
  makePlaneLVs(planes);
  

  return planes;
  
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::StrawTracker::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
  std::vector<G4VPhysicalVolume*> strawPVs;
  //strawPVs.resize(lvs().size());
  
  const VacGeometry vacg("vac");
  int i = 0;
  int tracebackIndex, tracebackNumber;
  int numberOfStraws = lvs().size();
  int numberOfStrawStationsPerTB = numberOfStraws/geom_.whichTracebackLocations.size();
  int stationInTBNumber;
  
  //loop over the logical volumes
  for ( auto aStrawLV : lvs() ) {
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    tracebackIndex = i/numberOfStrawStationsPerTB;
    tracebackNumber = geom_.whichTracebackLocations[tracebackIndex];
    stationInTBNumber = i%numberOfStrawStationsPerTB;

    
    std::string tracebackLabel( boost::str( boost::format("TracebackNumber[%d][%d]") %tracebackNumber %stationInTBNumber));

    G4double
    x = 7010,
    z = 0,
    //phi = 12.8,
    ds = geom_.strawStationLocation[stationInTBNumber],
    deltaX =0;
    
    int arcPosition = tracebackNumber % 2;
    int arcNumber = floor(tracebackNumber/2);
    double distance_from_edge = (geom_.strawStationSizeHalf[stationInTBNumber] + geom_.strawStationOffset[stationInTBNumber]);
    deltaX = ds*sin(vacg.phi_a);
    double deltaX_c = deltaX - distance_from_edge*cos(vacg.phi_a);
    x = x - deltaX_c;
    z = sqrt(ds*ds - deltaX*deltaX) + distance_from_edge*sin(vacg.phi_a) ;
    
    G4TwoVector fixup(x,z);
        
    fixup.rotate(15.*degree*arcPosition);
        
    G4Transform3D out_transform(G4RotationMatrix( -15*deg -vacg.phi_a*arcPosition, 0, 0),
                                    G4ThreeVector(fixup.x(), fixup.y(), 0. ) );

    strawPVs.push_back(new G4PVPlacement(out_transform,
                                         aStrawLV,
                                         tracebackLabel,
                                         vacs[ arcNumber ],
                                         false,
                                         0, true
                                        )
                           );
    
    i++;
  }
  return strawPVs;


}

// Declare to Art what we are producing
//void gm2ringsim::Traceback::doCallArtProduces(art::EDProducer * producer) {
//}


// Actually add the data to the event
//void gm2ringsim::Traceback::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//}

using gm2ringsim::StrawTracker;
DEFINE_ART_SERVICE(StrawTracker)
