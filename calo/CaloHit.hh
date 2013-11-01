#ifndef CaloHit_hh
#define CaloHit_hh

/** @file CaloHit.hh
 
    Declares the data structures required to store calorimeter hit
    information.
 
    Ported to Art from g2migtrace file caloHit.hh
        @author Kevn Lynch
        @date 2009
 
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
/** A G4VHit class to store data from calorimeter hits. */
class CaloHit : public G4VHit {
public:
    G4ThreeVector global_pos, global_mom; // get from prestep pt
    G4double time;
    G4int turnNum; // obtained from turn counter
    G4int trackID;
    G4int caloNum;
		//Added for straw analysis
    G4double energy;        
    G4String particle_name;
    G4int parent_ID;
		//--------------

    CaloHit(G4Step*);
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
        
    void Draw();
    void Print();
        
};
    
typedef G4THitsCollection<CaloHit> CaloHitsCollection;
extern G4Allocator<CaloHit> CaloHitAllocator;
} // namespace gm2ringsim
    
inline void* gm2ringsim::CaloHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) CaloHitAllocator.MallocSingle();
    return aHit;
}

inline void gm2ringsim::CaloHit::operator delete(void *aHit)
{
    CaloHitAllocator.FreeSingle ((CaloHit*) aHit);
}
    

#endif // CaloHit_hh
