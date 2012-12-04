#ifndef stationParameters_hh
#define stationParameters_hh

/** @file stationParameters.hh

    This file also provides the parameter structures that hold the
    configuratoin data for the detector stations and their
    subcomponents.  See the documentation in the @e refs directory

    @bug Actually @e write the documentation

    @sa stationConstruction

    @author Kevin Lynch
    @date 2011
*/

#include "G4String.hh"

#include <tr1/array>

class stationConstruction;

/** @bug Do we need to correct the ordering of parameters within these
    structs to improve cache utilization?  Probably not ... */

/** Parameters for the "wrapping" or physical spacers around hodoscope
    planes. */
struct plate_t {
  bool create;
  double thickness;
  G4String material;
};

/** Parameters for the hodoscope planes themselves. */
struct hodoscope_t {
  bool create;
  double thickness;
  bool is_vertical;
  int staves;
  G4String material;
};

/** Parameters for the structure and underlying model of the
    calorimeter block. */
struct calorimeter_t {
  G4String model;
  double thickness, radial, vertical;
  int nXtalRows, nXtalCols ;
  double xtalWidth, xtalHeight, xtalDepth, epsilon ;
  double epoxyDepth, pmtDepth, ; // total depth should be < calo thickness
  G4String photodetectorShape;
  double photodetectorSize;
  G4String frontWrapping, sideWrapping, backWrapping;
  bool tapered;
};

/** Parameters for the placement and rotation of the entire detector
    station. */
struct station_t {
  double t_offset, r_offset, v_offset;
  double t_rotation, r_rotation, v_rotation;
};

/** Collects all of the parameters for each station. */
struct station_params {
  plate_t front_plate, mid_plate, back_plate, final_plate, gap;
  hodoscope_t front_plane, rear_plane;
  calorimeter_t calorimeter;
  station_t station;
};


class stationParameters {
public:
  stationParameters(stationConstruction *);
  ~stationParameters();

  /** Calculate the station thickness in the beam-like direction */
  G4double thickness(G4int) const;
  /** Calculate the station thickness in the most radial direction */ 
  G4double radial(G4int) const;
  /** Calculate the station thickness in the most vertical direction
      */ 
  G4double vertical(G4int) const;

  /** Return the current default configuration file name */
  G4String default_config_file() const { return default_config_file_; }
  /** Change the current default configuration file name; return the
      previous filename */
  G4String default_config_file(G4String const&);

  /** Return the current non-default configuration file name */ 
  G4String config_file() const { return config_file_; }
  /** Change the current non-default configuration file name; return
      the previous filename */
  G4String config_file(G4String const&);

  /** Reload the current default and non-default configuration files;
      update stationConstruction */ 
  void reload_config();

  /** Return a handle to a single station's configuration parameters */
  station_params const& get_params(G4int station) const { 
    return configs[station];
  }
  /** Replace a single station's configuration parameters, and return
      the original configuration. */
  station_params set_params(G4int, station_params const&);

private:  
  void read_default_config();
  void read_config();

  stationConstruction *sConst_;
  G4String default_config_file_, config_file_;

  station_params default_config;
  std::tr1::array<station_params, 24> configs;
};

#include <exception>
#include <string>

class json_file_not_found : public std::exception {
public:
  json_file_not_found(std::string requested) : payload_(requested) {}
  const char* what() const throw();
  ~json_file_not_found() throw() {}
private:
  json_file_not_found(); // = deleted // don't implement
  std::string const payload_;
  mutable std::string message_;
};

class json_parse_failed : public std::exception {
public:
  json_parse_failed(std::string file) : payload_(file) {}
  const char* what() const throw();
  ~json_parse_failed() throw() {}
private:
  json_parse_failed(); // = deleted // don't implement
  std::string const payload_;
  mutable std::string message_;
};

class json_bad_format_version : public std::exception {
public:
  json_bad_format_version(int requested) : payload_(requested) {}
  const char* what() const throw();
  ~json_bad_format_version() throw() {}
private:
  json_bad_format_version(); // = deleted // don't implement
  int const payload_;
  mutable std::string message_;
};

#endif // stationParameters_hh
