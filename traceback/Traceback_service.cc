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



G4LogicalVolume* gm2ringsim::Traceback::makeATracebackLV(int tracebackNum) {
  
  const VacGeometry vacg("vac");

  G4Tubs *torus = new G4Tubs("torus",
                             vacg.torus_rmin,
                             vacg.torus_rmax[vacg.vacuumRegion],
                             vacg.torus_z[vacg.vacuumRegion],
                             vacg.torus_sphi, vacg.torus_dphi);
  
  G4Tubs *torus2 = new G4Tubs("torus2",
                             vacg.torus_rmin-10,
                             vacg.torus_rmax[vacg.vacuumRegion] + 10,
                             vacg.torus_z[vacg.vacuumRegion]+10,
                             vacg.torus_sphi-10*deg, vacg.torus_dphi+20*deg);

  G4double
  pPhi = 0., pAlp = 0.,
  pTheta = (vacg.phi_a - vacg.phi_b)/2.,
  pDz = vacg.z[vacg.vacuumRegion]/2.,
  pDy = vacg.torus_z[vacg.vacuumRegion],
  pDx12 = vacg.xL[vacg.vacuumRegion]/2.,
  pDx34 = vacg.xS[vacg.vacuumRegion]/2.;
  
  G4Trap *out_scallop =
  new G4Trap("out_scallop", pDz, pTheta, pPhi,
             pDy, pDx12, pDx12, pAlp,
             pDy, pDx34, pDx34, pAlp
             );
  
  G4double
  dz = -vacg.z[vacg.vacuumRegion]/2.,
  dx = -dz*std::tan( (vacg.phi_b-vacg.phi_a)/2. ) + vacg.xS[vacg.vacuumRegion]/2.;
  
  // The little rotation is in the coordinates of the trapezoid,
  G4TwoVector fixup(dz,dx);
  fixup.rotate( -vacg.phi_a );
  // flip to the coordinate system of the arcSection
  fixup.setX(-fixup.x());
  fixup += vacg.pt_a[vacg.vacuumRegion];
  
  
  int arcPosition = tracebackNum % 2;

  fixup.rotate(15.*degree*arcPosition);

  G4Transform3D
  out_transform(G4RotationMatrix( 0., 90.*degree, -vacg.phi_a+(-15*arcPosition-90.)*degree ),
                  G4ThreeVector( fixup.x(), fixup.y(), 0. ) );

  G4UnionSolid *torus_scallop = new G4UnionSolid("torus_scallop", torus, out_scallop, out_transform);
  G4SubtractionSolid *scallop = new G4SubtractionSolid("scallop", torus_scallop, torus2);

  //new G4UnionSolid("stupid",torus_scallop, torus_scallop);
  G4LogicalVolume *tracebackLV = new G4LogicalVolume(scallop,
                                                     artg4Materials::Vacuum(),
                                                     "tracebackLV");
  
  artg4::setVisAtts(tracebackLV, geom_.displayTraceback, geom_.tracebackColor);
  return tracebackLV;

}
// Build the logical volumes
void gm2ringsim::Traceback::makeTracebackLVs(std::vector<G4LogicalVolume*>& tracebacks) {
  
   // Build the logical volumes
  for ( unsigned int tracebackNumber = 0; tracebackNumber != geom_.whichTracebackLocations.size(); ++tracebackNumber ) {
    // Push this into the vector
    tracebacks.push_back( makeATracebackLV(geom_.whichTracebackLocations[tracebackNumber]));
  }
}

void gm2ringsim::Traceback::makeStrawDetectors(std::vector<G4VPhysicalVolume*>& straws,std::vector<G4LogicalVolume*>& tracebacks){
  
  const VacGeometry vacg("vac");
  
  for (unsigned int tb = 0; tb<geom_.whichTracebackLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawLocation.size(); sc++){
      G4double
      r = 7020,
      y = 0,
      phi = 12.8,
      ys = geom_.strawLocation[sc],
      deltaR =0;
      
      int arcPosition = geom_.whichTracebackLocations[tb] % 2;
      
      deltaR = ys * sin(phi * deg);
      r = r - deltaR;
      r = r + geom_.strawRadialExtentHalf[sc];
      y = sqrt(ys*ys - deltaR*deltaR);
      
      G4TwoVector fixup(r,y);

      fixup.rotate(15.*degree*arcPosition);
      G4Transform3D

      out_transform(G4RotationMatrix( -13*deg -vacg.phi_a*arcPosition, 0, 0),
                    G4ThreeVector(fixup.x(), fixup.y(), 0. ) );
      
      G4VSolid *strawSystem = new G4Box("strawSystem", geom_.strawRadialExtentHalf[sc], 20, geom_.tracebackZHalf-10);
    
      std::string strawLVName = artg4::addNumberToName("StrawChamberLV", sc);

      strawLVName = artg4::addNumberToName(strawLVName,tb);
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

      straws.push_back( new G4PVPlacement(out_transform,
                                            strawLV,
                                            strawLVName.c_str(),
                                            tracebacks[tb],
                                            false,
                                            0, true)
                   );
    
    }

  }
}


//Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Traceback::doBuildLVs() {
  geom_.print();
 
  std::vector<G4LogicalVolume*> tracebacks;
  makeTracebackLVs(tracebacks);
  std::vector<G4VPhysicalVolume*> straws;

  makeStrawDetectors(straws, tracebacks);
  return tracebacks;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Traceback::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
  std::vector<G4VPhysicalVolume*> tracebackPVs;
  tracebackPVs.resize(lvs().size());
  
    
  //loop over the logical volumes
  unsigned int i = 0;
  unsigned int tracebackNum;
  for ( auto aTracebackLV : lvs() ) {
    
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    tracebackNum= geom_.whichTracebackLocations[i];
    std::string tracebackLabel( boost::str( boost::format("TracebackNumber[%02d]") % tracebackNum ));

    
    // beamlike
      
    int arcNumber = floor(tracebackNum/2);
    
    tracebackPVs.push_back(
                           new G4PVPlacement(
                                             new G4RotationMatrix(0.0, 0.0, 0.0),
                                             G4ThreeVector(),
                                             aTracebackLV,
                                             tracebackLabel,
                                             vacs[ arcNumber ],
                                             false,
                                             0, true
                                             )
                         );
    
    i++;
  }
  return tracebackPVs;


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
    std::cout<<"The number of events in the geantHits is: "<<geantHits.size()<<std::endl;
    for ( auto e : geantHits ) {
      std::cout<<"The event number is: "<<i<<std::endl;
      e->Print();
      // Copy this hit into the Art hit
      std::cout<<"The Hit in position.x(): "<<e->position.x()<<std::endl;
      std::cout<<"The position in r: "<<e->position.r()<<std::endl;
      my_r = sqrt(e->position.x()*e->position.x() + e->position.z()*e->position.z());
      myArtHits->emplace_back( e->position.x(),e->position.y(),e->position.z(),e->position.r(),my_r,
                                e->local_position.x(),e->local_position.y(), e->local_position.z(),
                                e->momentum.x(),e->momentum.y(),e->momentum.z(),
                                e->local_momentum.x(),e->local_momentum.y(), e->local_momentum.z(),
                                e->time,
                                e->trackID,
                                e->volumeUID);
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
