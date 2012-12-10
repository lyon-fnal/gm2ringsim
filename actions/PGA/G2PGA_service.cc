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
#include "gm2ringsim/actions/PGA/g2GeneralParticleSource.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"

#define randFromDistr CLHEP::RandGeneral

#include <iostream>
#include <cmath>

using std::endl;

// Constructor
gm2ringsim::G2PGA::G2PGA(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
    artg4::PrimaryGeneratorActionBase(p.get<std::string>("name")),
    par_g2GPS_(p.get<fhicl::ParameterSet>("g2GPS", fhicl::ParameterSet())),
    g2GPS_( 0 )    // Must not intialize here because Geant isn't ready yet
{}

gm2ringsim::G2PGA::~G2PGA(){
  delete g2GPS_;
}


// initialize
void gm2ringsim::G2PGA::initialize() {
  
  //  particleGun_ = new G4ParticleGun(1);
  g2GPS_ = new g2GeneralParticleSource();

}

// EndOfG2PGA
void gm2ringsim::G2PGA::generatePrimaries(G4Event* evt) {

    g2GPS_->GeneratePrimaryVertex( evt );
 
  
} // generatePrimaries


using gm2ringsim::G2PGA;
DEFINE_ART_SERVICE(G2PGA)
