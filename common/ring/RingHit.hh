#ifndef G2MIGTRACE_RINGHIT_HH
#define G2MIGTRACE_RINGHIT_HH

/** @file RingHit.hh

    Provides the declarations for energy loss trackers in the physical
    ring objects.

    @author Kevin Lynch 
    @date 2009
    @ARTized based on previous ringHit.hh
    @author Brendan Kiburg
    @date Dec 2012
*/

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/globals.hh"

  
  /** Provides a G4VHit class to record energy loss and scattering data
      on physical objects in the ring.
      
      All information is stored straight away in global coordinates.
      Any conversions to local coordinates are provided in the
      conversion function in namespace converters in file
      rootStorageManager.cc 
  */
  class RingHit : public G4VHit {
    
  public:
    G4ThreeVector position, momentum; // get from prestep pt, which
    // must be in volume (right?) 
    
    G4ThreeVector local_position, local_momentum;
    
    
    G4double time;
    G4int turnNum; // obtained from turn counter
    G4int trackID;
    G4int volumeUID; // get from prestep
    
    G4ThreeVector momentum_final; // poststep
    G4double deltaE; 
    
    RingHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
  }; // end of class definition
  
typedef G4THitsCollection<RingHit> RingHitsCollection;

  //FIXME: What's the point of these functions ??

   //FIXME Whys is this extern
  extern G4Allocator<RingHit> RingHitAllocator;
  
   inline void* RingHit::operator new(size_t)
   {
     void *aHit;
     aHit = (void *) RingHitAllocator.MallocSingle();
     return aHit;
   }
  
   inline void RingHit::operator delete(void *aHit)
   {
     RingHitAllocator.FreeSingle ((RingHit*) aHit);
   }
  




#endif // G2MIGTRACE_RINGHIT_HH
