/** @file trackerHit.cc

    Implements the storage support for in-ring beam tracking data.

    - Kevin Lynch 2009
*/

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

#include "gm2ringsim/tracker/TrackerHit.hh"
#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "gm2ringsim/actions/muonStorageStatus/MuonStorageStatusAction_service.hh"


G4Allocator<TrackerHit> TrackerHitAllocator;

TrackerHit::TrackerHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  turnNum(turnCounter::getInstance().turns()),
  //turnNum(0), // FIXME: If i initialize with zero, update below
  trackID(step->GetTrack()->GetTrackID()),
  //FIXME:   volumeUID(get_uid(step->GetPreStepPoint()->GetPhysicalVolume()))
  volumeUID(0) //FIXME
{

  //art::ServiceHandle <artg4::MuonStorageStatusAction> mss;
  


  // FIXME: ART
  // Need to grab the TurnCounter from the muonStorageStatus service to
  // determine what turn we are on
  
  
  /*
  G4cout << "Pre: "
	 << step->GetPreStepPoint()->GetPhysicalVolume()->GetName() 
	 << '\n';
  G4cout << "Post: "
	 << step->GetPostStepPoint()->GetPhysicalVolume()->GetName() 
	 << '\n';
  */
}


void TrackerHit::Draw(){
  //FIXME: Implement in fhicl
  /*#ifdef G4VIS_USE
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
  */
}

void TrackerHit::Print(){
  G4cout << " turnNum: " << turnNum
	 << " volumeUID: " << volumeUID
	 << " time: " << time
	 << " position: " << position
	 << "\n";
}
