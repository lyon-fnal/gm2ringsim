// Implementation of FiberHarp
#include "gm2ringsim/fiberHarp/FiberHarp_service.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh" 

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4Box.hh"
#include "Geant4/G4Para.hh"
#include "Geant4/G4VSolid.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4RunManager.hh"

// There are two types of harps: X sensing and Y sensing.  We will also
// implement a broken Y-sensing harp, and the absence of a harp.
enum e_fiberHarp_type {HARP_OFF, HARP_X, HARP_Y, HARP_BROKEN_Y};

// Constructor for the service 
gm2ringsim::FiberHarp::FiberHarp(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "fiberHarp"),
	       p.get<std::string>("category", "fiberHarp"),
	       p.get<std::string>("mother_category", "vac")),
  geom_(myName())
{
  // Let's prepare the sensitive detector, no registration with G4SDManager necessary as 
  // this is done in FiberHarpSD constructor
  harpSD_ = new FiberHarpSD("fiberHarpSD");
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::FiberHarp::doBuildLVs() {
  std::vector<G4LogicalVolume *> fiberLVs;

  for(G4int harpNumber = 0; harpNumber < geom_.nHarps; harpNumber++){ 
    fiberLVs.push_back(BuildFiberHarp(harpNumber));
  }

  return fiberLVs;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::FiberHarp::doPlaceToPVs(std::vector<G4LogicalVolume*> vacWalls){
  
  std::vector<G4VPhysicalVolume *> fiberPVs;
  
  for(int harpNumber = 0; harpNumber < geom_.nHarps; harpNumber++){ 
    if(geom_.harpType[harpNumber] != HARP_OFF){
      // The wall should have one daughter (the vacuum chamber itself)
      if (vacWalls[geom_.vacWallPos[harpNumber]]->GetNoDaughters() != 1 ) {
	throw cet::exception("FIBERHARP") << "Whaaa?? My wall has no vacuum! Abort! \n";
      }
      
      // Get the vacuum chamber logical volume from the wall
      G4LogicalVolume* vacLV = vacWalls[geom_.vacWallPos[harpNumber]]->GetDaughter(0)->GetLogicalVolume();
      
      fiberPVs.push_back(new G4PVPlacement(new G4RotationMatrix(0, 0, geom_.azimuthalPos[harpNumber]),
					   G4ThreeVector(R_magic()*cos(geom_.azimuthalPos[harpNumber]),
							 R_magic()*sin(geom_.azimuthalPos[harpNumber]),
							 0*m),
					   lvs()[harpNumber],
					   lvs()[harpNumber]->GetName(),
					   vacLV,
					   false,
					   harpNumber));
    }
  }

  return fiberPVs;
}



// CHANGE_ME: You can delete the below if this detector creates no data

//// Declare to Art what we are producing
//void gm2ringsim::FiberHarp::doCallArtProduces(art::EDProducer * producer) {
//
//}
//
//// Actually add the data to the event
//void gm2ringsim::FiberHarp::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}


G4LogicalVolume* gm2ringsim::FiberHarp::BuildFiberHarp(G4int harpNumber){
  printf("%d HAAAAAAAAAAAAAAAAAAALLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n", 
	 harpNumber);

  if(geom_.harpType[harpNumber] == HARP_OFF){
    return 0;
  }

  char objectName[20];
  sprintf(objectName, "HARP[%d]", harpNumber);
  
  // Make an enclosing volume for the harp
  G4double dim = geom_.supportLength + 2*geom_.supportWidth;
  G4Box *enclosingBox = new G4Box(objectName, dim, geom_.supportDepth, dim);
  G4LogicalVolume *harp  = new G4LogicalVolume(enclosingBox, 
					       artg4Materials::Vacuum(), 
					       objectName);
  
  // Add support bars
  for(G4int supportNumber = 0; supportNumber < 2; supportNumber++){
    G4String name = objectName;
    char s[20];
    sprintf(s, ".support[%d]", supportNumber);
    name += s;
    
    G4VSolid *box;
    G4ThreeVector placement;
    G4double supportPos = (geom_.supportWidth + geom_.fiberLength)/2;
    if(supportNumber == 0)
      supportPos = -supportPos;
    
    if(geom_.harpType[harpNumber] == HARP_X){
      box = new G4Box(name, geom_.supportLength/2, geom_.supportDepth/2, geom_.supportWidth/2);
      placement = G4ThreeVector(0, 0, supportPos);
    } 
    else if(geom_.harpType[harpNumber] == HARP_Y){
      box = new G4Box(name, geom_.supportWidth/2, geom_.supportDepth/2, geom_.supportLength/2);
      placement = G4ThreeVector(supportPos, 0, 0);
    }
    else if(geom_.harpType[harpNumber] == HARP_BROKEN_Y){
      if(supportNumber == 1) {
	box = new G4Box(name, geom_.supportWidth/2, geom_.supportDepth/2, geom_.supportLength/2);
	placement = G4ThreeVector(supportPos, 0, 0);
      } else {
	box = new G4Para(name, geom_.supportWidth/2, geom_.supportDepth/2, geom_.supportLength/2,
			 0*deg, -30*deg, 0*deg);
	placement = G4ThreeVector(supportPos + geom_.supportLength/4, 0, 0);
      } 
    }

    G4LogicalVolume *boxLogical = new G4LogicalVolume(box,  
						      artg4Materials::Al(),
						      name);
    boxLogical->SetSensitiveDetector(harpSD_);

    new G4PVPlacement(0, 
		      placement,
		      boxLogical,
		      name, 
		      harp,
		      false,
		      0);
  }
  
  // Loop over fibers
  for(G4int fiberNumber = 0; fiberNumber < geom_.nFibers; fiberNumber++){
    G4String name = objectName;
    char s[20];
    sprintf(s, ".fiber[%d]", fiberNumber);
    name += s;
    
    G4Tubs *fiber = new G4Tubs(name,
			       0, 
			       geom_.fiberRadius,
			       geom_.fiberLength/2,
			       0,
			       360*deg); 
    
    G4LogicalVolume *fiberLogical = 
      new G4LogicalVolume(fiber,
			  artg4Materials::BCF10ScintFiber(),
			  "fiber");
    
    fiberLogical->SetSensitiveDetector(harpSD_);
    
    G4double fiberPos = (fiberNumber-geom_.nFibers/2) * geom_.fiberSpacing;
    
    if(geom_.harpType[harpNumber] == HARP_X){ 
      new G4PVPlacement(0,
			G4ThreeVector(fiberPos, 0, 0),
			fiberLogical,
			name,
			harp,
			false,
			0);
    } 
    else{ 
      new G4PVPlacement(new G4RotationMatrix(pi/2., pi/2., 0),
			G4ThreeVector(0, 0, fiberPos),
			fiberLogical,
			name,
			harp,
			false,
			0);
    } 
  }

  // Set the visualization attributes
  artg4::setVisAtts(harp, geom_.display, geom_.fiberHarpColor);

  return harp;
}  


using gm2ringsim::FiberHarp;
DEFINE_ART_SERVICE(FiberHarp)
