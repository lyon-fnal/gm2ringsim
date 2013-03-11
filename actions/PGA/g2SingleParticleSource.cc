/** @file G2SingleParticleSource.cc

    Provides the modified distribution of the G4SingleParticleSource
    that includes time distribution.

    - Zach Hartwig 2005
*/

#include "Geant4/G4PrimaryParticle.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/Randomize.hh"
#include <math.h>
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4Geantino.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4IonTable.hh"
#include "Geant4/G4Ions.hh"
#include "Geant4/G4TrackingManager.hh"
#include "Geant4/G4Track.hh"

#include "gm2ringsim/actions/PGA/G2SingleParticleSource.hh"
//#include "inflectorConstruction.hh"
#include "gm2ringsim/inflector/inflectorGeometry.hh"

gm2ringsim::G2SingleParticleSource::G2SingleParticleSource()
{
  // Initialise all variables with some reasonable defaults

  // Default particle parameters
  NumberOfParticlesToBeGenerated = 1;
  particle_definition = G4Geantino::GeantinoDefinition();
  G4ThreeVector zero;
  particle_momentum_direction = G4ParticleMomentum(1,0,0);
  particle_energy = 1.0*MeV;
  particle_position = zero;
  particle_time = 0.*ns;
  particle_polarization = zero;
  particle_charge = 0.0;
  particle_weight = 1.0;

  // Create all the necessary distribution generators
  biasRndm = new G4SPSRandomGenerator();
  posGenerator = new G4SPSPosDistribution();
  posGenerator->SetBiasRndm(biasRndm);
  angGenerator = new G4SPSAngDistribution();
  angGenerator->SetPosDistribution(posGenerator);
  angGenerator->SetBiasRndm(biasRndm);
  eneGenerator = new G4SPSEneDistribution();
  eneGenerator->SetBiasRndm(biasRndm);
  timeGenerator = new G2SPSTimeDistribution();
  timeGenerator->SetBiasRndm(biasRndm);

  // Set the verbosity default 
  verbosityLevel = 0;

  // Set the particle import defaults
  importFlag = false;
  importFileType = "turtle";
  fileSuccessfullyOpened = false;
    
  // Initialize the variables that will be used for calculating the
  // input particle parameters
  x_i = y_i = 0.;
  xPrime_i = yPrime_i = 0.;
  pX_i = pY_i = pZ_i = pTotal = 0.;
  x_w = y_w = z_w =0.;
  pX_w = pY_w = pZ_w = 0.;
  sX_w = sY_w = sZ_w = 0.;
  
}


gm2ringsim::G2SingleParticleSource::~G2SingleParticleSource()
{}


// Function called by G2GPS to create the vertex
void gm2ringsim::G2SingleParticleSource::GeneratePrimaryVertex(G4Event *evt)
{
  if(particle_definition==NULL) return;
  
  if(verbosityLevel > 1)
    G4cout << " NumberOfParticlesToBeGenerated: "
	   << NumberOfParticlesToBeGenerated 
	   << G4endl;

  // Generate the vertex using imported or internally generated particles
  if(importFlag && fileSuccessfullyOpened)
    UseImportedParticles(evt);
  else
    UseInternallyGenerateParticles(evt);
  
}


void gm2ringsim::G2SingleParticleSource::UseImportedParticles(G4Event *evt)
{
  // When this function is called, importVector has been loaded with
  // all the particles.  First, static member function of
  // "inflectorConstruction.hh" are called to set the inflector
  // position variables needed to calculate the correct position of
  // the particles.  Then, each particle is picked out by the current
  // eventID, which serves as a self-indexing number to cycle through
  // all particles in the vector

  // Below functions ensure that the new parameters of the imported
  // particles are calculated with the most up-to-date geometrical
  // values for the inflector!

  // ** WARNING ** The inflector parameters have changed!  'gamma' now
  // ** describes the total deviation from the tangent to the storage orbit
  // ** at the downstream point of injection, and 'tiltAngle' has been
  // ** replaced by 'zeta'.  This needs to be fixed!!
  // ** ZSH - 08 SEP 08

  inflectorGeometry& ig = inflectorGeometry::getInstance();
  G4double delta = ig.delta();
  G4double gamma = ig.gamma();
  G4double L = ig.half_length();
  G4double apRad = ig.aperture_off();

  G4int eventID = evt -> GetEventID();

  // Make sure that the values exist!
  if( !importVector.empty() )
    {
      x_i = (importVector.at(eventID).x) * cm;
      xPrime_i = importVector.at(eventID).xPrime * milliradian;
      y_i = (importVector.at(eventID).y) * cm;
      yPrime_i = importVector.at(eventID).yPrime * milliradian;
      pTotal = (importVector.at(eventID).pTotal) * GeV;
      sX_i = importVector.at(eventID).spin1;
      sY_i = importVector.at(eventID).spin2;
      sZ_i = importVector.at(eventID).spin3;
    }
  else
    G4cout << "\nWARNING: particle importVector was empty! Nothing injected!\n";
  
  {
    
    // Transform from inflector coordinates (at the upstream end of the
    // inflector) into world coordinates
    G4double point3_x = apRad*cos(delta) - (2*L*sin(delta+gamma));
    G4double point3_z = -apRad*sin(delta) - (2*L*cos(delta+gamma));
    
    x_w = point3_x - (x_i*cos(delta+gamma));
    y_w = y_i;
    z_w = point3_z + (x_i*sin(delta+gamma));
    
    // Using the momentum vectors angles with the x and y inflector
    // axes, calculate the momentum vector components in inflector
    // coordinates
    pY_i = pTotal * sin(xPrime_i);  
    pZ_i = ( pTotal * cos(yPrime_i) ) * cos(xPrime_i);
    pX_i = pZ_i * sin(xPrime_i);

    // Perform a rotation to transform the momentum vector components
    // from inflector coordinates to world coordinates.  
    G4double rAngle = delta + gamma;

    pX_w = ( pX_i * cos(rAngle) ) + ( pZ_i * sin(rAngle) );
    pY_w = pY_i;
    pZ_w = ( pX_i * (-1) * sin(rAngle) ) + ( pZ_i * cos(rAngle) );
    
    
    // Perfom a rotation to transform the imported spin components into
    // world spin coordinates
  }
  
  // Generate the position and time necessary to create the particle vertex
  // particle_position = posGenerator -> GenerateOne();
  particle_position = G4ThreeVector(x_w, y_w, z_w);
  particle_time = timeGenerator->GenerateOne(particle_definition);
  
  G4PrimaryVertex *vertex = new G4PrimaryVertex(particle_position, particle_time);
  
  // Set the remaining particle parameters
  for(G4int i=0; i<NumberOfParticlesToBeGenerated; i++)
    {
      G4double mass =  particle_definition->GetPDGMass();
      
      particle_energy = std::sqrt(pTotal*pTotal + mass*mass);      
      particle_momentum_direction = G4ParticleMomentum(pX_w,pY_w,pZ_w);
      
      if(verbosityLevel > 1)
	{
	  G4cout << "Particle name: "<<particle_definition->GetParticleName() << G4endl; 
	  G4cout << "       Energy: "<<particle_energy << G4endl;
	  G4cout << "     Position: "<<particle_position<< G4endl; 
	  G4cout << "    Direction: "<<particle_momentum_direction << G4endl;
	}
      
      G4PrimaryParticle* particle =
	new G4PrimaryParticle(particle_definition, pX_w, pY_w, pZ_w);
      particle->SetMass( mass );
      particle->SetCharge( particle_charge );
      particle->SetPolarization(0, 0, 0);
      //      particle->SetPolarization(Sx_w, Sy_w, Sz_w);
      
      vertex->SetPrimary( particle );
      
      // Set bweight equal to the multiple of all non-zero weights
      particle_weight = biasRndm->GetBiasWeight();
      // pass it to primary particle
      particle->SetWeight(particle_weight);
    }
  
  // Set the distribution weight and add particle to the primary vertex
  vertex->SetWeight(particle_weight);
  evt->AddPrimaryVertex( vertex );
  if(verbosityLevel > 1)
    G4cout << " Primary Vertex generated !"<< G4endl;   
  
}

 
void gm2ringsim::G2SingleParticleSource::UseInternallyGenerateParticles(G4Event *evt)
{
  // Generate a position
  particle_position = posGenerator->GenerateOne();
  // Generate a time
  particle_time = timeGenerator->GenerateOne(particle_definition);
  
  // Create a new vertex
  G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);
  
  for( G4int i=0; i<NumberOfParticlesToBeGenerated; i++ ) {
    // Generate an angular distribution value
    particle_momentum_direction = angGenerator->GenerateOne();
    // Generate an energy value
    particle_energy = eneGenerator->GenerateOne(particle_definition);
    
    if(verbosityLevel >= 2)
      G4cout << "Creating primaries and assigning to vertex" << G4endl;

    // Create new primaries and set them to the vertex
    G4double mass =  particle_definition->GetPDGMass();
    G4double energy = particle_energy + mass;
    G4double pmom = std::sqrt(energy*energy-mass*mass);
    G4double px = pmom*particle_momentum_direction.x();
    G4double py = pmom*particle_momentum_direction.y();
    G4double pz = pmom*particle_momentum_direction.z();

    if(verbosityLevel > 1){
      G4cout << "Particle name: "<<particle_definition->GetParticleName() << G4endl; 
      G4cout << "       Energy: "<<particle_energy << G4endl;
      G4cout << "     Position: "<<particle_position<< G4endl; 
      G4cout << "    Direction: "<<particle_momentum_direction << G4endl;
    }

    G4PrimaryParticle* particle =
      new G4PrimaryParticle(particle_definition,px,py,pz);
    particle->SetMass( mass );
    particle->SetCharge( particle_charge );
    particle->SetPolarization(particle_polarization.x(),
			      particle_polarization.y(),
			      particle_polarization.z());
    
    if(verbosityLevel > 1)
      G4cout << " Polarization: " << particle->GetPolarization() << G4endl;

    vertex->SetPrimary( particle );
      
    // Set bweight equal to the multiple of all non-zero weights
    particle_weight = biasRndm->GetBiasWeight();
    // Pass it to primary particle
    particle->SetWeight(particle_weight);
  }
  // Now pass the weight to the primary vertex
  vertex->SetWeight(particle_weight);
  evt->AddPrimaryVertex( vertex );
  if(verbosityLevel > 1)
    G4cout << " Primary Vertex generated !"<< G4endl;   
}



void gm2ringsim::G2SingleParticleSource::SetParticleDefinition
  (G4ParticleDefinition* aParticleDefinition)
{
  particle_definition = aParticleDefinition;
  particle_charge = particle_definition->GetPDGCharge();
}


// UI function to set the verbosity
void gm2ringsim::G2SingleParticleSource::SetVerbosity(int vL)
{
  verbosityLevel = vL;
  posGenerator->SetVerbosity(vL);
  angGenerator->SetVerbosity(vL);
  eneGenerator->SetVerbosity(vL);
  timeGenerator->SetVerbosity(vL);
  G4cout << "Verbosity Set to: " << verbosityLevel << G4endl;
}

// UI function to determine source of to-br-injected particles
void gm2ringsim::G2SingleParticleSource::SetImportFlag(G4bool flag)
{ importFlag = flag; }

// UI function to set the type of particle import file
void gm2ringsim::G2SingleParticleSource::SetImportFileType(G4String type)
{ importFileType = type; }

// UI function to set the structure of the particle import file
void gm2ringsim::G2SingleParticleSource::SetImportFileStructure(G4String /*type*/)
{
  G4cout << "\n" << "**** WARNING ****: THIS FUNCTION HAS NOT YET BEEN IMPLEMENTED!!"
	 << "\n" << "                   YOU, THEREFORE, HAVE ACCOMPLISHED NOTHING BY"
	 << "\n" << "                   ATTEMPTING TO USE THIS COMMAND!!\n\n";   
}

// UI function to load the specified particle file
void gm2ringsim::G2SingleParticleSource::LoadImportFile(G4String fName)
{
  // Create the stream and open the user-specified file
  std::ifstream importParticles;
  importParticles.open(fName);
    
  // Test to make sure that the file successfully opened
  if( !importParticles.fail() )
    { 
      // Determine the appropriate file type.  Then, line by line
      // inside the while loop, set the struct parameters and push a
      // completed struct into the importVector.  When the stream
      // reaches the end-of-file character, loading is terminated

      if(importFileType == "turtle")
	while(importParticles.good())
	  {
	    if( importParticles.eof() ){
	      importParticles.close();
	      break;
	    } 

	    importParticles >> import.x
			    >> import.xPrime 
			    >> import.y 
			    >> import.yPrime 
			    >> import.pTotal 
			    >> import.null1
			    >> import.null2;

	    importVector.push_back(import);
	    
	  }
      
      else if(importFileType == "btraf")
	while(importParticles.good())
	  {
	    if( importParticles.eof() ){
	      importParticles.close();
	      break;
	    } 
	    importParticles  >> import.pTotal
			     >> import.x
			     >> import.xPrime
			     >> import.y 
			     >> import.yPrime
			     >> import.spin1 
			     >> import.spin2 
			     >> import.spin3;
	    
	    importVector.push_back(import);
	    
	  }	
      
      else if(importFileType == "userDefined")
	G4cout << "\nERROR: This function is not yet available!!\n\n";
      
      else
	G4cout << "\nERROR: User specified file type does not exist!!\n\n";
      
      // Get and printout the number of particles that have been
      // successfully loaded
      G4int size = importVector.size();
      if(size>0)
	G4cout << "\n\n...Particle loading successful!\n\n"
	       << "There are "<< size << " particles ready for launch, Cap'n!\n\n";

      // Set the flag for a successfully opened file
      fileSuccessfullyOpened = true;
      importParticles.close();
    }
  
  else
    G4cout << "\n\nERROR: g2MIGTRACE was unable to open the specified file!!\n\n";
}


void gm2ringsim::G2SingleParticleSource::ClearImportData()
{ 
  // If the vector has data, clear it all out!
  if(importVector.size() != 0){
    importVector.clear();
    G4cout << "\nAll currently particles have been deleted!\n\n";
  }
  else
    G4cout << "\nThere are no particles to clear!\n\n";
}









