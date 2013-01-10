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
  strawSDName_("straw"),
  strawSD_(0)
{
  strawSD_ = artg4::getSensitiveDetector<StrawSD>(strawSDName_);
}



G4LogicalVolume* gm2ringsim::Traceback::makeATracebackLV(int tracebackNum) {
  
  const VacGeometry vacg("vac");

  G4Tubs *torus = new G4Tubs("torus",
                             vacg.torus_rmin,
                             vacg.torus_rmax[vacg.vacuumRegion],
                             vacg.torus_z[vacg.vacuumRegion],
                             vacg.torus_sphi, vacg.torus_dphi);
  
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
  G4SubtractionSolid *scallop = new G4SubtractionSolid("scallp", torus_scallop, torus);

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
  //std::vector<G4VPhysicalVolume*> straws;

  //makeStrawDetectors(straws, tracebacks);
  return tracebacks;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Traceback::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  
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
                                                 0);
                                                 //geom_.thetaIn[ arcPosition ] + geom_.windowAngle - geom_.vRotation+ 90*degree);
                                                 //0);
    pos = G4ThreeVector(0,0,0);
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
