#ifndef XtalPhotonHit_hh
#define XtalPhotonHit_hh

/** @file XtalPhotonHit.hh
 
    Declares the data structures required to store xtal photon hit
    information.
 
    Ported to Art from g2migtrace file xtalPhotonHit.hh
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
/** A G4VHit class to store data from xtalPhoton hits. */
class XtalPhotonHit : public G4VHit {
public:
    // local coordinates are, respectively:
    // x() : radial component, positive outward
    // y() : vertical component, positive up
    // z() : thickness component, positive downstream
    
    G4ThreeVector global_pos;
    G4ThreeVector local_pos;
    G4double cosTheta; // angle relative longitudinal crystal axis
    G4double phi;   // angle about longitudinal crystal axis
    G4int trackID;
    G4int caloNum;
    G4int xtalNum;
    G4double energy ;
    bool transmitted;  // this cerenkov photon made it through intermediate layers to photodetector volume
    bool detected;     // this cerenkov photon was transmitted and subsequently produced a photoelectron
    
    XtalPhotonHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
};

typedef G4THitsCollection<XtalPhotonHit> XtalPhotonHitsCollection;
extern G4Allocator<XtalPhotonHit> XtalPhotonHitAllocator;
} // namespace gm2ringsim

    
inline void* gm2ringsim::XtalPhotonHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) XtalPhotonHitAllocator.MallocSingle();
    return aHit;
}

inline void gm2ringsim::XtalPhotonHit::operator delete(void *aHit)
{
    XtalPhotonHitAllocator.FreeSingle ((XtalPhotonHit*) aHit);
}

#endif // XtalPhotonHit_hh
