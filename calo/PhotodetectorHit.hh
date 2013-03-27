#ifndef PhotodetectorHit_hh
#define PhotodetectorHit_hh

/** @file PhotodetectorHit.hh
 
    Declares the data structures required to store photodetector hit
    information.
 
    Ported to Art from g2migtrace file pmtHit.hh
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
/** A G4VHit class to store data from photodetector hits. */
class PhotodetectorHit : public G4VHit {
public:
    // local coordinates are, respectively:
    // x() : radial component, positive outward
    // y() : vertical component, positive up
    // z() : thickness component, positive downstream
    
    // Position is for center of face of photodetector window
    G4ThreeVector global_pos;             // get from prestep pt, which
    // is in the photodetector volume
    // (isn't it?), and get in the
    // coordinate system of the
    // photodetector
    G4ThreeVector local_pos ;
    G4double time;
    G4int turnNum; // obtained from turn counter
    G4int trackID;
    G4int caloNum;
    G4int photodetectorNum;
    G4int nphoton; // with photon detection efficiency applied
    G4double energy ;  // sum over detected photons
    
    PhotodetectorHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
};

typedef G4THitsCollection<PhotodetectorHit> PhotodetectorHitsCollection;
extern G4Allocator<PhotodetectorHit> PhotodetectorHitAllocator;
} // namespace gm2ringsim

inline void* gm2ringsim::PhotodetectorHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) PhotodetectorHitAllocator.MallocSingle();
    return aHit;
}

inline void gm2ringsim::PhotodetectorHit::operator delete(void *aHit)
{
    PhotodetectorHitAllocator.FreeSingle ((PhotodetectorHit*) aHit);
}

#endif // PhotodetectorHit_hh
