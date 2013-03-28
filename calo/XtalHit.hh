#ifndef XtalHit_hh
#define XtalHit_hh

/** @file XtalHit.hh
 
    Declares the data structures required to store xtal hit
    information.
 
    Ported to Art from g2migtrace file xtalHit.hh
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
/** A G4VHit class to store data from xtal hits. */
class XtalHit : public G4VHit {
public:
    // local coordinates are, respectively:
    // x() : radial component, positive outward
    // y() : vertical component, positive up
    // z() : thickness component, positive downstream
    
    // Position and momentum are for charged track at entrance to xtal.
    G4ThreeVector global_pos, global_mom; // get from prestep pt, which
    // is in the xtal volume
    // (isn't it?), and get in the
    // coordinate system of the
    // xtal
    G4ThreeVector local_pos, local_mom;
    G4double time;
    G4int turnNum; // obtained from turn counter
    G4int trackID;
    G4int parentID; // id of e+/e- that originates shower; can be same as trackID
    G4int caloNum;
    G4int xtalNum;
    G4double energy ;      // total energy of charged track
    G4double energyDep ;   // total energy deposited by charged track
    G4double trackLength ; // total path length of charged track
    G4int pdgID ;
    G4int nphoton ;
    G4double ephoton ;
    
    XtalHit(G4Step*, G4int trackInitiatingShower);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
};

typedef G4THitsCollection<XtalHit> XtalHitsCollection;
extern G4Allocator<XtalHit> XtalHitAllocator;
} // namespace gm2ringsim

inline void* gm2ringsim::XtalHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) XtalHitAllocator.MallocSingle();
    return aHit;
}

inline void gm2ringsim::XtalHit::operator delete(void *aHit)
{
    XtalHitAllocator.FreeSingle ((XtalHit*) aHit);
}

#endif // XtalHit_hh
