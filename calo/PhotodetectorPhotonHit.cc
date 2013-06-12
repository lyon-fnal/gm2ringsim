/** @file PhotodetectorPhotonHit.cc
 
    Implements the photodetector photon hit data storage classes.
 
    Ported to Art from g2migtrace file pmtPhotonHit.cc
        @author Werner Sun
        @date 2011
 
    @author Robin Bjorkquist
    @date 2013
*/

#include "PhotodetectorPhotonHit.hh"

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
G4Allocator<PhotodetectorPhotonHit> PhotodetectorPhotonHitAllocator;
} // namespace gm2ringsim

gm2ringsim::PhotodetectorPhotonHit::PhotodetectorPhotonHit(G4Step *step) :
global_pos(step->GetPreStepPoint()->GetPosition()),
global_mom(step->GetPreStepPoint()->GetMomentum()),
time(step->GetPreStepPoint()->GetGlobalTime()),
turnNum(TurnCounter::getInstance().turns()),
trackID(step->GetTrack()->GetTrackID()),
caloNum( -1 ),
photodetectorNum( -1 ),
energy( step->GetPreStepPoint()->GetTotalEnergy() ),
accepted( false )
{
    G4TouchableHandle const touchy = step->GetPreStepPoint()->GetTouchableHandle();
    local_pos =
    touchy->GetHistory()->GetTopTransform().TransformPoint(global_pos);
    // local coords are: radial is x, vertical is y, thickness is z
    
    G4VTouchable const *feely = step->GetPreStepPoint()->GetTouchable();
    G4RotationMatrix const *rot = feely->GetRotation();
    local_mom = global_mom;
    local_mom.transform(*rot);
}


void gm2ringsim::PhotodetectorPhotonHit::Draw(){
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


void gm2ringsim::PhotodetectorPhotonHit::Print(){
    G4cout << " turnNum: " << turnNum
    << " photodetector: " << photodetectorNum
    << " time: " << time
    << '\n';
    G4cout << "\tlpos: " << local_pos << '\n'
    << "\tlmom: " << local_mom << '\n';
}
