#ifndef G2MIGTRACE_TRACKERHIT_HH
#define G2MIGTRACE_TRACKERHIT_HH

/** @file trackerHit.hh

    Provides support for storing in-ring beam tracking data.
    
    - Kevin Lynch 2009
*/

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Step.hh"

namespace gm2ringsim{
  /** A G4VHit class for storing in-ring beam tracking data. */
  class TrackerHit : public G4VHit {
    
  public:
    G4ThreeVector position, momentum; // get from prestep pt, which
    // must be in volume (right?) 
    G4double time;
    G4int turnNum; // obtained from turn counter
    G4int trackID;
    G4int volumeUID; // get from prestep
    
    TrackerHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
  };
  
  typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;
  
  extern G4Allocator<TrackerHit> TrackerHitAllocator;
} //namespace gm2ringsim

inline void* gm2ringsim::TrackerHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) TrackerHitAllocator.MallocSingle();
  return aHit;
}

inline void gm2ringsim::TrackerHit::operator delete(void *aHit)
{
  TrackerHitAllocator.FreeSingle ((TrackerHit*) aHit);
}


#endif // G2MIGTRACE_TRACKERHIT_HH
