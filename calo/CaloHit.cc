/** @file CaloHit.cc
 
    Implements the calorimeter hit data storage classes.
 
    Ported to Art from g2migtrace file pmtHit.cc
        @author Kevin Lynch
        @date 2009
 
    @author Robin Bjorkquist
    @date 2013
 */

#include "CaloHit.hh"

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

// These two have to be included IN THIS ORDER due to a bug in
// G4TouchableHandle.hh in 4.9.2.p01, caught by gcc 4.3.2
#include "Geant4/G4NavigationHistory.hh"
#include "Geant4/G4TouchableHandle.hh"

#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

#include <string>
#include <sstream>

namespace gm2ringsim {
    G4Allocator<CaloHit> CaloHitAllocator;
} // namespace gm2ringsim
    
gm2ringsim::CaloHit::CaloHit(G4Step *step) :
global_pos(step->GetPreStepPoint()->GetPosition()),
global_mom(step->GetPreStepPoint()->GetMomentum()),
time(step->GetPreStepPoint()->GetGlobalTime()),
turnNum(TurnCounter::getInstance().turns()),
trackID(step->GetTrack()->GetTrackID()),
caloNum(step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo())
{
}


void gm2ringsim::CaloHit::Draw(){
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


void gm2ringsim::CaloHit::Print(){
    G4cout << " turnNum: " << turnNum
    << " calo: " << caloNum
    << " time: " << time
    << '\n';
    G4cout << "\tlpos: " << global_pos << '\n'
    << "\tlmom: " << global_mom << '\n';
}

