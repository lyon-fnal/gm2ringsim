/** @file trackerHit.cc

    Implements the storage support for in-ring beam tracking data.

    - Kevin Lynch 2009
*/

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

#include "gm2ringsim/tracker/TrackerHit.hh"
#include "turnCounter.hh"
#include "g2UniqueObjectManager.rhh"

G4Allocator<trackerHit> trackerHitAllocator;

trackerHit::trackerHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  turnNum(turnCounter::getInstance().turns()),
  trackID(step->GetTrack()->GetTrackID()),
  volumeUID(get_uid(step->GetPreStepPoint()->GetPhysicalVolume()))
{
  /*
  G4cout << "Pre: "
	 << step->GetPreStepPoint()->GetPhysicalVolume()->GetName() 
	 << '\n';
  G4cout << "Post: "
	 << step->GetPostStepPoint()->GetPhysicalVolume()->GetName() 
	 << '\n';
  */
}


void trackerHit::Draw(){
  //FIXME: Implement in fhicl
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

void trackerHit::Print(){
  G4cout << " turnNum: " << turnNum
	 << " volumeUID: " << volumeUID
	 << " time: " << time
	 << " position: " << position
	 << "\n";
}
