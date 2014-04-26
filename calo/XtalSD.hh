#ifndef XtalSD_hh
#define XtalSD_hh

/** @file XtalSD.hh
 
    Provides the declaration of a xtal sensitive detector.
 
    Ported to Art from g2migtrace file xtalSD.hh
    @author Werner Sun
    @date 2011
 
    @author Robin Bjorkquist
    @date 2013 port to art
    @date 2014
 */

#include "XtalHit.hh"
#include "XtalPhotonHit.hh"

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"

#include <unordered_set>

/** A sensitive detector for the xtals. */

namespace gm2ringsim {
    
class XtalSD : public G4VSensitiveDetector{
    
public:
    XtalSD(G4String name);
    ~XtalSD();
    
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    
    G4int PrintLevel() const { return printLevel; };
    G4int PrintLevel(G4int newLevel);
    
    G4int DrawLevel() const { return drawLevel; };
    G4int DrawLevel(G4int newLevel);
    
    bool killOpticalPhotons() const { return killOpticalPhotons_; };
    bool killOpticalPhotons(bool newLevel);
    
    void setTotalXtals(int n) { nXtalsTotal_ = n; };
    void setXtalsPerCalo(int n) { nXtalsPerCalo_ = n; };

private:
    XtalHitsCollection *thisHC;
    XtalPhotonHitsCollection *thisPhotonHC;
    
    G4int printLevel, drawLevel;
    
    bool killOpticalPhotons_;
            
    std::unordered_set<int> photonTracks_;
    
    int nShowerElectrons_;
    
    unsigned int nXtalsTotal_;
    int nXtalsPerCalo_;
    
    std::map< int, std::vector<int> > xtalHitList_ ;
    // key = track ID of calo hit
    // value = vector containing information about which crystals already have
    //         xtal hits associated with that calo hit
    //         vector[xtalCopyID] = -1 means that xtal has no hit yet
    //         vector[xtalCopyID] = n means this (calo hit, xtal) pair is the
    //                              nth entry in the hit collection
};

} // namespace gm2ringsim

#endif // XtalSD_hh

