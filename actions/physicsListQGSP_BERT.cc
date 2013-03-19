/** @file physicsListQGSP_BERT.cc

    Provides implementation of the required physics list for the
    simulation based on the QGSP_BERT physics list. 

    - Peter Winter Mar/2013
*/

#include "gm2ringsim/actions/physicsListQGSP_BERT.hh"

#include "Geant4/G4VPhysicsConstructor.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/G4EmStandardPhysics.hh"
#include "Geant4/G4HadronElasticPhysics.hh"

gm2ringsim::physicsListQGSP_BERT::physicsListQGSP_BERT() : physicsList() {
  printf("============= gm2ringsim::physicsListQGSP_BERT::physicsListQGSP_BERT() =======\n");

  // Let's first empty any elements in the PhysicsListVector physics_ 
  PhysicsListVector::const_iterator b = GetPhysicsListVector()->begin(), e = GetPhysicsListVector()->end();
  while( b!=e ){
    printf("===============%s\n", (*b)->GetPhysicsName().data());
    delete (*b);
    ++b;
  }
  GetPhysicsListVector()->clear();    

  printf("\n\nAfter deletion\n");
  b = GetPhysicsListVector()->begin(), e = GetPhysicsListVector()->end();
  while( b!=e ){
    printf("===============%s\n", (*b)->GetPhysicsName().data());
    ++b;
  }

  // Now the physicsList_ is empty and we can setup our own new physics list

  // EM Physics
  GetPhysicsListVector()->push_back( new G4EmStandardPhysics );

  // Hadron Elastic scattering
  GetPhysicsListVector()->push_back( new G4HadronElasticPhysics );

  // Hadron Physics
  //GetPhysicsListVector()->push_back( new HadronPhysicsQGSP_BERT );

  
  /*
  defaultCutValue = 0.7*mm;  
  this->SetVerboseLevel(ver);

  // EM Physics
  this->RegisterPhysics( new G4EmStandardPhysics(ver) );

  // Synchroton Radiation & GN Physics
  this->RegisterPhysics( new G4EmExtraPhysics(ver) );

  // Decays
  this->RegisterPhysics( new G4DecayPhysics(ver) );

   // Hadron Elastic scattering
  this->RegisterPhysics( new G4HadronElasticPhysics(ver) );

  // Hadron Physics
  this->RegisterPhysics( new HadronPhysicsQGSP_BERT(ver));

  // Stopping Physics
  this->RegisterPhysics( new G4QStoppingPhysics(ver) );

  // Ion Physics
  this->RegisterPhysics( new G4IonPhysics(ver));
  
  // Neutron tracking cut
  this->RegisterPhysics( new G4NeutronTrackingCut(ver));
  */
}

gm2ringsim::physicsListQGSP_BERT::~physicsListQGSP_BERT(){
}
