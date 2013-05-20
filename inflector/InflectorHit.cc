/** @file InflectorHit.cc

    Implements the member functions of the InflectorHit class.

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
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"

//FIXME: do i need this?#include "g2UniqueObjectManager.rhh"
namespace gm2ringsim {
  G4Allocator<InflectorHit> InflectorHitAllocator;
}
gm2ringsim::InflectorHit::InflectorHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  trackID(step->GetTrack()->GetTrackID()) //,
{

  art::ServiceHandle<artg4::PhysicalVolumeStoreService> pvs;
  volumeUID = pvs->idGivenPhysicalVolume( step->GetPreStepPoint()->GetPhysicalVolume() );

  local_position = position;
  local_momentum = momentum;
}


void gm2ringsim::InflectorHit::Draw(){
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

void gm2ringsim::InflectorHit::Print()
{
  G4cout << " InflectorHit::Print() --- volumeUID: " << volumeUID
	 << " time: " << time
	 << " position: " << position
	 << "\n";
}
