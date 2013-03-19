/** @file CaloElectronPGA_service.cc
 
 Based on SimpleParticleSource_service.cc
 This will generate electrons aimed at one of the calorimeters.
 Simple version with hard-coded position, momentum, and particle energy for now.
 
 @author  Robin Bjorkquist
 @date    2013
 
 */

// Get the PGA header
#include "gm2ringsim/actions/PGA/CaloElectronPGA_service.hh"


// ART includes
#include "art/Framework/Services/Registry/ServiceMacros.h"

// Geant includes
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4RotationMatrix.hh"


// Constructor
gm2ringsim::CaloElectronPGA::CaloElectronPGA(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
artg4::PrimaryGeneratorActionBase(p.get<std::string>("name", "CaloElectronPGA")),
particleGun_( 0 )     // Must not initialize here because Geant isn't ready yet
{}


// Destructor
gm2ringsim::CaloElectronPGA::~CaloElectronPGA(){
    delete particleGun_;
}


// initialize
void gm2ringsim::CaloElectronPGA::initialize() {
    
    particleGun_ = new G4ParticleGun(1);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* minusElec = particleTable->FindParticle("e-");
    particleGun_->SetParticleDefinition(minusElec);
    
    // Code from g2migtrace to set position & direction:
    // (Should make this better)
    
    G4RotationMatrix localToGlobalRot(G4ThreeVector( -0.647721, 0., 0.761878 ),
                                      G4ThreeVector( -0.761878, 0., -0.647721 ),
                                      G4ThreeVector( 0., -1., 0. ) );
    
    G4ThreeVector caloCenterGlobal(-4665.73, 0., +5090.03);
            
    // Additional angle adjustment
    G4double theta = 0. * deg;
    
    // horizontal momentum, perpendicular to axis of shot
    G4double xlocalMom = 0. * GeV;
    
    // horizontal momentum, parallel to axis of shot
    G4double ylocalMom = 2. * GeV;
    
    // vertical momentum, +z is down
    G4double zlocalMom = 0. * GeV;
    
    // translation relative to center of relevant calo face:
    // NOTE: Particles are already slightly outside calo, so don't adujst y value.
    G4ThreeVector localPos( -0.75*cm,
                            0.*cm,
                            -1.25*cm ) ; // adjusted for 6 by 9 array
    
    G4ThreeVector localMom( xlocalMom,
                           ylocalMom,
                           zlocalMom ) ;
    
    G4ThreeVector xhatLocal(1., 0., 0.);
	G4ThreeVector yhatLocal(0.*mm, 1.*mm, 0.*mm);

    G4double phi = -5. * deg; // aim particle straight into calo 8
	G4double angle = phi + theta;
	localPos = localPos - 74.5 * yhatLocal; // .5 mm in front of front face
	G4ThreeVector adjustedPos( localPos.x() * std::cos(phi) + std::sin(phi) * localPos.y(),
                              -localPos.x() * std::sin(phi) + std::cos(phi) * localPos.y() ,
                              localPos.z()) ;
	G4ThreeVector adjustedMom( xlocalMom * std::cos(angle) + std::sin(angle) * ylocalMom,
                              -xlocalMom * std::sin(angle) + std::cos(angle) * ylocalMom ,
                              zlocalMom) ;
    
    // Set particle position and momentum
	particleGun_ -> SetParticlePosition(caloCenterGlobal + adjustedPos.transform( localToGlobalRot ) );
    
	particleGun_ -> SetParticleMomentum( adjustedMom.transform( localToGlobalRot ) ) ;


}


// Generate Primaries
void gm2ringsim::CaloElectronPGA::generatePrimaries(G4Event* evt) {
    
    particleGun_->GeneratePrimaryVertex( evt );
        
}

using gm2ringsim::CaloElectronPGA;
DEFINE_ART_SERVICE(CaloElectronPGA)
