#ifndef g2SingleParticleSource_hh
#define g2SingleParticleSource_hh

/** @file g2SingleParticleSource.hh

    g2SingeParticleSource.hh is a modified and updated version of the
    SPS included with the distribution of GEANT4, details below.  Many
    changes have been made to this file to suit g2MIGTRACE needs.
    Much of the remaining original code has been reorganized and
    documented more extensively.  Time distribution functions were
    added, as well as the ability to import TURTLE, BTRAF, and
    userDefined particle files into g2MIGTRACE for particle injection.

    Thanks to the good people at QinetiQ ltd. for their generously
    allowing the use of their code...by which I mean I stole it
    unabashedly...
    
    - MODULE:       G4SingleParticleSource (modified version of G4GPS included
    in GEANT4.6.0 distribution)
    
    - Version:      2.0
    - Date:         5/02/04
    - Author:       Fan Lei 
    - Organisation: QinetiQ ltd.
    - Customer:     ESA/ESTEC

    Documentation avaialable at http://reat.space.qinetiq.com/gps

    - Zach Hartwig 2005
*/

#include "Geant4/G4VPrimaryGenerator.hh"
#include "Geant4/G4ParticleMomentum.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/G4SPSPosDistribution.hh"
#include "Geant4/G4SPSAngDistribution.hh"
#include "Geant4/G4SPSEneDistribution.hh"
#include "gm2ringsim/actions/g2SPSTimeDistribution.hh"
#include "Geant4/G4SPSRandomGenerator.hh"

#include <vector>

/** A modified version of G2SingleParticleSource, to support time
    distributions */
class g2SingleParticleSource : public G4VPrimaryGenerator
{
public:
  g2SingleParticleSource (); 
  ~g2SingleParticleSource ();

  // The three main g2SPS functions
  void GeneratePrimaryVertex(G4Event *evt);
  void UseImportedParticles(G4Event *evt);
  void UseInternallyGenerateParticles(G4Event *evt);

  // When called from g2GPS, returns the requested distribution generators
  G4SPSPosDistribution *GetPosDist() {return posGenerator;};
  G4SPSAngDistribution *GetAngDist() {return angGenerator;};
  G4SPSEneDistribution *GetEneDist() {return eneGenerator;};
  g2SPSTimeDistribution *GetTimeDist() {return timeGenerator;};
  G4SPSRandomGenerator *GetBiasRndm() {return biasRndm;};
  
  // Set the verbosity level.
  void SetVerbosity(G4int);

  // The five functions that implement the importation of particle files
  void SetImportFlag(G4bool newFlag); 
  void SetImportFileType(G4String);
  void SetImportFileStructure(G4String);
  void LoadImportFile(G4String);
  void ClearImportData();

  // Set the particle species
  void SetParticleDefinition (G4ParticleDefinition * aParticleDefinition);
  inline G4ParticleDefinition * GetParticleDefinition () { return particle_definition;} ;

  // Set the particle charge
  inline void SetParticleCharge(G4double aCharge) { particle_charge = aCharge; } ;

  // Set polarization
  inline void SetParticlePolarization (G4ThreeVector aVal) {particle_polarization = aVal;};
  inline G4ThreeVector GetParticlePolarization ()  {return particle_polarization;};

  // Set Time
  inline void SetParticleTime(G4double aTime)  { particle_time = aTime; };
  inline G4double GetParticleTime()  { return particle_time; };

  inline void SetNumberOfParticles(G4int i)  { NumberOfParticlesToBeGenerated = i; };
  //
  inline G4int GetNumberOfParticles() { return NumberOfParticlesToBeGenerated; };
  inline G4ThreeVector GetParticlePosition()  { return particle_position;};
  inline G4ThreeVector GetParticleMomentumDirection()  { return particle_momentum_direction;};
  inline G4double GetParticleEnergy()  {return particle_energy;};

private:
 
  // Create the distribution generators
  G4SPSPosDistribution *posGenerator;
  G4SPSAngDistribution *angGenerator;
  G4SPSEneDistribution *eneGenerator;
  g2SPSTimeDistribution *timeGenerator;
  G4SPSRandomGenerator *biasRndm;
  
  // Other particle properties used in the primary vertex
  G4int                  NumberOfParticlesToBeGenerated;
  G4ParticleDefinition  *particle_definition;
  G4ParticleMomentum     particle_momentum_direction;
  G4double               particle_energy;
  G4double               particle_charge;
  G4ThreeVector          particle_position;
  G4double               particle_time;
  G4ThreeVector          particle_polarization;
  G4double               particle_weight;

  // Verbosity
  G4int verbosityLevel;

  // particle import variables and parameters
  // parameters set by the g2GPS messenger for particle importation
  G4bool importFlag;
  G4String importFileType;
  G4String importFileName;
  G4bool fileSuccessfullyOpened;

  // Position of particles in inflector coordinates.  NOTE: z_i = 0 
  // for all particles (they begin at inflector upstream face)
  G4double x_i, y_i;
  
  // Angles made by momentum vector with inflector x and y axes
  G4double xPrime_i, yPrime_i;

  // Momentum components of momentum vector in inflector coordinates
  G4double pX_i, pY_i, pZ_i;

  // Total scalar value of momentum vector
  G4double pTotal;

  // Spin componenets of spin vector in inflector coordinates
  G4double sX_i, sY_i, sZ_i;
  
  // Position of particles in world coordinates
  G4double x_w, y_w, z_w;
  
  // Momentum components of momentum vector in world coordinates
  G4double pX_w, pY_w, pZ_w;
  
  // Spin components of spin vector in world coordinates
  G4double sX_w, sY_w, sZ_w;
  

  // In order to read in the variables from a file, a struct is
  // created containing appropriate data members for each line
  // (representing one particle) of the file.  Then, a vector of these
  // structs is created to hold all the lines (thus, all the
  // particles).  Later, according to the eventID, each particle can
  // be easily and individually accessed
  struct importContainer
  {
    G4double x;
    G4double xPrime;
    G4double y;
    G4double yPrime;
    G4double pTotal;
    G4double null1;
    G4double null2;
    G4double spin1;
    G4double spin2;
    G4double spin3;
  } import;
  
  std::vector<importContainer> importVector;

};

#endif




