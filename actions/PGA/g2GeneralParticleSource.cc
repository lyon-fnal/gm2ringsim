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
  particleTable_ = G4ParticleTable::GetParticleTable();
  

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

  implementSettings();

}
  
  void g2GeneralParticleSource::implementSettings(){
    
    setParticleDefinition(settings_.particle);
    setPosType(settings_.pos_type);
    setPosRot1(settings_.pos_rot1);
    setPosRot2(settings_.pos_rot2);    
    
    setPosShape(settings_.pos_shape);
    setCentreCoords(settings_.pos_centre);
    setHalfX(settings_.pos_halfx);
    setHalfY(settings_.pos_halfy);
    setPosSigmaX(settings_.pos_sigma_x);
    setPosSigmaY(settings_.pos_sigma_y);


    setAngRot1(settings_.ang_rot1);
    setAngRot2(settings_.ang_rot2);
    setAngType(settings_.ang_type);
    setAngSigmaX(settings_.ang_sigma_x); 
    setAngSigmaY(settings_.ang_sigma_y);
    
    setEnergyType(settings_.ene_type);
    setMonoEnergy(settings_.ene_mono);
    setEnergySigma(settings_.ene_sigma);

    setTimeType(settings_.tType);
    setTimeMono(settings_.tMono);
    setTimeSigma(settings_.tSigma);

    

    

  settings_.print();
}


void g2GeneralParticleSource::setParticleDefinition(std::string newValues){
  //FIXME: implement fShootIon
  //if (newValues =="ion") {
  //  fShootIon = true;
  //} else {
  //fShootIon = false;
  if (newValues == "muminus")
    newValues = "mu-";
  if (newValues == "muplus")
    newValues = "mu+";
  
  G4ParticleDefinition* pd = particleTable_->FindParticle(newValues);
  if(pd != NULL)
    SetParticleDefinition( pd ); 
  //}

}


void g2GeneralParticleSource::setPosType(std::string newValues){
  //else if(command == typeCmd1)
  currentSource->GetPosDist()->SetPosDisType(newValues);
}

void g2GeneralParticleSource::setPosShape(std::string shape){
  //else if(command == typeCmd1)
  currentSource->GetPosDist()->SetPosDisShape(shape);
}


void g2GeneralParticleSource::setCentreCoords(std::vector<double> pos){
  G4ThreeVector p(pos[0],pos[1],pos[2]);
  currentSource->GetPosDist()->SetCentreCoords(p);
  
}

void g2GeneralParticleSource::setPosRot1(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
  currentSource->GetPosDist()->SetPosRot1(r);
}
void g2GeneralParticleSource::setPosRot2(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
  currentSource->GetPosDist()->SetPosRot2(r);
}

// void g2GeneralParticleSource::
void g2GeneralParticleSource::setHalfX(double hx){
  
  currentSource->GetPosDist()->SetHalfX(hx);
}

void g2GeneralParticleSource::setHalfY(double hy){

  currentSource->GetPosDist()->SetHalfY(hy);
}

void g2GeneralParticleSource::setPosSigmaX(double sigx){
  currentSource->GetPosDist()->SetBeamSigmaInX(sigx);
}
 
void g2GeneralParticleSource::setPosSigmaY(double sigy){
  currentSource->GetPosDist()->SetBeamSigmaInY(sigy);
}


void g2GeneralParticleSource::setAngRot1(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
  G4String a = "angref1";
  currentSource->GetAngDist()->DefineAngRefAxes(a,r);
  
}
void g2GeneralParticleSource::setAngRot2(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
   G4String a = "angref2";
  currentSource->GetAngDist()->DefineAngRefAxes(a,r);
  
}
  
void g2GeneralParticleSource::setAngType(std::string ang_type){
  currentSource->GetAngDist()->SetAngDistType(ang_type);
}
void g2GeneralParticleSource::setAngSigmaX(double sigx){
  currentSource->GetAngDist()->SetBeamSigmaInAngX(sigx);

 }
void g2GeneralParticleSource::setAngSigmaY(double sigy){
  currentSource->GetAngDist()->SetBeamSigmaInAngY(sigy); }

void g2GeneralParticleSource::setEnergyType(std::string eType){
  currentSource->GetEneDist()->SetEnergyDisType(eType);
}
void g2GeneralParticleSource::setMonoEnergy(double newValues){
  currentSource->GetEneDist()->SetMonoEnergy(newValues);//monoenergyCmd1->GetNewDoubleValue(newValues));
  
}
void g2GeneralParticleSource::setEnergySigma(double eSigma){
  currentSource->GetEneDist()->SetBeamSigmaInE(eSigma);
}

void g2GeneralParticleSource::setTimeType(std::string tType){
  currentSource->GetTimeDist()->SetTimeDisType(tType);
 }
void g2GeneralParticleSource::setTimeMono(double tMono){ 
  currentSource->GetTimeDist()->SetTimeMono(tMono);
}
void g2GeneralParticleSource::setTimeSigma(double tSigma){
  currentSource->GetTimeDist()->SetBeamSigmaInT(tSigma);
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
