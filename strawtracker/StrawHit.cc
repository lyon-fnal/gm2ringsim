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
#include "gm2dataproducts/strawtracker/WireID.hh"
using gm2strawtracker::WireID;

#include <string>
#include <sstream>
#include <iostream>
using std::ostringstream;
using std::istringstream;
using std::string;


//FIXME: do i need this?#include "g2UniqueObjectManager.rhh"
namespace gm2ringsim {
    G4Allocator<StrawHit> StrawHitAllocator;
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

  // Get the WireID from the logical volume's name!
  istringstream thing(name);
  string parseString; 
  // First remove the "SingleStrawPV - " from the front.
  std::getline(thing, parseString, '-');

  // Next get the rest of the line (parses to the next \n)
  std::getline(thing, parseString);

  // And finally, make a WireID from that string
  WireID wire = gm2strawtracker::wireIDfromString(parseString);

  strawInRow = wire.getWire();
  layerNumber = wire.getLayer();
  viewNumber = wire.getView();
  stationNumber = wire.getStation();
  // No longer using the overall straw number
  strawNumber = -1;
  trackerNumber = wire.getTrackerNumber();
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
  scallop_position.set(station_position.x() + 
          g.strawStationCenterFromEdge[wire.getStation()],
          station_position.y() + g.strawStationLocation[wire.getStation()],
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
