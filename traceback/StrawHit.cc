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
  // The name is HARP[m].fiber[n] or HARP[m].support[n]
  std::string name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  
  // get traceback number
  std::string::size_type left_tb = name.find_first_of('[');
  std::string::size_type right_tb = name.find_first_of(']');
  std::string num_tb(name, left_tb+1, right_tb-1);
  std::istringstream iss_tb(num_tb);
  iss_tb >> traceback;
  
  // get straw chamber number
  std::string::size_type left_sc = name.find_last_of('[');
  std::string::size_type right_sc = name.find_last_of(']');
  std::string num_sc(name, left_sc+1, right_sc-1);
  std::istringstream iss_sc(num_sc);
  iss_sc >> straw;

  particle_name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  parent_ID = step->GetTrack()->GetParentID();
  
  G4StepPoint* preStepPoint  = step->GetPreStepPoint();
  //G4StepPoint* postStepPoint = step->GetPostStepPoint();
  
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
  << " \n\t particle: " << particle_name
  << " \n\t parentID: " << parent_ID
  << "\n";
}
