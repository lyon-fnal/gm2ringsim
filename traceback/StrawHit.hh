#ifndef StrawHit_hh
#define StrawHit_hh

/** @file StrawHit.hh

    Declares the data structures required to store strawscope hit
    information.
    
    @author Kevin Lynch
    @date 2011
*/

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/globals.hh"

/** A G4VHit class to store data from strawscope hits. */
class StrawHit : public G4VHit {
public:
  // local coordinates are, respectively:
  // x() : the radial component, positive outward
  // y() : the thickness (beamlike) component, positive downstream
  // z() : the vertical component, positive up

  G4ThreeVector global_pos;
  G4ThreeVector local_pos;
  G4double energy_dep;
  G4double time;
  G4int turnNum, trackID, station, StrawPlane, StrawStave;

  StrawHit(G4Step*);

  inline void* operator new(size_t);
  inline void operator delete(void*);

  void Draw();
  void Print();
};

typedef G4THitsCollection<StrawHit> StrawHitsCollection;
extern G4Allocator<StrawHit> StrawHitAllocator;
    
inline void* StrawHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) StrawHitAllocator.MallocSingle();
  return aHit;
}

inline void StrawHit::operator delete(void *aHit)
{
  StrawHitAllocator.FreeSingle ((StrawHit*) aHit);
}

#endif // StrawHit_hh