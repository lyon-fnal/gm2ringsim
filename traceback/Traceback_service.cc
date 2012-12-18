// Implementation of Traceback


#include "gm2ringsim/traceback/Traceback_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "gm2ringsim/traceback/TracebackGeometry.hh"

#include "boost/format.hpp"

#include "Geant4/G4Box.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4UnionSolid.hh"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Traceback::Traceback(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
                   p.get<std::string>("name", "traceback"),
                   p.get<std::string>("category", "traceback"),
                   p.get<std::string>("mother_category", "vac")),
  geom_(myName()),
  strawSDName_("straw"),
  strawSD_(0)
{
  strawSD_ = artg4::getSensitiveDetector<StrawSD>(strawSDName_);
}
G4LogicalVolume* gm2ringsim::Traceback::makeATracebackLV() {
  
  G4double moveTheta;
  G4double moveR;

  G4VSolid *solid = new G4Box("traceback_S1", geom_.tracebackRadialHalf[0], geom_.tracebackThetaHalf, geom_.tracebackZHalf);

  for(int i = 1; i!= 44; ++i){
       
    G4VSolid *s = new G4Box("traceback_this", geom_.tracebackRadialHalf[i], geom_.tracebackThetaHalf, geom_.tracebackZHalf);
    
    moveTheta = geom_.tracebackTheta + geom_.tracebackTheta*(i-1);
    
    moveR = (geom_.tracebackRadialHalf[0]-geom_.tracebackRadialHalf[i])+ moveTheta*geom_.tanTracebackRadialShiftAngle;
    
    G4ThreeVector moveThis (-moveR, -moveTheta, 0.0);
    G4UnionSolid *solidThis = new G4UnionSolid("traceback_S",solid, s, 0, moveThis);
    solid = solidThis;
    
  }

  G4LogicalVolume *tracebackLV = new G4LogicalVolume(solid,
                                                     artg4Materials::Vacuum(),
                                                     "tracebackLV");
   
  artg4::setVisAtts(tracebackLV, geom_.displayTraceback, geom_.tracebackColor);
  return tracebackLV;

}
// Build the logical volumes
void gm2ringsim::Traceback::makeTracebackLVs(std::vector<G4LogicalVolume*>& tracebacks) {
  
  // Create the vector of logical volumes
  //std::vector<G4LogicalVolume*> tracebackLVs;
  
  // Build the logical volumes
  for ( unsigned int tracebackNumber = 0; tracebackNumber != geom_.whichTracebackLocations.size(); ++tracebackNumber ) {
    // Push this into the vector
    tracebacks.push_back( makeATracebackLV());
  }
}

void gm2ringsim::Traceback::makeStrawDetectors(std::vector<G4VPhysicalVolume*>& straws,std::vector<G4LogicalVolume*>& tracebacks){
  for (unsigned int tb = 0; tb<geom_.whichTracebackLocations.size() ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawLocation.size(); sc++){
    
      G4double moveTheta=0.0;
      G4double moveR=0.0;
    
      G4VSolid *strawSystem = new G4Box("strawSystem", geom_.tracebackRadialHalf[geom_.strawLocation[sc]]-10, geom_.tracebackThetaHalf-10, geom_.tracebackZHalf-10);
    
      std::string strawLVName = artg4::addNumberToName("StrawChamberLV", sc);

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

      std::string pvName = artg4::addNumberToName("StrawChamberPV", sc);
      if(geom_.strawLocation[sc] == 0){
        moveTheta = 0.0;
        moveR=0;
      }
      
      else{
        moveTheta = geom_.tracebackTheta*(geom_.strawLocation[sc]);

        moveR =  geom_.tracebackRadialHalf[0]-geom_.tracebackRadialHalf[geom_.strawLocation[sc]]
                + moveTheta*geom_.tanTracebackRadialShiftAngle;
      
      }
      
      std::cout<<"geom_.strawLocation[sc]: "<<geom_.strawLocation[sc]<<std::endl;
      std::cout<<"moveTheta: "<<moveTheta<<std::endl;
      
      G4ThreeVector position (-moveR, -moveTheta, 0.0);
      std::cout<<"Straw Chamber: "<<sc<<std::endl;
      std::cout<<"Position: "<<position <<std::endl;
      // We can make the physical volumes here
      straws.push_back( new G4PVPlacement(0,
                                            position,
                                            strawLV,
                                            pvName.c_str(),
                                            tracebacks[tb],
                                            false,
                                            0)
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
  //TracebackGeometry tg(myName());
  
  std::vector<G4VPhysicalVolume*> tracebackPVs;
  tracebackPVs.resize(lvs().size());
  
  G4double const rHalf = geom_.r/2.;
  G4double const tHalf = geom_.t/2.;
  
  //loop over the logical volumes
  unsigned int i = 0;
  unsigned int tracebackNum;
  for ( auto aTracebackLV : lvs() ) {
    
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    tracebackNum= geom_.whichTracebackLocations[i];
    std::string tracebackLabel( boost::str( boost::format("TracebackNumber[%02d]") % tracebackNum ));
    int arcPosition = tracebackNum % 2;
    
    G4ThreeVector windowEdge(geom_.rOut*std::cos(geom_.thetaOut[ arcPosition ]),
                              geom_.rOut*std::sin(geom_.thetaOut[ arcPosition ]),
                              0.);

    G4ThreeVector unitAlong(std::cos(geom_.thetaIn[ arcPosition ] + geom_.windowAngle),
                             std::sin(geom_.thetaIn[ arcPosition ] + geom_.windowAngle),
                             0.);
    
    G4ThreeVector unitVertical(0,0,-1.); // OK ... yes, up :-(
    
    G4ThreeVector unitNormal = unitAlong.cross(unitVertical); // OK
    
    G4double const vrots = std::sin( std::abs(geom_.vRotation) );
    G4double const vrotc = std::cos(geom_.vRotation);
    G4double const correctionAlong = (vrotc-1.)*rHalf + vrots*tHalf;
    G4double const correctionNormal = vrots*rHalf + (vrotc-1.)*tHalf;
    // beamlike
    
    // for the real g-2 station
    G4ThreeVector pos =
    // center the station on the center of the outer window edge
    windowEdge
    // move it in by the radial half width
    - rHalf * unitAlong
    // move it downstream by the thickness half width
    + tHalf * unitNormal
    // We have to move the station around so that it doesn't "whack"
    // (technical term, that) into the outside of the vacuum walls
    // when it's rotated
    /** @bug doesn't include the r_rotation and t_rotation corrections
     ... should probably get this by multiplying appropriate
     rotation matrices, but this will take some thought... */
    - correctionAlong * unitAlong
    + correctionNormal * unitNormal
    // finally, let's apply the user defined offsets
    +geom_.rOffset * unitAlong
    +geom_.tOffset * unitNormal
    +geom_.vOffset * unitVertical;
    
    G4RotationMatrix *rot = new G4RotationMatrix(0,
                                                 0,
                                                 geom_.thetaIn[ arcPosition ] + geom_.windowAngle - geom_.vRotation);
    
    int arcNumber = floor(tracebackNum/2);

    tracebackPVs.push_back(
                           new G4PVPlacement(
                                             rot,
                                             pos,
                                             aTracebackLV,
                                             tracebackLabel,
                                             vacs[ arcNumber ]->GetDaughter(0)->GetLogicalVolume(),
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
//void gm2ringsim::Traceback::doCallArtProduces(art::EDProducer * producer) {

//}

// Actually add the data to the event
//void gm2ringsim::Traceback::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
//}

using gm2ringsim::Traceback;
DEFINE_ART_SERVICE(Traceback)
