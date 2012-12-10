/** @file inflectorHit.cc

    Implements the member functions of the inflectorHit class.

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

#include "gm2ringsim/inflector/InflectorHit.hh"
//FIXME: do i need this?#include "g2UniqueObjectManager.rhh"

G4Allocator<inflectorHit> inflectorHitAllocator;

inflectorHit::inflectorHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  trackID(step->GetTrack()->GetTrackID()) //,
  //FIXME  volumeUID(get_uid(step->GetPreStepPoint()->GetPhysicalVolume()))
{
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
  G4ThreeVector worldPosition = preStepPoint->GetPosition();

  const G4NavigationHistory *history =  theTouchable->GetHistory();

  //const G4AffineTransform top = history->GetTopTransform();
  //G4ThreeVector localPosition = top.TransformPoint(worldPosition);

  //local_position = history->GetTopTransform().TransformPoint(worldPosition);
  //local_momentum = history->GetTopTransform().TransformPoint(momentum);
  //G4ThreeVector localPosition = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
  
  //G4ThreeVector localPosition = theTouchable->GetHistory()->
  //GetTopTransform().TransformPoint(worldPosition);

  G4ThreeVector myPoint = position;
  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* myVolume = theNavigator->LocateGlobalPointAndSetup(myPoint);
  //FIXME: printing this since it is needed to compile
  G4cout << "Volume Name = " << myVolume->GetName() << G4endl;

  //local_position = worldPosition;
  local_momentum = momentum;

  G4int start = history->GetDepth();
  G4int depth = start;
  G4VPhysicalVolume *vol = history->GetVolume(depth);
  if ( 1 ) {
    if ( vol ) {
      G4RotationMatrix rotInv = history->GetTransform(depth).NetRotation().inverse();
      local_momentum = local_momentum.transform(rotInv); //.transform(rotM);
      local_position = history->GetTransform(depth).TransformPoint(worldPosition);
      //G4cout << "Volume Name = " << vol->GetName() << "\t";
      //PrintV(local_momentum);
    }
  }

  /*
  G4cout << "Pre: "
	 << step->GetPreStepPoint()->GetPhysicalVolume()->GetName() 
	 << '\n';
  G4cout << "Post: "
	 << step->GetPostStepPoint()->GetPhysicalVolume()->GetName() 
	 << '\n';
  */
}


void inflectorHit::Draw(){
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

void inflectorHit::Print(){
  G4cout << " volumeUID: " << volumeUID
	 << " time: " << time
	 << " position: " << position
	 << "\n";
}
