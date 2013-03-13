// G2GPSSettings.hh
// @author Brendan Kiburg 
// @date DEC 2012

#ifndef G2GPS_SETTINGS_HH
#define G2GPS_SETTINGS_HH

#include "artg4/Core/RunSettingsBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  class G2GPSSettings : public artg4::RunSettingsBase {
  public: 
    G2GPSSettings(std::string const &);
    void print() const;
    virtual bool contains(std::string) override;

    std::string particle;
    std::string pos_type;
    std::vector<double> pos_rot1;
    std::vector<double> pos_rot2;
    std::string pos_shape;
    std::vector<double> pos_centre;
    const double pos_halfx;
    const double pos_halfy;
    const double pos_sigma_x;
    const double pos_sigma_y;
    const double pos_radius;
    const double pos_sigma_r;
    std::vector<double> ang_rot1;
    std::vector<double> ang_rot2;
    std::string ang_type;
    const double ang_sigma_x;
    const double ang_sigma_y;
    const double ang_sigma_r;

    std::string ene_type;
    const double ene_mono;
    const double ene_sigma;
    std::string tType;
    const double tMono;
    const double tSigma;    
    
  private:
    std::vector<std::string> parSetKeys_;
    

  };
}

#endif //G2GPSSETTINGS_HH
