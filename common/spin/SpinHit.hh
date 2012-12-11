#ifndef G2MIGTRACE_SPINHIT_HH
#define G2MIGTRACE_SPINHIT_HH

/** @file spinHit.hh

    Provides declarations for recording particle spin data during
    tracking. 

    - Kevin Lynch 2009
*/

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Step.hh"

/** A G4VHit for storing a spin and momentum three vector during
    tracking. */

//FIXME: Need to move spinHit stuff into gm2ringsim. However
//       I had trouble doing this with the inline functions below

class SpinHit : public G4VHit {

public:
  G4ThreeVector momentum, spin;

  SpinHit(G4Step*);

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void draw();
  void print();

};

typedef G4THitsCollection<SpinHit> SpinHitsCollection;
extern G4Allocator<SpinHit> spinHitAllocator;

inline void* SpinHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) spinHitAllocator.MallocSingle();
  return aHit;
}

inline void SpinHit::operator delete(void *aHit)
{
  spinHitAllocator.FreeSingle ((SpinHit*) aHit);
}


#endif // G2MIGTRACE_SPINHIT_HH
