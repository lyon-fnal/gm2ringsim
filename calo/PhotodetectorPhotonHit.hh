#ifndef PhotodetectorPhotonHit_hh
#define PhotodetectorPhotonHit_hh

/** @file PhotodetectorPhotonHit.hh
 
    Declares the data structures required to store photodetector photon hit
    information.
 
    Ported to Art from g2migtrace file pmtPhotonHit.cc
        @author Werner Sun
        @date 2011
 
    @author Robin Bjorkquist
    @date 2013
*/

#include "Geant4/G4VHit.hh"
#include "Geant4/G4THitsCollection.hh"
#include "Geant4/G4Allocator.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/globals.hh"

namespace gm2ringsim{
/** A G4VHit class to store data from PhotodetectorPhoton hits. */
class PhotodetectorPhotonHit : public G4VHit {
public:
    // local coordinates are, respectively:
    // x() : radial-ish component, positive outward
    // y() : thickness component, positive downstream
    // z() : vertical component, positive up
    
    G4ThreeVector global_pos, global_mom; // get from prestep pt, which
    // is in the photodetector volume
    // (isn't it?), and get in the
    // coordinate system of the
    // photodetector
    G4ThreeVector local_pos, local_mom;
    G4double time;
    G4int turnNum; // obtained from turn counter
    G4int trackID;
    G4int caloNum;
    G4int photodetectorNum;
    G4double energy ;
    G4bool accepted ; // does photon pass efficiency simulation?
    
    PhotodetectorPhotonHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
};

typedef G4THitsCollection<PhotodetectorPhotonHit> PhotodetectorPhotonHitsCollection;
extern G4Allocator<PhotodetectorPhotonHit> PhotodetectorPhotonHitAllocator;
} // namespace gm2ringsim

inline void* gm2ringsim::PhotodetectorPhotonHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) PhotodetectorPhotonHitAllocator.MallocSingle();
    return aHit;
}

inline void gm2ringsim::PhotodetectorPhotonHit::operator delete(void *aHit)
{
    PhotodetectorPhotonHitAllocator.FreeSingle ((PhotodetectorPhotonHit*) aHit);
}

#endif // PhotodetectorPhotonHit_hh
