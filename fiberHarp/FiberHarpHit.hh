#ifndef FiberHarpHit_hh
#define FiberHarpHit_hh

/** @file FiberHarpHit.hh

    Declares the data structures required to store fiber harp hit
    information.

    Derived from work of Kevin Lynch, 2011, Fred Gray, 2012
    
    @author Peter Winter
    @date 2012
*/

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/globals.hh"

/** A G4VHit class to store data from fiber harp hits. */
class FiberHarpHit : public G4VHit {
public:

  G4ThreeVector global_pos;
  G4double energy_dep;
  G4double time;
  G4int turnNum, trackID, harp, fiber;

  FiberHarpHit(G4Step*);

  inline void* operator new(size_t);
  inline void operator delete(void*);

  void Draw();
  void Print();
};

typedef G4THitsCollection<FiberHarpHit> FiberHarpHitsCollection;
extern G4Allocator<FiberHarpHit> FiberHarpHitAllocator;
    
inline void* FiberHarpHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) FiberHarpHitAllocator.MallocSingle();
  return aHit;
}

inline void FiberHarpHit::operator delete(void *aHit)
{
  FiberHarpHitAllocator.FreeSingle ((FiberHarpHit*) aHit);
}

#endif // FiberHarpHit_hh
