#ifndef G2MIGTRACE_INFLECTORHIT_HH
#define G2MIGTRACE_INFLECTORHIT_HH

/** @file inflectorHit.hh

    Declares the inflectorHit class for beam particle tracking inside
    the inflector aperture.

    @author Kevin Lynch
    @date 2009
*/

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Step.hh"


namespace gm2ringsim {
  /** A G4VHit derived class to store particle tracking data inside the
      inflector aperture as collected by the inflectorSD. */
  class inflectorHit : public G4VHit {
    
  public:
    
    /** The position of the tracked particle on the inflector tracker. */
    G4ThreeVector position, local_position;
    /** The momentum of the tracked particle on the inflector tracker. */
    G4ThreeVector momentum, local_momentum; 
    /** Global time of the particle tracking hit. */
    G4double time;
    /** Global track ID for the tracked particle. */
    G4int trackID;
    /** Global volume ID for the hit inflector tracking volume. */
    G4int volumeUID;
    
    inflectorHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
  }; // class inflectorHit
  

  typedef G4THitsCollection<inflectorHit> inflectorHitsCollection;
  extern G4Allocator<inflectorHit> inflectorHitAllocator;
  
} //namespace gm2ringsim
inline void* gm2ringsim::inflectorHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) inflectorHitAllocator.MallocSingle();
  return aHit;
}

inline void gm2ringsim::inflectorHit::operator delete(void *aHit)
{
  inflectorHitAllocator.FreeSingle ((inflectorHit*) aHit);
}


#endif // G2MIGTRACE_INFLECTORHIT_HH
