/** @file storageRingField.cc

    Provides implementations of a number of classes related to the
    storage ring field and its current behavior.

    - Zach Hartwig 2005
    - Kevin Lynch 2009
*/

#include "gm2ringsim/arc/StorageRingField.hh"

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <cstdlib>

#include "gm2ringsim/inflector/inflectorGeometry.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"

#include "Geant4/G4PhysicalConstants.hh"

////////////////////////////////////////////////
////////////////////////////////////////////////
////// storageRingField
////////////////////////////////////////////////
////////////////////////////////////////////////

void gm2ringsim::storageRingField::GetFieldValue( const double Point[3],
				      double *Bfield ) const {
  storageFieldController::getInstance().GetFieldValue(Point, Bfield);
}

void gm2ringsim::storageRingEMField::GetFieldValue( const double Point[3],
				      double *Bfield ) const {
  storageFieldController::getInstance().GetFieldValue(Point, Bfield);
  Bfield[3] = 0.0;
  Bfield[4] = 0.0;
  Bfield[5] = 0.0;
  for ( int i = 0; i < 6; i++ ) {
    G4cout << "Bfield[" << i << "] = " << Bfield[i] << G4endl;
  }
}


////////////////////////////////////////////////
////////////////////////////////////////////////
////// storageFieldController
////////////////////////////////////////////////
////////////////////////////////////////////////

gm2ringsim::storageFieldController::storageFieldController() : 
  central_(-B_magic()),
  centralimpl_(new uniformStorageImpl(central_)),
  fringeimpl_(new uniformStorageImpl(central_)),
  sfm_(new storageFieldMessenger(this)),
  centralname_("uniform"), fringename_("uniform")
{}

gm2ringsim::storageFieldController const& gm2ringsim::storageFieldController::getInstance() {
  static storageFieldController s;
  return s;
}

/** @bug Fix the hard coded constants in this member. */
void gm2ringsim::storageFieldController::GetFieldValue( const double Point[3],
					    double *Bfield ) const {

  double const xc = sqrt(Point[0]*Point[0]+Point[2]*Point[2])-gm2ringsim::R_magic();
  double const rc = std::sqrt(Point[1]*Point[1] + xc*xc);
  std::cout << "xc rc " << xc << ' ' << rc << '\n';
  if( rc <= 45.*mm ){
    std::cout << "central\n";
    centralimpl_->GetFieldValue(Point,Bfield);
  }
  else {
    std::cout << "fringe\n";
    fringeimpl_->GetFieldValue(Point,Bfield);
  }

  G4cout << "storageFieldController::GetFieldValue()" << G4endl;
  for ( int i = 0; i < 6; i++ ) {
    G4cout << "  Bfield[" << i << "] = " << Bfield[i] << G4endl;
  }
}

/** @bug There's some missing implementation in this member. */
std::string gm2ringsim::storageFieldController::setCentralFieldImpl(std::string const& val){
  std::string old = centralname_;
  if( val == "uniform" ){
    centralimpl_ = 
      std::tr1::shared_ptr<storageFieldImpl>(new uniformStorageImpl(central_)); 
    centralname_ = val;
  } else if( val == "averagemultipole" ){
    // FIXME!!!!
    //    centralname_ = val;
    G4cout << "The averagemultipole expansion is not yet supported\n;";
  } else if( val == "detailedmultipole" ){
    centralimpl_ = 
      std::tr1::shared_ptr
      <storageFieldImpl>(new detailedMultipoleStorageImpl(central_));
    centralname_ = val;
  }
  return old;
}

std::string gm2ringsim::storageFieldController::setFringeFieldImpl(std::string const& val){
  std::string old = fringename_;
  if( val == "uniform" ){
    fringeimpl_ = 
      std::tr1::shared_ptr<storageFieldImpl>(new uniformStorageImpl(central_));
    fringename_ = val;
  } else if( val == "nonuniform" ){
    fringeimpl_ = 
      std::tr1::shared_ptr<storageFieldImpl>(new fringeStorageImpl(central_));
    fringename_ = val;
  }
  return old;
}

double gm2ringsim::storageFieldController::central_field(double central){
  double old = central_;
  central_ = central;
  centralimpl_->set_central(central_);
  fringeimpl_->set_central(central_);
  return old;
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////// uniformStorageImpl
////////////////////////////////////////////////
////////////////////////////////////////////////

gm2ringsim::uniformStorageImpl::uniformStorageImpl(double central) :
  storageFieldImpl(central) {
  Field[0] = Field[2] = 0.;
  Field[1] = central_;
}

#include <cstring>

void gm2ringsim::uniformStorageImpl::GetFieldValue(const double * /*Point[3]*/, 
					double *Bfield) const {
  std::memcpy(Bfield,Field,3*sizeof(double));
}


////////////////////////////////////////////////
////////////////////////////////////////////////
////// fringeStorageImpl
////////////////////////////////////////////////
////////////////////////////////////////////////

/** @bug This needs to use the error returns from the called members. */
gm2ringsim::fringeStorageImpl::fringeStorageImpl(double central) :
  storageFieldImpl(central),
  fringe_map_name_("g2RunTimeFiles/g2StorageFringeFieldMap.dat"),
  mess_(new fringeStorageMessenger(this))
{
  load_fringe_map();
}

std::string gm2ringsim::fringeStorageImpl::fringe_map_name(std::string const& new_name){
  if( new_name == fringe_map_name_ )
    return fringe_map_name_;

  std::string old = fringe_map_name_;
  fringe_map_name_ = new_name;

  if(!load_fringe_map())
    fringe_map_name_ = old;

  return old;
}

/** @bug The return value doesn't mean anything. */
bool gm2ringsim::fringeStorageImpl::load_fringe_map(){
  // The format of the fringe map is expected to be as in the g2geant
  // bfield_dat files, with four columns:
  // R Y Br By
  // Increment first R at constant Y from low R to high R; then, reset
  // R and increment Y.
  // R and Y are in centimeters, while Br and By are in gauss

  std::ifstream is(fringe_map_name_.c_str());

  std::string str;

  std::vector<double> vals;
  std::set<double> rs, ys;
  std::vector<double> br, by;

  while( is.good() ){
    std::getline(is, str);
    //    std::cout << str << '\n';
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
    if( toks != 4 )
      G4cout << "Bad line!  Saw " << toks << " tokens!\n";

    vals.clear();
    while(b!=e){
      //      std::cout << *b << ' ';
      //      std::cout << std::strtod(b->c_str(), 0) << ' ';
      vals.push_back(std::strtod( (*b++).c_str(), 0));
    }
    //    std::cout << '\n';
    //    std::cout << vals.size() << '\n';
    //    std::cout << vals[0] << ' ' << vals[1] << ' '
    //    	      << vals[2] << ' ' << vals[3] << '\n';

    rs.insert(vals[0]*centimeter);
    ys.insert(vals[1]*centimeter);
    br.push_back(vals[2]*gauss);
    by.push_back(vals[3]*gauss);

    // leave at end!
    if( is.eof() )
      break;
  }

  //  std::cout << "rs: " << rs.size() << '\n';
  //  std::cout << "ys: " << ys.size() << '\n';
  //  std::cout << "br: " << br.size() << '\n';
  //  std::cout << "by: " << by.size() << '\n';
  
  // merge field values
  std::vector<G4ThreeVector> bfield;
  for(std::size_t i=0; i!=br.size(); ++i)
    bfield.push_back(G4ThreeVector(br[i], by[i], 0.));

  //  std::cout << "bfields: " << bfield.size() << '\n';

  double minr, maxr, miny, maxy;
  minr = *rs.begin();
  maxr = *rs.rbegin();
  miny = *ys.begin();
  maxy = *ys.rbegin();

  //  std::cout << "minr: " << minr
  //  	    << " maxr: " << maxr
  //  	    << " miny: " << miny
  //  	    << " maxy: " << maxy
  //  	    << '\n';
  
  map_ = 
    std::tr1::shared_ptr<uniform_xy_bicubic_interpolator<G4ThreeVector> > 
    (new 
     uniform_xy_bicubic_interpolator<G4ThreeVector>(
						    minr, maxr, rs.size(),
						    miny, maxy, ys.size(),
						    bfield.begin(), bfield.end())
     );

  //  std::cout << "value at (711.2,0.): " 
  //    	    << map_->value(7112., 0.) 
  //  	    << " central storage field: " << central_
  //  	    << '\n';
  
  set_scale();

  return true;
}

void gm2ringsim::fringeStorageImpl::set_scale(){
  G4ThreeVector B = map_->value(R_magic(),0.);
  scale_ = central_/B.y(); // this line gets the sign and magnitude rigt
  //  G4cout << "scale = " << scale_ << '\n';
}

void gm2ringsim::fringeStorageImpl::GetFieldValue( const double Point[3],
				       double *Bfield ) const {

  //  NSF: g2MIGTRACE coordinates
  double const& x = Point[0]; //   radial   coordinate @12 o'clock, i.e. "y" in the traditional global-Cartesian sense
  double const& y = Point[1]; //  vertical  coordinate @lways     , i.e. "z" in the traditional global-Cartesian sense
  double const& z = Point[2]; // downstream coordinate @12 o'clock, i.e. "x" in the traditional global-Cartesian sense

  //  NSF: Since the fieldMap is only defined ABOVE the plane of the ring, then 
  //  if the particle is BELOW the plane of the ring, flip the radial component 
  //  of the storage+fringe magnetic field (which is related by symmetry -- 
  //  think of a pure dipole field).  The actual flipping occurs further down.
  double sign = 1.;
  if( y<0. ){
    sign = -1.;
  }

  double const r = std::sqrt(x*x+z*z);  // NSF: "rho" in cylindrical coordinates
  //  double const theta = std::atan2(-z,x); // NSF: I verified with Kevin Lynch on 2012/08/09 that this implementation is incorrect
  double const theta = std::atan2(z,x); // NSF: clockwise angle from 12 o'clock

  //  NSF: Get the value of the storage+magnetic field from the fieldMap
  G4ThreeVector B = map_->value(r,y*sign)*scale_;

  //  NSF: If the particle is below the plane of the ring, flip the radial component of the storage+fringe magnetic field
  B[0]*=sign;

  //  NSF: Evaluate the storage+fringe magnetic field at the particle's position by "rotating the field clockwise from 12 o'clock to the particle's position"
  B.rotateY(-theta); // clockwise in GEANT4/CLHEP

  /*  NSF: Print some stuff -- good for testing purposes
  B.rotateY(+theta); // @12 o'clock
  printf("  B_before: ( Bx , By , Bz ) = (%9.4f Tesla,%9.4f Tesla,%9.4f Tesla) // @12 o'clock\n",B.z()*1e3,B.x()*1e3,B.y()*1e3);
  printf("  position: (  x ,  y ,  z ) = (%9.2f  mm  ,%9.2f  mm  ,%9.2f  mm  ) // @particle\n",z,x,y);
  printf("            ( rho, psi,  z ) = (%9.2f  mm  ,%9.2f  deg ,%9.2f  mm  ) // @particle\n",r,theta*180./M_PI,y);
  printf("    Rotate magnetic field by   (%9.1f degrees clockwise from 12 o'clock    ) // @particle\n",theta*180./M_PI);
  B.rotateY(-theta); // @particle
  printf("   B_after: ( Bx , By , Bz ) = (%9.4f Tesla,%9.4f Tesla,%9.4f Tesla) // @particle\n\n",B.z()*1e3,B.x()*1e3,B.y()*1e3);
  */

  std::memcpy(Bfield,&B[0],3*sizeof(double));   
}


////////////////////////////////////////////////
////////////////////////////////////////////////
////// detailedMultipoleStorageImpl
////////////////////////////////////////////////
////////////////////////////////////////////////

/** @bug Use the return value of the called member functions. */
gm2ringsim::detailedMultipoleStorageImpl::detailedMultipoleStorageImpl(double central) :
  storageFieldImpl(central),
  multipole_map_name_("g2RunTimeFiles/g2StorageMultipoleField.dat"),
  data_(), theta_offset_(inflectorGeometry::getInstance().delta()),
  mess_(new detailedMultipoleMessenger(this))
{
  load_multipole_map();
  //  std::cout << "theta_offset: " << theta_offset_ << '\n';
}

void gm2ringsim::detailedMultipoleStorageImpl::GetFieldValue( const double Point[3],
						  double *Bfield ) const {
  double const& x = Point[0];
  double const& y = Point[1];
  double const& z = Point[2];

  double const rglobal = std::sqrt(x*x+z*z);
  // smash theta into range 0->2pi
  // but the theta of the tables is, I think, theta along curve:
  //  double const tglobal = twopi - std::fmod(std::atan2(z,x)+twopi,
  // twopi);
  double const tglobal = std::atan2(-z,x);
  double const theta = twopi - 
    std::fmod(tglobal - theta_offset_+twopi, twopi);
  //  std::cout << "(rglobal, theta) " << rglobal << ' ' << theta << '\n';

  // find low and high offsets into table, and linear interpolation
  // mixing constant
  double const dtheta = twopi/(data_.size()-1);
  int const low = static_cast<int>(theta/dtheta), high = (low+1)%data_.size();
  double const t = (theta - low*dtheta)/dtheta;

  //  std::cout << dtheta << ' '
  //  	    << low << ' '
  //   	    << high << ' '
  //	    << t << '\n';

  //  if( low < 0 || low >= 8999 )
  //    std::cout << low << ' ' << high << '\n';

  double const xaperture = rglobal - gm2ringsim::R_magic();
  double const raperture= std::sqrt(xaperture*xaperture + y*y);
  double const phiaperture = std::atan2(y,xaperture);

  G4ThreeVector lowvec = data_[low].calcB(raperture, phiaperture);
  G4ThreeVector highvec = data_[high].calcB(raperture, phiaperture);

  double const sign = data_[low].By0*central_ > 0 ? 1. : -1.;

  G4ThreeVector Bresult = (lowvec*(1.-t) + highvec*t)*sign;

  //  G4cout << x << ' ' << z << '\n';
  //  G4cout << Bresult << '\n';
  //  Bresult.rotateY(tglobal);
  //  G4cout << Bresult << '\n';
  //  G4cout << std::atan2(z,x) << ' ' 
  //	 << std::atan2(Bresult.z(),Bresult.x()) << "\n\n";

  std::memcpy(Bfield,&Bresult[0],3*sizeof(double));   
}

G4ThreeVector gm2ringsim::detailedMultipoleStorageImpl::data_holder::
calcB(double r, double phi) const {
  
  double Br = 0., By = 1.;
  double const rr0const = r/(45.*mm);
  double rr0 = rr0const;

  //  std::cout << ab.size() << '\n';
  for(std::size_t i=0;i!=ab.size();++i, rr0 *= rr0const ){
    double const cphi = std::cos((i+1)*phi);
    double const sphi = std::sin((i+1)*phi);
    By += (ab[i].first*cphi + ab[i].second*sphi)*rr0;
    Br += (ab[i].first*sphi - ab[i].second*cphi)*rr0;
  }

  By *= By0;
  Br *= By0;
  
  return G4ThreeVector(Br,By,0);
}

double gm2ringsim::detailedMultipoleStorageImpl::theta_offset(double to){
  double temp = theta_offset_;
  theta_offset_ = std::fmod(to, twopi);
  return temp;
}

/** @bug Check the state of the is variable. */
/** @bug The return value doesn't currently mean anything. */
bool gm2ringsim::detailedMultipoleStorageImpl::load_multipole_map(){
  // The format of the multipole map is expected to be as in the
  // following format:
  // each entry consists of two lines, labeled in the g2track code as
  // line 1: num_probes, pos
  // line 2: a[i]
  // num_probes runs from 0->8999, while "pos" is not used
  // The a[i] are not the a[i] of the Fourier sum, but are:
  // By(0,0) a_1 ... a_n b_1 ... b_n
  // By is given in Tesla, the rest are dimensionless
  // By = By0 + sum_i={1..n} a_i (r/r0)^i cos(i phi) + b_i (r/r0)^2
  // sin(i phi)
  // Br = sum_i={1..n} a_i (r/r0)^i sin(i phi) - b_i (r/r0)^i cos(i
  // phi)
  // with r0 = 45mm, r^2 = xaperture^2 + yaperture^2 
  // phi = atan(yaperture/xaperture) 

  
  std::ifstream is(multipole_map_name_.c_str());

  std::string str;

  std::vector<double> vals;

  std::vector<data_holder> newdata;

  while( is.good() ){
    // have to read TWO lines
  line1:
    std::getline(is, str);
    if( is.eof() )
      break;
    boost::trim(str);
    //    std::cout << str << '\n';
    if( str[0] == '#' || str.size() == 0)
      goto line1;

  line2:
    std::getline(is, str);
    if( is.eof() )
      break;
    boost::trim(str);
    //    std::cout << str << '\n';
    if( str[0] == '#' || str.size() == 0)
      goto line2;
    
    // tokenize
    boost::char_separator<char> sep(" \t");
    typedef boost::tokenizer<boost::char_separator<char> > tokenize_on_blanks;
    tokenize_on_blanks tokens(str,sep);
    tokenize_on_blanks::const_iterator b = tokens.begin(),
      e = tokens.end();
    
    vals.clear();
    while( b!=e ){
      vals.push_back(std::strtod( (*b++).c_str(),0));
    }

    //    std::cout << vals.size() << '\n';
    std::size_t const size = vals.size();
    if( size < 1 || size%2 == 0 )
      continue;

    std::size_t const coeffs = (size-1)/2;
    //    std::cout << coeffs << '\n';
    data_holder dh;
    dh.By0 = vals[0]*tesla;

    for( std::size_t i=0; i!=coeffs; ++i){
      dh.ab.push_back( std::make_pair(vals[i+1],vals[i+1+coeffs]) );
    }

    //    std::cout << dh.ab.size() << '\n';

    newdata.push_back( dh );

    if( is.eof() )
      break;
  }

  std::swap(data_,newdata);
  //  std::cout << data_.size() << '\n';
  //  std::vector<data_holder>::const_iterator b = data_.begin(), e = data_.end();
  //  while( b!=e ){
  //    std::cout << b->By0 << ' ' << b->ab[0].first << '\n';
  //    ++b;
  //  }

  return true;
}

std::string 
gm2ringsim::detailedMultipoleStorageImpl::multipole_map_name(std::string const& new_name){
  if( new_name == multipole_map_name_ )
    return multipole_map_name_;

  std::string old = multipole_map_name_;
  multipole_map_name_ = new_name;

  if(!load_multipole_map())
    multipole_map_name_ = old;

  return old;
}


////////////////////////////////////////////////
////////////////////////////////////////////////
////// storageFieldMessenger
////////////////////////////////////////////////
////////////////////////////////////////////////

gm2ringsim::storageFieldMessenger::storageFieldMessenger(storageFieldController *sfc) :
  controller_(sfc)
{
  std::string dir = "/g2MIGTRACE/storageField/";
  storageFieldDir_ = new G4UIdirectory(dir.c_str());
  storageFieldDir_ -> SetGuidance("Storage field settings");

  centralFieldCmd_ = 
    new G4UIcmdWithAString( std::string(dir+"centralField").c_str(), this);
  centralFieldCmd_->SetGuidance("Set the type of the central field implementation");
  centralFieldCmd_->SetParameterName("Choices", true);
  centralFieldCmd_->SetCandidates("uniform detailedmultipole averagemultipole get");
  centralFieldCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

  fringeFieldCmd_ = 
    new G4UIcmdWithAString( std::string(dir+"fringeField").c_str(), this);
  fringeFieldCmd_->SetGuidance("Set the type of the fringe field implementation");
  fringeFieldCmd_->SetParameterName("Choices", true);
  fringeFieldCmd_->SetCandidates("uniform nonuniform get");
  fringeFieldCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

  centralValueCmd_ =
    new G4UIcmdWithADoubleAndUnit( std::string(dir+"setCentralValue").c_str(), this);
  centralValueCmd_->SetGuidance("Set the vertical field magnitude at "
				"the nominal storage orbit");
  centralValueCmd_->SetParameterName("Field Value",false);
  centralValueCmd_->SetDefaultUnit("gauss");
  centralValueCmd_->SetUnitCandidates("tesla gauss kilogauss");
  centralValueCmd_->SetUnitCategory("Magnetic flux density");
  centralValueCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

  getCentralValueCmd_ =
    new G4UIcmdWithoutParameter( std::string(dir+"getCentralValue").c_str(),this);
  getCentralValueCmd_->SetGuidance("Get the vertical field magnitude at "
				   "the nominal storage orbit");
  getCentralValueCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

#if 0
  ////
  std::string fdir = dir+"fringeFieldImpl/";
  fringeImplDir_ = new G4UIdirectory(fdir.c_str());
  fringeImplDir_->SetGuidance("Fringe field implementation specific settings");

  fringeMapCmd_ =
    new G4UIcmdWithAString( std::string(fdir+"fringeMapName").c_str(),this);
  fringeMapCmd_->SetGuidance("Get/Set the fringe field value map filename");
  fringeMapCmd_->SetParameterName("Filename",true);
  fringeMapCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);


  ////
  std::string dmdir = dir+"detailedMultipoleFieldImpl/";
  detailedMultipoleImplDir_ = new G4UIdirectory(dmdir.c_str());
  detailedMultipoleImplDir_->SetGuidance("Central field detailed mapped multipole "
					 "implementation specific settings");
  detailedMapCmd_ =
    new G4UIcmdWithAString( std::string(dmdir+"detailedMapName").c_str(),this);
  detailedMapCmd_->SetGuidance("Get/Set the detailed central field"
			       " value multipole map filename");
  detailedMapCmd_->SetParameterName("Filename",true);
  detailedMapCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);
#endif
}

gm2ringsim::storageFieldMessenger::~storageFieldMessenger(){
  delete getCentralValueCmd_;
  delete centralValueCmd_;
  delete fringeFieldCmd_;
  delete centralFieldCmd_;
  delete storageFieldDir_;
}

void gm2ringsim::storageFieldMessenger::SetNewValue(G4UIcommand* command,G4String newval){
  if( command == centralFieldCmd_ ){
    if( newval == "get" || newval.size() == 0 ){
      G4cout << "The storage ring central field uses a "
	     << controller_->getCentralFieldImpl()
	     << " distribution.\n";
    } else {
      controller_->setCentralFieldImpl(newval);
    }
    return;
  }

  if( command == fringeFieldCmd_ ){
    if( newval == "get" || newval.size() == 0 ){
      G4cout << "The storage ring fringe field uses a "
	     << controller_->getFringeFieldImpl()
	     << " distribution.\n";
    } else {
      controller_->setFringeFieldImpl(newval);
    }
    return;
  }

  if( command == centralValueCmd_ ){
    double temp = centralValueCmd_->GetNewDoubleValue(newval);
    controller_->central_field(temp);
    return;
  }

  if( command == getCentralValueCmd_ ){
    G4cout << "The current vertical field at the nominal storage orbit is "
	   << getCentralValueCmd_->ConvertToString(controller_->central_field(),
						   "tesla")
	   << '\n';
  }

}


gm2ringsim::detailedMultipoleMessenger::
detailedMultipoleMessenger(detailedMultipoleStorageImpl* dmsi) :
  dmsi_(dmsi)
{
  std::string dir = "/g2MIGTRACE/storageField/";
  std::string dmdir = dir+"detailedMultipoleFieldImpl/";
  detailedMultipoleImplDir_ = new G4UIdirectory(dmdir.c_str());
  detailedMultipoleImplDir_->SetGuidance("Central field detailed mapped multipole "
					 "implementation specific settings");
  detailedMapCmd_ =
    new G4UIcmdWithAString( std::string(dmdir+"detailedMapName").c_str(),this);
  detailedMapCmd_->SetGuidance("Get/Set the detailed central field "
			       "value multipole map filename");
  detailedMapCmd_->SetParameterName("Filename",true);
  detailedMapCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

  setThetaCmd_ =
    new G4UIcmdWithADoubleAndUnit( std::string(dmdir+"setThetaOffset").c_str(), this);
  setThetaCmd_->SetGuidance("Set the theta offset of the map in global coordinates");
  setThetaCmd_->SetParameterName("theta", false);
  setThetaCmd_->SetDefaultUnit("radian");
  setThetaCmd_->SetUnitCandidates("radian degree");
  setThetaCmd_->SetUnitCategory("Angle");
  setThetaCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

  getThetaCmd_ =
    new G4UIcmdWithoutParameter( std::string(dmdir+"getThetaOffset").c_str(), this);
  getThetaCmd_->SetGuidance("Get the theta offset");
  getThetaCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);
  
}

gm2ringsim::detailedMultipoleMessenger::~detailedMultipoleMessenger(){
  delete getThetaCmd_;
  delete setThetaCmd_;
  delete detailedMapCmd_;
  delete detailedMultipoleImplDir_;
}
void gm2ringsim::detailedMultipoleMessenger::SetNewValue(G4UIcommand *command, G4String newval){

  if( command == detailedMapCmd_ ){
    if( newval.size() == 0 )
      G4cout << "The detailed multipole map is loaded from "
	     << dmsi_->multipole_map_name() << '\n';
    else
      dmsi_->multipole_map_name(newval);
    return;
  } else if ( command == setThetaCmd_ ) {
    dmsi_->theta_offset(setThetaCmd_->GetNewDoubleValue(newval));
    return;
  } else if ( command == getThetaCmd_ ){
    G4cout << "The detailed multipole map is offset by "
	   << dmsi_->theta_offset() << " radians\n";
    return;
  }

}


gm2ringsim::fringeStorageMessenger::fringeStorageMessenger(fringeStorageImpl* fsi) :
  fsi_(fsi)
{

  std::string dir = "/g2MIGTRACE/storageField/";
  std::string fdir = dir+"fringeFieldImpl/";
  fringeImplDir_ = new G4UIdirectory(fdir.c_str());
  fringeImplDir_->SetGuidance("Fringe field implementation specific settings");

  fringeMapCmd_ =
    new G4UIcmdWithAString( std::string(fdir+"fringeMapName").c_str(),this);
  fringeMapCmd_->SetGuidance("Get/Set the fringe field value map filename");
  fringeMapCmd_->SetParameterName("Filename",true);
  fringeMapCmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

}

gm2ringsim::fringeStorageMessenger::~fringeStorageMessenger(){
  delete fringeMapCmd_;
  delete fringeImplDir_;
}

void gm2ringsim::fringeStorageMessenger::SetNewValue(G4UIcommand* command, G4String newval){
  if( command == fringeMapCmd_ ){
    if( newval.size() == 0 )
      G4cout << "The fringe field map is loaded from "
	     << fsi_->fringe_map_name() << '\n';
    else
      fsi_->fringe_map_name(newval);
    return;
  }
}


/** @bug Hack!  I need the storageFieldController to be built before
    it is referenced the first time "for real".  There must be a
    better way...  <-- WTF does this comment mean? I'm not even sure
    it's right anymore. */
namespace {
  gm2ringsim::storageFieldController const& r = gm2ringsim::storageFieldController::getInstance();
}



