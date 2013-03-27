#ifndef G2GENERALPARTICLESOUCE_HHa
#define G2GENERALPARTICLESOUCE_HHa

/** @file G2GeneralParticleSource.hh

    G2GeneralParticleSource is a modified and updated version of the
    GPS included with the distribution of GEANT4, details below.  I
    will add brief code documentation throughout the g2GPS files, with
    more detailed comments in the sections more relevant to g2MIGTRACE.
    
    Thanks to the good people at QinetiQ ltd. for their generously
    allowing the use of their code...by which I mean I stole it
    unabashedly...
    
    G2GPS uses the following modified modules:
    - G2GeneralParticleSource -> expanded from original G4
    - G2GeneralParticleSourceMessenger -> expanded and completely overhauled
    - G2SingleParticleSource -> expanded and completely overhauled
    - G2SPSTimeDistribution -> newly scripted for G2GPS
    

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
#include "Geant4/G4ParticleTable.hh"
#include "gm2ringsim/actions/PGA/G2SingleParticleSource.hh"
#include "gm2ringsim/actions/PGA/G2GPSSettings.hh"

/** Provides a modified General Particle Source to include time
    distribution */

namespace gm2ringsim {
  class G2GeneralParticleSource : public G4VPrimaryGenerator
  {
    //
  public:
    
    G2GeneralParticleSource();
    ~G2GeneralParticleSource();
    
    void GeneratePrimaryVertex(G4Event*) override;
    
    G4int GetNumberofSource() { return G4int(sourceVector.size()); };
    void ListSource();
    void SetCurrentSourceto(G4int) ;
    void SetCurrentSourceIntensity(G4double);
    G2SingleParticleSource* GetCurrentSource() {return currentSource;};
    G4int GetCurrentSourceIndex() { return currentSourceIdx; };
    G4double GetCurrentSourceIntensity() { return sourceIntensity[currentSourceIdx]; };
    void ClearAll();
    void AddaSource (G4double);
    void DeleteaSource(G4int);
    
    // G2GeneralParticleSourceMessengerSettings
    void implementSettings();
  
    void setParticleDefinition(std::string newValues);
    void setPosType(std::string newValues);
    
    void setPosRot1(std::vector<double> rot1);
    void setPosRot2(std::vector<double> rot2);
    
    void setPosShape(std::string shape);
    void setCentreCoords(std::vector<double> pos);
    void setPosRadius(double rad);
    void setHalfX(double hx);
    void setHalfY(double hy);
    
    void setPosSigmaX(double sigx);
    void setPosSigmaY(double sigy);
    void setPosSigmaR(double sigr);
    
    void setAngRot1(std::vector<double> rot1);
    void setAngRot2(std::vector<double> rot2);
    void setAngType(std::string ang_type);
    void setAngSigmaX(double sigx);
    void setAngSigmaY(double sigy);
    void setAngSigmaR(double sigr);
    
    void setEnergyType(std::string eType);
    void setMonoEnergy(double newValues);
    void setEnergySigma(double eSigma);
    
    void setTimeType(std::string tType);
    void setTimeMono(double tMono);
    void setTimeSigma(double tSigma);
    
    void setPolarization(std::vector<double> pol);
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
    G4ParticleTable *particleTable_;
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
    G2SingleParticleSource* currentSource;
    std::vector <G2SingleParticleSource*> sourceVector;
    std::vector <G4double> sourceIntensity;
    std::vector <G4double>sourceProbability;
    
    
  };
  
} //namespace gm2ringsim

#endif
