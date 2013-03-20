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

namespace gm2ringsim {
  /** A G4VHit class to store data from strawscope hits. */
  class StrawHit : public G4VHit {
  public:
    
    G4ThreeVector position, local_position;
    G4ThreeVector momentum, local_momentum;
    G4double time;
    G4int trackID;
    G4int volumeUID;
    G4int traceback, straw;

    StrawHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void operator delete(void*);
    
    void Draw();
    void Print();
  }; // class StrawHit


  typedef G4THitsCollection<StrawHit> StrawHitsCollection;
  extern G4Allocator<StrawHit> StrawHitAllocator;

}//namespace gm2ringsim
    
inline void* gm2ringsim::StrawHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) StrawHitAllocator.MallocSingle();
  return aHit;
}

inline void gm2ringsim::StrawHit::operator delete(void *aHit)
{
  StrawHitAllocator.FreeSingle ((StrawHit*) aHit);
}

#endif // StrawHit_hh
