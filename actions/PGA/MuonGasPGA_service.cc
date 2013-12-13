/** @file PGA.cc prior to ART
    @file G2PGA_service.cc
    Implements PGA, the required primary generator action.
    
    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
    @author Nathan Froemming
    @date August 2012
    @author Brendan Kiburg
    @date 2012 Port to ART                                                                                
*/

// Get the PGA header
#include "gm2ringsim/actions/PGA/MuonGasPGA_service.hh"


// ART includes
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Geant includes
#include "Geant4/G4Event.hh"
#include "Geant4/G4GeneralParticleSource.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/globals.hh"
#include "Geant4/Randomize.hh"

// g2migtrace helpers
#include "gm2ringsim/actions/PGA/G2GeneralParticleSource.hh"
#include "gm2ringsim/actions/PGA/G2MuonGasSource.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#define randFromDistr CLHEP::RandGeneral

#include <iostream>
#include <cmath>

using std::endl;

// Constructor
gm2ringsim::MuonGasPGA::MuonGasPGA(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::PrimaryGeneratorActionBase(p.get<std::string>("name")),
  settings_("G2GPSSettings"),
  //par_g2GPS_(p.get<fhicl::ParameterSet>("G2GPSSettings", fhicl::ParameterSet())),
  gps_( 0 ),     // Must not initialize here because Geant isn't ready yet
  muonGasVerbosity_ (p.get<bool>("muonGasVerbosity", 0)),
  EmittanceX_(p.get<double>("EmittanceX", -9999.9)*mm*mrad),
  EmittanceY_(p.get<double>("EmittanceY", -9999.9)*mm*mrad),
  BetaX_(p.get<double>("BetaX", 7.6)*m),
  BetaY_(p.get<double>("BetaY", 19.1)*m),
  AlphaX_(p.get<double>("AlphaX", -9999.9)),
  AlphaY_(p.get<double>("AlphaY", -9999.9)),
  UseConstBeta_(p.get<bool>("UseConstBeta", true)),
  FlatDecayTime_(p.get<bool>("FlatDecayTime", false)),
  MaxDecayTime_(p.get<double>("MaxDecayTime", -1.0)*microsecond),
  Particle_(p.get<std::string>("Particle", "mu+")),  
  DecayScaleFactor_(p.get<int>("DecayScaleFactor", 1)),
  Polarization_(p.get<std::string>("Polarization", "E821")),
  NoTransMom_ (p.get<bool>("NoTransMom", false)),
  PosOffset_ (p.get<bool>("PosOffset", false)),
  StartPerfect_ (p.get<bool>("StartPerfect", false)),
  RotAngle_ (p.get<double>("RotAngle", 0.0)),
  Kick_ (p.get<int>("Kick", 0)),
  Debug_ (p.get<bool>("Debug", false))
{
  if ( EmittanceX_ <= 0.0 ) { EmittanceX_ = 0.0; }
  if ( EmittanceY_ <= 0.0 ) { EmittanceY_ = 0.0; }

  if ( muonGasVerbosity_ > 0 || Debug_ ) {
    G4cout << "================== MuonGasPGA ==================" << G4endl;
    G4cout << "  EmittanceX          " << EmittanceX_ << G4endl;
    G4cout << "  EmittanceY          " << EmittanceY_ << G4endl;
    G4cout << "  BetaX               " << BetaX_ << G4endl;
    G4cout << "  BetaY               " << BetaY_ << G4endl;
    G4cout << "  AlphaX              " << AlphaX_ << G4endl;
    G4cout << "  AlphaY              " << AlphaY_ << G4endl;
    G4cout << "  UseConstBeta        " << UseConstBeta_ << G4endl;
    G4cout << "  FlatDecayTime       " << FlatDecayTime_ << G4endl;
    G4cout << "  MaxDecayTime        " << MaxDecayTime_ << G4endl;
    G4cout << "  Particle            " << Particle_ << G4endl;
    G4cout << "  DecayScaleFactor    " << DecayScaleFactor_ << G4endl;   
    G4cout << "  Polarization        " << Polarization_ << G4endl;    
    G4cout << "  NoTransMom          " << NoTransMom_ << G4endl;    
    G4cout << "  PosOffset           " << PosOffset_ << G4endl;      
    G4cout << "  StartPerfect        " << StartPerfect_ << G4endl;    
    G4cout << "  RotAngle            " << RotAngle_ << G4endl;       
    G4cout << "  Kick                " << Kick_ << G4endl;    
  }
}

gm2ringsim::MuonGasPGA::~MuonGasPGA(){
  delete gps_;
}


// initialize
void gm2ringsim::MuonGasPGA::initialize() {
  
  //  particleGun_ = new G4ParticleGun(1);
  // Read the parameter set and figure out what sort of gun to use
  gps_ = new G2MuonGasSource();
  gps_ -> SetVerbosity(muonGasVerbosity_);
  gps_ -> SetEmittanceX(EmittanceX_);
  gps_ -> SetEmittanceY(EmittanceY_);
  gps_ -> SetBetaX(BetaX_);
  gps_ -> SetBetaY(BetaY_);
  gps_ -> SetAlphaX(AlphaX_);
  gps_ -> SetAlphaY(AlphaY_);
  gps_ -> SetUseConstBeta(UseConstBeta_);
  gps_ -> SetFlatDecayTime(FlatDecayTime_);
  gps_ -> SetMaxDecayTime(MaxDecayTime_);
  gps_ -> SetParticle(Particle_);
  gps_ -> SetPolarization(Polarization_);
  gps_ -> SetNoTransMom(NoTransMom_);
  gps_ -> SetPosOffset(PosOffset_);
  gps_ -> SetStartPerfect(StartPerfect_);
  gps_ -> SetRotAngle(RotAngle_);
  gps_ -> SetKick(Kick_);
  gps_ -> SetDebug(Debug_);
}

// EndOfMuonGasPGA
void gm2ringsim::MuonGasPGA::generatePrimaries(G4Event* evt) {

  gps_->GeneratePrimaryVertex( evt );

  
} // generatePrimaries


using gm2ringsim::MuonGasPGA;
DEFINE_ART_SERVICE(MuonGasPGA)
