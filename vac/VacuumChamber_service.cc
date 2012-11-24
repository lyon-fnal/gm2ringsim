// Implementation of VacuumChamber

#include "gm2ringsim/vac/VacuumChamber_service.hh"

#include "artg4/material/Materials.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4Box.hh"
#include "Geant4/G4Tubs.hh"
#include "Geant4/G4Trap.hh"
#include "Geant4/G4SubtractionSolid.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"

// Constructor for the service 
gm2ringsim::VacuumChamber::VacuumChamber(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "vac"),
                   p.get<std::string>("category", "vac"),
                   p.get<std::string>("mother_category", "arc"))
{}

G4UnionSolid* gm2ringsim::VacuumChamber::buildUnionSolid(const VacGeometry& g, VacGeometry::typeToBuild which, unsigned int arc) {
  
  // This was cut and pasted from vacChamberConstruction
  
  G4Tubs *torus = new G4Tubs("torus",
                             g.torus_rmin,
                             g.torus_rmax[which] + g.inflectorExtension(arc),
                             g.torus_z[which],
                             g.torus_sphi, g.torus_dphi);
  
  G4double
  pPhi = 0., pAlp = 0.,
  pTheta = (g.phi_a - g.phi_b)/2.,
  pDz = g.z[which]/2.,
  pDy = g.torus_z[which],
  pDx12 = g.xL[which]/2.,
  pDx34 = g.xS[which]/2.;
  
  G4Trap *out_scallop =
  new G4Trap("out_scallop", pDz, pTheta, pPhi,
             pDy, pDx12, pDx12, pAlp,
             pDy, pDx34, pDx34, pAlp
             );
  
  G4double
  dz = -g.z[which]/2.,
  dx = -dz*std::tan( (g.phi_b-g.phi_a)/2. ) + g.xS[which]/2.;
  
  // The little rotation is in the coordinates of the trapezoid,
  G4TwoVector fixup(dz,dx);
  fixup.rotate( -g.phi_a );
  // flip to the coordinate system of the arcSection
  fixup.setX(-fixup.x());
  fixup += g.pt_a[which];
  
  G4Transform3D
  out_transform_1(G4RotationMatrix( 0., 90.*degree, -g.phi_a-90.*degree ),
                  G4ThreeVector( fixup.x(), fixup.y(), 0. ) );
  
  fixup.rotate(15.*degree);
  
  G4Transform3D
  out_transform_2(G4RotationMatrix( 0., 90.*degree, -g.phi_a+(-15.-90.)*degree ),
                  G4ThreeVector( fixup.x(), fixup.y(), 0. ) );
  
  pPhi = pAlp = 0.;
  pTheta = g.phi_q/2;
  pDz = g.zz[which]/2.;
  pDy = g.torus_z[which];
  pDx12 = g.xO[which]/2.;
  pDx34 = g.xI[which]/2.;
  
  G4Trap *in_scallop =
  new G4Trap("in_scallop", pDz, pTheta, pPhi,
             pDy, pDx12, pDx12, pAlp,
             pDy, pDx34, pDx34, pAlp
             );
  
  dz = g.zz[which]/2.;
  dx = -g.xI[which]/2.-g.zz[which]/2.*std::tan(g.phi_q/2.);
  fixup = G4TwoVector(dz, dx);
  fixup.rotate( g.phi_q );
  //    fixup += G4TwoVector( 277.*in, 0 );
  fixup+= G4TwoVector(g.pt_p[which].r(),0);
  fixup.rotate( g.pt_p[which].phi() );
  
  G4Transform3D
  in_transform_1(G4RotationMatrix( 0., 90.*degree,
                                  -g.phi_q+(-15.-90.)*degree ),
                 G4ThreeVector( fixup.x(), fixup.y(), 0. ) );
  
  
  fixup.rotate(15.*degree);
  
  G4Transform3D
  in_transform_2(G4RotationMatrix( 0., 90.*degree,
                                  -g.phi_q+(-30.-90.)*degree ),
                 G4ThreeVector( fixup.x(), fixup.y(), 0. ) );
  
  G4UnionSolid *us =
  new G4UnionSolid("union1", torus, out_scallop, out_transform_1);
  us = new G4UnionSolid("union2", us, out_scallop, out_transform_2);
  us = new G4UnionSolid("union3", us, in_scallop, in_transform_1);
  us = new G4UnionSolid("union4", us, in_scallop, in_transform_2);
  
  return us;
}

void gm2ringsim::VacuumChamber::makeWallLVs(
            std::vector<G4LogicalVolume*>& walls, const VacGeometry& g) {
  
  for ( unsigned int arcNum=0; arcNum != 12; ++arcNum) {
    
    G4UnionSolid* us = buildUnionSolid(g, g.wallRegion, arcNum);
    
    G4LogicalVolume* wallLV = new G4LogicalVolume(
                                                  us,
                                                  artg4Materials::Al(),
                                                  "vacChamber",
                                                  0,
                                                  0);
    
    G4VisAttributes *vacWallVisAtt =
    new G4VisAttributes(G4Colour(0.5, 0.5, 0.5,1.0));
    //      vacWallVisAtt -> SetForceSolid(1);
    vacWallVisAtt -> SetForceWireframe(1);
    vacWallVisAtt -> SetVisibility(1);
    wallLV -> SetVisAttributes(vacWallVisAtt);
    
    walls.push_back(wallLV);
    
  }
}

void gm2ringsim::VacuumChamber::makeVacuumPVs(
            std::vector<G4VPhysicalVolume*>& vacs,
            std::vector<G4LogicalVolume*>& walls,
            const VacGeometry& g) {
  
  for ( unsigned int arcNum=0; arcNum != 12; ++arcNum) {
    
    G4UnionSolid* us = buildUnionSolid(g, g.vacuumRegion, arcNum);
    
    G4LogicalVolume* vacLV = new G4LogicalVolume(
                                                 us,
                                                 artg4Materials::Vacuum(),
                                                 "vacChamber",
                                                 0,
                                                 0);
    
    
    G4VisAttributes *vacWallVisAtt =
    new G4VisAttributes(G4Colour(0.5, 0.5, 0.5,1.0));
    vacWallVisAtt -> SetForceWireframe(1);
    vacWallVisAtt -> SetVisibility(1);
    vacLV -> SetVisAttributes(vacWallVisAtt);
    
    std::ostringstream name;
    name << "VacuumChamberRegion[" << std::setfill('0') << std::setw(2)
    << arcNum << ']';
    
    // We can make the physical volumes here
    vacs.push_back( new G4PVPlacement(0,
                                      G4ThreeVector(),
                                      vacLV,
                                      name.str().c_str(),
                                      walls[arcNum],
                                      false,
                                      0)
                   );
  }
}

void gm2ringsim::VacuumChamber::makeTrackerPVs(
          std::vector<G4VPhysicalVolume*>& vacs,
          const VacGeometry& g) {
  
  for(int arc=0; arc!=12; ++arc){
    // In ring, non-physical beam trackers
    LOG_INFO("VAC") << "A " << g.track_rMax << "\n";
    G4Tubs *trackerTubs_S= new G4Tubs("trackerTubs",
                                      g.track_rMin,
                                      g.track_rMax,
                                      g.ZachIsAwesome_Z,
                                      g.tracker_sphi,
                                      g.tracker_dphi);
    G4LogicalVolume *trackerTubs_L =
    new G4LogicalVolume(trackerTubs_S,
                        artg4Materials::Vacuum(),
                        "trackerTubs");
    
    std::ostringstream name;
    name << "TrackerTubs[" << std::setfill('0') << std::setw(2) << arc << ']';
    new G4PVPlacement(new G4RotationMatrix(0,0,0),
                                           G4ThreeVector(0,0,0),
                                           trackerTubs_L,
                                           name.str().c_str(),
                                           vacs[arc] -> GetLogicalVolume(),
                                           false,
                                           0);
    
    G4VisAttributes *tcbVisAtt =
    new G4VisAttributes(G4Colour(1,0,0,1.0));
    tcbVisAtt -> SetForceSolid(1);
    trackerTubs_L -> SetVisAttributes(tcbVisAtt);
    
    // TODO - handle sensitive detectors
    //trackerSD *tracker = SDHandleOwner::getInstance().getTrackerSD();
    //trackerTubs_L->SetSensitiveDetector( tracker );
    
    // In arcNumber 11, put a turnCounter at the inflector aperture
    // position
    if(arc == 11){
      LOG_INFO("VAC") << "B " << g.turn_rMax << "\n";
      G4Tubs *turnCounterTubs_S = new G4Tubs("turnCounterTubs",
                                             g.turn_rMin,
                                             g.turn_rMax,
                                             g.ZachIsAwesome_Z,
                                             g.turn_sphi,
                                             g.turn_dphi);
      
      G4LogicalVolume *turnCounterTubs_L =
      new G4LogicalVolume(turnCounterTubs_S,
                          artg4Materials::Vacuum(),
                          "turnCounterTubs");
      
      new G4PVPlacement(new G4RotationMatrix(0,0,0),
                                            G4ThreeVector(0,0,0),
                                            turnCounterTubs_L,
                                            "turnCounterTubs",
                                            vacs[arc] -> GetLogicalVolume(),
                                            false,
                                            0);
      
      G4VisAttributes *tcbVisAtt = 
      new G4VisAttributes(G4Colour(1,0,0,1.0));
      tcbVisAtt -> SetForceSolid(1);
      turnCounterTubs_L -> SetVisAttributes(tcbVisAtt); 
      
      // TODO - handle sensitive detectors
      //turnCounterSD *tcsd = SDHandleOwner::getInstance().getTurnCounterSD();
      //turnCounterTubs_L->SetSensitiveDetector( tcsd );
    }
  }
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::VacuumChamber::doBuildLVs() {
  
  VacGeometry g(myName());
  g.print();
  
  // Note how this works -
  // The walls live within the arcs
  // The chamber regions live within the walls
  // The various trackers live within the chamber regions
  //
  // So only the walls will be passed back as everything else is internal
  
  // First. make the 12 wall regions
  std::vector<G4LogicalVolume*> walls;
  makeWallLVs(walls, g);
    
  // Make the 12 vacuum regions
  std::vector<G4VPhysicalVolume*> vacs;
  makeVacuumPVs(vacs, walls, g);
  
  // Make the tracker volumes
  LOG_DEBUG("VAC") << "Making tracker\n";
  makeTrackerPVs(vacs, g);
  LOG_DEBUG("VAC") << "Done tracker\n";
  
  return walls;

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::VacuumChamber::doPlaceToPVs( std::vector<G4LogicalVolume*> arcsLVs) {
  
  std::vector<G4VPhysicalVolume*> wallPVs;
  
  for (unsigned int i = 0; i < 12; ++i) {
    std::ostringstream name;
    name << "VacuumChamberWall[" << std::setfill('0') << std::setw(2) << i << ']';
    
    wallPVs.push_back(new G4PVPlacement(0,
                                      G4ThreeVector(),
                                      lvs()[i],
                                      name.str().c_str(),
                                      arcsLVs[i],
                                      false,
                                      0)
                    );

  }
  
  return wallPVs;
}

using gm2ringsim::VacuumChamber;
DEFINE_ART_SERVICE(VacuumChamber)
