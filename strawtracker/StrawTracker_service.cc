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
#include "Geant4/G4Tubs.hh"
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
    for (unsigned int sc =0 ; sc<geom_.strawStationType.size(); sc++){
      
      G4VSolid *strawStation = new G4Box("strawSystem", geom_.strawStationSizeHalf[sc], geom_.strawStationWidthHalf[sc], geom_.strawStationHeightHalf);
      
      //Build Manifold structure. Hollow box built by subtracting a slightly smaller box from the full size. 
      G4VSolid *outerStationManifold = new G4Box("outerManifoldSystem", geom_.strawStationSizeHalf[sc], geom_.strawStationManifoldWidth/2, geom_.strawStationManifoldHeightHalf);
      G4VSolid *innerStationManifold = new G4Box("innerManifoldSystem", geom_.strawStationSizeHalf[sc]-geom_.strawStationManifoldThickness, geom_.strawStationManifoldWidth/2-geom_.strawStationManifoldThickness, geom_.strawStationManifoldHeightHalf-geom_.strawStationManifoldThickness);
      G4SubtractionSolid *stationManifold = new G4SubtractionSolid("stationManifold", outerStationManifold, innerStationManifold);


      //Build Support post
      G4VSolid *supportPost = new G4Tubs("supportPost",0, 
                                                       geom_.supportPostRadius, 
																											 geom_.halfHeightOfTheStraw,
																											 geom_.startAngleOfTheStraw,
																											 geom_.spanningAngleOfTheStraw
                                        );
      //Build Support Plate
                                                            
      std::string strawStationLVName = artg4::addNumberToName("StationChamberLV-%d", sc+tb);
      G4Material *stationMaterial = artg4Materials::Al();
      G4Material *supportPostMaterial = artg4Materials::C();
    
      G4LogicalVolume* strawStationLV = new G4LogicalVolume(
                                                            strawStation,
                                                            artg4Materials::Vacuum(),
                                                            strawStationLVName,
                                                            0,
                                                            0);

      G4LogicalVolume* manifoldLV = new G4LogicalVolume(
                                                        stationManifold,
                                                        stationMaterial,
                                                        "stationManifold",
                                                         0,
                                                         0);

      G4LogicalVolume* supportPostLV = new G4LogicalVolume(
                                                            supportPost, 
                                                            supportPostMaterial,
                                                            "stationSupportPost",
                                                            0,
                                                            0
                                                          );



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
      double xpospost = geom_.strawStationSizeHalf[sc]-geom_.supportPostRadius;
      double ypospost = geom_.supportPostYPosition - geom_.strawStationWidthHalf[sc];
      new G4PVPlacement(0,
                      G4ThreeVector(xpospost,ypospost,0),
                      supportPostLV,
                      "supportPostLV",
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

      artg4::setVisAtts( manifoldLV, geom_.displayStationMaterial, geom_.manifoldColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(1);
                          att->SetVisibility(1);
                        }
                        );
      artg4::setVisAtts( supportPostLV, geom_.displayStationMaterial, geom_.manifoldColor,
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

void gm2ringsim::StrawTracker::getXYCoordinatesForPlacement(double distAlongScallop, double distShift, double &x, double &y){

    const gm2geom::VacGeometry vacg("vac");
    //move long the scallop line
    x = x - distAlongScallop*sin(vacg.phi_a);
    y = distAlongScallop*cos(vacg.phi_a);

    //move inward to be in correct position
    x = x - distShift*cos(vacg.phi_a);
    y = y - distShift*sin(vacg.phi_a);
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::StrawTracker::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
  std::vector<G4VPhysicalVolume*> strawStationPVs;
  
  const gm2geom::VacGeometry vacg("vac");
  int i = 0;
  int strawTrackerIndex, strawTrackerNumber;
  int numberOfStationsPerTracker = geom_.strawStationSize.size();
  int stationIndex;
  geom_.print();
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
    x = vacg.trackerExtPlacementX,
    y = 0,
    distAlongScallop = geom_.strawStationLocation[stationIndex]; 
    
    int arcPosition = strawTrackerNumber % 2;
    int arcNumber = floor(strawTrackerNumber/2);
    

    double distShift = vacg.trackerExtensionW+ vacg.outerWallThickness -  geom_.strawStationSizeHalf[stationIndex];
    
    getXYCoordinatesForPlacement(distAlongScallop,distShift,x,y);  
    
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
