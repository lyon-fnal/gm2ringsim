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
#include "gm2ringsim/actions/PGA/g2GeneralParticleSource.hh"
#include "gm2ringsim/actions/PGA/g2MuonGasSource.hh"

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
  muonGasVerbosity_ (p.get<bool>("muonGasVerbosity", 0))
{}

gm2ringsim::MuonGasPGA::~MuonGasPGA(){
  delete gps_;
}


// initialize
void gm2ringsim::MuonGasPGA::initialize() {
  
  //  particleGun_ = new G4ParticleGun(1);
  // Read the parameter set and figure out what sort of gun to use
  gps_ = new g2MuonGasSource();
  gps_ -> SetVerbosity(muonGasVerbosity_);
}

// EndOfMuonGasPGA
void gm2ringsim::MuonGasPGA::generatePrimaries(G4Event* evt) {

  gps_->GeneratePrimaryVertex( evt );

  
} // generatePrimaries


using gm2ringsim::MuonGasPGA;
DEFINE_ART_SERVICE(MuonGasPGA)
