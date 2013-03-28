/** @file PhotodetectorHit.cc
 
    Implements the photodetector hit data storage classes.
 
    Ported to Art from g2migtrace file pmtHit.cc
        @author Werner Sun
        @date 2011
 
    @author Robin Bjorkquist
    @date 2013
 */

#include "PhotodetectorHit.hh"

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

// These two have to be included IN THIS ORDER due to a bug in
// G4TouchableHandle.hh in 4.9.2.p01, caught by gcc 4.3.2
#include "Geant4/G4NavigationHistory.hh"
#include "Geant4/G4TouchableHandle.hh"
//

#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"
//#include "rootStorageManager.hh"

#include <string>
#include <sstream>

namespace gm2ringsim {
G4Allocator<PhotodetectorHit> PhotodetectorHitAllocator;
} // namespace gm2ringsim
    
gm2ringsim::PhotodetectorHit::PhotodetectorHit(G4Step *step) :
global_pos(step->GetPreStepPoint()->GetPosition()),
time(step->GetPreStepPoint()->GetGlobalTime()),
turnNum(TurnCounter::getInstance().turns()),
trackID(step->GetTrack()->GetTrackID()),
nphoton( 0 ),
energy( 0. )
{
    G4TouchableHandle const touchy = step->GetPreStepPoint()->GetTouchableHandle();
    local_pos =
    touchy->GetHistory()->GetTopTransform().TransformPoint(global_pos);
    // local coords are: radial is x, vertical is y, thickness is z
    
    // get photodetector number...
    
    /** @bug This is much nastier than it should be. */
    std::string name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    int left = name.find('[');
    std::string num(name, left+1, left+2);
    std::istringstream iss(num);
    iss >> caloNum;
    
    photodetectorNum = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() ;
}


void gm2ringsim::PhotodetectorHit::Draw(){
#ifdef G4VIS_USE
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(!pVVisManager)
        return;
    
    G4Circle circle(global_pos);
    circle.SetScreenSize(5);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.,1.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
#endif
}


void gm2ringsim::PhotodetectorHit::Print(){
    G4cout << " turnNum: " << turnNum
    << " photodetector: " << photodetectorNum
    << " time: " << time
    << '\n';
    G4cout << "\tlpos: " << local_pos << '\n';
}

