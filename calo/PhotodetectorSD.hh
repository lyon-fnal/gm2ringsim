#ifndef PhotodetectorSD_hh
#define PhotodetectorSD_hh

/** @file PhotodetectorSD.hh
 
    Provides the declaration of a "stops and kills" photodetector
    sensitive detector.
 
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
 
 Currently, this class will "swallow" any particle that enters a photodetector module, and output various information via a PhotodetectorHit. The particle track is then "stopped and killed".
 
 @bug This is long-term unacceptable behavior, but suffices for
 now.  Kevin has side documentation on the various behaviors that
 should be implemented in future.
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
    
    void setPhotodetectorNum(int n) { nPhotodetectors_ = n; };
    
    
private:
    PhotodetectorHitsCollection *thisHC;
    PhotodetectorPhotonHitsCollection *thisPhotonHC;
    
    G4int printLevel, drawLevel;
    
    int nPhotodetectors_;
    
    std::vector<int> photodetectorID_ ;
};

} // namespace gm2ringsim

#endif // PhotodetectorSD_hh
