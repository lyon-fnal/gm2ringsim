/** @file physicsListMessenger.cc

    Provides the implementation for the messenger interface to the
    custom physics options.

    - Kevin Lynch 2009
*/

#include "gm2ringsim/actions/physicsListMessenger.hh"

#include "gm2ringsim/actions/physicsList.hh"

#include <string>
#include <boost/algorithm/string.hpp>

gm2ringsim::physicsListMessenger::physicsListMessenger(physicsList *pl) :
  pl_(pl) {

  dir_ = new G4UIdirectory("/g2MIGTRACE/spin_decays/");
  dir_ -> SetGuidance("Control of Spin Dependent decays");

  decayCmd_ = 
    new G4UIcmdWithAString("/g2MIGTRACE/spin_decays/muonDecayMode", this);
  decayCmd_ -> SetGuidance("Set the type of muon decays allowed");
  decayCmd_ -> SetParameterName("Choice", true);
  decayCmd_ -> SetCandidates("get none isotropic standard");
  decayCmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);
}

gm2ringsim::physicsListMessenger::~physicsListMessenger() {
  delete decayCmd_;
  delete dir_;
}

void gm2ringsim::physicsListMessenger::SetNewValue(G4UIcommand *command, G4String newValue){
  if( command == decayCmd_ ){
    std::string downval(newValue);
    boost::trim(downval);
    boost::to_lower(downval);
    if( downval == "get" || newValue.size()==0 )
      G4cout << "The current muon decay mode is " << pl_->currentDecay() << '\n';
    else if( downval == "none" || downval == "disable" )
      pl_->disableDecay();
    else if( downval.substr(0,3) == "iso" )
      pl_->enableIsotropicDecay();
    else if( downval == "sm" || downval == "standard" )
      pl_->enableSMDecay();
  }
}


