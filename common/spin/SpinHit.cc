/** @file SpinHit.cc

    Implements the spin tracking recorder.

    - Kevin Lynch 2009
*/

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

#include "gm2ringsim/common/spin/SpinHit.hh"
#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"
//#include "g2UniqueObjectManager.rhh"

G4Allocator<SpinHit> spinHitAllocator;

SpinHit::SpinHit(G4Step* step) : 
  momentum(step->GetPreStepPoint()->GetMomentum()),
  spin(step->GetPreStepPoint()->GetPolarization())
{
  // Nothing to do...  
}


void SpinHit::draw(){
  // Do Nothing...
}

void SpinHit::print(){
  G4cout << " spin: " << spin
	 << "\n";
}
