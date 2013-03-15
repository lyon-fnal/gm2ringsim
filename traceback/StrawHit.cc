/** @file StrawHit.cc
 
 Implements the member functions of the StrawHit class.
 
 @author Kevin Lynch
 @date 2009
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

#include "gm2ringsim/traceback/StrawHit.hh"
//FIXME: do i need this?#include "g2UniqueObjectManager.rhh"
namespace gm2ringsim {
    G4Allocator<StrawHit> StrawHitAllocator;
}
gm2ringsim::StrawHit::StrawHit(G4Step* step) :
    position(step->GetPreStepPoint()->GetPosition()),
    momentum(step->GetPreStepPoint()->GetMomentum()),
    time(step->GetPreStepPoint()->GetGlobalTime()),
    trackID(step->GetTrack()->GetTrackID()) //,
    //FIXME  volumeUID(get_uid(step->GetPreStepPoint()->GetPhysicalVolume()))
{
  
  
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
  //position in world coordinates

  G4ThreeVector worldPosition = preStepPoint->GetPosition();
  const G4NavigationHistory *history =  theTouchable->GetHistory();
  //momentum in world coordinates

  G4ThreeVector world_momentum = preStepPoint->GetMomentum();
  
  G4int depth = history->GetDepth();
  G4VPhysicalVolume *vol = history->GetVolume(depth);
  if ( vol ) {
    G4RotationMatrix rotInv = history->GetTransform(depth).NetRotation().inverse();
    //momentum in detector coordinates
    local_momentum = world_momentum.transform(rotInv);
    //position in detector coordinates
    local_position = history->GetTransform(depth).TransformPoint(worldPosition);
  }
  
}


void gm2ringsim::StrawHit::Draw(){
#ifdef G4VIS_USE
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(!pVVisManager)
        return;
    
    G4Circle circle(position);
    circle.SetScreenSize(10);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.,1.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
#endif
}

//#include "rootStorageManager.hh"

void gm2ringsim::StrawHit::Print(){
  G4cout << "StrawHit::Print()" <<
  " \n\tvolumeUID: " << volumeUID
  << " \n\t time: " << time
  << " \n\t position: " << position
  << "\n";
}
