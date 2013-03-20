// Implementation of Traceback


#include "gm2ringsim/traceback/Traceback_service.hh"

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

#include "gm2ringsim/vac/VacGeometry.hh"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Traceback::Traceback(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
                   p.get<std::string>("name", "traceback"),
                   p.get<std::string>("category", "traceback"),
                   p.get<std::string>("mother_category", "vac")),
  geom_(myName()),
  strawSDname_("strawSD"),
  strawSD_(0)
{
  //strawSD_ = artg4::getSensitiveDetector<StrawSD>(strawSDname_);
}


//void gm2ringsim::Traceback::makeStrawLVDetectors(std::vector<G4LogicalVolume*>& straws){
  
//  }


//Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Traceback::doBuildLVs() {
  geom_.print();
  const VacGeometry vacg("vac");

  std::vector<G4LogicalVolume*> straws;
  
  for (unsigned int tb = 0; tb<geom_.whichTracebackLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawLocation.size(); sc++){
      
      G4VSolid *strawSystem = new G4Box("strawSystem", geom_.strawRadialExtentHalf[sc], 20, geom_.tracebackZHalf-10);
      
      std::string strawLVName = artg4::addNumberToName("StrawChamberLV", sc+tb);
      
      G4LogicalVolume* strawLV = new G4LogicalVolume(
                                                     strawSystem,
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
      
      
      // We can make the physical volumes here
      StrawSD* strawSD_ = artg4::getSensitiveDetector<StrawSD>(strawSDname_);
      strawLV->SetSensitiveDetector( strawSD_ );
      
      straws.push_back(strawLV);
      
    }
  }
  return straws;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Traceback::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
  std::vector<G4VPhysicalVolume*> strawPVs;
  //strawPVs.resize(lvs().size());
  
  const VacGeometry vacg("vac");
  int i = 0;
  int tracebackIndex, tracebackNumber;
  int numberOfStraws = lvs().size();
  int numberOfStrawsPerTB = numberOfStraws/geom_.whichTracebackLocations.size();
  int strawInTBNumber;
  
  //loop over the logical volumes
  for ( auto aStrawLV : lvs() ) {
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    tracebackIndex = i/numberOfStrawsPerTB;
    tracebackNumber = geom_.whichTracebackLocations[tracebackIndex];
    strawInTBNumber = i%numberOfStrawsPerTB;

    
    std::string tracebackLabel( boost::str( boost::format("TracebackNumber[%d][%d]") %tracebackNumber %strawInTBNumber));

    G4double
    x = 7020,
    z = 0,
    phi = 12.8,
    ds = geom_.strawLocation[strawInTBNumber],
    deltaX =0;
    
    int arcPosition = tracebackNumber % 2;
    int arcNumber = floor(tracebackNumber/2);

    deltaX = ds * sin(phi * deg);
    x = x - deltaX;
    x = x + geom_.strawRadialExtentHalf[strawInTBNumber];
    z = sqrt(ds*ds - deltaX*deltaX);
    
    G4TwoVector fixup(x,z);
        
    fixup.rotate(15.*degree*arcPosition);
        
    G4Transform3D out_transform(G4RotationMatrix( -13*deg -vacg.phi_a*arcPosition, 0, 0),
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


//i want to take a vector of traceback locations [1,3,5] for example.
//for each of those locations I want to put two straw chambers (planes for now)
//those straw chambers should be at 20mm on either side of the edges of the box
//they should be 5mm thick

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
void gm2ringsim::Traceback::doCallArtProduces(art::EDProducer * producer) {
  producer->produces<StrawArtRecordCollection>(category());

}

// Actually add the data to the event
void gm2ringsim::Traceback::doFillEventWithArtHits(G4HCofThisEvent * hc) {

  
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
    int i = 0;
    float my_r;
    for ( auto e : geantHits ) {
      e->Print();
      // Copy this hit into the Art hit
      my_r = sqrt(e->position.x()*e->position.x() + e->position.z()*e->position.z());
      myArtHits->emplace_back( e->position.x(),e->position.y(),e->position.z(),e->position.r(),my_r,
                                e->local_position.x(),e->local_position.y(), e->local_position.z(),
                                e->momentum.x(),e->momentum.y(),e->momentum.z(),
                                e->local_momentum.x(),e->local_momentum.y(), e->local_momentum.z(),
                                e->time,
                                e->trackID,
                                e->volumeUID,
                                e->traceback,
                                e->straw);
      i++;
      
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

using gm2ringsim::Traceback;
DEFINE_ART_SERVICE(Traceback)
