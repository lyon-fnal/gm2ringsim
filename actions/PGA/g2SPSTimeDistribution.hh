#ifndef G2SPSTimeDistribution_hh
#define G2SPSTimeDistribution_hh

/** @file G2SPSTimeDistribution.hh

    The Geant4 General Particle Source doesn't provide distributions
    in time as part of its options.  This file provides declarations
    for a class adding those distributions.

    - Zach Hartwig 2005
*/

#include "Geant4/G4PhysicsOrderedFreeVector.hh"
#include "Geant4/G4ParticleMomentum.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4DataInterpolation.hh"


#include "Geant4/G4SPSRandomGenerator.hh"

/** Provides a time distribution to the General Particle Source gun. 

    G2SPSTimeDistribution is a modified and updated version of the
    G4SPSEneDistribution included with the distribution of GEANT4,
    details below.  This function allows the G2GPS and G2SPS to
    generate certain types of time distributions for particles.
    
    Thanks to the good people at QinetiQ ltd. for their generously
    allowing the use of their code...by which I mean I stole it
    unabashedly...
    
    - MODULE:       G4SPSEneDistribution
    - Version:      2.0
    - Date:         5/02/04
    - Author:       Fan Lei
    - Organisation: QinetiQ ltd.
    - Customer:     ESA/ESTEC
    
    Documentation avaialable at http://reat.space.qinetiq.com/gps
*/
namespace gm2ringsim {
  class G2SPSTimeDistribution
  {
  public:
    G2SPSTimeDistribution (); 
    ~G2SPSTimeDistribution ();

    void SetTimeDisType(G4String);
    inline G4String GetTimeDisType() {return timeDisType;};

    void SetTimeMin(G4double);
    inline G4double GetTimeMin() {return timeMin;} ;

    void SetTimeMax(G4double);
    inline G4double GetTimeMax() {return timeMax;} ;

    void SetTimeMono(G4double);
    void SetBeamSigmaInT(G4double);
    void SetTimeZero(G4double);
    void SetTimeGradient(G4double);
    void SetTimeInterCept(G4double);
  
    void SetBiasRndm(G4SPSRandomGenerator* a) {timeRndm = a; };
  
    void SetVerbosity(G4int a) {verbosityLevel = a; } ;
  
    G4double GenerateOne(G4ParticleDefinition*);
  
private:
  // The following methods generate times according to the spectral
  // parameters defined above.
  void GenerateMonoChronologic();
  void GenerateGaussEnergies();
  void GenerateLinearEnergies(G4bool);

  private:

    G4String timeDisType; // energy dis type Variable  - Mono,Lin,Exp,etc
    G4double timeMono; //Mono-energteic energy
    G4double timeSE ; // Standard deviation for Gaussion distrbution in energy
    G4double timeMin, timeMax; // emin and emax
    G4double timeAlpha, timeZero; // alpha (pow), E0 (exp)
    G4double timeGrad, timeCept; // gradient and intercept for linear spectra
    G4PhysicsOrderedFreeVector UDefEnergyH; // energy hist data
    G4PhysicsOrderedFreeVector IPDFEnergyH;  
    G4bool IPDFEnergyExist, IPDFArbExist, Epnflag;
    G4PhysicsOrderedFreeVector ArbEnergyH; // Arb x,y histogram
    G4PhysicsOrderedFreeVector IPDFArbEnergyH; // IPDF for Arb
    G4PhysicsOrderedFreeVector EpnEnergyH; 
    G4double CDGhist[3]; // cumulative histo for cdg
    G4double BBHist[10001], Bbody_x[10001];
    G4String IntType; // Interpolation type

    G4double               particle_time;
    G4ParticleDefinition*  particle_definition;

    G4SPSRandomGenerator* timeRndm;

    // Verbosity
    G4int verbosityLevel;

    G4PhysicsOrderedFreeVector ZeroPhysVector ; // for re-set only 

  };

}

#endif




