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
#include "gm2geom/strawtracker/StrawTrackerGeometry.hh"

#include "gm2ringsim/strawtracker/StrawHit.hh"
//FIXME: do i need this?#include "g2UniqueObjectManager.rhh"
namespace gm2ringsim {
    G4Allocator<StrawHit> StrawHitAllocator;
}

G4int gm2ringsim::StrawHit::extractValueFromName(std::string indicator, std::string name){
  
  G4int value;
  
  unsigned first = name.find(indicator);
  first = first+indicator.length();
  std::string str = name.substr(first);
  unsigned second = str.find("-");
  std::string num(name, first, second);
  
  std::istringstream iss(num);
  iss >> value;
  
  return value;
  
  
}

gm2ringsim::StrawHit::StrawHit(G4Step* step) :
    global_position(step->GetPreStepPoint()->GetPosition()),
    momentum(step->GetPreStepPoint()->GetMomentum()),
    time(step->GetPreStepPoint()->GetGlobalTime()),
    trackID(step->GetTrack()->GetTrackID()) //,
    //FIXME  volumeUID(get_uid(step->GetPreStepPoint()->GetPhysicalVolume()))
{
  // The name is HARP[m].fiber[n] or HARP[m].support[n]
  std::string name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  
  // get straw number
  G4StepPoint* preStepPoint  = step->GetPreStepPoint();

  strawInRow = extractValueFromName("strawInRow", preStepPoint->GetPhysicalVolume()->GetName());
  layerNumber = extractValueFromName("layer", preStepPoint->GetPhysicalVolume()->GetName());
  viewNumber = extractValueFromName("view", preStepPoint->GetPhysicalVolume()->GetName());
  stationNumber = extractValueFromName("stationNumber", preStepPoint->GetPhysicalVolume()->GetName());
  strawNumber = extractValueFromName("strawNumber", preStepPoint->GetPhysicalVolume()->GetName());
  
  particle_name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  parent_ID = step->GetTrack()->GetParentID();
    
  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
  
  //position in world coordinates
  G4ThreeVector worldPosition = preStepPoint->GetPosition();
  const G4NavigationHistory *history =  theTouchable->GetHistory();
  //momentum in world coordinates

  G4ThreeVector world_momentum = preStepPoint->GetMomentum();
  
  G4int depth = history->GetDepth();
  G4VPhysicalVolume *straw_vol = history->GetVolume(depth);
  if ( straw_vol ) {
    G4RotationMatrix rotInv = history->GetTransform(depth).NetRotation().inverse();
    //momentum in detector coordinates
    local_momentum = world_momentum.transform(rotInv);
    //position in detector coordinates
    local_position = history->GetTransform(depth).TransformPoint(worldPosition);
  }
  
  G4VPhysicalVolume *station_vol = history->GetVolume(depth-1);
  
  if (station_vol){
    G4RotationMatrix rotInv = history->GetTransform(depth-1).NetRotation().inverse();
    //position in detector coordinates
    station_position = history->GetTransform(depth-1).TransformPoint(worldPosition);

  }
  gm2geom::StrawTrackerGeometry g;
  scallop_position.set(station_position.x() + g.strawStationCenterFromEdge[stationNumber],
                         station_position.y() + g.strawStationLocation[stationNumber],
                         station_position.z());
  
}


void gm2ringsim::StrawHit::Draw(){
#ifdef G4VIS_USE
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(!pVVisManager)
        return;
    
    G4Circle circle(global_position);
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
  << " \n\t global position: " << global_position
  << " \n\t local position: " << local_position
  << " \n\t station position: "<< station_position
  << " \n\t particle: " << particle_name
  << " \n\t parentID: " << parent_ID
  <<"\n\n\t strawInRow: " << strawInRow
  <<" layerNumber: " <<layerNumber
  <<" viewNumber: "<<viewNumber
  <<" stationNumber: " <<stationNumber
  <<" strawNumber: " << strawNumber

  << "\n";
}
