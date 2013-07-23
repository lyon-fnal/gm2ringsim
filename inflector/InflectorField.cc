/** @file InflectorField.cc

    Implements all the members of the vanishingInflectorField,
    simpleInflectorField and mappedInflectorField classes.

    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2011
*/

#include "Geant4/G4Track.hh"
#include "Geant4/G4RunManagerKernel.hh"
#include "Geant4/G4UnitsTable.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <utility>
#include <algorithm>

#include <cmath>

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2geom/inflector/inflectorGeometry.hh"
using gm2geom::inflectorGeometry;
#include "gm2ringsim/inflector/InflectorField.hh"
#include "gm2ringsim/arc/StorageRingField.hh"

//////////// vanishingInflectorField

void gm2ringsim::VanishingInflectorField::GetFieldValue(const double */*Point*/,
					    double *Bfield) const
{
  Bfield[0] = Bfield[1] = Bfield[2] = 0.;
}




//////////// simpleInflectorField

gm2ringsim::SimpleInflectorField::SimpleInflectorField(G4double fieldNormConstant, int Charge) :
  epsilon(30*degree),fieldNormConst(fieldNormConstant), Charge_(Charge)
{
  // Set data members to latest position of inflector
  inflectorGeometry const& ig = inflectorGeometry::getInstance();

  delta = ig.delta();
  gamma = ig.gamma();
  zeta = ig.zeta();
  apertureRadius = R_magic() + ig.aperture_off();
  inflectorTotalLength = ig.length();

  // Read in the default inflector axial field map
  // FIXME: Should use ART file i/o to get the .dat files 
  // FIXME: Should copy *.dat from every directory to the build directory ala *.fcl
  
  fieldMapFName = "g2RunTimeFiles/g2InflectorFieldMap.dat";
  readInflectorFieldMap.open(fieldMapFName);
  
  if(readInflectorFieldMap.fail())
    G4cout << "\nERROR: g2MIGTRACE failed to find the inflector axial field map!"
	   <<   "       Currently looking for the inflector field map at: "
	   <<   "       $G4INSTALL/bin/$G4System/g2RunTimeFiles/" << fieldMapFName << G4endl;
  else
    while(readInflectorFieldMap.good())
      {
	// Create two vectors, one for position and one for magnitude
	// of the field at that position.  Note that the axial field
	// map is been normalized to the value described by
	// 'fieldNormConst' so we must multiply 'val' by this value
	if( readInflectorFieldMap.eof() ){
	  readInflectorFieldMap.close();
	  break;
	}
	G4double pos, val;
	readInflectorFieldMap >> pos >> val;
	pos *= (-1)*mm;
	val *= fieldNormConst;;
	fieldPos.push_back(pos);
	fieldVal.push_back(val);
      }
}


gm2ringsim::SimpleInflectorField::~SimpleInflectorField()
{;}


void gm2ringsim::SimpleInflectorField::GetFieldValue(const double *Point,
				   double *Bfield) const
{
  const G4double x_muon = Point[0];
  const G4double y_muon = Point[1];
  const G4double z_muon = Point[2];

  // Note that the purpose of the inflector field is to *cancel* the
  // yoke storage field; therefore, since g2MIGTRACE uses negative
  // muons and the storage field is subsequently in the negative y
  // direction, the inflector field must be in positive y
  storageFieldController::getInstance().GetFieldValue(Point, Bfield, Charge_);

  //----------------------
  // Assumes negative beam
  //----------------------
  G4double SimpleFieldMagnitude = CalculateFieldValue(x_muon,y_muon,z_muon);

  //----------------------
  // Negative field if positive beam
  //----------------------
  if ( Charge_ == 1 ) { SimpleFieldMagnitude *= -1; }


  Bfield[1] += SimpleFieldMagnitude;
}


G4double gm2ringsim::SimpleInflectorField::CalculateFieldValue(const G4double x_muon,
					     const G4double /*y_muon*/,
					     const G4double z_muon) const
{
  // While the particle is being tracked in global coordinates, the
  // field map is aligned with the axis of the inflector, which can be
  // at any arbitrary position within the vacuum chamber.  Thus we
  // must do the coordinate transformations FROM global TO inflector
  // if we are to correctly and consistently assign correct values of
  // the field.

  // 'x' and 'z' represent global coordinates
  // 'xPrime' and 'zPrime' represent inflector coordinates
  //    where '+zPrime-hat' points down the inflector towards
  //    the storage ring
  // 'd' represent absolute distances of intermediate triangles
    
  // Calculate the inflector injection point in global coordinates
  const G4double x_injection = apertureRadius*std::cos(delta);
  const G4double z_injection = -apertureRadius*std::sin(delta);

  // Calculate intermediate triangle to aid calculation.  See this
  // coordinate transformation in the references section for more.
  G4double d_x = std::abs(x_injection - x_muon);
  G4double d_z = std::abs(z_injection - z_muon);
  G4double d_hypot = hypot(d_x,d_z);
  G4double kappa = std::atan(d_x / d_z);

  // Calculate the angle between the hypotenuse of intermediate
  // triangle and the z-axis of the inflector
  G4double lambda = kappa - gamma - delta;

  // The angle lambda and side d_hypot now form a right triangle,
  // whose legs represent the position of the muon in inflector
  // coordinates.  Note that if [lamdba < 0] then [xPrime_muon < 0]
  // as it should be for our inflector coordinate system definition.

  //  G4double xPrime_muon = d_hypot*sin(lambda);
  G4double zPrime_muon = inflectorTotalLength - d_hypot*std::cos(lambda);

  // At this point, the muon is now in inflector coordinates.  Note
  // that zPrime_muon should always be positive but that xPrime_muon can be
  // either positive or negative depending on which side of the
  // inflector it is on.

  // Now do the field interpolation using the field maps loaded in the
  // constructor.  

  // NOTE: As of this writing, only a vertical inflector field map
  // along the z-axis is available (ZSH - 08 SEP 08)
  
  G4double leftPosBin = 0., rightPosBin = 0.;
  G4double leftFieldBin = 0., rightFieldBin = 0.;
  G4double slope = 0., verticalFieldValue = 0.;

  for(size_t i=0; i<fieldPos.size(); i++)
    {
      if(zPrime_muon <= fieldPos.at(i) && zPrime_muon > 0.)
	{
	  leftPosBin = fieldPos.at(i-1);
	  leftFieldBin = fieldVal.at(i-1);
	  
	  rightPosBin = fieldPos.at(i);
	  rightFieldBin = fieldVal.at(i);
	  break;
	}
    }
  
  slope = (leftFieldBin - rightFieldBin) / (leftPosBin - rightPosBin);
  
  verticalFieldValue = (slope * (zPrime_muon - rightPosBin)) + rightFieldBin;
  /*
  G4cout << "zPrime_muon = " << zPrime_muon/mm << "\n"
	 << "leftPosBin = " << leftPosBin/mm << "\n"
	 << "leftFieldBin = " << leftFieldBin/tesla << "\n"
	 << "rightPosBin = " << rightPosBin/mm << "\n"
	 << "rightFieldBin = " << rightFieldBin/tesla << "\n"
	 << "interpolated field Value = " << verticalFieldValue/tesla << "\n"
	 << G4endl;
  */
  if(zPrime_muon > 0.)
    return verticalFieldValue;
  else  
    return 0.*tesla;
}


///////////////// mappedInflectorField

/** @bug Need to fix things so that these field maps don't get
    reloaded when the field gets reinstantiated. */
gm2ringsim::MappedInflectorField::MappedInflectorField(int Charge) :
  magnet_file_("g2RunTimeFiles/injec_fld.dat"), 
  inflector_file_("g2RunTimeFiles/inf_field_alone.dat"),
  Charge_(Charge)
	      
{
  inflectorGeometry const& ig = inflectorGeometry::getInstance();
  delta_ = ig.delta();
  gamma_ = ig.gamma();
  zeta_ = ig.zeta();
  
  load_maps();
}

#include <tr1/functional>

void gm2ringsim::MappedInflectorField::load_maps(){
  // open files and toss if not exist
  // parse file ... write a function that returns vector of data
  // points?   add some comments ... strip/lstrip/rstrip/tokenize/etc.
  // stuff into kd-trees ... transform before stuffing, or do that in
  // lookup?   for now, do it in lookup.  Then, I only need to read in
  // once, at instantiation time
  // close files

  G4cout << "Open magnet files\n";
  std::ifstream magnet_if(magnet_file_.c_str()), 
    inflector_if(inflector_file_.c_str());

  if( !magnet_if.is_open() )
    throw new no_magnet_file;

  if( !inflector_if.is_open() )
    throw new no_inflector_file;

  // parse each file into a vector of data_points
  G4cout << "Reading storage field from file " << magnet_file_ << '\n';
  data_vec vec = parse_file(magnet_if,7);
  G4cout << "Read " << vec.size() 
	 << " data points from storage field file " 
	 << magnet_file_ << ".\n"; 
  reflect_and_merge(vec);
  std::for_each(vec.begin(), vec.end(), 			   
		std::tr1::bind(&gm2ringsim::MappedInflectorField::magnet_transform,this,
			       std::tr1::placeholders::_1));
  //FIXME
  magnet_tree_ = data_tree(vec.begin(), vec.end());


  G4cout << "Reading inflector field from file " << inflector_file_ << '\n';
  vec = parse_file(inflector_if,6);
  G4cout << "Read " << vec.size() 
	 << " data points from inflector field file "
	 << inflector_file_ << ".\n";
  reflect_and_merge(vec);
  std::for_each(vec.begin(), vec.end(), 			   
		std::tr1::bind(&gm2ringsim::MappedInflectorField::inflector_transform,this,
			       std::tr1::placeholders::_1));
  //FIXME
  inflector_tree_ = data_tree(vec.begin(), vec.end());

#if 0
  inflectorGeometry const& ig = inflectorGeometry::getInstance();

  // printout a map of the fields along the long axis of the inflector
  G4ThreeVector upmandrel(0, 0, 
		       -ig.mandrel_length() );
  upmandrel.rotateX(-ig.zeta()); // why negative?  WTF?
  upmandrel.rotateY(ig.gamma());
  upmandrel += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upmandrel.rotateY(ig.delta());
  
  G4ThreeVector downmandrel(R_magic()+ig.aperture_off(),0.,0.);
  downmandrel.rotateY(ig.delta());
  
  G4int steps = 100;
  G4ThreeVector const dvec = (downmandrel-upmandrel)/(steps+1);
  G4double const size = dvec.mag();
  G4ThreeVector tvec = upmandrel;
  G4ThreeVector const zero(0,0,0);
  data_vec found;
    G4double const search_rad = 5.*millimeter;
  
  std::ofstream out("inflector_field.dat");
  for( int i=0; i!=steps; ++i, tvec+=dvec){
    data_point dp(tvec,zero);
    out << i*size << ' ';

    found.clear();
    //FIXME
    magnet_tree_.find_within_range(dp, search_rad, std::back_inserter(found));
    G4ThreeVector Bf = shepard_interpolate(dp,found);
    out << Bf.y() << ' ';

    
    found.clear();
    //FIXME
    inflector_tree_.find_within_range(dp, search_rad, std::back_inserter(found));
    G4ThreeVector Bi = shepard_interpolate(dp, found);
    out << Bi.y() << ' ';

    out << (Bf+Bi).y() << '\n';
  }
  out.close();
#endif

}

void gm2ringsim::MappedInflectorField::magnet_transform(data_point& dp){
  // rotate from Wuzheng system to g2MIGTRACE system
  G4ThreeVector rot_pt = rotationYOf(dp.point(), delta_);
  // This transform does the right thing (carefully checked with
  // models), but I don't get the signs
  G4ThreeVector rot_field = rotationYOf(dp.payload(), delta_);
  dp = data_point(rot_pt, rot_field);
}

void gm2ringsim::MappedInflectorField::inflector_transform(data_point& dp){ 
  // translate from Wuzheng system to origin

  //  G4cout << "Inflector transform: " <<  dp.point() << '\n';
  G4ThreeVector const Wuzheng_inflection_pt(718.9*centimeter,0.,0.);
  G4ThreeVector pt = dp.point()-Wuzheng_inflection_pt;
  G4ThreeVector field = dp.payload();
  //  G4cout << "Rebase: " << pt << '\n';

  // unrotate from Wuzheng's base angle
  G4double const Wuzheng_angle = 2.4 * milliradian;
  pt.rotateY(-Wuzheng_angle);
  //  G4cout << "Unrotate: " << pt << '\n';

  // rotate out of plane
  pt.rotateX(zeta_);
  //  G4cout << "Out of plane: " << pt << '\n';

  // rotate by gamma, in plane
  pt.rotateY(gamma_);
  //  G4cout << "In plane: " << pt << '\n';

  // translate
  pt += Wuzheng_inflection_pt;
  //  G4cout << "Translate: " << pt << '\n';

  // rotate around ring
  pt.rotateY(delta_);
  //  G4cout << "Rotate around ring: " << pt << '\n';

  // deal with field ... these transforms do the right thing, (checked
  // with models :-) but I don't get the signs
  field.rotateY(Wuzheng_angle);
  field.rotateX(-zeta_);
  field.rotateY(delta_+gamma_);
  //  G4cout << "field: " << dp.payload() << ' ' << field << '\n';

  dp = data_point(pt, field);
}

std::string gm2ringsim::MappedInflectorField::magnet_file(std::string name){
  std::string old(magnet_file_);
  magnet_file_ = name;
  return old;
}

std::string gm2ringsim::MappedInflectorField::inflector_file(std::string name){
  std::string old(inflector_file_);
  inflector_file_ = name;
  return old;
}

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <cstdlib>

void gm2ringsim::MappedInflectorField::reflect_and_merge(data_vec & vec){
  data_vec reflected;
  data_vec::const_iterator b = vec.begin(),
    e = vec.end();
  while(b!=e){
    data_point dp = *b;
    G4ThreeVector pt = dp.point();
    G4ThreeVector field = dp.payload();
    if( pt.y() != 0 ){
      pt.setY(-1.*pt.y());
      field.setX(-1.*field.x());
      field.setZ(-1.*field.z());
      reflected.push_back(data_point(pt,field));
    }
    ++b;
  }
  // merge reflected onto vec
  std::copy(reflected.begin(), reflected.end(), std::back_inserter(vec));
}

// assume open, valid file.  gonna puke hard if not!
gm2ringsim::MappedInflectorField::data_vec gm2ringsim::MappedInflectorField::
parse_file(std::istream& is, int exp_toks){
  data_vec temp;
  std::string str;

  // data file format described in comments in data file, as well as
  // in ref/wuzhengEmail_7.txt

  while( is.good() ){
    std::getline(is, str);

    // clean leading and trailing whitespace
    boost::trim(str);
    // skip comment lines and blank lines
    if( str[0] == '#' || str.size() == 0){
      continue;
    }
    // tokenize
    boost::char_separator<char> sep(" \t");
    typedef boost::tokenizer<boost::char_separator<char> > tokenize_on_blanks;
    tokenize_on_blanks tokens(str,sep);
    tokenize_on_blanks::const_iterator b = tokens.begin(),
      e = tokens.end();
    int toks = std::distance(b,e);
    if( toks != exp_toks )
      G4cout << "Bad line!  Saw " << toks << " tokens!\n";

    // parse tokens into doubles into G4ThreeVectors into field_point
    // objects
    double x = std::strtod( (*b++).c_str(),0 )*centimeter;
    double y = std::strtod( (*b++).c_str(),0 )*centimeter;
    double z = std::strtod( (*b++).c_str(),0 )*centimeter;
    G4ThreeVector pos(x,y,z);
      
    double bx = std::strtod( (*b++).c_str(),0 )*gauss;
    double by = std::strtod( (*b++).c_str(),0 )*gauss;
    double bz = std::strtod( (*b++).c_str(),0 )*gauss;
    G4ThreeVector bfield(bx,by,bz);

    //    G4cout << bfield << '\n';

    temp.push_back(data_point(pos,bfield));

    // leave at end!
    if( is.eof() )
      break;
  }

  return temp;
}

G4double distance(G4ThreeVector const& left,G4ThreeVector const& right){
  return (left-right).mag();
}

G4double const search_rad = 5.*millimeter;

void gm2ringsim::MappedInflectorField::
GetFieldValue( double const * Point, double *Bfield) const {
  G4ThreeVector point(Point[0], Point[1], Point[2]);
  G4ThreeVector zero(0,0,0);

  data_point dp(point,zero);

  data_vec found;

  // calculate the storage field

  // find_within_range seems to be quite "fuzzy", so we'll have to
  // actually check.
  //
  //FIXME
  magnet_tree_.find_within_range(dp, search_rad, std::back_inserter(found));

  /** @bug Need to do something more intelligent if we don't find any
      internal points, that is, if we are outside the mapped field
      region. */
  G4ThreeVector B = shepard_interpolate(dp,found);
  found.clear();

  // calculate the inflector field
  //FIXME
  inflector_tree_.find_within_range(dp, search_rad, std::back_inserter(found));

  B += shepard_interpolate(dp, found);

  //  G4cout << "Result: " << B << '\n';


  //-------------------------
  // Assumes negative beam
  //-------------------------
  Bfield[0] = B[0];
  Bfield[1] = B[1];
  Bfield[2] = B[2];

  //------------------------
  // Negate if positive beam
  //-----------------------
  if ( Charge_ == 1 ) {
    for ( int i = 0; i < 3; i++ ) { Bfield[i] *= -1; }
  }


  
  
  bool debug = false;
  if ( debug ) {
    double r = sqrt(Point[0]*Point[0] + Point[2]*Point[2]) - R_magic();
    double y = Point[1];
    std::cout.precision(3);
    double dB = Bfield[1] - 0.00145;
    dB = 0.0;
    if ( Bfield[0] > 0.01 || Bfield[2] > 0.01 || Bfield[0] < -0.01 || Bfield[2] < -0.01 || dB > 0.01 || dB < -0.01 || 1 ) {
      std::cout << "InflectorField::GetFieldValue(" << r << " , " << y << ") = ["
		<< Bfield[0] << " , "
		<< Bfield[1] << " , "
		<< Bfield[2] << "]" << std::endl;
    }
  }
}


G4ThreeVector gm2ringsim::MappedInflectorField::
shepard_interpolate(data_point const& dp, data_vec const& dv) const {
  //  G4cout << "shepard_interpolate:\n";

  G4double const lim = 0.01*mm;
  G4double const p = 2.;

  // phi = r^-p, but I moderate it with lim to prevent overflows and
  // crashing 

  data_vec::const_iterator b = dv.begin(), e = dv.end();
  G4double dweight = 0.;
  if(dv.size() == 0){
    //    G4cout << "\tNo matches in range! return 0\n";
    return G4ThreeVector(0.,0.,0.);
  }
  if(dv.size() == 1){ // wrong, but still as good as anything else!
    //    G4cout << "\tOnly one match in range!  return closest point\n";
    return dv[0].payload();
  }

  G4ThreeVector B;
  while(b!=e){
    //    G4cout << "\t\tPayload: " << b->payload() << '\n';
    G4double dist = distance(b->point(),dp.point());
    //    if( dist < lim ) dist = lim;
    //    G4cout << "\t\tdist [lim] " << dist << ' ' << lim << '\n';;
    if( dist < lim ) return dp.payload(); // this line isn't right,
					  // and never has been!
    G4double nweight = std::pow(dist,-p);
    B += nweight*b->payload();
    dweight += nweight;
    //    G4cout << "\t\tnweight " << nweight << " dweight " << dweight << '\n';
    ++b;
  }

  //  G4cout << "\tdone: " << B/dweight << '\n';

  return B/dweight;
}
