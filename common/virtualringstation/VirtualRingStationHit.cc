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
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"
#include "gm2ringsim/common/UsefulVariables.hh"

namespace gm2ringsim {
G4Allocator<VirtualRingStationHit> VirtualRingStationHitAllocator;
} //namespace gm2ringsim
gm2ringsim::VirtualRingStationHit::VirtualRingStationHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  polarization(step->GetPreStepPoint()->GetPolarization()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  turnNum(TurnCounter::getInstance().turns()),
  PDGID((int)step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()),
  trackID(step->GetTrack()->GetTrackID())
{
  art::ServiceHandle<artg4::PhysicalVolumeStoreService> pvs;
  volumeUID = pvs->idGivenPhysicalVolume( step->GetPreStepPoint()->GetPhysicalVolume() );
  //  G4cout << "VolID: " << volumeUID << "\t" << pvs->stringGivenID(volumeUID) << G4endl;
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
  if ( trackID == 1 || 1 ) {
    //-------------------
    // Position variables
  //-------------------
    G4double rhat = ComputeRhat(&position);
    G4double vhat = ComputeVhat(&position);
    G4double theta = ComputeTheta(&position);
    //-------------------
    // Momentum variables
    //-------------------
    G4double prhat = ComputePrhat(&position, &momentum);
    G4double pvhat = ComputePvhat(&position, &momentum);
    G4cout << " VirtualRingStationHit::Print() --- ";
    G4cout << "  Particle: " << PDGID
	   << "  turnNum: " << turnNum
	   << "  theta: " << theta
	   << "  rhat: " << rhat
	   << "  vhat: " << vhat
	   << "  prhat: " << prhat
	   << "  pvhat: " << pvhat
	   << "\n";
  }
}
