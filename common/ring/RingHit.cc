/** @file ringHit.cc

    Provides implementations for the energy loss trackers in the
    physical ring objects.

     Kevin Lynch 2009
*/


#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

#include "Geant4/G4SDManager.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4Transform3D.hh"
#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4TransportationManager.hh" 
#include "Geant4/G4Navigator.hh" 

#include "gm2ringsim/common/ring/RingHit.hh"

#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"
//#include "rootStorageManager.hh"

namespace gm2ringsim {
  G4Allocator<RingHit> RingHitAllocator;
}

gm2ringsim::RingHit::RingHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  turnNum(turnCounter::getInstance().turns()),
  trackID(step->GetTrack()->GetTrackID()),
  //FIXMEvolumeUID(get_uid(step->GetPreStepPoint()->GetPhysicalVolume())),
  volumeUID(0),
  momentum_final(step->GetPostStepPoint()->GetMomentum()),
  deltaE(step->GetPostStepPoint()->GetKineticEnergy() -
	 step->GetPreStepPoint()->GetKineticEnergy())
{

  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
  G4ThreeVector worldPosition = preStepPoint->GetPosition();
  const G4NavigationHistory *history =	theTouchable->GetHistory();
  local_position.set(0, 0, 0);
  local_momentum.set(0, 0, 0);
  
  //FIXME: unused var G4int inflector_depth = history->GetDepth();
  for ( G4int depth = 0; depth < history->GetMaxDepth(); depth++ ) {
    G4VPhysicalVolume *vol = history->GetVolume(depth);
    if ( !vol ) { continue; }
    G4String name = vol->GetName();
    if ( vol->GetName() == "Inflector" ) {
      G4RotationMatrix rotInv = history->GetTransform(depth).NetRotation().inverse();
      local_momentum = local_momentum.transform(rotInv); //.transform(rotM);
      local_position = history->GetTransform(depth).TransformPoint(worldPosition);
    }
  }
}

void gm2ringsim::RingHit::Draw(){
// #ifdef G4VIS_USE
//   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
//   if(!pVVisManager)
//     return;

//   G4Circle circle(position);
//   circle.SetScreenSize(5);
//   circle.SetFillStyle(G4Circle::filled);
//   G4Colour colour(0.,1.,0.);
//   G4VisAttributes attribs(colour);
//   circle.SetVisAttributes(attribs);
//   pVVisManager->Draw(circle);
// #endif
}

void gm2ringsim::RingHit::Print(){
  //FIXME:
  //g2UniqueObjectManager const& uom = 
  // rootStorageManager::getInstance().getUOM();
  //  std::string volume(uom.lookup(volumeUID);
  //G4cout << " turnNum: " << turnNum
  //	 << " volume: " << volume
  //	 << " time: " << time
  //	 << " deltaE: " << deltaE
  //	 << "\n";
  // END FIXME
}


  
  
