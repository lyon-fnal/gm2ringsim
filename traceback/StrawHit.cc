/** @file StrawHit.cc

    Implements the strawscope hit data storage classes.

    @author Kevin Lynch 
    @date 2011
*/

#include "StrawHit.hh"

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
  G4Allocator<StrawHit> StrawHitAllocator;
}//namespace gm2ringsim
gm2ringsim::StrawHit::StrawHit(G4Step *step) :
  position(step->GetPreStepPoint()->GetPosition()),
  momentum(step->GetPreStepPoint()->GetMomentum()),
  energy_dep(step->GetTotalEnergyDeposit()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  turnNum(TurnCounter::getInstance().turns()),
  trackID(step->GetTrack()->GetTrackID())
{
  G4TouchableHandle const touchy = step->GetPreStepPoint()->GetTouchableHandle();
  // from calorimeterConstruction.hh
  // radial,vertical,horizontal thickness
  local_position =
    touchy->GetHistory()->GetTopTransform().TransformPoint(position);
  // see calorimeterConstruction.cc ... local coords are messed up,
  // but it's easier to fix it here than in the geometry... need a
  // reflection transform for the vertical coordinate, z().  The
  // radial is x(), while the thickness is y()
  local_position.setZ(-local_position.z());

  /** @bug This is icky... */
  //DetectorStation[<num>].strawscope_{front,rear}_plane[<num>]
  std::string name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

  // get station number
  std::string::size_type left = name.find_first_of('[');
  std::string num(name, left+1, left+2);
  std::istringstream iss(num);
  iss >> station;



  // get plane number
  std::string::size_type leftP = name.find_first_of('P');
  std::string numP(name, leftP+1, leftP+2);
  std::istringstream issP(numP);
  issP >> StrawPlane;


  /*

  // get plane
  if( std::string::npos != name.find("front") ){
    StrawPlane = 0;


  }

  if( std::string::npos != name.find("mid") ){
    StrawPlane = 1;


  }
  if( std::string::npos != name.find("rear")){
    StrawPlane = 2;

  }

  */

  // get stave
  left = name.find_last_of('[');
  num = std::string(name, left+1,left+2);
  iss.str(num);
  iss >> StrawStave;
}

void gm2ringsim::StrawHit::Draw(){
#ifdef G4VIS_USE
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(!pVVisManager)
    return;

  G4Circle circle(position);
  circle.SetScreenSize(5);
  circle.SetFillStyle(G4Circle::filled);
  G4Colour colour(0.,1.,0.);
  G4VisAttributes attribs(colour);
  circle.SetVisAttributes(attribs);
  pVVisManager->Draw(circle);
#endif
}


void gm2ringsim::StrawHit::Print(){
  G4cout << " turnNum: " << turnNum
	 << " StrawPlane: " << StrawPlane
	 << " StrawStave: " << StrawStave
	 << " time: " << time
	 << '\n';
  G4cout << "\tlpos: " << local_position << '\n';
}



