// Implementation of VacuumChamber

#include "gm2ringsim/vac/VacuumChamber_service.hh"
#include "gm2geom/strawtracker/StrawTrackerGeometry.hh"

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

#include "CLHEP/Vector/TwoVector.h"
using CLHEP::Hep2Vector;


// VacGeometry now in gm2geom, let's not add the namespace specifier every time
// we use it.
using gm2geom::VacGeometry;

// Constructor for the service 
gm2ringsim::VacuumChamber::VacuumChamber(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "vac"),
	       p.get<std::string>("category", "vac"),
	       p.get<std::string>("mother_category", "arc")),
  turnCounterSDName_("TurnCounter"),
  virtualringstationSDName_("VirtualRingStationSD"),
  turnSD_(0),   // will set below
  virtualringstationSD_(0), // will set below
  wallLVs_(),
  g(myName())
{
  //creates or gets the turnCounterSD depending on whether it exists or not.
  turnSD_ = artg4::getSensitiveDetector<TurnCounterSD>(turnCounterSDName_);
  virtualringstationSD_ = artg4::getSensitiveDetector<VirtualRingStationSD>(virtualringstationSDName_);
}

void gm2ringsim::VacuumChamber::getXYCoordinatesForPlacement(double distAlongScallop, double distShift, double &x, double&y){

    //move long the scallop line
    x = x - distAlongScallop*sin(g.phi_a);
    y = distAlongScallop*cos(g.phi_a);

    //move inward to be in correct position
    x = x - distShift*cos(g.phi_a);
    y = y - distShift*sin(g.phi_a);

}     

G4UnionSolid* gm2ringsim::VacuumChamber::buildUnionSolid(VacGeometry::typeToBuild which, unsigned int arc) {
  
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
  Hep2Vector fixup(dz,dx);
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
  fixup = Hep2Vector(dz, dx);
  fixup.rotate( g.phi_q );
  //    fixup += Hep2Vector( 277.*in, 0 );
  fixup+= Hep2Vector(g.pt_p[which].r(),0);
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
  
  //Add in extension to vacuum chamber for sections with 
  //tracker systems in place. 
  

  bool firstpos = false;
  bool secondpos = false;
   
  FindScallopPos(arc, firstpos, secondpos); 

  if(firstpos || secondpos){
  
    G4double
    x = g.trackerExtPlacementX,
    y = 0,
    distShift = 0,
    distAlongScallop = g.distCenterExtAlongScallop;

    if(which == g.wallRegion){
      distShift = g.trackerExtBuildW[which];
      getXYCoordinatesForPlacement(distAlongScallop,distShift,x,y);
    }

    if (which == g.vacuumRegion){
      distShift = g.trackerExtBuildW[g.wallRegion] - (g.trackerExtBuildW[g.vacuumRegion] - g.trackerExtBuildW[g.wallRegion]) -g.trackerExtWallThick;
      getXYCoordinatesForPlacement(distAlongScallop,distShift,x,y);
    }

    G4Box *scallop_extension = new G4Box("scallop_extension",g.trackerExtBuildW[which],g.trackerExtBuildL[which],g.trackerExtBuildH[which]);

    Hep2Vector extensionPlacement(x,y);

    if(firstpos){
      G4Transform3D out_transform_scallop_ext_1(G4RotationMatrix( 0., 0, -g.phi_a ),
                                            G4ThreeVector( extensionPlacement.x(),extensionPlacement.y(), 0. ) );
      us = new G4UnionSolid("union1_5",us, scallop_extension,out_transform_scallop_ext_1);
    }
    if(secondpos){
      extensionPlacement.rotate(15.*deg);
      G4Transform3D out_transform_scallop_ext_2(G4RotationMatrix( 0., 0, -g.phi_a-15*degree),
          G4ThreeVector( extensionPlacement.x(),extensionPlacement.y(), 0. ) );
      us = new G4UnionSolid("union2_5",us, scallop_extension,out_transform_scallop_ext_2);
    } 
  }  //end add in scallop extension for tracker 
  return us;

}
void gm2ringsim::VacuumChamber::FindScallopPos(int arcNum, bool &firstpos, bool &secondpos){
  
    gm2geom::StrawTrackerGeometry strawgeom; 

    std::vector<int>::iterator it; 
    int scallopNumber = arcNum*2;

    it = std::find (strawgeom.whichScallopLocations.begin(), strawgeom.whichScallopLocations.end(), scallopNumber);
    if (it != strawgeom.whichScallopLocations.end()) firstpos = true;

    scallopNumber = arcNum*2 + 1;
    it = std::find (strawgeom.whichScallopLocations.begin(), strawgeom.whichScallopLocations.end(), scallopNumber);
    if (it != strawgeom.whichScallopLocations.end()) secondpos = true;
}

void gm2ringsim::VacuumChamber::makeWallLVs(const VacGeometry& g) {
 
  for ( unsigned int arcNum=0; arcNum != 12; ++arcNum) {

    G4UnionSolid* us = buildUnionSolid(g.wallRegion, arcNum);
    
    std::string wallName = artg4::addNumberToName("VacuumChamberWallLV", arcNum);

    G4LogicalVolume* wallLV = new G4LogicalVolume(
                                                  us,
                                                  artg4Materials::Al(),
                                                  wallName.c_str(),
                                                  0,
                                                  0);
    
    // Set the attributes (using a C++11 lambda function)
    artg4::setVisAtts( wallLV, g.displayWall, g.wallColor,
                      [] (G4VisAttributes* att) {
                              att->SetForceWireframe(1);
                              att->SetVisibility(1);
                      }
    );

    wallLVs_.push_back(wallLV);
    
  }
}

void gm2ringsim::VacuumChamber::makeVacuumLVs(
            std::vector<G4LogicalVolume*>& vacLVs,
            const VacGeometry& g ) {
  
  for ( unsigned int arcNum=0; arcNum != 12; ++arcNum) {
    
    G4UnionSolid* us = buildUnionSolid(g.vacuumRegion, arcNum);
    
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
    
    vacLVs.push_back(vacLV);
  }

}

void gm2ringsim::VacuumChamber::makeVacuumPVs(
                    std::vector<G4LogicalVolume*>& vacLVs) {
  for ( unsigned int arcNum=0; arcNum != 12; ++arcNum) {
        
    std::string pvName = artg4::addNumberToName("VacuumChamberPV", arcNum);
    
    // We can make the physical volumes here (we don't need to keep them)
    new G4PVPlacement(0,
                      G4ThreeVector(),
                      vacLVs[arcNum],
                      pvName.c_str(),
                      wallLVs_[arcNum],
                      false,
                      0);
  }
}

void gm2ringsim::VacuumChamber::makeVirtualRingStationPVs(
							  std::vector<G4LogicalVolume*>& vacLVs,
							  const VacGeometry& g)
{
  int freq = g.Frequency;

  
  for(int arc=0; arc!=12; ++arc){
    
    if ( arc != 11 ) {
      if ( freq == 12 ) { ; } // Build all
      else if ( freq == 6 ) { // Build every other one
	if ( arc == 11 ) {;}
	else if ( arc == 9 ) {;}
	else if ( arc == 7 ) {;}
	else if ( arc == 5 ) {;}
	else if ( arc == 3 ) {;}
	else if ( arc == 1 ) {;}
	else { continue; }
      }
      else if ( freq == 4 ) { // Build every third one
	if ( arc == 11 ) {;}
	else if ( arc == 8 ) {;}
	else if ( arc == 5 ) {;}
	else if ( arc == 2 ) {;}
	else { continue; }
      }
      else if ( freq == 3 ) { // Build every quarter one
	if ( arc == 11 ) {;}
	else if ( arc == 7 ) {;}
	else if ( arc == 3 ) {;}
	else { continue; }
      }
      else if ( freq == 2 ) { // Build every 6th one
	if ( arc == 11 ) {;}
	else if ( arc == 8 ) {;}
	else { continue; }
      }
      else if ( freq == 1 ) { // Build every 6th one
	if ( arc == 11 ) {;}
	else { continue; }
      }
    }


    // In ring, non-physical beam ring stations
    G4Tubs *virtualringstationTubs_S= new G4Tubs("virtualringstationTubs",
                                      g.track_rMin,
                                      g.track_rMax,
                                      g.ZachIsAwesome_Z,
                                      g.tracker_sphi,
                                      g.tracker_dphi);

    std::string ttLVName = artg4::addNumberToName("VirtualRingStationLV", arc);
    
    G4LogicalVolume *virtualringstationTubs_L =
    new G4LogicalVolume(virtualringstationTubs_S,
                        artg4Materials::Vacuum(),
                        ttLVName.c_str());
    
    std::string ttPVName = artg4::addNumberToName("VirtualRingStationPV", arc);

    G4cout << "Building " << ttPVName << G4endl;

    new G4PVPlacement(new G4RotationMatrix(0,0,0),
                                           G4ThreeVector(0,0,0),
                                           virtualringstationTubs_L,
                                           ttPVName.c_str(),
                                           vacLVs[arc],
                                           false,
                                           0);

    // Set the attributes
    artg4::setVisAtts( virtualringstationTubs_L, g.displayTracker, g.trackerColor,
                      [] (G4VisAttributes* att) {
                        att->SetForceSolid(1);
                      }
    );
    
    // TODO - handle sensitive detectors
    //virtualringstationSD *tracker = SDHandleOwner::getInstance().getVirtualRingStationSD();
    // Brendan: Now the virtualringstationSD_ is grabbed from the SDManager in the constructor
    // using a function in util.hh. We could also rever to the SDHandleOwner if preferred
    // simply by copying the SDHandleOwner class over
    virtualringstationTubs_L->SetSensitiveDetector( virtualringstationSD_ );
    
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
                                            vacLVs[arc],
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
  
  //VacGeometry g(myName());
  g.print();
  
  // Note how this works -
  // The walls live within the arcs
  // The chamber regions live within the walls
  // The various virtualringstations live within the chamber regions
  //
  // We actually want Art to only know about the chamber regions, since
  // that's what everything else is going to live in. So that's what we're going
  // to pass back. We still need to hold onto the walls, because that's what
  // we're actually going to place within the mother volume. 
  
  // First. make the 12 wall regions (the wallLVs are member data).
  // The wallLVs go into member data
  makeWallLVs(g);
  
  // Make the 12 vacuum LVs (we're going to store these)
  std::vector<G4LogicalVolume*> vacLVs;
  makeVacuumLVs(vacLVs, g);
  
  // Make the 12 vacuum PVs (we in fact don't need to store the vacPVs at this point)
  makeVacuumPVs(vacLVs);
  
  // Make the Virtual Ring Station volumes
  makeVirtualRingStationPVs(vacLVs, g);
  
  return vacLVs;
}

// Build the physical volumes.

// Note that this is a little tricky. The top level objects are the vacuum walls, but in fact what we really
// want to store are the vacuum chambers (the vacuum between the walls) because that's where everything else
// is going to go. So we're going to place the wall, but fill the vector with the daughter, the chamber.
std::vector<G4VPhysicalVolume *> gm2ringsim::VacuumChamber::doPlaceToPVs( std::vector<G4LogicalVolume*> arcsLVs) {
  
  std::vector<G4VPhysicalVolume*> chamberPVs;
  
  for (unsigned int i = 0; i < 12; ++i) {
    
    std::string wallName = artg4::addNumberToName("VacuumChamberWallPV", i);
    
    new G4PVPlacement(0,
                                      G4ThreeVector(),
                                      wallLVs_[i],
                                      wallName.c_str(),
                                      arcsLVs[i],
                                      false,
                                      0);
    
    // We actually want ArtG4 to keep track of the chamber (the daughter of the wall)
    chamberPVs.push_back( wallLVs_[i]->GetDaughter(0) );
    
    // Let's just check this for fun - comment this out if things work (should go into a test)
    #if 1
    if ( wallLVs_[i]->GetNoDaughters() != 1 ) cet::exception("VACUUM_CHAMBER") << "Bad number of daughters" << "\n";
    
    // Get the daughter
    G4VPhysicalVolume* daughter = wallLVs_[i]->GetDaughter(0);
    mf::LogInfo("Vacuum_chamber") << " Daughter is " << daughter->GetName() << "\n";
    mf::LogInfo("Vacuum_chamber") << " chamberPV is " << chamberPVs[i]->GetName() << "\n";
    #endif
  }
  
  return chamberPVs;
}


using gm2ringsim::VacuumChamber;
DEFINE_ART_SERVICE(VacuumChamber)
