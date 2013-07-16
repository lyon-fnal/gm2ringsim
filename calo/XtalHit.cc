/** @file XtalHit.cc
 
    Implements the xtal hit data storage classes.
 
    Ported to Art from g2migtrace file xtalHit.cc
        @author Werner Sun
        @date 2011
 
    @author Robin Bjorkquist
    @date 2013
 */

#include "XtalHit.hh"

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
#include <iostream>

namespace gm2ringsim {
G4Allocator<XtalHit>XtalHitAllocator;
} // namespace gm2ringsim

gm2ringsim::XtalHit::XtalHit(G4Step *step, G4int trackInitiatingShower ) :
global_pos(step->GetPreStepPoint()->GetPosition()),
global_mom(step->GetPreStepPoint()->GetMomentum()),
time(step->GetPreStepPoint()->GetGlobalTime()),
turnNum(TurnCounter::getInstance().turns()),
trackID(step->GetTrack()->GetTrackID()),
parentID(trackInitiatingShower),
caloNum( -1 ),
xtalNum( -1 ),
energy( step->GetPreStepPoint()->GetTotalEnergy() ),
energyDep( 0. ),
trackLength( 0. ),
pdgID( step->GetTrack()->GetDefinition()->GetPDGEncoding() ),
nphoton( 0 ),
ephoton( 0. )
{
    G4TouchableHandle const touchy = step->GetPreStepPoint()->GetTouchableHandle();
    G4NavigationHistory const *navHistory = touchy->GetHistory();
    int motherLocation = navHistory->GetDepth() - 1;
    // transform into the mother calorimeter volume coordinates (not the individual crystal coords)
    local_pos =
    navHistory->GetTransform(motherLocation).TransformPoint(global_pos);
    // x = radially outward, y = vertical, z = thickness (into calo)
    
    local_mom = global_mom;
    G4VTouchable const *feely = step->GetPreStepPoint()->GetTouchable();
    G4RotationMatrix const *rot = feely->GetRotation();
    local_mom.transform(*rot); // in crystal coordinates
    G4RotationMatrix l2mrot(navHistory->GetTopVolume()->GetObjectRotationValue());
    local_mom.transform(l2mrot); // now in calorimeter coordinates
}


void gm2ringsim::XtalHit::Draw(){
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


void gm2ringsim::XtalHit::Print(){
    G4cout << " turnNum: " << turnNum
    << " xtal: " << xtalNum
    << " time: " << time
    << " edep: " << energyDep
    << " length: " << trackLength
    << '\n';
    G4cout << "\tlpos: " << local_pos << '\n'
    << "\tlmom: " << local_mom << '\n';
}
