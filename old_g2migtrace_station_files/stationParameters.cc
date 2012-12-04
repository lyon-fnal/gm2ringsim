/** @file stationParameters.cc

    Implements the detector station parameters manipulator.  

    @author Kevin Lynch
    @date 2011
*/

#include "stationParameters.hh"
#include "stationConstruction.hh"

#include "G4String.hh"

#include "json/json.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>

namespace {
  
  plate_t def_parse_plate(Json::Value const&);
  hodoscope_t def_parse_hodo(Json::Value const&);
  calorimeter_t def_parse_calo(Json::Value const&);
  station_t def_parse_station(Json::Value const&);

  plate_t parse_plate(plate_t const&, Json::Value const&);
  hodoscope_t parse_hodo(hodoscope_t const&, Json::Value const&);
  calorimeter_t parse_calo(calorimeter_t const&, Json::Value const&);
  station_t parse_station(station_t const&, Json::Value const&);
}

stationParameters::stationParameters(stationConstruction *sc) : 
  sConst_(sc),
  default_config_file_("g2RunTimeFiles/station_default.json"),
  config_file_("g2RunTimeFiles/stations.json")
{
  read_default_config();
  read_config();

//G4cout << thickness(0) << ' ' << radial(0) << ' ' << vertical(0) << '\n';
}

stationParameters::~stationParameters(){}

void stationParameters::read_default_config(){
  std::ifstream in(default_config_file_);
  G4cout << "Load default detector station configuration...\n";
  G4cout << "\tOpening " << default_config_file_ << '\n';
  if( !in ){
    G4cout << "\tFailed!  Throwing exception...\n";
    throw json_file_not_found(default_config_file_);
  }

  /** @bug These initial checks need more robust error checking and
      recovery. */
  Json::Value root;
  Json::Reader reader;
  bool success = reader.parse(in, root);
  if( !success ) {
    G4cout << "\tParsing error!  Partial parse follows:\n";
    G4cout << root["default_station"] << '\n';
    G4cout << "\tThowing exception...\n";
    throw json_parse_failed(default_config_file_);
  } else {
    G4cout << "\tParse successful\n";
  }  

  // check format version
  int const format_version = 
    root["default_station"]["format_version"].asInt();
  if( 1 != format_version ){
    G4cout << "\tInvalid format_version: " << format_version << '\n';
    G4cout << "\tThrowing exception...\n";
    throw json_bad_format_version(format_version);
  }

  // plates/gaps
  default_config.front_plate = 
    def_parse_plate(root["default_station"]["front_plate"]);
  default_config.mid_plate = 
    def_parse_plate(root["default_station"]["mid_plate"]);
  default_config.back_plate = 
    def_parse_plate(root["default_station"]["back_plate"]);
  default_config.final_plate = 
    def_parse_plate(root["default_station"]["final_plate"]);
  default_config.gap = 
    def_parse_plate(root["default_station"]["gap"]);
  // hodoscope planes
  default_config.front_plane = 
    def_parse_hodo(root["default_station"]["hodo_front_plane"]);
  default_config.rear_plane = 
    def_parse_hodo(root["default_station"]["hodo_rear_plane"]);
  // calorimeter
  default_config.calorimeter = 
    def_parse_calo(root["default_station"]["calorimeter"]);
  // station placement
  default_config.station = 
    def_parse_station(root["default_station"]["station"]);
}

void stationParameters::read_config(){
  std::ifstream in(config_file_);
  G4cout << "Load additional detector station configuration...\n";
  G4cout << "\tOpening " << config_file_ << '\n';
  if( !in ){
    G4cout << "\tFailed!  No such file.\n";
    return;
  }

  /** @bug These initial checks need more robust error checking and
      recovery. */
  Json::Value root;
  Json::Reader reader;
  bool success = reader.parse(in, root);
  if( !success ) {
    G4cout << "\tParsing error!  Partial parse follows:\n";
    G4cout << root["detector_stations"] << '\n';
    G4cout << "\tThowing exception...\n";
    throw json_parse_failed(config_file_);
  } else {
    G4cout << "\tParse successful\n";
  }  

  // check format version
  int const format_version = 
    root["detector_stations"]["format_version"].asInt();
  if( 1 != format_version ){
    G4cout << "\tInvalid format_version: " << format_version << '\n';
    G4cout << "\tThrowing exception...\n";
    throw json_bad_format_version(format_version);
  }

  std::ostringstream oss;
  for(int i=0; i!=24; ++i){
    // build and find station in config_file
    oss.str("");
    oss << "station_" << (i+1);
    //    G4cout << "Station: " << oss.str() << '\n';
    Json::Value station = root["detector_stations"][oss.str()];
    if( Json::Value::null == station ){
      configs[i] = default_config;
      continue;
    }
    //    G4cout << station << '\n';
    // The rest here is icky ... but I can't think of a better way;
    // OK, more indirection would do it, but that's no less icky here
    configs[i].front_plate = 
      parse_plate(default_config.front_plate, station["front_plate"]);
    configs[i].mid_plate = 
      parse_plate(default_config.mid_plate, station["mid_plate"]);
    configs[i].back_plate = 
      parse_plate(default_config.back_plate, station["back_plate"]);   
    configs[i].final_plate = 
      parse_plate(default_config.final_plate, station["final_plate"]);
    configs[i].gap = parse_plate(default_config.gap, station["gap"]);
    configs[i].front_plane = 
      parse_hodo(default_config.front_plane, station["hodo_front_plane"]);
    configs[i].rear_plane = 
      parse_hodo(default_config.rear_plane, station["hodo_rear_plane"]);
    configs[i].calorimeter = 
      parse_calo(default_config.calorimeter, station["calorimeter"]);
    configs[i].station =
      parse_station(default_config.station, station["station"]);
  }
}

G4double stationParameters::thickness(G4int station) const {
  double temp = 0;
  station_params const& sp = configs[station];
  temp += (sp.front_plate.create ? sp.front_plate.thickness : 0.);
  temp += (sp.mid_plate.create ? sp.mid_plate.thickness : 0.);
  temp += (sp.back_plate.create ? sp.back_plate.thickness : 0.);
  temp += (sp.final_plate.create ? sp.final_plate.thickness : 0.);
  temp += (sp.gap.create ? sp.gap.thickness : 0.);
  temp += (sp.front_plane.create ? sp.front_plane.thickness : 0.);
  temp += (sp.rear_plane.create ? sp.rear_plane.thickness : 0.);
  temp += sp.calorimeter.thickness;
  temp += sp.calorimeter.epsilon;
  return temp;
}

G4double stationParameters::radial(G4int station) const {
  return configs[station].calorimeter.radial + configs[station].calorimeter.epsilon;
}

G4double stationParameters::vertical(G4int station) const {
  return configs[station].calorimeter.vertical + configs[station].calorimeter.epsilon;
}


G4String stationParameters::default_config_file(G4String const& newfile){
  G4String temp = default_config_file_;
  default_config_file_ = newfile;
  reload_config();
  return temp;
}

G4String stationParameters::config_file(G4String const& newfile){
  G4String temp = config_file_;
  config_file_ = newfile;
  reload_config();
  return temp;
}

void stationParameters::reload_config() {
  read_default_config();
  read_config();
  sConst_ -> rebuildAll();
}

station_params stationParameters::
set_params(G4int station, station_params const& new_params){
  station_params old = configs[station];
  configs[station] = new_params;
  sConst_ -> rebuildStation(station);
  return old;
}


namespace {
  /** @bug These functions need more robust error checking and
      recovery. */ 
  plate_t def_parse_plate(Json::Value const& plate){
    plate_t p;
    //    G4cout << plate << '\n';
    p.create = plate["create"].asBool();
    //    G4cout << std::boolalpha << p.create << ' ';
    p.thickness = plate["thickness"].asDouble();
    //    G4cout << p.thickness << ' ';
    p.material = plate["material"].asString();
    //    G4cout << p.material << '\n';
    return p;
  }

  hodoscope_t def_parse_hodo(Json::Value const& hodo){
    hodoscope_t h;
    //    G4cout << hodo << '\n';
    h.create = hodo["create"].asBool();
    //    G4cout << std::boolalpha << h.create << ' ';
    h.thickness = hodo["thickness"].asDouble();
    //    G4cout << h.thickness << ' ';
    h.is_vertical = hodo["vertical_orientation"].asBool();
    //    G4cout << std::boolalpha << h.vertical << ' ';
    h.staves = hodo["num_staves"].asInt();
    //    G4cout << h.staves << ' ';
    h.material = hodo["material"].asString();
    //    G4cout << h.material << '\n';
    return h;
  }

  calorimeter_t def_parse_calo(Json::Value const& calo){
    calorimeter_t c;
    //    G4cout << calo << '\n';
    c.model = calo["model"].asString();
    //    G4cout << c.model << ' ';
    c.thickness = calo["thickness"].asDouble();
    //    G4cout << c.thickness << ' ';
    c.vertical = calo["vertical"].asDouble();
    //    G4cout << c.vertical << ' ';
    c.radial = calo["radial"].asDouble();
    //    G4cout << c.radial << ' ';
    c.nXtalRows = calo["n_xtal_rows"].asInt() ;
    //    G4cout << c.nXtalRows << ' ';
    c.nXtalCols = calo["n_xtal_cols"].asInt() ;
    //    G4cout << c.nXtalCols << ' ';
    c.xtalWidth = calo["xtal_width"].asDouble() ;
    //    G4cout << c.xtalWidth << ' ';
    c.xtalHeight = calo["xtal_height"].asDouble() ;
    //    G4cout << c.xtalHeight << ' ';
    c.xtalDepth = calo["xtal_depth"].asDouble() ;
    //    G4cout << c.xtalDepth << ' ';
    c.epsilon = calo["epsilon"].asDouble() ;
    //    G4cout << c.epsilon << ' ';
    c.epoxyDepth = calo["epoxy_depth"].asDouble() ;
    //    G4cout << c.epoxyDepth << ' ';
    c.pmtDepth = calo["pmt_depth"].asDouble() ;
    //    G4cout << c.pmtDepth << ' ';
    c.frontWrapping = calo["front_face_wrapping"].asString();
    //    G4cout << c.frontWrapping << ' ';
    c.sideWrapping = calo["side_face_wrapping"].asString();
    //    G4cout << c.sideWrapping << ' ';
    c.backWrapping = calo["back_face_wrapping"].asString();
    //    G4cout << c.backWrapping << ' ';
    c.photodetectorShape = calo["photodetector_shape"].asString();
    //    G4cout << c.photodetectorShape << ' ';
    c.photodetectorSize = calo["photodetector_size"].asDouble();
    //    G4cout << c.photodetectorSize << ' ';
    c.tapered = calo["tapered"].asBool();
    //    G4cout << c.tapered << "\n";
    return c;
  }

  station_t def_parse_station(Json::Value const& station){
    station_t s;
    //    G4cout << station << '\n';
    s.t_offset = station["t_offset"].asDouble();
    //    G4cout << s.t_offset << ' ';
    s.r_offset = station["r_offset"].asDouble();
    //    G4cout << s.r_offset << ' ';
    s.v_offset = station["v_offset"].asDouble();
    //    G4cout << s.v_offset << ' ';
    s.t_rotation = station["t_rotation"].asDouble()*M_PI/180.;
    //    G4cout << s.t_rotation << ' ';
    s.r_rotation = station["r_rotation"].asDouble()*M_PI/180.;
    //    G4cout << s.r_rotation << ' ';
    s.v_rotation = station["v_rotation"].asDouble()*M_PI/180.;
    //    G4cout << s.v_rotation << ' ';
    return s;
  }

  plate_t parse_plate(plate_t const& def, Json::Value const& plate){
    //    G4cout << plate << '\n';
    if( Json::Value::null == plate )
      return def;
    plate_t p;
    p.create = plate.get("create", def.create).asBool();
    //    G4cout << p.create << ' ';
    p.thickness = plate.get("thickness", def.thickness).asDouble();
    //    G4cout << p.thickness << ' ';
    p.material = plate.get("material", def.material).asString();
    //    G4cout << p.material << '\n';
    return p;
  }

  hodoscope_t parse_hodo(hodoscope_t const& def, Json::Value const& hodo){
    //    G4cout << hodo << '\n';
    if( Json::Value::null == hodo)
      return def;
    hodoscope_t h;
    h.create = hodo.get("create", def.create).asBool();
    //    G4cout << h.create << ' ';
    h.thickness = hodo.get("thickness", def.thickness).asDouble();
    //    G4cout << h.thickness << ' ';
    h.is_vertical = hodo.get("vertical_orientation", def.is_vertical).asBool();
    //    G4cout << h.vertical << ' ';
    h.staves = hodo.get("num_staves", def.staves).asInt();
    //    G4cout << h.staves << ' ';
    h.material = hodo.get("material", def.material).asString();
    //    G4cout << h.material << '\n';
    return h;
  }

  calorimeter_t parse_calo(calorimeter_t const& def, 
			   Json::Value const& calo){
    //    G4cout << calo << '\n';
    if( Json::Value::null == calo )
      return def;
    calorimeter_t c;
    c.model = calo.get("model", def.model).asString();
    //    G4cout << c.model << ' ';
    c.thickness = calo.get("thickness", def.thickness).asDouble();
    //    G4cout << c.thickness << ' ';
    c.radial = calo.get("radial", def.radial).asDouble();
    //    G4cout << c.radial << ' ';
    c.vertical = calo.get("vertical", def.vertical).asDouble();
    //    G4cout << c.vertical << ' ';
    c.nXtalRows = calo.get("n_xtal_rows", def.nXtalRows).asInt() ;
    //    G4cout << c.nXtalRows << ' ';
    c.nXtalCols = calo.get("n_xtal_cols", def.nXtalCols).asInt() ;
    //    G4cout << c.nXtalCols << ' ';
    c.xtalWidth = calo.get("xtal_width", def.xtalWidth).asDouble() ;
    //    G4cout << c.xtalWidth << ' ';
    c.xtalHeight = calo.get("xtal_height", def.xtalHeight).asDouble() ;
    //    G4cout << c.xtalHeight << ' ';
    c.xtalDepth = calo.get("xtal_depth", def.xtalDepth).asDouble() ;
    //    G4cout << c.xtalDepth << ' ';
    c.epsilon = calo.get("epsilon", def.epsilon).asDouble() ;
    //    G4cout << c.epsilon << ' ';
    c.epoxyDepth = calo.get("epoxy_depth", def.epoxyDepth).asDouble() ;
    //    G4cout << c.epoxyDepth << ' ';
    c.pmtDepth = calo.get("pmt_depth", def.pmtDepth).asDouble() ;
    //    G4cout << c.pmtDepth << '\n';

    return c;
  }

  station_t parse_station(station_t const& def, Json::Value const& station){
    //    G4cout << station << '\n';
    if( Json::Value::null == station )
      return def;
    station_t s;
    s.t_offset = station.get("t_offset", def.t_offset).asDouble();
    //    G4cout << s.t_offset << ' ';
    s.r_offset = station.get("r_offset", def.r_offset).asDouble();
    //    G4cout << s.r_offset << ' ';
    s.v_offset = station.get("v_offset", def.v_offset).asDouble();
    //    G4cout << s.v_offset << ' ';
    // KRL Bug 1616 ... not my favorite solution, but it keeps the
    // values of def.t_rotation and s.t_rotation in the same units
    s.t_rotation = 
      station.get("t_rotation", def.t_rotation*180./M_PI).asDouble()*M_PI/180.;
    //    G4cout << s.t_rotation << ' ';
    s.r_rotation = 
      station.get("r_rotation", def.r_rotation*180./M_PI).asDouble()*M_PI/180.;
    //    G4cout << s.r_rotation << ' ';
    s.v_rotation = 
      station.get("v_rotation", def.v_rotation*180./M_PI).asDouble()*M_PI/180.;
    //    G4cout << s.v_rotation << ' ';
    return s;
  }
}


const char* json_file_not_found::what() const throw(){
  std::ostringstream o;
  o << "JSON file " << payload_ << " not found";
  message_ = o.str();
  return message_.c_str();
}

const char* json_parse_failed::what() const throw(){
  std::ostringstream o;
  o << "JSON parse failed for file " << payload_;
  message_ = o.str();
  return message_.c_str();
}

const char* json_bad_format_version::what() const throw(){
  std::ostringstream o;
  o << "JSON file has invalid format version " << payload_;
  message_ = o.str();
  return message_.c_str();
}

