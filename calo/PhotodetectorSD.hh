#ifndef PhotodetectorSD_hh
#define PhotodetectorSD_hh

/** @file PhotodetectorSD.hh
 
    Provides the declaration of a photodetector sensitive detector.
 
    Ported to Art from g2migtrace file pmtSD.hh
        @author Werner Sun
        @date 2011
 
    @author Robin Bjorkquist
    @date 2013
*/

#include "PhotodetectorHit.hh"
#include "PhotodetectorPhotonHit.hh"

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"

/** A sensitive detector for the photodetectors.
 
 Currently, this class creates a Photodetector Photon Hit for each optical photon that enters a photodetector volume. The optical photons tracks are "stopped and killed," but other particle types are allowed to pass through.
 
 Before 10/24/2013, all tracks were killed by the photodetector SD.

 */

namespace gm2ringsim {

class PhotodetectorSD : public G4VSensitiveDetector{
    
public:
    PhotodetectorSD(G4String name);
    ~PhotodetectorSD();
    
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    
    G4int PrintLevel() const { return printLevel; };
    G4int PrintLevel(G4int newLevel);
    
    G4int DrawLevel() const { return drawLevel; };
    G4int DrawLevel(G4int newLevel);
    
    void setTotalPhotodetectors(int n) { nPhotodetectorsTotal_ = n; };
    void setPhotodetectorsPerCalo(int n) { nPhotodetectorsPerCalo_ = n; };
    
    
private:
    PhotodetectorHitsCollection *thisHC;
    PhotodetectorPhotonHitsCollection *thisPhotonHC;
    
    G4int printLevel, drawLevel;
    
    unsigned int nPhotodetectorsTotal_;
    int nPhotodetectorsPerCalo_;
    
    std::vector<int> photodetectorID_ ;
};

} // namespace gm2ringsim

#endif // PhotodetectorSD_hh
