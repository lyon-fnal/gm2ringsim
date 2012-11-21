/** @file PGA.cc prior to ART
    @file PrimaryGeneratorAction_service.cc
    Implements PGA, the required primary generator action.
    
    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
    @author Nathan Froemming
    @date August 2012
    @author Brendan Kiburg
    @date 2012 Port to ART                                                                                
*/

#include "gm2ringsim/actions/PGA/PrimaryGeneratorAction_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4Event.hh"
#include "Geant4/G4GeneralParticleSource.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/globals.hh"
#include "Geant4/Randomize.hh"

#include "gm2ringsim/actions/PGA/g2GeneralParticleSource.hh"
#include "gm2ringsim/actions/PGA/g2PreciseValues.hh"

#define randFromDistr CLHEP::RandGeneral

#include <iostream>
#include <cmath>
using std::cout;
using std::endl;


// FIXME : Set this as a fhicl default 
int muonGasVerbosity = 1; // @bug: Allow user to set via GUI    



// Constructor
gm2ringsim::PrimaryGeneratorAction::PrimaryGeneratorAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
    artg4::PrimaryGeneratorActionBase(p.get<std::string>("name")),
    g2GPS_( 0 ),    // Must not intialize here because Geant isn't ready yet
    muonGasGun_ ( 0 ) ,
    inflectorGun_ ( 0 ) 
{}

gm2ringsim::PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  delete g2GPS_;
  delete muonGasGun_;
  delete inflectorGun_;
}


// initialize
void gm2ringsim::PrimaryGeneratorAction::initialize() {
  
  //  particleGun_ = new G4ParticleGun(1);
  g2GPS_ = new g2GeneralParticleSource();
  muonGasGun_ = new G4ParticleGun();
  inflectorGun_ = new G4ParticleGun();

  //G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //  G4ParticleDefinition* particle = particleTable->FindParticle("proton");
  
  //  particleGun_->SetParticleDefinition(particle);
  //  particleGun_->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  //particleGun_->SetParticleEnergy(3.0*GeV);
  muonGasGun_ -> SetParticleEnergy(3.0*GeV);
}

// EndOfPrimaryGeneratorAction
void gm2ringsim::PrimaryGeneratorAction::generatePrimaries(G4Event* evt) {
  
  

  
  muonGasGun_->GeneratePrimaryVertex(evt);

}

using gm2ringsim::PrimaryGeneratorAction;
DEFINE_ART_SERVICE(PrimaryGeneratorAction)
