// Settings for SpinTracking

#include "gm2ringsim/actions/PGA/G2GPSSettings.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <string>
#include <algorithm>

//FIXME: Need to add defaults

gm2ringsim::G2GPSSettings::G2GPSSettings(std::string const & detName) :
  RunSettingsBase(detName),
  particle( p.get<std::string>("particle", "geantino") ),
  pos_type( p.get<std::string>("pos_type", "Point") ), //"Point Beam Plane Surface Volume");
  pos_rot1( p.get<std::vector<double>>("pos_rot1") ),
  pos_rot2( p.get<std::vector<double>>("pos_rot2") ),
  pos_shape( p.get<std::string>("pos_shape","default")),
  pos_centre( p.get<std::vector<double>>("pos_centre") ),
  pos_halfx( p.get<double>("pos_halfx",0) * mm),
  pos_halfy( p.get<double>("pos_halfy",0) * mm),
  pos_sigma_x( p.get<double>("pos_sigma_x",0) * mm),
  pos_sigma_y( p.get<double>("pos_sigma_y",0) * mm),
  pos_radius(p.get<double>("pos_radius",0) * mm),
  pos_sigma_r(p.get<double>("pos_sigma_r",0) * mm),
  ang_rot1( p.get<std::vector<double>>("ang_rot1") ),
  ang_rot2( p.get<std::vector<double>>("ang_rot2") ),
  ang_type( p.get<std::string>("ang_type", "default") ),
  ang_sigma_x( p.get<double>("ang_sigma_x",0.0) * mrad),
  ang_sigma_y( p.get<double>("ang_sigma_y",0.0) * mrad),
  ang_sigma_r( p.get<double>("ang_sigma_r",0.0) * mrad),
  ene_type( p.get<std::string>("ene_type","default") ),
  ene_mono( p.get<double>("ene_mono",0.0) * GeV),
  ene_sigma( p.get<double>("ene_sigma",0.0) * GeV),
  tType( p.get<std::string>("tType","default")),
  tMono( p.get<double>("tMono",0.0) * ns),
  tSigma( p.get<double>("tSigma",0.0) * ns),
  polarization( p.get<std::vector<double>>("polarization", {0,0,0} ) ),
  parSetKeys_(p.get_keys())
{
  G4cout << "================ G2GPSSettings ===============" << G4endl;
  G4cout << "| Particle = " << particle << G4endl;
  G4cout << "==============================================" << G4endl;

  // radians??
  //FIXME: Need to check angular units
  // for (auto& entry : pos_rot1 ) { entry *= ; }
  // for (auto& entry : pos_rot2 ) { entry *= mm; }
  for (auto& entry : pos_centre ) { entry *= mm; }
  //  for (auto& entry : ang_rot1 ) { entry *= ; }
  //for (auto& entry : ang_rot2 ) { entry *= mm; }
}

void gm2ringsim::G2GPSSettings::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  //  mf::LogInfo("G2GPSSettings") << "G2GPS settings are:"
  std::ostringstream oss;
  oss << "  particle=" << particle << "\n";
  oss << "  pos_type=" << pos_type << "\n";
  oss << "  pos_rot1= "; for (auto entry : pos_rot1) { oss << " " << entry; }; oss << "\n";
  oss << "  pos_rot2= "; for (auto entry : pos_rot2) { oss << " " << entry; }; oss << "\n";
  oss << "  pos_shape=" << pos_shape << "\n";
  oss << "  pos_centre= "; for (auto entry : pos_centre) { oss << " " << entry; }; oss << "\n";
  oss << "  pos_halfx=" << pos_halfx << "\n";
  oss << "  pos_halfy=" << pos_halfy << "\n";
  oss << "  pos_sigma_x=" << pos_sigma_x << "\n";
  oss << "  pos_sigma_y=" << pos_sigma_y << "\n";
  oss << " pos_radius=" <<pos_radius<<"\n";
  oss << " pos_sigmar_r=" <<pos_sigma_r<<"\n";
  oss << "  ang_rot1= "; for (auto entry : ang_rot1) { oss << " " << entry; }; oss << "\n";
  oss << "  ang_rot2= "; for (auto entry : ang_rot2) { oss << " " << entry; }; oss << "\n";
  oss << "  ang_type=" << ang_type << "\n";
  oss << "  ang_sigma_x=" << ang_sigma_x << "\n";
  oss << "  ang_sigma_y=" << ang_sigma_y << "\n";
  oss << "  ang_sigma_r=" << ang_sigma_r << "\n";
  
  oss << "  ene_type=" << ene_type << "\n";
  oss << "  ene_mono=" << ene_mono << "\n";
  oss << "  ene_sigma=" << ene_sigma << "\n";
  oss << "  tType=" << tType << "\n";
  oss << "  tMono=" << tMono << "\n";
  oss << "  tSigma=" << tSigma << "\n";
  oss << "  polarization= "; for (auto entry : polarization) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("G2GPSSettings") << oss.str();
}


bool gm2ringsim::G2GPSSettings::contains(std::string s) {
  // parSetKeys_ is a vector of strings. If it contains s
  // return true, otherwise return false;
  mf::LogDebug("G2GPSSettings")<<"Looking for a string "<<s;

  auto result1 = std::find(parSetKeys_.begin(), parSetKeys_.end(), s);
  
  if (result1 != parSetKeys_.end()) 
    return true;
  
  mf::LogDebug("G2GPSSettings") <<"string "<<s<<" was not found";
  return false;
  
}
