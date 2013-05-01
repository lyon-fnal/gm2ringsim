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

#include "boost/format.hpp"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Straws::Straws(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "straws"),
                   p.get<std::string>("category", "straws"),
                   p.get<std::string>("mother_category", "traceback"))
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Straws::doBuildLVs() {

  std::vector<G4LogicalVolume*> straws;
  
  
  for (unsigned int tb = 0; tb<geom_.whichTracebackLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawStationLocation.size(); sc++){
      for (int row = 0 ; row<4 ; row++){

        for (int st = 0; st<geom_.strawStationType[sc]; st++){
          
          G4Tubs* tracker_tube = new G4Tubs("tracker_tube",
                                    geom_.innerRadiusOfTheStraw,
                                    geom_.outerRadiusOfTheStraw,
                                    geom_.heightOfTheStraw,
                                    geom_.startAngleOfTheStraw,
                                    geom_.spanningAngleOfTheStraw
                                    );
      
          int stationNumber = sc + tb*geom_.strawStationLocation.size();
          
          std::string strawLVName( boost::str( boost::format("SingleStrawLV(%d)[%d]{%d}") %st
                                                                                          %row
                                                                                          %stationNumber));

          G4LogicalVolume* strawLV = new G4LogicalVolume(
                                                       tracker_tube,
                                                       artg4Materials::Vacuum(),
                                                       strawLVName,
                                                       0,
                                                       0);
      
          artg4::setVisAtts( strawLV, geom_.displayStraw, geom_.strawColor,
                    [] (G4VisAttributes* att) {
                      att->SetForceSolid(1);
                      att->SetVisibility(1);
                    }
                    );

          straws.push_back(strawLV);
        }
      }
    }
  }
  return straws;
}

int gm2ringsim::Straws::FindValue(char indicator, std::string name){

  int value;
  std::string::size_type left_pn = name.find_first_of(indicator);
  std::string::size_type right_pn = name.find_first_of(indicator);
  std::string num_pn(name, left_pn+1, right_pn-1);
  std::istringstream iss_pn(num_pn);
  iss_pn >> value;
  
  return value;
  
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Straws::doPlaceToPVs( std::vector<G4LogicalVolume*> planes) {
  std::vector<G4VPhysicalVolume*> strawPVs;
  
  int strawNumber =0;
  double x,y;
  
  int strawInRow;
  int rowNumber;
  int stationNumber;
  

  for ( auto aStrawLV : lvs() ) {
  
    strawInRow = FindValue('(', aStrawLV->GetName());
    rowNumber = FindValue ('[', aStrawLV->GetName());
    stationNumber = FindValue('{', aStrawLV->GetName());

    std::string strawPVName = artg4::addNumberToName("StrawPV", strawNumber);
    int stationIndex = stationNumber % geom_.strawStationSize.size();
    
    x = geom_.x_position_straw0[rowNumber] - geom_.strawStationSizeHalf[stationIndex] + geom_.dist_btwn_wires *strawInRow;
    if(rowNumber<2) x = x + 6.58;
    else x = x - 6.58;
    y= geom_.y_position[rowNumber];

    G4RotationMatrix* yRot = new G4RotationMatrix; // Rotates X and Z axes only
    double rot = 7.5*deg;
    if(rowNumber > 1) rot = -rot;

    yRot -> rotateY(rot); // Rotates 45 degrees
    G4ThreeVector placement(x, y, 0);
    
    strawPVs.push_back(new G4PVPlacement(G4Transform3D(*yRot, placement),//
                                         aStrawLV,
                                         strawPVName,
                                         planes[stationNumber],
                                         false,
                                         0
                                         )
                       );

    strawNumber++;
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
