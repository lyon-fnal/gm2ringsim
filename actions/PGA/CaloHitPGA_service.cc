/** @file CaloHitPGA_service.cc
 
 Based on SimpleParticleSource_service.cc
 This will generate electrons aimed at one of the calorimeters.
 Simple version with hard-coded position, momentum, and particle energy for now.
 
 @author  Robin Bjorkquist
 @date    2013
 
 */

// Get the PGA header
#include "gm2ringsim/actions/PGA/CaloHitPGA_service.hh"


// ART includes
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Principal/Event.h"

#include "artg4/services/ActionHolder_service.hh"
#include "artg4/services/DetectorHolder_service.hh"
#include "artg4/Core/DetectorBase.hh"

#include "gm2ringsim/calo/CaloArtRecord.hh"

// Geant includes
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4RotationMatrix.hh"
#include "Geant4/G4LogicalVolume.hh"


// Constructor
gm2ringsim::CaloHitPGA::CaloHitPGA(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
artg4::PrimaryGeneratorActionBase(p.get<std::string>("name", "CaloHitPGA")),
particleGun_( 0 )     // Must not initialize here because Geant isn't ready yet
{}


// Destructor
gm2ringsim::CaloHitPGA::~CaloHitPGA(){
    delete particleGun_;
}


// initialize
void gm2ringsim::CaloHitPGA::initialize() {
    
    particleGun_ = new G4ParticleGun();
    
    // Just use electrons for now -- need to change to particle type of each calo hit
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* minusElec = particleTable->FindParticle("e-");
    particleGun_->SetParticleDefinition(minusElec);
    
}


// Generate Primaries
void gm2ringsim::CaloHitPGA::generatePrimaries(G4Event* evt) {
    
    // get the event
    art::ServiceHandle<artg4::ActionHolderService> actionHolder;
    art::Event &e = actionHolder->getCurrArtEvent();
    
    // get calo hits from the event
    art::Handle<CaloArtRecordCollection> caloHitHandle;
    e.getByLabel("artg4", "calorimeter", caloHitHandle);
    const CaloArtRecordCollection & caloHits = *caloHitHandle;
    
    particleGun_ -> SetNumberOfParticles( caloHits.size() );
    
    // initiate primary vertex for each calo hit
    for (auto entry : caloHits)
    {        
        G4ThreeVector position( entry.x, entry.y, entry.z );
        particleGun_ -> SetParticlePosition( position );
        
        G4ThreeVector momentum( entry.px, entry.py, entry.pz );
        particleGun_ -> SetParticleMomentum( momentum );
        
        particleGun_ -> SetParticleTime( entry.time );
        
        particleGun_->GeneratePrimaryVertex( evt );

    }
    

    
    
}

using gm2ringsim::CaloHitPGA;
DEFINE_ART_SERVICE(CaloHitPGA)
