//  PhysicsService.cc
//  artg4example_xcode
//
//  Created by Adam Lyon on 11/6/12.
//  Copyright (c) 2012 Adam Lyon. All rights reserved.
//

#include "gm2ringsim/actions/PhysicsList_service.hh"

// Services
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/PhysicsListHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/globals.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4ParticleTypes.hh"

// Constructor
gm2ringsim::PhysicsListService::PhysicsListService(fhicl::ParameterSet const &, art::ActivityRegistry & )
{
  // Register the physics list itself
  art::ServiceHandle<artg4::PhysicsListHolderService> holder;
  
  // PhysicsList must be on the heap because Geant is going to delete it
  holder->registerPhysicsList(new PhysicsList);
  
}

gm2ringsim::PhysicsList::PhysicsList() :
  G4VUserPhysicsList()
{
  defaultCutValue = 1.0*cm;
  SetVerboseLevel(1);

}

// The rest are copied verbaim from the Geant code


void gm2ringsim::PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.
}



void gm2ringsim::PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
  AddStepMax();
}

#include "Geant4/G4PhysicsListHelper.hh"

#include "Geant4/G4ComptonScattering.hh"
#include "Geant4/G4GammaConversion.hh"
#include "Geant4/G4PhotoElectricEffect.hh"

#include "Geant4/G4eMultipleScattering.hh"
#include "Geant4/G4eIonisation.hh"
#include "Geant4/G4eBremsstrahlung.hh"
#include "Geant4/G4eplusAnnihilation.hh"

#include "Geant4/G4MuMultipleScattering.hh"
#include "Geant4/G4MuIonisation.hh"
#include "Geant4/G4MuBremsstrahlung.hh"
#include "Geant4/G4MuPairProduction.hh"

#include "Geant4/G4hMultipleScattering.hh"
#include "Geant4/G4hIonisation.hh"
#include "Geant4/G4hBremsstrahlung.hh"
#include "Geant4/G4hPairProduction.hh"

#include "Geant4/G4ionIonisation.hh"


void gm2ringsim::PhysicsList::ConstructEM()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // gamma
      ph->RegisterProcess(new G4PhotoElectricEffect, particle);
      ph->RegisterProcess(new G4ComptonScattering,   particle);
      ph->RegisterProcess(new G4GammaConversion,     particle);
      
    } else if (particleName == "e-") {
      //electron
      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);
      
    } else if (particleName == "e+") {
      //positron
      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);
      ph->RegisterProcess(new G4eplusAnnihilation,   particle);
      
    } else if( particleName == "mu+" ||
              particleName == "mu-"    ) {
      //muon
      ph->RegisterProcess(new G4MuMultipleScattering, particle);
      ph->RegisterProcess(new G4MuIonisation,         particle);
      ph->RegisterProcess(new G4MuBremsstrahlung,     particle);
      ph->RegisterProcess(new G4MuPairProduction,     particle);
      
    } else if( particleName == "proton" ||
              particleName == "pi-" ||
              particleName == "pi+"    ) {
      //proton
      ph->RegisterProcess(new G4hMultipleScattering, particle);
      ph->RegisterProcess(new G4hIonisation,         particle);
      ph->RegisterProcess(new G4hBremsstrahlung,     particle);
      ph->RegisterProcess(new G4hPairProduction,     particle);
      
    } else if( particleName == "alpha" ||
              particleName == "He3" )     {
      //alpha
      ph->RegisterProcess(new G4hMultipleScattering, particle);
      ph->RegisterProcess(new G4ionIonisation,       particle);
      
    } else if( particleName == "GenericIon" ) {
      //Ions
      ph->RegisterProcess(new G4hMultipleScattering, particle);
      ph->RegisterProcess(new G4ionIonisation,       particle);
      
    } else if ((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) &&
               (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      ph->RegisterProcess(new G4hMultipleScattering, particle);
      ph->RegisterProcess(new G4hIonisation,         particle);
    }
  }
}

#include "Geant4/G4Decay.hh"

void gm2ringsim::PhysicsList::ConstructGeneral()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if (theDecayProcess->IsApplicable(*particle)) {
      ph->RegisterProcess(theDecayProcess, particle);
    }
  }
}

#include "Geant4/G4StepLimiter.hh"
#include "Geant4/G4UserSpecialCuts.hh"

void gm2ringsim::PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  ////G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    
    if (particle->GetPDGCharge() != 0.0)
    {
      pmanager ->AddDiscreteProcess(stepLimiter);
      ////pmanager ->AddDiscreteProcess(userCuts);
    }
  }
}

void gm2ringsim::PhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWithDefault method sets
  //the default cut value for all particle types
  //
  SetCutsWithDefault();
  
  if (verboseLevel>0) DumpCutValuesTable();
}

using gm2ringsim::PhysicsListService;
DEFINE_ART_SERVICE(PhysicsListService)
