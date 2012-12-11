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
  
  printf("Particle Lifetime after implementSettings()== %f\n", (double)(currentSource->GetParticleDefinition()->GetPDGLifeTime()));


}
  
void g2GeneralParticleSource::implementSettings(){
  if (settings_.contains("particle"))
    setParticleDefinition(settings_.particle);
  if (settings_.contains("pos_type"))
    setPosType(settings_.pos_type);
  if (settings_.contains("pos_rot1"))
    setPosRot1(settings_.pos_rot1);
  if (settings_.contains("pos_rot2"))
    setPosRot2(settings_.pos_rot2);    
  if (settings_.contains("pos_shape"))
    setPosShape(settings_.pos_shape);
  if (settings_.contains("pos_centre"))
    setCentreCoords(settings_.pos_centre);
  if (settings_.contains("pos_halfx"))
    setHalfX(settings_.pos_halfx);
  if (settings_.contains("pos_halfy"))
    setHalfY(settings_.pos_halfy);
  if (settings_.contains("pos_sigma_x"))
    setPosSigmaX(settings_.pos_sigma_x);
  if (settings_.contains("pos_sigma_y"))
    setPosSigmaY(settings_.pos_sigma_y);
  if (settings_.contains("pos_radius"))
    setPosRadius(settings_.pos_radius);
  if (settings_.contains("pos_sigma_r"))
    setPosSigmaR(settings_.pos_sigma_r);
  
  if (settings_.contains("ang_rot1"))
    setAngRot1(settings_.ang_rot1);
  if (settings_.contains("ang_rot2"))
    setAngRot2(settings_.ang_rot2);
  if (settings_.contains("ang_type"))
    setAngType(settings_.ang_type);
  if (settings_.contains("ang_sigma_x"))
    setAngSigmaX(settings_.ang_sigma_x); 
  if (settings_.contains("ang_sigma_y"))
    setAngSigmaY(settings_.ang_sigma_y);
  if (settings_.contains("ang_sigma_r"))
    setAngSigmaR(settings_.ang_sigma_r);

  if (settings_.contains("ene_type"))
    setEnergyType(settings_.ene_type);
  if (settings_.contains("ene_mono"))
    setMonoEnergy(settings_.ene_mono);
  if (settings_.contains("ene_sigma"))
    setEnergySigma(settings_.ene_sigma);

  if (settings_.contains("tType"))
    setTimeType(settings_.tType);
  if (settings_.contains("tMono"))
    setTimeMono(settings_.tMono);
  if (settings_.contains("tSigma"))
    setTimeSigma(settings_.tSigma);

    settings_.print();
}


void g2GeneralParticleSource::setParticleDefinition(std::string newValues){
  //FIXME: implement fShootIon
  //if (newValues =="ion") {
  //  fShootIon = true;
  //} else {
  //fShootIon = false;
  
  printf("size of particle table is %d\n",particleTable_->size());

  if (!strcmp(newValues.c_str(),"muminus"))
    { printf("identified that we have muminus particles\n");
      newValues = "mu-";
    }
  if (!strcmp(newValues.c_str(),"muplus"))
    { printf("identified that we have muplus particles\n");
      newValues = "mu+";
    }
  
  const G4String name = "mu-";
  G4ParticleDefinition* pd = particleTable_->FindParticle(name);
  if(pd != NULL)
    {
      std::cout<<"setting particle to "<<newValues<<std::endl;
      SetParticleDefinition( pd ); 
    }
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

void g2GeneralParticleSource::setPosSigmaR(double sigr){
  currentSource->GetPosDist()->SetBeamSigmaInR(sigr);
}

void g2GeneralParticleSource::setPosRadius(double rad){
  currentSource->GetPosDist()->SetRadius(rad);
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

void g2GeneralParticleSource::setAngSigmaR(double sigr){
  currentSource->GetAngDist()->SetBeamSigmaInAngR(sigr); 
  //  currentSource->GetAngDist()->SetBeamSigmaInAngR(
}

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
