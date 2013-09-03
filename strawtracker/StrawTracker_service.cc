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

#include "Geant4/G4Box.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4TwoVector.hh"
#include "Geant4/G4SubtractionSolid.hh"

#include "gm2geom/strawtracker/StrawTrackerGeometry.hh"
#include "gm2ringsim/strawtracker/StrawSD.hh"
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include "gm2ringsim/strawtracker/StrawHit.hh"

#include "gm2geom/vac/VacGeometry.hh"

#include "boost/format.hpp"


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
      
      G4VSolid *strawStation = new G4Box("strawSystem", geom_.strawStationSizeHalf[sc], geom_.strawStationWidthHalf[sc], geom_.strawStationHeightHalf);
      G4VSolid *outerStationManifold = new G4Box("outerManifoldSystem", geom_.strawStationSizeHalf[sc], geom_.strawStationWidthHalf[sc], geom_.strawStationManifoldHeightHalf);
      G4VSolid *innerStationManifold = new G4Box("innerManifoldSystem", geom_.strawStationSizeHalf[sc]-1.5*mm, geom_.strawStationWidthHalf[sc]-1.5*mm, geom_.strawStationManifoldHeightHalf-1.5*mm);
       

      G4SubtractionSolid *stationManifold = new G4SubtractionSolid("stationManifold", outerStationManifold, innerStationManifold);

      std::string strawStationLVName = artg4::addNumberToName("StationChamberLV-%d", sc+tb);
      
      G4LogicalVolume* strawStationLV = new G4LogicalVolume(
                                                            strawStation,
                                                            artg4Materials::Vacuum(),
                                                            strawStationLVName,
                                                            0,
                                                            0);

      G4LogicalVolume* manifoldLV = new G4LogicalVolume(
                                                        stationManifold,
                                                        artg4Materials::Al(),
                                                        "stationManifold",
                                                         0,
                                                         0);
       
      double manifoldPlacement = geom_.strawStationHeightHalf-geom_.strawStationManifoldHeightHalf;

	  new G4PVPlacement(0,
					    G4ThreeVector(0,0,-1*manifoldPlacement),
						manifoldLV,
					    "manifoldLV-top",
					    strawStationLV,
						0,
					    0
                        );

	  new G4PVPlacement(0,
					    G4ThreeVector(0,0,manifoldPlacement),
						manifoldLV,
					    "manifoldLV-bottom",
					    strawStationLV,
						0,
					    0
                       );

      artg4::setVisAtts( strawStationLV, geom_.displayStation, geom_.stationColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(0);
                          att->SetVisibility(1);
                        }
                        );

      artg4::setVisAtts( manifoldLV, geom_.displayStation, geom_.manifoldColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(1);
                          att->SetVisibility(1);
                        }
                        );
      
      
      stations.push_back(strawStationLV);
      
    }
  }
  
  return stations;
  
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::StrawTracker::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
  std::vector<G4VPhysicalVolume*> strawStationPVs;
  
  const gm2geom::VacGeometry vacg("vac");
  int i = 0;
  int strawTrackerIndex, strawTrackerNumber;
  int numberOfStationsPerTracker = geom_.strawStationSize.size();
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
    deltaX = 0;
    
    int arcPosition = strawTrackerNumber % 2;
    int arcNumber = floor(strawTrackerNumber/2);
    
    deltaX = ds*sin(vacg.phi_a);
    
    double deltaX_c = deltaX - geom_.strawStationCenterFromEdge[stationIndex]*cos(vacg.phi_a);
    x = x - deltaX_c;

    y = sqrt(ds*ds - deltaX*deltaX) + geom_.strawStationCenterFromEdge[stationIndex]*sin(vacg.phi_a) ;
    
    G4TwoVector fixup(x,y);
        
    fixup.rotate(15*degree*arcPosition);
        
    G4Transform3D out_transform(G4RotationMatrix( -vacg.phi_a -vacg.phi_a*arcPosition, 0, 0),
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
