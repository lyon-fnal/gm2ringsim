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
      
          int sn = st + row*geom_.strawStationType[sc];
          
          //std::string strawLVName = artg4::addNumberToName("SingleStrawLV", sc+tb+st);
          std::string strawLVName( boost::str( boost::format("SingleStrawLV[%d][%d]") %sc
                                                                                      %sn));

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

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Straws::doPlaceToPVs( std::vector<G4LogicalVolume*> planes) {
  std::vector<G4VPhysicalVolume*> strawPVs;
  
  int i =0;
  double x,y;
  for ( auto aStrawLV : lvs() ) {
    
    
    int strawNumber;
    int planeNumber;
    
    std::string name = aStrawLV->GetName();
    std::string::size_type left_pn = name.find_first_of('[');
    std::string::size_type right_pn = name.find_first_of(']');
    std::string num_pn(name, left_pn+1, right_pn-1);
    std::istringstream iss_pn(num_pn);
    iss_pn >> planeNumber;

    std::string::size_type left_sn = name.find_last_of('[');
    std::string::size_type right_sn = name.find_last_of(']');
    std::string num_sn(name, left_sn+1, right_sn-1);
    std::istringstream iss_sn(num_sn);
    iss_sn >> strawNumber;
    
    
    std::string strawPVName = artg4::addNumberToName("StrawPV", i);

    int row = strawNumber/geom_.strawStationType[planeNumber];
    int strawInRow = strawNumber % geom_.strawStationType[planeNumber];
    int x_shift = 2.75 * (row % 2);
    x=-((geom_.strawStationType[planeNumber]/2 - 1)*5.5 + 2.55) + 5.5*(strawInRow)+2.75 - x_shift;
    
    y= -(2.55+5.5) + 5.5*row;
    
    G4Transform3D out_transform(G4RotationMatrix(0, 0, 0),
                                G4ThreeVector(x, y, 0. ) );
    
    strawPVs.push_back(new G4PVPlacement(out_transform,
                                         aStrawLV,
                                         strawPVName,
                                         planes[planeNumber],
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
