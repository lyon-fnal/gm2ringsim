/** @file G2GeneralParticleSource.cc

    Provides the implementations of the modified General Particle
    Source.

    - Zach Hartwig 2005
    - Moved to ART 2012, Brendan Kiburg
*/


#include "Geant4/G4Event.hh"
#include "Geant4/Randomize.hh"
#include "gm2ringsim/actions/PGA/G2GeneralParticleSource.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "gm2ringsim/actions/SpinTrackingSettings.hh"

#include "Geant4/G4ProcessTable.hh"

#include "Geant4/G4MuonPlus.hh"
#include "Geant4/G4MuonMinus.hh"
#include "Geant4/G4PionPlus.hh"
#include "Geant4/G4PionMinus.hh"

#include "Geant4/G4ThreeVector.hh"

gm2ringsim::G2GeneralParticleSource::G2GeneralParticleSource(std::string set)
  : settings_(set), //This should be in the fcl:
    multiple_vertex(false), 
    bypass_gps(false),
    muonGasGunLongitudinalDistrDefault("e989"),
    muonGasGunTransverseDistrDefault("e989")
{
  
  particleTable_ = G4ParticleTable::GetParticleTable();
  

  sourceVector.clear();
  sourceIntensity.clear();
  sourceProbability.clear();
  currentSource = new G2SingleParticleSource();
  sourceVector.push_back(currentSource);
  sourceIntensity.push_back(1.);
  currentSourceIdx = G4int(sourceVector.size() - 1);
  consistencyChecked = false;
  //theMessenger = new G2GeneralParticleSourceMessenger(this);
  //theMessenger->SetParticleGun(currentSource);
  IntensityNormalization();

  implementSettings();
  
  printf("Particle Lifetime after implementSettings()== %f\n", (double)(currentSource->GetParticleDefinition()->GetPDGLifeTime()));


}
  
void gm2ringsim::G2GeneralParticleSource::implementSettings(){
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
  if (settings_.contains("polarization"))
    setPolarization(settings_.polarization);

  settings_.print();
}

void gm2ringsim::G2GeneralParticleSource::checkForConsistency(){
  // This function checks that we actually have a polarized gun or pion V-A decay and subsequent V-A muon decay if we 
  // do spin tracking and vice versa. If any of these options are on, we want them all on or we might have additional CPU usage for nothing
  
  // Let's check which of the following are switched on:
  // 
  // 1) Spin tracking
  SpinTrackingSettings sts_("SpinTracking");
  bool const spin_tracking = sts_.spinTrackingEnabled;

  // 2) muon V-A decay
  bool muonVAdecay = false;
  G4ProcessTable* table = G4ProcessTable::GetProcessTable();
  // for mu+
  if(table->FindProcess("DecayWithSpin",G4MuonPlus::MuonPlus()) && 
     table->FindProcess("DecayWithSpin",G4MuonMinus::MuonMinus())){
    muonVAdecay = true;
  }

  // 3) muon gun with polarization
  bool polarizedMuonGun = false;
  //    a) do we have a muon gun? and b) is it polarized?
  if(GetParticleDefinition() == G4MuonPlus::MuonPlus() || GetParticleDefinition() == G4MuonMinus::MuonMinus()){
    if(GetParticlePolarization() != G4ThreeVector(0., 0., 0.)) polarizedMuonGun = true;
  }

  // 4) pion gun with V-A decay 
  bool polarizedPionGun = false;
  //    a) do we have a pion gun? and b) is pion decay producing polarized muons?
  if(GetParticleDefinition() == G4PionPlus::PionPlus() || GetParticleDefinition() == G4PionMinus::PionMinus()){
    // FIXME: Not sure if the following will identify that the pion V-A decay is on. Will need to revisit once the V-A decay has been implemented.
    if(table->FindProcess("DecayWithSpin",G4PionPlus::PionPlus()) && 
       table->FindProcess("DecayWithSpin",G4PionMinus::PionMinus())){
      polarizedPionGun = true;
    }
  }
  
  G4String message;
  message = "";
  bool throwException = false;
  // Now let's check all cases
  if(muonVAdecay){
    message += "Muon V-A decay is on but\n";
    if(!spin_tracking){
      throwException = true;
      message += "    - no spin tracking set\n";
    }
    
    if(!polarizedMuonGun && !polarizedPionGun){
      throwException = true;
      message += "    - neither polarized muon gun nor pion gun with V-A pion decay\n";
    }
  } // muonDecay is on
  if(throwException) throw cet::exception("G2GeneralParticleSource") << message.data();
  
  message = "";
  if(spin_tracking){ 
    message += "Spin tracking set but\n" ;
    if(!muonVAdecay){
      throwException = true;
      message += "    - no muon V-A decay\n";
    }
    
    if(!polarizedMuonGun && !polarizedPionGun){
      throwException = true;
      message += "    - neither polarized muon gun nor pion gun with V-A pion decay\n";
    }
  } // spin tracking is on
  if(throwException) throw cet::exception("G2GeneralParticleSource") << message.data();

  message = "";
  if(polarizedMuonGun){
    message += "Polarized muon gun but\n" ;
   if(!spin_tracking){
      throwException = true;
      message += "    - no spin tracking set\n";
    }

    if(!muonVAdecay){
      throwException = true;
      message += "    - no muon V-A decay\n";
    } 
  }
  if(throwException) throw cet::exception("G2GeneralParticleSource") << message.data();

  message = "";
  if(polarizedPionGun){
    message += "Pion gun with V-A pion decay chosen but\n" ;
    if(!spin_tracking){
      throwException = true;
      message += "    - no spin tracking set\n";
    }

    if(!muonVAdecay){
      throwException = true;
      message += "    - no muon V-A decay\n";
    } 
  }
  if(throwException) throw cet::exception("G2GeneralParticleSource") << message.data();

  
  return;
}

void gm2ringsim::G2GeneralParticleSource::setParticleDefinition(std::string newValues){
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
  if (!strcmp(newValues.c_str(),"piminus"))
    { printf("identified that we have piminus particles\n");
      newValues = "pi-";
    }
  if (!strcmp(newValues.c_str(),"piplus"))
    { printf("identified that we have piplus particles\n");
      newValues = "pi+";
    }
    
  const G4String name = G4String(newValues);
  G4ParticleDefinition* pd = particleTable_->FindParticle(name);
  if(pd != NULL)
    {
      mf::LogDebug("G2GeneralParticleSource") << "setting particle to "<<newValues;
      SetParticleDefinition( pd ); 
    }
  //}

}


void gm2ringsim::G2GeneralParticleSource::setPosType(std::string newValues){
  //else if(command == typeCmd1)
  currentSource->GetPosDist()->SetPosDisType(newValues);
}

void gm2ringsim::G2GeneralParticleSource::setPosShape(std::string shape){
  //else if(command == typeCmd1)
  currentSource->GetPosDist()->SetPosDisShape(shape);
}


void gm2ringsim::G2GeneralParticleSource::setCentreCoords(std::vector<double> pos){
  G4ThreeVector p(pos[0],pos[1],pos[2]);
  currentSource->GetPosDist()->SetCentreCoords(p);
  
}

void gm2ringsim::G2GeneralParticleSource::setPosRot1(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
  currentSource->GetPosDist()->SetPosRot1(r);
}
void gm2ringsim::G2GeneralParticleSource::setPosRot2(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
  currentSource->GetPosDist()->SetPosRot2(r);
}

// void G2GeneralParticleSource::
void gm2ringsim::G2GeneralParticleSource::setHalfX(double hx){
  
  currentSource->GetPosDist()->SetHalfX(hx);
}

void gm2ringsim::G2GeneralParticleSource::setHalfY(double hy){

  currentSource->GetPosDist()->SetHalfY(hy);
}

void gm2ringsim::G2GeneralParticleSource::setPosSigmaX(double sigx){
  currentSource->GetPosDist()->SetBeamSigmaInX(sigx);
}
 
void gm2ringsim::G2GeneralParticleSource::setPosSigmaY(double sigy){
  currentSource->GetPosDist()->SetBeamSigmaInY(sigy);
}

void gm2ringsim::G2GeneralParticleSource::setPosSigmaR(double sigr){
  currentSource->GetPosDist()->SetBeamSigmaInR(sigr);
}

void gm2ringsim::G2GeneralParticleSource::setPosRadius(double rad){
  currentSource->GetPosDist()->SetRadius(rad);
}

void gm2ringsim::G2GeneralParticleSource::setAngRot1(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
  G4String a = "angref1";
  currentSource->GetAngDist()->DefineAngRefAxes(a,r);
  
}
void gm2ringsim::G2GeneralParticleSource::setAngRot2(std::vector<double> rot){
  G4ThreeVector r(rot[0],rot[1],rot[2]);
   G4String a = "angref2";
  currentSource->GetAngDist()->DefineAngRefAxes(a,r);
  
}
  
void gm2ringsim::G2GeneralParticleSource::setAngType(std::string ang_type){
  currentSource->GetAngDist()->SetAngDistType(ang_type);
}
void gm2ringsim::G2GeneralParticleSource::setAngSigmaX(double sigx){
  currentSource->GetAngDist()->SetBeamSigmaInAngX(sigx);

 }
void gm2ringsim::G2GeneralParticleSource::setAngSigmaY(double sigy){
  currentSource->GetAngDist()->SetBeamSigmaInAngY(sigy); }

void gm2ringsim::G2GeneralParticleSource::setAngSigmaR(double sigr){
  currentSource->GetAngDist()->SetBeamSigmaInAngR(sigr); 
  //  currentSource->GetAngDist()->SetBeamSigmaInAngR(
}

void gm2ringsim::G2GeneralParticleSource::setEnergyType(std::string eType){
  currentSource->GetEneDist()->SetEnergyDisType(eType);
}
void gm2ringsim::G2GeneralParticleSource::setMonoEnergy(double newValues){
  currentSource->GetEneDist()->SetMonoEnergy(newValues);//monoenergyCmd1->GetNewDoubleValue(newValues));
  
}
void gm2ringsim::G2GeneralParticleSource::setEnergySigma(double eSigma){
  currentSource->GetEneDist()->SetBeamSigmaInE(eSigma);
}

void gm2ringsim::G2GeneralParticleSource::setTimeType(std::string tType){
  currentSource->GetTimeDist()->SetTimeDisType(tType);
 }
void gm2ringsim::G2GeneralParticleSource::setTimeMono(double tMono){ 
  currentSource->GetTimeDist()->SetTimeMono(tMono);
}
void gm2ringsim::G2GeneralParticleSource::setTimeSigma(double tSigma){
  currentSource->GetTimeDist()->SetBeamSigmaInT(tSigma);
 }

void gm2ringsim::G2GeneralParticleSource::setPolarization(std::vector<double> pol){
  G4ThreeVector p(pol[0],pol[1],pol[2]);
  SetParticlePolarization(p);  
}

gm2ringsim::G2GeneralParticleSource::~G2GeneralParticleSource()
{
  //  delete theMessenger;
}

void gm2ringsim::G2GeneralParticleSource::AddaSource(G4double aV)
{
  currentSource = new G2SingleParticleSource();
  //theMessenger->SetParticleGun(currentSource);
  sourceVector.push_back(currentSource);
  sourceIntensity.push_back(aV);
  currentSourceIdx = G4int(sourceVector.size() - 1);
  IntensityNormalization();
}

void gm2ringsim::G2GeneralParticleSource::IntensityNormalization()
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

void gm2ringsim::G2GeneralParticleSource::ListSource()
{
  G4cout << " The number of particle source is " << sourceIntensity.size() << G4endl;
  for (size_t i = 0 ; i < sourceIntensity.size(); i++) 
    G4cout << "   source " << i << " intensity is " << sourceIntensity[i] << G4endl;
}

void gm2ringsim::G2GeneralParticleSource::SetCurrentSourceto(G4int aV)
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

void gm2ringsim::G2GeneralParticleSource::SetCurrentSourceIntensity(G4double aV)
{
  sourceIntensity[currentSourceIdx] = aV;
  normalised = false;
}

void gm2ringsim::G2GeneralParticleSource::ClearAll()
{
  currentSourceIdx = -1;
  currentSource = 0;
  sourceVector.clear();
  sourceIntensity.clear();
  sourceProbability.clear();
}

void gm2ringsim::G2GeneralParticleSource::DeleteaSource(G4int aV)
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

void gm2ringsim::G2GeneralParticleSource::GeneratePrimaryVertex(G4Event* evt)
{
  // Let's first check that we have a consistent setup wrt spin tracking, polarization and V-A decays
  if(!consistencyChecked){
    checkForConsistency();
    consistencyChecked = true;
  }

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
