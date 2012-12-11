// Settings for SpinTracking

#include "gm2ringsim/actions/PGA/G2GPSSettings.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <string>

//FIXME: Need to add defaults

gm2ringsim::G2GPSSettings::G2GPSSettings(std::string const & detName) :
  RunSettingsBase(detName),
  particle( p.get<std::string>("particle", "geantino") ),
  pos_type( p.get<std::string>("pos_type", "Point") ), //"Point Beam Plane Surface Volume");
  pos_rot1( p.get<std::vector<double>>("pos_rot1") ),
  pos_rot2( p.get<std::vector<double>>("pos_rot2") ),
  pos_shape( p.get<std::string>("pos_shape")),
  pos_centre( p.get<std::vector<double>>("pos_centre") ),
  pos_halfx( p.get<double>("pos_halfx") * mm),
  pos_halfy( p.get<double>("pos_halfy") * mm),
  pos_sigma_x( p.get<double>("pos_sigma_x") * mm),
  pos_sigma_y( p.get<double>("pos_sigma_y") * mm),
  ang_rot1( p.get<std::vector<double>>("ang_rot1") ),
  ang_rot2( p.get<std::vector<double>>("ang_rot2") ),
  ang_type( p.get<std::string>("ang_type") ),
  ang_sigma_x( p.get<double>("ang_sigma_x") * mrad),
  ang_sigma_y( p.get<double>("ang_sigma_y") * mrad),
  ene_type( p.get<std::string>("ene_type") ),
  ene_mono( p.get<double>("ene_mono") * GeV),
  ene_sigma( p.get<double>("ene_sigma") * GeV),
  tType( p.get<std::string>("tType") ),
  tMono( p.get<double>("tMono") * ns),
  tSigma( p.get<double>("tSigma") * ns)
{
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
oss << "  ang_rot1= "; for (auto entry : ang_rot1) { oss << " " << entry; }; oss << "\n";
oss << "  ang_rot2= "; for (auto entry : ang_rot2) { oss << " " << entry; }; oss << "\n";
oss << "  ang_type=" << ang_type << "\n";
oss << "  ang_sigma_x=" << ang_sigma_x << "\n";
oss << "  ang_sigma_y=" << ang_sigma_y << "\n";
oss << "  ene_type=" << ene_type << "\n";
oss << "  ene_mono=" << ene_mono << "\n";
oss << "  ene_sigma=" << ene_sigma << "\n";
oss << "  tType=" << tType << "\n";
oss << "  tMono=" << tMono << "\n";
oss << "  tSigma=" << tSigma << "\n";
mf::LogInfo("CATEGORY") << oss.str();
}

