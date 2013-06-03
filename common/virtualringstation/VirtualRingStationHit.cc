/** @file virtualringstationHit.cc

    Implements the storage support for in-ring beam tracking data.

    - Kevin Lynch 2009
*/

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

#include "gm2ringsim/common/virtualringstation/VirtualRingStationHit.hh"
#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "gm2ringsim/actions/muonStorageStatus/MuonStorageStatusAction_service.hh"

namespace gm2ringsim {
G4Allocator<VirtualRingStationHit> VirtualRingStationHitAllocator;
} //namespace gm2ringsim
gm2ringsim::VirtualRingStationHit::VirtualRingStationHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  polarization(step->GetPreStepPoint()->GetPolarization()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  turnNum(TurnCounter::getInstance().turns()),
  trackID(step->GetTrack()->GetTrackID()),
  volumeUID(0)
{
  ;
  //Print();
}


void gm2ringsim::VirtualRingStationHit::Draw(){
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

void gm2ringsim::VirtualRingStationHit::Print(){
  G4cout.precision(3);
  if ( trackID == 1 ) {
    G4cout << " VirtualRingStationHit::Print() --- turnNum: " << turnNum
	   << " time: " << time
	   << " polarization: " << polarization
	   << " position: " << position
	   << "\n";
  }
}
