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
  strawg_(myName()),
  vacg_("vac")
{
  strawg_.print();
}

//Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::StrawTracker::doBuildLVs() {

  std::vector<G4LogicalVolume*> modules;
  for (unsigned int tb = 0; tb<strawg_.whichScallopLocations.size(); tb++){
    for (unsigned int sc =0 ; sc<strawg_.strawModuleType.size(); sc++){
      
      G4VSolid *strawModule = new G4Box("strawSystem", strawg_.strawModuleSizeHalf[sc], 
                                                        strawg_.strawModuleWidthHalf, 
                                                        strawg_.strawModuleHeightHalf);
      
      //Build Manifold structure. Hollow box built by subtracting a slightly smaller box from the full size. 
      G4VSolid *outerModuleManifold = new G4Box("outerManifoldSystem", strawg_.strawModuleSizeHalf[sc], 
                                                                        strawg_.strawModuleManifoldWidthHalf, 
                                                                        strawg_.strawModuleManifoldHeightHalf);

      G4VSolid *innerModuleManifold = new G4Box("innerManifoldSystem", strawg_.strawModuleSizeHalf[sc] - strawg_.strawModuleManifoldThickness, 
                                                                        strawg_.strawModuleManifoldWidthHalf-strawg_.strawModuleManifoldThickness, 
                                                                        strawg_.strawModuleManifoldHeightHalf-strawg_.strawModuleManifoldThickness);
      
      G4SubtractionSolid *moduleManifold = new G4SubtractionSolid("moduleManifold", outerModuleManifold, innerModuleManifold);


      //Build Support post
                                                            
      G4Material *moduleMaterial = artg4Materials::Al();
    
      G4LogicalVolume* strawModuleLV = new G4LogicalVolume(
                                                            strawModule,
                                                            artg4Materials::Vacuum(),
                                                            "ModuleChamberLV",
                                                            0,
                                                            0);

      G4LogicalVolume* manifoldLV = new G4LogicalVolume(
                                                        moduleManifold,
                                                        moduleMaterial,
                                                        "moduleManifold",
                                                         0,
                                                         0);



      double manifoldPlacement = strawg_.strawModuleHeightHalf-strawg_.strawModuleManifoldHeightHalf;

	    new G4PVPlacement(0,
					            G4ThreeVector(0,0,-1*manifoldPlacement),
						          manifoldLV,
					            "manifoldLV-top",
					            strawModuleLV,
						          0,
					            0
                     );

	    new G4PVPlacement(0,
					            G4ThreeVector(0,0,manifoldPlacement),
						          manifoldLV,
					            "manifoldLV-bottom",
					            strawModuleLV,
						          0,
					            0
                     );

      
      artg4::setVisAtts( strawModuleLV, strawg_.displayModule, strawg_.moduleColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(0);
                          att->SetVisibility(1);
                        }
                        );

      artg4::setVisAtts( manifoldLV, strawg_.displayModuleMaterial, strawg_.manifoldColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(1);
                          att->SetVisibility(1);
                        }
                        );
      
      //Only build the support post if that request is set to true in the fcl file. 
      //At the time of this writing it was unsure if the support post was going to be 
      //needed. Do all the building and placement within this if statement.  
      if(strawg_.buildSupportPost){
        G4VSolid *supportPost = new G4Tubs("supportPost",0, 
                                                       strawg_.supportPostRadius, 
																											 strawg_.halfHeightOfTheStraw,
																											 strawg_.startAngleOfTheStraw,
																											 strawg_.spanningAngleOfTheStraw
                                        );
        G4Material *supportPostMaterial = artg4Materials::C();
        G4LogicalVolume* supportPostLV = new G4LogicalVolume(
                                                            supportPost, 
                                                            supportPostMaterial,
                                                            "moduleSupportPost",
                                                            0,
                                                            0
                                                          );
        double xpospost = strawg_.strawModuleSizeHalf[sc]-strawg_.supportPostRadius;

        new G4PVPlacement(0,
                      G4ThreeVector(xpospost,0,0),
                      supportPostLV,
                      "supportPostLV",
                      strawModuleLV,
                      0,
                      0
                     );
        artg4::setVisAtts( supportPostLV, strawg_.displayModuleMaterial, strawg_.manifoldColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(1);
                          att->SetVisibility(1);
                        }
                        );
      }
      modules.push_back(strawModuleLV);
      
    }
  }
  
  return modules;
  
}

void gm2ringsim::StrawTracker::getXYCoordinatesForPlacement(double distAlongScallop, double distShift, double &x, double &y){

    //move long the scallop line
    x = x - distAlongScallop*sin(vacg_.phi_a);
    y = distAlongScallop*cos(vacg_.phi_a);

    //move inward to be in correct position
    x = x - distShift*cos(vacg_.phi_a);
    y = y - distShift*sin(vacg_.phi_a);
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::StrawTracker::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
  std::vector<G4VPhysicalVolume*> strawModulePVs;
  
  int i = 0;
  int strawStationIndex, strawStationNumber;
  int numberOfModulesPerStation = strawg_.strawModuleType.size();
  int moduleIndex;

  //loop over the logical volumes
  for ( auto aStrawModuleLV : lvs() ) {
    // We to name the module including its module number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    
    strawStationIndex = i/numberOfModulesPerStation;
    strawStationNumber = strawg_.whichScallopLocations[strawStationIndex];
    moduleIndex = i%numberOfModulesPerStation;

    std::string strawModuleLabel( boost::str( boost::format("strawModuleNumber[%d][%d]") %strawStationNumber %moduleIndex));

    
    G4double
    x = vacg_.trackerExtPlacementX,
    y = 0,
    distAlongScallop = strawg_.strawModuleLocation[moduleIndex]; 
    
    int arcPosition = strawStationNumber % 2;
    int arcNumber = floor(strawStationNumber/2);
    
    getXYCoordinatesForPlacement(distAlongScallop,strawg_.distShift[moduleIndex],x,y);  
    
    G4TwoVector fixup(x,y);
        
    fixup.rotate(15*degree*arcPosition);
    G4Transform3D out_transform(G4RotationMatrix( 0, 0, -vacg_.phi_a -15*degree*arcPosition),
                                    G4ThreeVector(fixup.x(), fixup.y(), 0. ) );

    strawModulePVs.push_back(new G4PVPlacement(out_transform,
                                         aStrawModuleLV,
                                         strawModuleLabel,
                                         vacs[ arcNumber ],
                                         false,
                                         0, true
                                        )
                           );
    
    i++;
  }
  return strawModulePVs;


}

using gm2ringsim::StrawTracker;
DEFINE_ART_SERVICE(StrawTracker)
