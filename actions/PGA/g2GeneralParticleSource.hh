#ifndef G2GENERALPARTICLESOUCE_HHa
#define G2GENERALPARTICLESOUCE_HHa

/** @file g2GeneralParticleSource.hh

    g2GeneralParticleSource is a modified and updated version of the
    GPS included with the distribution of GEANT4, details below.  I
    will add brief code documentation throughout the g2GPS files, with
    more detailed comments in the sections more relevant to g2MIGTRACE.
    
    Thanks to the good people at QinetiQ ltd. for their generously
    allowing the use of their code...by which I mean I stole it
    unabashedly...
    
    g2GPS uses the following modified modules:
    - g2GeneralParticleSource -> expanded from original G4
    - g2GeneralParticleSourceMessenger -> expanded and completely overhauled
    - g2SingleParticleSource -> expanded and completely overhauled
    - g2SPSTimeDistribution -> newly scripted for g2GPS
    

    Original authorship information:

    - MODULE:       G4GeneralParticleSource

    - Version:      2.0
    - Date:         5/02/04
    - Author:       Fan Lei 
    - Organisation: QinetiQ ltd.
    - Customer:     ESA/ESTEC

    Documentation avaialable at http://reat.space.qinetiq.com/gps

    - Zach Hartwig 2005
    - Moved to ART 2012 Brendan Kiburg
*/

#include "Geant4/globals.hh"
#include <vector>
#include <string>

#include "Geant4/G4Event.hh"
#include "gm2ringsim/actions/PGA/g2SingleParticleSource.hh"
#include "gm2ringsim/actions/PGA/G2GPSSettings.hh"

/** Provides a modified General Particle Source to include time
    distribution */
class g2GeneralParticleSource : public G4VPrimaryGenerator
{
  //
public:
  
  g2GeneralParticleSource();
  ~g2GeneralParticleSource();
  
  void GeneratePrimaryVertex(G4Event*);
  
  G4int GetNumberofSource() { return G4int(sourceVector.size()); };
  void ListSource();
  void SetCurrentSourceto(G4int) ;
  void SetCurrentSourceIntensity(G4double);
  g2SingleParticleSource* GetCurrentSource() {return currentSource;};
  G4int GetCurrentSourceIndex() { return currentSourceIdx; };
  G4double GetCurrentSourceIntensity() { return sourceIntensity[currentSourceIdx]; };
  void ClearAll();
  void AddaSource (G4double);
  void DeleteaSource(G4int);

  // Set the verbosity level.
  void SetVerbosity(G4int i) {currentSource->SetVerbosity(i);} ;

  // Set if multiple vertex per event.
  void SetMultipleVertex(G4bool av) {multiple_vertex = av;} ;

  // Set the particle species
  void SetParticleDefinition (G4ParticleDefinition * aParticleDefinition) 
    {currentSource->SetParticleDefinition(aParticleDefinition); } ;

  G4ParticleDefinition * GetParticleDefinition () { return currentSource->GetParticleDefinition();} ;

  void SetParticleCharge(G4double aCharge) { currentSource->SetParticleCharge(aCharge); } ;

  // Set polarization
  void SetParticlePolarization (G4ThreeVector aVal) {currentSource->SetParticlePolarization(aVal);};
  G4ThreeVector GetParticlePolarization ()  {return currentSource->GetParticlePolarization();};

  // Set Time.
  void SetParticleTime(G4double aTime)  { currentSource->SetParticleTime(aTime); };
  G4double GetParticleTime()  { return currentSource->GetParticleTime(); };

  void SetNumberOfParticles(G4int i)  { currentSource->SetNumberOfParticles(i); };
  //
  G4int GetNumberOfParticles() { return currentSource->GetNumberOfParticles(); };
  G4ThreeVector GetParticlePosition()  { return currentSource->GetParticlePosition();};
  G4ThreeVector GetParticleMomentumDirection()  { return currentSource->GetParticleMomentumDirection();};
  G4double GetParticleEnergy()  {return currentSource->GetParticleEnergy();};



  ///////////////////////////////////////////
  ///////////////////////////////////////////
  void SetBypassGPS(G4bool b) {bypass_gps=b;}
  G4bool IsBypassed() {return bypass_gps;}

  void SetMuonGasGun( G4bool aBool ){ useMuonGasGun=aBool; }
  G4bool GetMuonGasGun( ){ return useMuonGasGun; }

  void SetMuonGasGunLongitudinalDistr( G4String aString ){ muonGasGunLongitudinalDistrDefault=aString; }
  G4String GetMuonGasGunLongitudinalDistr( ){ return muonGasGunLongitudinalDistrDefault; }

  void SetMuonGasGunTransverseDistr( G4String aString ){ muonGasGunTransverseDistrDefault=aString; }
  G4String GetMuonGasGunTransverseDistr( ){ return muonGasGunTransverseDistrDefault; }
  ///////////////////////////////////////////
  ///////////////////////////////////////////



private:

  void IntensityNormalization();

private:
  gm2ringsim::G2GPSSettings settings_;
  G4bool multiple_vertex;
  G4bool normalised;
  ///////////////////////////////////////////////////
  G4bool bypass_gps;
  ///////////////////////////////////////////////////
  G4bool useMuonGasGun;
  G4String muonGasGunLongitudinalDistrDefault;
  G4String muonGasGunTransverseDistrDefault;
  ///////////////////////////////////////////////////
  G4int currentSourceIdx;
  g2SingleParticleSource* currentSource;
  std::vector <g2SingleParticleSource*> sourceVector;
  std::vector <G4double> sourceIntensity;
  std::vector <G4double>sourceProbability;
  
  
};

#endif
