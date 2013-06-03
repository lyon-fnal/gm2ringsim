#ifndef G2MIGTRACE_VIRTUALRINGSTATIONHIT_HH
#define G2MIGTRACE_VIRTUALRINGSTATIONHIT_HH

/** @file virtualringstationHit.hh

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
  class VirtualRingStationHit : public G4VHit {
    
  public:
    G4ThreeVector position, momentum, polarization; // get from prestep pt, which
    // must be in volume (right?) 
    G4double time;
    G4int turnNum; // obtained from turn counter
    G4int trackID;
    G4int volumeUID; // get from prestep
    
    VirtualRingStationHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
  };
  
  typedef G4THitsCollection<VirtualRingStationHit> VirtualRingStationHitsCollection;
  
  extern G4Allocator<VirtualRingStationHit> VirtualRingStationHitAllocator;
} //namespace gm2ringsim

inline void* gm2ringsim::VirtualRingStationHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) VirtualRingStationHitAllocator.MallocSingle();
  return aHit;
}

inline void gm2ringsim::VirtualRingStationHit::operator delete(void *aHit)
{
  VirtualRingStationHitAllocator.FreeSingle ((VirtualRingStationHit*) aHit);
}


#endif // G2MIGTRACE_VIRTUALRINGSTATIONHIT_HH
