// Implementation of VacuumChamber

#include "gm2ringsim/vac/VacuumChamber_service.hh"

#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4Box.hh"
#include "Geant4/G4Tubs.hh"
#include "Geant4/G4Trap.hh"
#include "Geant4/G4SubtractionSolid.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4SDManager.hh"

// Constructor for the service 
gm2ringsim::VacuumChamber::VacuumChamber(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "vac"),
	       p.get<std::string>("category", "vac"),
	       p.get<std::string>("mother_category", "arc")),
  turnCounterSDName_("TurnCounter"),
  trackerSDName_("Tracker"),
  turnSD_(0),   // will set below
  trackerSD_(0) // will set below
{
  //creates or gets the turnCounterSD depending on whether it exists or not.
  turnSD_ = artg4::getSensitiveDetector<TurnCounterSD>(turnCounterSDName_);
  trackerSD_ = artg4::getSensitiveDetector<TrackerSD>(trackerSDName_);
}

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
    
    std::string wallName = artg4::addNumberToName("VacuumChamberWallLV", arcNum);

    G4LogicalVolume* wallLV = new G4LogicalVolume(
                                                  us,
                                                  artg4Materials::Al(),
                                                  wallName.c_str(),
                                                  0,
                                                  0);
    
    // Set the attributes
    artg4::setVisAtts( wallLV, g.displayWall, g.wallColor,
                      [] (G4VisAttributes* att) {
                              att->SetForceWireframe(1);
                              att->SetVisibility(1);
                      }
    );

    walls.push_back(wallLV);
    
  }
}

void gm2ringsim::VacuumChamber::makeVacuumPVs(
            std::vector<G4VPhysicalVolume*>& vacs,
            std::vector<G4LogicalVolume*>& walls,
            const VacGeometry& g) {
  
  for ( unsigned int arcNum=0; arcNum != 12; ++arcNum) {
    
    G4UnionSolid* us = buildUnionSolid(g, g.vacuumRegion, arcNum);
    
    std::string lvName = artg4::addNumberToName("VacuumChamberLV", arcNum);
    
    G4LogicalVolume* vacLV = new G4LogicalVolume(
                                                 us,
                                                 artg4Materials::Vacuum(),
                                                 lvName.c_str(),
                                                 0,
                                                 0);
    

    // Set the attributes
    artg4::setVisAtts( vacLV, g.displayVac, g.vacColor,
                      [] (G4VisAttributes* att) {
                        att->SetForceWireframe(1);
                        att->SetVisibility(1);
                      }
    );

    
    std::string pvName = artg4::addNumberToName("VacuumChamberPV", arcNum);
    
    // We can make the physical volumes here
    vacs.push_back( new G4PVPlacement(0,
                                      G4ThreeVector(),
                                      vacLV,
                                      pvName.c_str(),
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
    G4Tubs *trackerTubs_S= new G4Tubs("trackerTubs",
                                      g.track_rMin,
                                      g.track_rMax,
                                      g.ZachIsAwesome_Z,
                                      g.tracker_sphi,
                                      g.tracker_dphi);

    std::string ttLVName = artg4::addNumberToName("TrackerLV", arc);
    
    G4LogicalVolume *trackerTubs_L =
    new G4LogicalVolume(trackerTubs_S,
                        artg4Materials::Vacuum(),
                        ttLVName.c_str());
    
    std::string ttPVName = artg4::addNumberToName("TrackerPV", arc);

    new G4PVPlacement(new G4RotationMatrix(0,0,0),
                                           G4ThreeVector(0,0,0),
                                           trackerTubs_L,
                                           ttPVName.c_str(),
                                           vacs[arc] -> GetLogicalVolume(),
                                           false,
                                           0);

    // Set the attributes
    artg4::setVisAtts( trackerTubs_L, g.displayTracker, g.trackerColor,
                      [] (G4VisAttributes* att) {
                        att->SetForceSolid(1);
                      }
    );
    
    // TODO - handle sensitive detectors
    //trackerSD *tracker = SDHandleOwner::getInstance().getTrackerSD();
    // Brendan: Now the trackerSD_ is grabbed from the SDManager in the constructor
    // using a function in util.hh. We could also rever to the SDHandleOwner if preferred
    // simply by copying the SDHandleOwner class over
    trackerTubs_L->SetSensitiveDetector( trackerSD_ );
    
    // In arcNumber 11, put a turnCounter at the inflector aperture
    // position
    if(arc == 11){
      G4Tubs *turnCounterTubs_S = new G4Tubs("turnCounterTubs",
                                             g.turn_rMin,
                                             g.turn_rMax,
                                             g.ZachIsAwesome_Z,
                                             g.turn_sphi,
                                             g.turn_dphi);
      
      G4LogicalVolume *turnCounterTubs_L =
      new G4LogicalVolume(turnCounterTubs_S,
                          artg4Materials::Vacuum(),
                          "turnCounterLV");
      
      new G4PVPlacement(new G4RotationMatrix(0,0,0),
                                            G4ThreeVector(0,0,0),
                                            turnCounterTubs_L,
                                            "turnCounterPV",
                                            vacs[arc] -> GetLogicalVolume(),
                                            false,
                                            0);
      
      // Set the attributes
      artg4::setVisAtts( turnCounterTubs_L, g.displayTurnCounter, g.turnCounterColor,
                        [] (G4VisAttributes* att) {
                          att->SetForceSolid(1);
                        }
      );
      
      // Unnecessary in ART since the turnCounters are managed by SDManager
      //turnCounterSD *tcsd = SDHandleOwner::getInstance().getTurnCounterSD();
      turnCounterTubs_L->SetSensitiveDetector( turnSD_ );
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
  makeTrackerPVs(vacs, g);
  
  return walls;

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::VacuumChamber::doPlaceToPVs( std::vector<G4LogicalVolume*> arcsLVs) {
  
  std::vector<G4VPhysicalVolume*> wallPVs;
  
  for (unsigned int i = 0; i < 12; ++i) {
    
    std::string wallName = artg4::addNumberToName("VacuumChamberWallPV", i);
    
    wallPVs.push_back(new G4PVPlacement(0,
                                      G4ThreeVector(),
                                      lvs()[i],
                                      wallName.c_str(),
                                      arcsLVs[i],
                                      false,
                                      0)
                    );

  }
  
  return wallPVs;
}

using gm2ringsim::VacuumChamber;
DEFINE_ART_SERVICE(VacuumChamber)
