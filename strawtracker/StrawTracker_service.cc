// Implementation of Straw Tracker


#include "gm2ringsim/strawtracker/StrawTracker_service.hh"

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

#include "gm2ringsim/strawtracker/StrawTrackerGeometry.hh"
#include "gm2ringsim/strawtracker/StrawSD.hh"
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include "gm2ringsim/strawtracker/StrawHit.hh"

#include "gm2ringsim/vac/VacGeometry.hh"

#include "boost/format.hpp"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::StrawTracker::StrawTracker(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
                   p.get<std::string>("name", "strawtracker"),
                   p.get<std::string>("category", "strawtracker"),
                   p.get<std::string>("mother_category", "vac")),
  geom_(myName())
{
  geom_.print();
}

//Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::StrawTracker::doBuildLVs() {

  std::vector<G4LogicalVolume*> stations;
  for (unsigned int tb = 0; tb<geom_.whichScallopLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawStationLocation.size(); sc++){
      
      G4VSolid *strawStation = new G4Box("strawSystem", geom_.strawStationSizeHalf[sc], geom_.strawStationWidthHalf, geom_.strawStationHeightHalf);
      
      std::string strawStationLVName = artg4::addNumberToName("StationChamberLV", sc+tb);
      
      G4LogicalVolume* strawStationLV = new G4LogicalVolume(
                                                            strawStation,
                                                            artg4Materials::Vacuum(),
                                                            strawStationLVName,
                                                            0,
                                                            0);
      
      artg4::setVisAtts( strawStationLV, geom_.displayStation, geom_.stationColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(0);
                          att->SetVisibility(1);
                        }
                        );
      
      
      // We can make the physical volumes here
      //strawLV->SetSensitiveDetector( strawSD_ );
      
      stations.push_back(strawStationLV);
      
    }
  }

  return stations;
  
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::StrawTracker::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
  std::vector<G4VPhysicalVolume*> strawStationPVs;
  
  const VacGeometry vacg("vac");
  int i = 0;
  int strawTrackerIndex, strawTrackerNumber;
  int numberOfStations = lvs().size();
  int numberOfStationsPerTracker = numberOfStations/geom_.whichScallopLocations.size();
  int stationIndex;
  //loop over the logical volumes
  for ( auto aStrawStationLV : lvs() ) {
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    strawTrackerIndex = i/numberOfStationsPerTracker;
    strawTrackerNumber = geom_.whichScallopLocations[strawTrackerIndex];
    stationIndex = i%numberOfStationsPerTracker;

    
    std::string strawStationLabel( boost::str( boost::format("strawStationNumber[%d][%d]") %strawTrackerNumber %stationIndex));

    G4double
    x = 7010,
    y = 0,
    ds = geom_.strawStationLocation[stationIndex],
    deltaX =0;
    
    int arcPosition = strawTrackerNumber % 2;
    int arcNumber = floor(strawTrackerNumber/2);
    
    deltaX = ds*sin(vacg.phi_a);
    double deltaX_c = deltaX - geom_.straw_station_center_from_edge[stationIndex]*cos(vacg.phi_a);
    x = x - deltaX_c;
    y = sqrt(ds*ds - deltaX*deltaX) + geom_.straw_station_center_from_edge[stationIndex]*sin(vacg.phi_a) ;
    
    G4TwoVector fixup(x,y);
        
    fixup.rotate(15*degree*arcPosition);
        
    G4Transform3D out_transform(G4RotationMatrix( -13*deg -vacg.phi_a*arcPosition, 0, 0),
                                    G4ThreeVector(fixup.x(), fixup.y(), 0. ) );

    strawStationPVs.push_back(new G4PVPlacement(out_transform,
                                         aStrawStationLV,
                                         strawStationLabel,
                                         vacs[ arcNumber ],
                                         false,
                                         0, true
                                        )
                           );
    
    i++;
  }
  return strawStationPVs;


}

using gm2ringsim::StrawTracker;
DEFINE_ART_SERVICE(StrawTracker)
