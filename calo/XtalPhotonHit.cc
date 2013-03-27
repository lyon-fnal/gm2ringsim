/** @file xtalPhotonHit.cc
 
    Implements the xtalPhoton hit data storage classes.
 
    Ported to Art from g2migtrace file xtalPhotonHit.cc
        @author Werner Sun
        @date 2011
 
    @author Robin Bjorkquist
    @date 2013
 */

#include "XtalPhotonHit.hh"

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4Box.hh"

// These two have to be included IN THIS ORDER due to a bug in
// G4TouchableHandle.hh in 4.9.2.p01, caught by gcc 4.3.2
#include "Geant4/G4NavigationHistory.hh"
#include "Geant4/G4TouchableHandle.hh"

#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

#include <string>
#include <sstream>

namespace gm2ringsim {
G4Allocator<XtalPhotonHit> XtalPhotonHitAllocator;
} // namespace gm2ringsim

gm2ringsim::XtalPhotonHit::XtalPhotonHit(G4Step *step) :
global_pos(step->GetPreStepPoint()->GetPosition()),
trackID(step->GetTrack()->GetTrackID()),
energy( step->GetPreStepPoint()->GetTotalEnergy() ),
transmitted(false),
detected(false)
{
    G4TouchableHandle const touchy = step->GetPreStepPoint()->GetTouchableHandle();
    G4NavigationHistory const *navHistory = touchy->GetHistory();
    // local coords will be x = radial, y = vertical, z = thickness
    // 0 in z = front of calorimeter, x and y are local to xtal
    local_pos =
    touchy->GetHistory()->GetTopTransform().TransformPoint(global_pos);
    
    // Use caloBound coordinates for local z-position
    //           (for consistency between xtal and optical coupling volumes)
    // First, locate the calobound volume (one level up from xtal, 2 levels up from optical coupling):
    std::string volname = touchy->GetVolume()->GetName();
    std::string tester = volname.substr(0,15);
    int calobound_depth;
    G4VSolid* caloBound;
    if( tester.compare("opticalCoupling") == 0 )
    {
        calobound_depth = navHistory->GetDepth() - 2; // calobound is 2 up from optical coupling
        caloBound = touchy->GetVolume(2)->GetLogicalVolume()->GetSolid();
    }
    else
    {
        calobound_depth = navHistory->GetDepth() - 1; // calobound is 1 up from xtal
        caloBound = touchy->GetVolume(1)->GetLogicalVolume()->GetSolid();
    }
    // Then get the position in the calobound local coordinates
    G4ThreeVector depth_pos =
    navHistory->GetTransform(calobound_depth).TransformPoint(global_pos);
    // Find size of calobound volume so we can set z=0 at front face (instead of center)
    G4ThreeVector origin(0., 0., 0.);
    G4ThreeVector zhat(0., 0., 1.);
    G4double offset = caloBound->DistanceToOut(origin, zhat);
    // Swap out the old local position z-coordinate for our calobound z-coordinate
    local_pos.setZ(depth_pos.z() + offset);
    
    // G4VTouchable const *feely = step->GetPreStepPoint()->GetTouchable();
    G4RotationMatrix const *rot = step->GetPreStepPoint()->GetTouchable()->GetRotation();
    G4ThreeVector global_mom = step->GetPreStepPoint()->GetMomentum();
    G4ThreeVector local_mom = global_mom;
    local_mom.transform(*rot);
    
    // now store the direction information
    cosTheta = local_mom.cosTheta();
    phi      = local_mom.phi();
    
    // get xtal and calo number...
    /** @bug This is much nastier than it should be. */
    std::string name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    int left = name.find('[');
    std::string num(name, left+1, left+2);
    std::istringstream iss(num);
    iss >> caloNum;
    
    xtalNum = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() ;
    
}


void gm2ringsim::XtalPhotonHit::Draw(){
#ifdef G4VIS_USE
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(!pVVisManager)
        return;
    
    G4Circle circle(global_pos);
    circle.SetScreenSize(2);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.,1.,1.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
#endif
}


void gm2ringsim::XtalPhotonHit::Print(){
    G4cout
    << " caloNum: " << caloNum
    << " xtal:    " << xtalNum
    << '\n';
    G4cout
    << "\tlpos:   " << local_pos << '\n'
    << "cosTheta: " << cosTheta << '\n'
    << "phi:      " << phi << '\n';
}
