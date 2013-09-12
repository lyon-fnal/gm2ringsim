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
#include "gm2ringsim/actions/PGA/InflectorPGA_service.hh"

// Random numbers
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/RandomNumberGenerator.h"

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
#include "gm2ringsim/actions/PGA/G2InflectorSource.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#define randFromDistr CLHEP::RandGeneral

#include <iostream>
#include <cmath>

using std::endl;

// Constructor
gm2ringsim::InflectorPGA::InflectorPGA(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::PrimaryGeneratorActionBase(p.get<std::string>("name")),
  gps_( 0 ),     // Must not initialize here because Geant isn't ready yet
  inflectorVerbosity_ (p.get<bool>("inflectorVerbosity", 0)),
  StartUpstream_(p.get<bool>("StartUpstream", false)),
  StartUpstreamCryo_(p.get<bool>("StartUpstreamCryo", false)),
  StartDownstream_(p.get<bool>("StartDownstream", false)),
  StartPerfect_(p.get<bool>("StartPerfect", false)),
  GenGaussian_(p.get<bool>("GenGaussian", true)),
  LaunchAngle_(p.get<double>("LaunchAngle", -9999.9)),
  StorageOffset_(p.get<double>("StorageOffset", -9999.9)),
  TurnCounter_(p.get<int>("TurnCounter", 11)),
  RotAngle_(p.get<double>("RotAngle", 0.0)),
  EmittanceX_(p.get<double>("EmittanceX", -9999.9)),
  EmittanceY_(p.get<double>("EmittanceY", -9999.9)),
  BetaX_(p.get<double>("BetaX", -9999.9)),
  BetaY_(p.get<double>("BetaY", -9999.9)),
  AlphaX_(p.get<double>("AlphaX", -9999.9)),
  AlphaY_(p.get<double>("AlphaY", -9999.9)),
  Pmean_(p.get<double>("Pmean", -9999.9)),
  dPOverP_(p.get<double>("dPOverP", -9999.9)),
  SigmaT_(p.get<double>("SigmaT", -9999.9)),
  FlatDecayTime_(p.get<bool>("FlatDecayTime", false)),
  MaxDecayTime_(p.get<double>("MaxDecayTime", -1.0)),
  Particle_(p.get<std::string>("Particle", "mu+")),
  NumParticles_(p.get<int>("NumParticles", 1)),
  DecayScaleFactor_(p.get<int>("DecayScaleFactor", 1)),
  Polarization_(p.get<std::string>("Polarization", "E821"))
{
  if ( EmittanceX_ <= 0.0 ) { EmittanceX_ = 0.0; }
  if ( EmittanceY_ <= 0.0 ) { EmittanceY_ = 0.0; }

  if ( inflectorVerbosity_ > 0 ) {
    G4cout << "================== InflectorPGA ==================" << G4endl;
    G4cout << "  inflectorVerbosity  " << inflectorVerbosity_ << G4endl;
    G4cout << "  StartUpstream       " << StartUpstream_ << G4endl;
    G4cout << "  StartUpstreamCryo   " << StartUpstreamCryo_ << G4endl;
    G4cout << "  StartDownstream     " << StartDownstream_ << G4endl;
    G4cout << "  StartPerfect        " << StartPerfect_ << G4endl;
    G4cout << "  GenGaussian         " << GenGaussian_ << G4endl;
    G4cout << "  LaunchAngle         " << LaunchAngle_ << G4endl;
    G4cout << "  StorageOffset       " << StorageOffset_ << G4endl;
    G4cout << "  TurnCounter         " << TurnCounter_ << G4endl;
    G4cout << "  RotAngle            " << RotAngle_ << G4endl;
    G4cout << "  EmittanceX          " << EmittanceX_ << G4endl;
    G4cout << "  EmittanceY          " << EmittanceY_ << G4endl;
    G4cout << "  BetaX               " << BetaX_ << G4endl;
    G4cout << "  BetaY               " << BetaY_ << G4endl;
    G4cout << "  AlphaX              " << AlphaX_ << G4endl;
    G4cout << "  AlphaY              " << AlphaY_ << G4endl;
    G4cout << "  dPOverP             " << dPOverP_ << G4endl;
    G4cout << "  Pmean               " << Pmean_ << G4endl;
    G4cout << "  SigmaT              " << SigmaT_ << G4endl;
    G4cout << "  FlatDecayTime       " << FlatDecayTime_ << G4endl;
    G4cout << "  MaxDecayTime        " << MaxDecayTime_ << G4endl;
    G4cout << "  Particle            " << Particle_ << G4endl;
    G4cout << "  NumParticles        " << NumParticles_ << G4endl;    
    G4cout << "  DecayScaleFactor    " << DecayScaleFactor_ << G4endl;   
    G4cout << "  Polarization        " << Polarization_ << G4endl;    
  }
}

gm2ringsim::InflectorPGA::~InflectorPGA(){
  delete gps_;
}


// initialize
void gm2ringsim::InflectorPGA::initialize() {
  
  //  particleGun_ = new G4ParticleGun(1);
  // Read the parameter set and figure out what sort of gun to use
  //G4cout << "CALLING InflectorPGA::initialize()" << G4endl;
  gps_ = new G2InflectorSource();
  gps_ -> SetVerbosity(inflectorVerbosity_);
  gps_ -> SetStartUpstream(StartUpstream_);
  gps_ -> SetStartUpstreamCryo(StartUpstreamCryo_);
  gps_ -> SetStartDownstream(StartDownstream_);
  gps_ -> SetStartPerfect(StartPerfect_);
  gps_ -> SetGenGaussian(GenGaussian_);
  gps_ -> SetLaunchAngle(LaunchAngle_);
  gps_ -> SetStorageOffset(StorageOffset_);
  gps_ -> SetTurnCounter(TurnCounter_);
  gps_ -> SetRotAngle(RotAngle_);
  gps_ -> SetEmittanceX(EmittanceX_);
  gps_ -> SetEmittanceY(EmittanceY_);
  gps_ -> SetBetaX(BetaX_);
  gps_ -> SetBetaY(BetaY_);
  gps_ -> SetAlphaX(AlphaX_);
  gps_ -> SetAlphaY(AlphaY_);
  gps_ -> SetdPOverP(dPOverP_);
  gps_ -> SetPmean(Pmean_);
  gps_ -> SetSigmaT(SigmaT_);
  gps_ -> SetFlatDecayTime(FlatDecayTime_);
  gps_ -> SetMaxDecayTime(MaxDecayTime_);
  gps_ -> SetParticle(Particle_);
  gps_ -> SetNumParticles(NumParticles_);
  gps_ -> SetDecayScaleFactor(DecayScaleFactor_);
  gps_ -> SetPolarization(Polarization_);
  //G4cout << "Done calling InflectorPGA::initialize()" << G4endl;
  //////////////////////////////////
}

// EndOfInflectorPGA
void gm2ringsim::InflectorPGA::generatePrimaries(G4Event* evt) {

  gps_->GeneratePrimaryVertex( evt );

  
} // generatePrimaries


using gm2ringsim::InflectorPGA;
DEFINE_ART_SERVICE(InflectorPGA)
