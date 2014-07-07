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

#include "gm2geom/strawtracker/StrawTrackerGeometry.hh"
#include "gm2ringsim/strawtracker/StrawSD.hh"
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include "gm2ringsim/strawtracker/StrawHit.hh"

#include "gm2dataproducts/strawtracker/WireID.hh"

#include "boost/format.hpp"
#include <sstream>

#include "CLHEP/Vector/ThreeVector.h"

using gm2strawtracker::WireID;
using gm2strawtracker::StrawView;
using CLHEP::Hep3Vector;
using std::ostringstream;
using std::istringstream;
using std::string;

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Straws::Straws(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
      p.get<std::string>("name", "straws"),
      p.get<std::string>("category", "straws"),
      p.get<std::string>("mother_category", "strawtracker")),
  strawSDname_("strawSD"),
  strawSD_(artg4::getSensitiveDetector<StrawSD>(strawSDname_))
{
  //strawSD_ = artg4::getSensitiveDetector<StrawSD>(strawSDname_);
}
//Build the straw walls
//Build the gas straw 
//Build the wire

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Straws::doBuildLVs() {

  std::vector<G4LogicalVolume*> straws;

  for (unsigned int tb = 0; tb<geom_.whichScallopLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawModuleLocation.size(); sc++){
      for (int view = 0 ; view<geom_.strawView ; view++){
        for (int layer = 0 ; layer<geom_.strawLayers; layer++){
          for (int st = 0; st<geom_.strawModuleType[sc]; st++){


            // Create a WireID to identify this straw
            WireID currentWire;
            currentWire.setTrackerNumber(geom_.whichScallopLocations[tb]);
            currentWire.setModule(sc);
            currentWire.setView( view == 0 ? gm2strawtracker::u_view : 
                (view == 1 ? gm2strawtracker::v_view : gm2strawtracker::na_view));
            currentWire.setLayer(layer);
            currentWire.setWire(st);

            // Make the name for this logical volume
            ostringstream lvStream;
            lvStream << "SingleStrawLV - " << currentWire;
            string strawLVName = lvStream.str();


            G4Tubs* tracker_tube = new G4Tubs("tracker_tube",
                geom_.innerRadiusOfTheStraw,
                geom_.outerRadiusOfTheStraw,
                geom_.halfLengthOfTheStraw,
                geom_.startAngleOfTheStraw,
                geom_.spanningAngleOfTheStraw
                );


            G4Tubs* gas_tube = new G4Tubs("gas_tube",
                geom_.innerRadiusOfTheStraw,
                geom_.outerRadiusOfTheGas,
                geom_.halfLengthOfTheStraw,
                geom_.startAngleOfTheStraw,
                geom_.spanningAngleOfTheStraw
                );

            G4Tubs* wire_tube = new G4Tubs("wire_tube",
                geom_.innerRadiusOfTheStraw,
                geom_.outerRadiusOfTheWire,
                geom_.halfLengthOfTheStraw,
                geom_.startAngleOfTheStraw,
                geom_.spanningAngleOfTheStraw
                );


            G4LogicalVolume* strawLV = new G4LogicalVolume(tracker_tube,
                artg4Materials::Mylar(),
                strawLVName,
                0,
                0);

            G4LogicalVolume* gasLV = new G4LogicalVolume(gas_tube,
                artg4Materials::ArCO2(),
                strawLVName,
                0,
                0);

            G4LogicalVolume* wireLV = new G4LogicalVolume(wire_tube,
                artg4Materials::W(),
                "wireLV",
                0,
                0);

            //Make the Gas layer

            new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                wireLV,
                "wireLV",
                gasLV,
                0,
                0
                );

            new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                gasLV,
                strawLVName,
                strawLV,
                0,
                0
                );


            artg4::setVisAtts( strawLV, geom_.displayStraw, geom_.strawColor,
                [] (G4VisAttributes* att) {
                att->SetForceSolid(1);
                att->SetVisibility(1);
                }
                );

            artg4::setVisAtts( gasLV, geom_.displayStraw, geom_.gasColor,
                [] (G4VisAttributes* att) {
                att->SetForceSolid(1);
                att->SetVisibility(1);
                }
                );

            artg4::setVisAtts( wireLV, geom_.displayStraw, geom_.wireColor,
                [] (G4VisAttributes* att) {
                att->SetForceSolid(1);
                att->SetVisibility(1);
                }
                );
            gasLV->SetSensitiveDetector( strawSD_ );
            straws.push_back(strawLV);
          }
        }
      }
    }
  }
  return straws;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Straws::doPlaceToPVs( std::vector<G4LogicalVolume*> planes) {
  std::vector<G4VPhysicalVolume*> strawPVs;


  int strawNumber =0;
  double x,y;

  std::ostringstream oss;
  oss << "strawInRow, " 
    << "layer, " 
    << "view, " 
    << "module, "
    << "tracker, "
    << "x, "
    << "y, "
    << "xTracker, " 
    << "zTrackerB, "
    << "yRot " << "\n" ;

  for ( auto aStrawLV : lvs() ) {
    string lvName = aStrawLV->GetName();

    // Get the WireID from the logical volume's name!
    istringstream thing(lvName);
    string parseString; 
    // First remove the "SingleStrawLV - " from the front.
    std::getline(thing, parseString, '-');

    // Next get the rest of the line (parses to the next \n)
    std::getline(thing, parseString);

    // And finally, make a WireID from that string
    WireID wire = gm2strawtracker::wireIDfromString(parseString);
    // Make the physical volume's name
    ostringstream pvStream;
    pvStream << "SingleStrawPV - " << wire;
    string strawPVName = pvStream.str();

    x = geom_.wireXPosition(wire) - geom_.strawModuleSizeHalf[wire.getModule()];
    y = geom_.wireYPosition(wire);


    G4RotationMatrix* yRot = new G4RotationMatrix;

    double rot = geom_.layerAngle;
    if( wire.getView() == gm2strawtracker::u_view ) rot = -rot;
    yRot -> rotateY(rot);
    G4ThreeVector placement(x, y, 0);

    Hep3Vector trackerLocation = geom_.trackerPosition(wire);

    oss << wire.getWire() << ", " 
      << wire.getLayer() << ", " 
      << wire.getView() << ", " 
      << wire.getModule() << ", " 
      << wire.getTrackerNumber() << ", "
      << x << ", " 
      << y << ", "
      << trackerLocation.getX() << ", "
      << trackerLocation.getZ() << ", "
      << rot << "\n";


    strawPVs.push_back(new G4PVPlacement(G4Transform3D(*yRot, placement),
          aStrawLV,
          strawPVName,
          planes[geom_.TotalModuleNumber(wire)],
          false,
          0
          )
        );

    strawNumber++;
  }
  mf::LogInfo("StrawGeometryPlacementDetails") << oss.str();
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

      // Copy this hit into the Art hit

      myArtHits->emplace_back( e->global_position.x(),e->global_position.y(),e->global_position.z(),e->global_position.r(),
          e->momentum.x(),e->momentum.y(),e->momentum.z(),
          e->local_position.x(),e->local_position.y(), e->local_position.z(),
          e->local_momentum.x(),e->local_momentum.y(), e->local_momentum.z(),
          e->module_position.x(), e->module_position.y(), e->module_position.z(),
          e->scallop_position.x(), e->scallop_position.y(), e->scallop_position.z(),
          e->time,
          e->trackID,
          e->volumeUID,
          e->strawInRow, e->layerNumber, e->viewNumber, e->moduleNumber, e->strawNumber, e->trackerNumber,
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
