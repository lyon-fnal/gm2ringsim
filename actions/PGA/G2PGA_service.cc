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
#include "gm2ringsim/actions/PGA/G2PGA_service.hh"

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
gm2ringsim::G2PGA::G2PGA(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::PrimaryGeneratorActionBase(p.get<std::string>("name")),
  G2GPSString_("G2GPSSettings"),
  settings_(G2GPSString_),
  //par_g2GPS_(p.get<fhicl::ParameterSet>("G2GPSSettings", fhicl::ParameterSet())),
  gps_( 0 )     // Must not intialize here because Geant isn't ready yet
{}

gm2ringsim::G2PGA::~G2PGA(){
  delete gps_;
}


// initialize
void gm2ringsim::G2PGA::initialize() {

  gps_ = new G2GeneralParticleSource();

}

// EndOfG2PGA
void gm2ringsim::G2PGA::generatePrimaries(G4Event* evt) {

  gps_->GeneratePrimaryVertex( evt );
 
  
} // generatePrimaries


using gm2ringsim::G2PGA;
DEFINE_ART_SERVICE(G2PGA)
