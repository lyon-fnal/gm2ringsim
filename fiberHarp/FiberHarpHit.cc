/** @file FiberHarpHit.cc

    Implements the fiber harp hit data storage classes.

    Derived from work of Kevin Lynch, 2011, Fred Gray, 2012

    @author Peter Winter
    @date 2012
*/

#include "FiberHarpHit.hh"

#include "Geant4/G4VVisManager.hh"
#include "Geant4/G4Circle.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

// These two have to be included IN THIS ORDER due to a bug in
// G4TouchableHandle.hh in 4.9.2.p01, caught by gcc 4.3.2
#include "Geant4/G4NavigationHistory.hh"
#include "Geant4/G4TouchableHandle.hh"

// TODO #include "turnCounter.hh"

#include <string>
#include <sstream>

G4Allocator<FiberHarpHit> FiberHarpHitAllocator;

FiberHarpHit::FiberHarpHit(G4Step *step) : 
  global_pos(step->GetPreStepPoint()->GetPosition()),
  energy_dep(step->GetTotalEnergyDeposit()),
  time(step->GetPreStepPoint()->GetGlobalTime()),
  turnNum(0), // TODO turnCounter::getInstance().turns()),
  trackID(step->GetTrack()->GetTrackID())
{
  // The name is HARP[m].fiber[n] or HARP[m].support[n]
  std::string name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

  // get harp number
  std::string::size_type left = name.find_first_of('[');
  std::string num(name, left+1, left+2);
  std::istringstream iss(num);
  iss >> harp;

  // get fiber number
  std::string::size_type leftP = name.find_last_of('[');
  std::string numP(name, leftP+1, leftP+2);
  std::istringstream issP(numP);
  issP >> fiber;

  // if it's a support, make it negative
  if(name.find("support") != std::string::npos) {
    fiber = -fiber + -10;
  }
}

void FiberHarpHit::draw(){
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


void FiberHarpHit::print(){
  G4cout << " turnNum: " << turnNum
	 << " harp: " << harp
	 << " fiber: " << fiber
	 << " time: " << time
	 << '\n';
  G4cout << "\tgpos: " << global_pos << '\n';
}



