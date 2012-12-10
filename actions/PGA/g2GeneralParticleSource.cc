/** @file g2GeneralParticleSource.cc

    Provides the implementations of the modified General Particle
    Source.

    - Zach Hartwig 2005
*/


#include "Geant4/G4Event.hh"
#include "Geant4/Randomize.hh"
#include "gm2ringsim/actions/PGA/g2GeneralParticleSource.hh"

g2GeneralParticleSource::g2GeneralParticleSource()
  : settings_("G2GPSSettings"), //This should be in the fcl:
    multiple_vertex(false), 
    bypass_gps(false),
    muonGasGunLongitudinalDistrDefault("e989"),
    muonGasGunTransverseDistrDefault("e989")
{
  sourceVector.clear();
  sourceIntensity.clear();
  sourceProbability.clear();
  currentSource = new g2SingleParticleSource();
  sourceVector.push_back(currentSource);
  sourceIntensity.push_back(1.);
  currentSourceIdx = G4int(sourceVector.size() - 1);
  //theMessenger = new g2GeneralParticleSourceMessenger(this);
  //theMessenger->SetParticleGun(currentSource);
  IntensityNormalization();
}
  
g2GeneralParticleSource::~g2GeneralParticleSource()
{
  //  delete theMessenger;
}

void g2GeneralParticleSource::AddaSource(G4double aV)
{
  currentSource = new g2SingleParticleSource();
  //theMessenger->SetParticleGun(currentSource);
  sourceVector.push_back(currentSource);
  sourceIntensity.push_back(aV);
  currentSourceIdx = G4int(sourceVector.size() - 1);
  IntensityNormalization();
}

void g2GeneralParticleSource::IntensityNormalization()
{
  G4double total  = 0.;
  size_t i = 0 ;
  for (i = 0; i < sourceIntensity.size(); i++) 
    total += sourceIntensity[i] ;
  //
  sourceProbability.clear();
  sourceProbability.push_back(sourceIntensity[0]/total);
  for ( i = 1 ;  i < sourceIntensity.size(); i++) 
    sourceProbability.push_back(sourceIntensity[i]/total + sourceProbability[i-1]); 
  normalised = true;
} 

void g2GeneralParticleSource::ListSource()
{
  G4cout << " The number of particle source is " << sourceIntensity.size() << G4endl;
  for (size_t i = 0 ; i < sourceIntensity.size(); i++) 
    G4cout << "   source " << i << " intensity is " << sourceIntensity[i] << G4endl;
}

void g2GeneralParticleSource::SetCurrentSourceto(G4int aV)
{
  size_t id = size_t (aV) ;
  if ( id <= sourceIntensity.size() ) {
    currentSourceIdx = aV;
    currentSource = sourceVector[id];
    //theMessenger->SetParticleGun(currentSource);
    //
  } else {
    G4cout << " source index is invalid " << G4endl;
    G4cout << "    it shall be <= " << sourceIntensity.size() << G4endl;
  }
}

void g2GeneralParticleSource::SetCurrentSourceIntensity(G4double aV)
{
  sourceIntensity[currentSourceIdx] = aV;
  normalised = false;
}

void g2GeneralParticleSource::ClearAll()
{
  currentSourceIdx = -1;
  currentSource = 0;
  sourceVector.clear();
  sourceIntensity.clear();
  sourceProbability.clear();
}

void g2GeneralParticleSource::DeleteaSource(G4int aV)
{
  size_t id = size_t (aV) ;
  if ( id <= sourceIntensity.size() ) {
    sourceVector.erase(sourceVector.begin()+aV);
    sourceIntensity.erase(sourceIntensity.begin()+aV);
    normalised = false ;
    if (currentSourceIdx == aV ) { 
	if ( sourceIntensity.size() > 0 ) { 
	  currentSource = sourceVector[0];
	  currentSourceIdx = 1;
	} else {
	  currentSource = 0;
	  currentSourceIdx = -1;
	}
    }	  		
  } else {
    G4cout << " source index is invalid " << G4endl;
    G4cout << "    it shall be <= " << sourceIntensity.size() << G4endl;
  }
} 

void g2GeneralParticleSource::GeneratePrimaryVertex(G4Event* evt)
{
  if (!multiple_vertex){
    if (sourceIntensity.size() > 1) {
      if (!normalised) IntensityNormalization();
      G4double rndm = G4UniformRand();
      size_t i = 0 ;
      while ( rndm > sourceProbability[i] ) i++;
      (currentSource = sourceVector[i]);
    }
  currentSource-> GeneratePrimaryVertex(evt);
  } 
  else {
    for (size_t i = 0; i <  sourceIntensity.size(); i++) {
      sourceVector[i]->GeneratePrimaryVertex(evt); 
    }
  }
}
