// Implementation of Straws

#include "gm2ringsim/strawtracker/Straws_service.hh"

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

#include "gm2ringsim/strawtracker/StrawTrackerGeometry.hh"
#include "gm2ringsim/strawtracker/StrawSD.hh"
#include "gm2ringsim/strawtracker/arthits/StrawArtRecord.hh"
#include "gm2ringsim/strawtracker/StrawHit.hh"

#include "boost/format.hpp"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Straws::Straws(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "straws"),
                   p.get<std::string>("category", "straws"),
                   p.get<std::string>("mother_category", "strawtracker")),
    strawSDname_("strawSD"),
    strawSD_(0)
{
  strawSD_ = artg4::getSensitiveDetector<StrawSD>(strawSDname_);
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Straws::doBuildLVs() {

  std::vector<G4LogicalVolume*> straws;
  
  
  for (unsigned int tb = 0; tb<geom_.whichScallopLocations.size() ;tb++){
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
          
          std::string strawLVName( boost::str( boost::format("SingleStrawLV-strawInRow%d-rowNumber%d-stationNumber%d") %st
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
          strawLV->SetSensitiveDetector( strawSD_ );
          straws.push_back(strawLV);
        }
      }
    }
  }
  return straws;
}

int gm2ringsim::Straws::extractValueFromName(std::string indicator, std::string name){
  
  int value;
  
  unsigned first = name.find(indicator);
  first = first+indicator.length();
  std::string str = name.substr(first);
  unsigned second = str.find("-");
  std::string num(name, first, second);
  
  std::istringstream iss(num);
  iss >> value;
  
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
    
    strawInRow = extractValueFromName("strawInRow", aStrawLV->GetName());
    rowNumber = extractValueFromName("rowNumber",aStrawLV->GetName());
    stationNumber = extractValueFromName("stationNumber",aStrawLV->GetName());
    
    std::string strawPVName(
                  boost::str(
                    boost::format("SingleStrawPV-strawInRow%d-rowNumber%d-stationNumber%d-strawNumber%d") %strawInRow
                                                                                                          %rowNumber
                                                                                                          %stationNumber
                                                                                                          %strawNumber));

    int stationIndex = stationNumber % geom_.strawStationSize.size();
    
    x = geom_.x_position_straw0[rowNumber] - geom_.strawStationSizeHalf[stationIndex] + geom_.dist_btwn_wires *strawInRow;
    if(rowNumber<2) x = x + 6.58;
    else x = x - 6.58;
    y= geom_.y_position[rowNumber];

    G4RotationMatrix* yRot = new G4RotationMatrix;
    double rot = 7.5*deg;
    if(rowNumber > 1) rot = -rot;

    yRot -> rotateY(rot); // Rotates 45 degrees
    G4ThreeVector placement(x, y, 0);
    
    strawPVs.push_back(new G4PVPlacement(G4Transform3D(*yRot, placement),
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
void gm2ringsim::Straws::doCallArtProduces(art::EDProducer * producer) {
  producer->produces<StrawArtRecordCollection>(category());

}

// Actually add the data to the event
void gm2ringsim::Straws::doFillEventWithArtHits(G4HCofThisEvent * hc) {

  
  std::unique_ptr<StrawArtRecordCollection> myArtHits(new StrawArtRecordCollection);
  
  // Find the collection ID for the hits
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();
  
  // The string here is unfortunately a magic constant. It's the string used
  // by the sensitive detector to identify the collection of hits.
  G4int collectionID = fSDM->GetCollectionID("strawSD");
  
  StrawHitsCollection* myCollection =
  static_cast<StrawHitsCollection*>(hc->GetHC(collectionID));
  // Check whether the collection exists
  if (NULL != myCollection) {
    std::vector<StrawHit*> geantHits = *(myCollection->GetVector());
    // Copy this hit into the Art hit
    for ( auto e : geantHits ) {
      
      e->Print();
      
      // Copy this hit into the Art hit
      
      //std::cout<<"Straw Number is: "<<e->straw<<std::endl;
      myArtHits->emplace_back( e->position.x(),e->position.y(),e->position.z(),e->position.r(),
                              e->local_position.x(),e->local_position.y(), e->local_position.z(),
                              e->momentum.x(),e->momentum.y(),e->momentum.z(),
                              e->local_momentum.x(),e->local_momentum.y(), e->local_momentum.z(),
                              e->time,
                              e->trackID,
                              e->volumeUID,
                              e->strawInRow, e->rowNumber, e->stationNumber, e->strawNumber,
                              e->particle_name, e->parent_ID);
      
    } //loop over geantHits
  } //if we have a myCollection
  
  else {
    throw cet::exception("Straw") << "Null collection of Geant tracker hits"
    << ", aborting!" << std::endl;
  }
  // Now that we have our collection of artized hits, add them to the event.
  // Get the event from the detector holder service
  art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
  art::Event & e = detectorHolder -> getCurrArtEvent();
  
  // Put the hits into the event
  e.put(std::move(myArtHits), category());
  
}

using gm2ringsim::Straws;
DEFINE_ART_SERVICE(Straws)
