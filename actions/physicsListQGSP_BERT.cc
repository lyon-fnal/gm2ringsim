/** @file physicsListQGSP_BERT.cc

    Provides implementation of the required physics list for the
    simulation based on the QGSP_BERT physics list. 

    - Peter Winter Mar/2013
*/

#include "gm2ringsim/actions/physicsListQGSP_BERT.hh"

#include "Geant4/G4VPhysicsConstructor.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/G4EmStandardPhysics.hh"
#include "Geant4/G4EmExtraPhysics.hh"
#include "Geant4/G4IonPhysics.hh"
#include "Geant4/G4QStoppingPhysics.hh"
#include "Geant4/G4HadronElasticPhysics.hh"
#include "Geant4/G4NeutronTrackingCut.hh"
#include "Geant4/HadronPhysicsQGSP_BERT.hh"

gm2ringsim::physicsListQGSP_BERT::physicsListQGSP_BERT(fhicl::ParameterSet const & p) : physicsList() {

  // Let's first empty any elements in the PhysicsListVector physics_ 
  PhysicsListVector::const_iterator b = GetPhysicsListVector()->begin(), e = GetPhysicsListVector()->end();
  while( b!=e ){
    delete (*b);
    ++b;
  }
  GetPhysicsListVector()->clear();    

  // Now the physicsList_ is empty and we can setup our own new physics list
  float cutValue = p.get<float>("defaultCutValue", -1000.);
  if(cutValue > 0.){
    defaultCutValue = cutValue * mm;
  }

  // EM Physics
  GetPhysicsListVector()->push_back( new G4EmStandardPhysics );

  if(p.get<bool>("useHadronicPhysics", false) == true){
    // Hadron Elastic scattering
    GetPhysicsListVector()->push_back( new G4HadronElasticPhysics);
    
    // Hadron Physics
    GetPhysicsListVector()->push_back( new HadronPhysicsQGSP_BERT );
  }
  
  if( p.get<bool>("useSynchrotronPhysics", false)){
    // Synchroton Radiation & GN Physics
    GetPhysicsListVector()->push_back( new G4EmExtraPhysics );
  }
  
  if( p.get<bool>("useQStoppingPhysics", false)){
    // Stopping Physics
    GetPhysicsListVector()->push_back( new G4QStoppingPhysics );
  }
  
  if( p.get<bool>("useIonPhysics", false)){
    // Ion Physics
    GetPhysicsListVector()->push_back( new G4IonPhysics );
  }

  if( p.get<bool>("useNeutronTrackingCut", false)){
    // Neutron tracking cut
    GetPhysicsListVector()->push_back( new G4NeutronTrackingCut );
  }
}

gm2ringsim::physicsListQGSP_BERT::~physicsListQGSP_BERT(){
}
