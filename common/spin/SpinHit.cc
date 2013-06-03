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

namespace gm2ringsim{
  G4Allocator<SpinHit> spinHitAllocator;
}

gm2ringsim::SpinHit::SpinHit(G4Step* step) : 
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  spin(step->GetPreStepPoint()->GetPolarization())
{
  // Nothing to do...  
}


void gm2ringsim::SpinHit::draw(){
  // Do Nothing...
}

void gm2ringsim::SpinHit::print(){
  G4cout << " SpinHit::Print() --- momentum: " << momentum
	 << " position: " << position
	 << " spin: " << spin
	 << "\n";
}
