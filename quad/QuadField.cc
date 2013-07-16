/** @file QuadField.cc

    Implements the quadField class and all of the supporting classes:
    quadFieldImpl, innerFieldImpl, vanishingInnerImpl,
    simpleInnerImpl, mappedInnerImpl, outerFieldImpl,
    vanishingOuterImpl, simpleOuterImpl, quadFieldFactory,
    quadFieldMessenger, mappedInnerMessenger, simpleInnerMessenger,
    and simpleOuterMessenger.

    The quadField class, which provides multiple models of more or
    less detailed quadrupole electric fields, including time dependent
    scraping.

    The field parameters are derived from the Quad NIM Paper,
    including the distorted values (due to the vacuum walls).

    The parameter data used in this implementation correspond to
    negative muon injection, with a quadrupole field gradient n =
    0.137, corresponding to plate voltage of 24kV.  We choose to
    implement this operating point, as we have a precision multipole
    expansion for this condition (both with and without scraping
    enabled). 

    For this state of affairs, the top/bottom plates are charged to
    negative voltages, with inner/outer plates at positive voltage.

    At injection, with scraping enabled, the plates are charged to the
    following voltages (from Quad NIM paper):
    <table>
    <tr><th>Region</th><th>Top</th><th>Bottom</th><th>Inner</th><th>Outer</th></tr>
    <tr><td>Q1</td><td>-24</td><td>-17</td><td>+24</td><td>+24</td></tr>
    <tr><td>Q2</td><td>-24</td><td>-17</td><td>+17</td><td>+24</td></tr>
    <tr><td>Q3</td><td>-24</td><td>-17</td><td>+24</td><td>+24</td></tr>
    <tr><td>Q4</td><td>-24</td><td>-17</td><td>+24</td><td>+17</td></tr>
    </table>

    To calculate the quadrupole E fields from the polar maps produced
    below, we find it necessary to convert (x,y,z)_world into
    (r_q,th_q)_quadrupole (See below coordates system).  Then do the
    interpolation on the polar grid, convert (E_rq,E_thq) into
    (E_xq,E_yq), and then finally convert (E_xq,E_yz) into
    (E_x,E_y,E_z) in world coordinates!
    
    To avoid confusion, some important definitions: 
    a) subscript _q indicates value is in quadrupole coordinates defined below
    b) "r" is radial distance in storage planr from center of ring to particle 
    c) "r_q" is distance from center of storage region to particle 
    
    Quadrupole coordinates :

    <em>"Friggin' awesome ASCII art!" raves the New York Times</em>
    <pre>    
               +r_q ^
                     \
          +th_q  ____>\
                /      \   ^ +y_q
               /       _\\__|____
              |       |  \ |    | 
              |       |   \|    |
        +x_q <--------|----0    |    +z_q is into the emacs/downstream (ha! funny!)
                      |         |
                      |_________|
    </pre>      
    Note: Coordinate systems should always be right handed!  Hence,
    +x_q points to the left so that +z_q points in the downstream
    direction.

    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
    
    Artified as part of the g2migtrace port
    @author Brendan Kiburg
    @date Dec 2012

*/

#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4Track.hh"

#include <fstream>
#include <cmath>
#include <cstring>
//#define MATRIX_BOUND_CHECK
#include <CLHEP/Matrix/Matrix.h>
using namespace CLHEP;
#include <utility>

#include "TMath.h"

#include "gm2ringsim/quad/QuadField.hh"
#include "gm2ringsim/arc/StorageRingField.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"


namespace gm2ringsim {
  
  //FIXME: These used to be part of QuadConstruction.hh private vars
  enum plate_type {INNERPLATE, OUTERPLATE, TOPPLATE, BOTTOMPLATE, plate_type_end};
  enum plate_section {SECTION13, SECTION26, plate_section_end};

  // Source: Quad NIM Paper, Table 5 - page 476.  Note: the distorted
  // values (due to the vacuum walls) have been input in this table
  // Cosine Term/Normal mulitpole expansion coefficients
  G4double const storageMC_normal[14] = 
    {   -12.* volt,
	20177.8 * volt,
	-28. * volt,
	-153. * volt,
	-10. * volt,
	-26. * volt,
	0. * volt,
	-5.5 * volt,
	0. * volt,
	-391.3 * volt,
	0. * volt,
	18. * volt,
	0. * volt,
	52.3 * volt};
  
  // Sine Term/Skew multipole expansion coefficients
  G4double const storageMC_skew[14] = 
    {  0. * volt,
       0.1 * volt,
       0. * volt,
       0.1 * volt,
       -8. * volt,
       0.1 * volt,
       0. * volt,
       0.2 * volt,
       0. * volt,
       0.1 * volt,
       0. * volt,
       0. * volt,
       0. * volt,
       -0.1* volt};
  
  
  // Source: Quad NIM Paper, Table 7 - page 477
  // Cosine Term/Normal multipole expansion coefficients
  G4double const scrapingMC_normalVert[14] = 
    {    0. * volt,
	 18706. * volt,
	 0. * volt,
	 375. * volt,
	 0. * volt,
	 -41. * volt,
	 0. * volt,
	 -76. * volt,
	 0. * volt,
	 -364. * volt,
	 0. * volt,
	 -3. * volt,
	 0. * volt,
	 49. * volt};
  
  // Sine Term/Skew Multipole expansion coefficients
  G4double const scrapingMC_skewVert[14] = 
    {    -2155. * volt,
	 0. * volt,
	 837. * volt,
	 0. * volt,
	 -117. * volt,
	 0. * volt,
	 -57.5 * volt,
	 0. * volt,
	 52. * volt,
	 0. * volt,
	 -8. * volt,
	 0. * volt,
	 -5. * volt,
	 0. * volt};
  
  
  
  // Source: Quad NIM Paper, Table 8 - page 477
  // Cosine Term/Normal multipole expansion coefficients
  G4double const scrapingMC_normalHoriz[14] = 
    {    -2150. * volt,
	 17235. * volt,
	 -840. * volt,
	 28. * volt,
	 -121. * volt,
	 -39. * volt,
	 -57. * volt,
	 -5. * volt,
	 53. * volt,
	 -334. * volt,
	 9. * volt,
	 -6. * volt,
	 -5. * volt,
	 45. * volt};
  
  // Sine Term/Skew multipole expansion coefficients
  G4double const scrapingMC_skewHoriz[14] = 
    {    -2156. * volt,
	 0. * volt,
	 837. * volt,
	 0. * volt,
	 -117. * volt,
	 0. * volt,
	 -57. * volt,
	 0. * volt,
	 52. * volt,
	 0. * volt,
	 -8. * volt,
	 0. * volt,
	 -5. * volt,
	 0. * volt};



  struct volt_holder {
    G4double scraping, steady_state;
  };
  struct voltage {
    volt_holder v[4];
  };

 voltage volts[] = {
    // Q1
    {{   {+24*kilovolt, +24*kilovolt}, {+24*kilovolt, +24*kilovolt}, 
	 {-24*kilovolt, -24*kilovolt}, {-17*kilovolt, -24*kilovolt}   }}, 
    // Q2
    {{   {+17*kilovolt, +24*kilovolt}, {+24*kilovolt, +24*kilovolt}, 
	 {-24*kilovolt, -24*kilovolt}, {-17*kilovolt, -24*kilovolt}   }}, 
    // Q3
    {{   {+24*kilovolt, +24*kilovolt}, {+24*kilovolt, +24*kilovolt}, 
	 {-24*kilovolt, -24*kilovolt}, {-17*kilovolt, -24*kilovolt}   }}, 
    // Q4
    {{   {+24*kilovolt, +24*kilovolt}, {+17*kilovolt, +24*kilovolt}, 
	 {-24*kilovolt, -24*kilovolt}, {-17*kilovolt, -24*kilovolt}   }}, 
  };

 voltage volts_n18[] = {
    // Q1
    {{   {+40*kilovolt, +40*kilovolt}, {+40*kilovolt, +40*kilovolt}, 
	 {-40*kilovolt, -40*kilovolt}, {-34*kilovolt, -40*kilovolt}   }}, 
    // Q2
    {{   {+34*kilovolt, +40*kilovolt}, {+40*kilovolt, +40*kilovolt}, 
	 {-40*kilovolt, -40*kilovolt}, {-34*kilovolt, -40*kilovolt}   }}, 
    // Q3
    {{   {+40*kilovolt, +40*kilovolt}, {+40*kilovolt, +40*kilovolt}, 
	 {-40*kilovolt, -40*kilovolt}, {-34*kilovolt, -40*kilovolt}   }}, 
    // Q4
    {{   {+40*kilovolt, +40*kilovolt}, {+34*kilovolt, +40*kilovolt}, 
	 {-40*kilovolt, -40*kilovolt}, {-34*kilovolt, -40*kilovolt}   }}, 
  };

  /** @bug Units should be defined correctly, and elsewhere */
  G4double in = 25.4*mm;

  /** @bug These numbers are copied from quadConstruction.cc.   C&P is
      evil... */
  G4double dist[] = {
    // don't treat Q1out differently here ... it doesn't matter unless
    //  we actually use measured fields...
    //  INNER  R_magic()-plateSep/2.-innerOuterThickness  - QFR_min
    R_magic()-10.*cm/2.-0.05*cm-276.707*in,
    //  OUTER  QFR_max - (R_magic()+plateSep/2+innerOuterThickness)
    284.370*in - (R_magic()+10.*cm/2.+0.05*cm),
    // TOP topBottomWall-outerWallThickness - plateSep/2.
    7.865*m-0.965*cm - 10.*cm/2.,
    // BOTTOM topBottomWall-outerWallThickness - plateSep/2.
    7.865*m-0.965*cm - 10.*cm/2.
  };
  
}//namespace gm2ringsim


// Quad field implementation
gm2ringsim::QuadField::QuadField(int quadNumber, int quadSection, InnerFieldImpl *ifi, OuterFieldImpl *ofi, bool DoScraping, double ScrapeHV, double StoreHV, int Charge) :
  ifi_(ifi), ofi_(ofi),
  scrapingTurnOffTime(7.*microsecond), quadTimeConstant(5.*microsecond),
  timeOffset(0.), do_scraping_(DoScraping), ScrapeHV_(ScrapeHV), StoreHV_(StoreHV), Charge_(Charge)
{
  int i = quadNumber;
  int j = quadSection;
  G4cout << "============ QuadField ===============" << G4endl;
  G4cout << "| Beam Charge:  " << Charge_ << G4endl;
  G4ThreeVector pnt_uo(gm2ringsim::R_magic()+10*mm, 10*mm, 0);
  G4ThreeVector pnt_do(gm2ringsim::R_magic()+10*mm, -10*mm, 0);
  G4ThreeVector pnt_ui(gm2ringsim::R_magic()-10*mm, 10*mm, 0);
  G4ThreeVector pnt_di(gm2ringsim::R_magic()-10*mm, -10*mm, 0);
  G4ThreeVector v(0, 0, gm2ringsim::R_magic());
  pnt_uo.rotateY(i*TMath::Pi()/2.0);
  pnt_do.rotateY(i*TMath::Pi()/2.0);
  pnt_ui.rotateY(i*TMath::Pi()/2.0);
  pnt_di.rotateY(i*TMath::Pi()/2.0);
  v.rotateY(i*TMath::Pi()/2.0);
  double offset;
  if ( j == 0 ) { offset = 20.0*TMath::Pi()/180.0; }
  if ( j == 1 ) { offset = 45.0*TMath::Pi()/180.0; }
  pnt_uo.rotateY(offset);
  pnt_do.rotateY(offset);
  pnt_ui.rotateY(offset);
  pnt_di.rotateY(offset);
  v.rotateY(offset);
  G4cout.precision(4);
  G4cout << "| Quad[" << i << "][" << j << "]" << G4endl;
  for ( int p = 0; p < 4; p++ ) {
    double Point[4];
    Point[3] = 0.0;
    if ( p == 0 ) { Point[0] = pnt_uo.x(); Point[1] = pnt_uo.y(); Point[2] = pnt_uo.z(); }
    if ( p == 1 ) { Point[0] = pnt_do.x(); Point[1] = pnt_do.y(); Point[2] = pnt_do.z(); }
    if ( p == 2 ) { Point[0] = pnt_ui.x(); Point[1] = pnt_ui.y(); Point[2] = pnt_ui.z(); }
    if ( p == 3 ) { Point[0] = pnt_di.x(); Point[1] = pnt_di.y(); Point[2] = pnt_di.z(); }
    double field[6];
    ifi->GetStorageFieldValue(Point, field);
    G4ThreeVector B(field[0], field[1], field[2]);
    G4ThreeVector E(field[3], field[4], field[5]);
    G4ThreeVector F = E;
    bool downward;
    bool inward;
    if ( Charge * E.y() < 0 ) { downward = true; }
    else { downward = false; }
    if ( Charge * (E.x()*Point[0] + E.z()*Point[2]) < 0 ) { inward = true; }
    else { inward = false; }

    G4cout << "| Force is ";
    if ( downward ) { G4cout << "down and "; }
    if ( !downward ) { G4cout << "up and "; }
    if ( inward ) { G4cout << "toward the ring center "; }
    if ( !inward ) { G4cout << "out from the ring center "; }
    
    if ( p == 0 ) { G4cout << "for +y and +r."; }
    if ( p == 1 ) { G4cout << "for -y and +r."; }
    if ( p == 2 ) { G4cout << "for +y and -r."; }
    if ( p == 3 ) { G4cout << "for -y and -r."; }

    if ( p == 0 ) { 
      if ( downward && !inward ) { G4cout << " Vertical Focus. Horizontal Defocus."; }
    }
    if ( p == 1 ) { 
      if ( !downward && !inward ) { G4cout << " Vertical Focus. Horizontal Defocus."; }
    }
    if ( p == 2 ) { 
      if ( downward && inward ) { G4cout << " Vertical Focus. Horizontal Defocus."; }
    }
    if ( p == 3 ) { 
      if ( !downward && inward ) { G4cout << " Vertical Focus. Horizontal Defocus."; }
    }
    G4cout << G4endl;

  }
  G4cout << "=====================================" << G4endl;

}

void gm2ringsim::QuadField::GetFieldValue( const double *Point,
			       double *EMfield ) const {
  double const time = Point[3] + timeOffset;
  double const r = hypot(Point[0], Point[2]);
  double const xq = r - R_magic();
  double const yq = Point[1];

  // 1) Get the B-field component
  storageFieldController::getInstance().GetFieldValue(Point,EMfield,Charge_);

  // 2) Figure out whether we're inside or outside the inner region
  /** @bug Should get platesep from somewhere else rather than hard
      coding */
  bool const inner = (std::abs(xq) < 5.*cm) && (std::abs(yq) < 5.*cm);
  QuadFieldImpl const *pf = inner ? 
    static_cast<QuadFieldImpl const*>(ifi_) : 
    static_cast<QuadFieldImpl const*>(ofi_);

  // 3) Get the E-field component
  if( time <= scrapingTurnOffTime && do_scraping_ ){
    pf->GetScrapingFieldValue(Point, EMfield);
  } else if( time > scrapingTurnOffTime && 
	     time <= scrapingTurnOffTime + 5.*quadTimeConstant &&
	     do_scraping_ ){
    G4double const u = std::exp(-(time-scrapingTurnOffTime) / quadTimeConstant);
    double Escr[6], Estor[6];
    pf->GetStorageFieldValue(Point, Estor);
    pf->GetScrapingFieldValue(Point, Escr);
    for(int i=3; i!=6; ++i){
      EMfield[i] = u*Escr[i] + (1-u)*Estor[i];
    }
  } else {
    pf->GetStorageFieldValue(Point, EMfield);
  }


  
  bool debug = false;
  if ( debug ) {
    std::cout.precision(3);
    double dB = EMfield[1] - 0.00145;
    dB = 0.0;
    if ( EMfield[0] > 0.01 || EMfield[2] > 0.01 || EMfield[0] < -0.01 || EMfield[2] < -0.01 || dB > 0.01 || dB < -0.01 ) {
      std::cout << "QuadField::GetFieldValue(" << xq << " , " << yq << ") = ["
		<< EMfield[0] << " , "
		<< EMfield[1] << " , "
		<< EMfield[2] << "]" << std::endl;
      std::cout << "QuadField::GetFieldValue(" << xq << " , " << yq << ") = ["
		<< EMfield[3] << " , "
		<< EMfield[4] << " , "
		<< EMfield[5] << "]" << std::endl;
    }
  }
}
  
double gm2ringsim::QuadField::scraping_turnoff(double offtime){
  std::swap(offtime, scrapingTurnOffTime);
  return offtime;
}

double gm2ringsim::QuadField::scraping_timeconst(double timeconst){
  std::swap(timeconst, quadTimeConstant);
  return timeconst;
}

double gm2ringsim::QuadField::scraping_offset(double offset){
  std::swap(offset, timeOffset);
  return offset;
}

bool gm2ringsim::QuadField::do_scraping(bool enable){
  std::swap(enable, do_scraping_);
  return enable;
}

// Vanishing inner field
void gm2ringsim::VanishingInnerImpl::GetScrapingFieldValue(double const */*Point*/,
					       double *EMfield) const {
  EMfield[3] = EMfield[4] = EMfield[5] = 0.;
}

void gm2ringsim::VanishingInnerImpl::GetStorageFieldValue(double const */*Point*/,
					      double *EMfield) const {
  EMfield[3] = EMfield[4] = EMfield[5] = 0.;
}

// Vanishing outer field
void gm2ringsim::VanishingOuterImpl::GetScrapingFieldValue(double const */*Point*/,
					       double *EMfield) const {
  EMfield[3] = EMfield[4] = EMfield[5] = 0.;
}

void gm2ringsim::VanishingOuterImpl::GetStorageFieldValue(double const */*Point*/,
					      double *EMfield) const {
  EMfield[3] = EMfield[4] = EMfield[5] = 0.;
}



// simple inner field
gm2ringsim::SimpleInnerImpl::SimpleInnerImpl(double tb_voltage, double delta_volts[4], bool DoScraping) :
  storage_tb_volts_(tb_voltage), 
  DoScraping_(DoScraping)
{
  std::memcpy(scraping_dvolts_, delta_volts, 4*sizeof(double));
}

double gm2ringsim::SimpleInnerImpl::storage_tb_volts(double v){
  std::swap(v,storage_tb_volts_);
  return v;
}

void gm2ringsim::SimpleInnerImpl::scraping_dvolts(double dv[4]) const {
  std::memcpy(dv, scraping_dvolts_, 4*sizeof(double));
}

void gm2ringsim::SimpleInnerImpl::scraping_dvolts(double dv[4]){
  for(int i=0; i!=4; ++i)
    std::swap(dv[i], scraping_dvolts_[i]);
}

/** @bug Can these SimpleInnerImpl calculations be sped up during the
    transition period by cacheing? */
void gm2ringsim::SimpleInnerImpl::GetScrapingFieldValue(const double *Point,
					    double *EMfield) const {
  // first, load the storage value
  GetStorageFieldValue(Point,EMfield);
  // then, modify is with linear fields
  double const r = hypot(Point[0],Point[2]);

  // vertical shift
  // (dVtop - dVbottom)/platesep
  /** @bug Get the plate sep from quadConstruction somehow */
  double const dEyq = (scraping_dvolts_[2]-scraping_dvolts_[3])/(10.*cm);
  EMfield[4] += dEyq;
  
  // horizontal shift
  // (dVout - dVin)/platesep
  /** @bug Get the plate sep from quadConstruction somehow */
  double const dExq = (scraping_dvolts_[1]-scraping_dvolts_[0])/(10.*cm);
  EMfield[3] += dExq*Point[0]/r;
  EMfield[5] += dExq*Point[2]/r;
}

void gm2ringsim::SimpleInnerImpl::GetStorageFieldValue(const double *Point,
					   double *EMfield) const {
  /** @bug Get the plate sep from quadConstruction somehow */
  static double const d = 5*cm*5*cm; // (platesep/2)**2

  double const b2 = -storage_tb_volts_/d;
  double const r = hypot(Point[0],Point[2]);
  double const xq = r - R_magic();
  double const yq = Point[1];


  // horizontal field
  // in quad coordinates
  double const Ex = -2.*b2*xq;
  // convert to global coordinates
  EMfield[3] = Ex*Point[0]/r;
  EMfield[5] = Ex*Point[2]/r;

  // the vertical field is already in global coordinates
  EMfield[4] = 2.*b2*yq;
  //G4cout << "SimpleInnerImpl::GetStorageFieldValue(): " << xq << "\t" << r << "\t" << Ex << "\t" << b2 << G4endl;
}


// Simple outer field
gm2ringsim::SimpleOuterImpl::SimpleOuterImpl(double tb_voltage, double delta_volts[4], bool DoScraping) :
  storage_tb_volts_(tb_voltage),
  DoScraping_(DoScraping)

{
  std::memcpy(scraping_dvolts_, delta_volts, 4*sizeof(double));
}

double gm2ringsim::SimpleOuterImpl::storage_tb_volts(double v){
  std::swap(v,storage_tb_volts_);
  return v;
}

void gm2ringsim::SimpleOuterImpl::scraping_dvolts(double dv[4]) const {
  std::memcpy(dv, scraping_dvolts_, 4*sizeof(double));
}

void gm2ringsim::SimpleOuterImpl::scraping_dvolts(double dv[4]){
  for(int i=0; i!=4; ++i)
    std::swap(dv[i], scraping_dvolts_[i]);
}

double const gm2ringsim::SimpleOuterImpl::rmax = 5.*cm;

void gm2ringsim::SimpleOuterImpl::GetScrapingFieldValue(const double *Point,
					    double *EMfield) const {
  double const r = hypot(Point[0],Point[2]);
  double const xq = r - R_magic();
  double const yq = Point[1];

  double Exq = 0., Eyq = 0.;

  /** @bug Need symbolic indices here, too. */
  if( std::abs(xq) > rmax ){
    int const idx = xq > 0 ? 1 : 0; // OUT/IN
    G4double const v = -storage_tb_volts_ - scraping_dvolts_[idx];
    Exq = copysign(1.,xq)*v/dist[idx];
  } else { // abs(yq) > 0
    int const idx = yq > 0 ? 2 : 3 ;// TOP/BOTTOM
    G4double const v = storage_tb_volts_ - scraping_dvolts_[idx];
    Eyq = copysign(1.,yq)*v/dist[idx];
  }

  // vertical
  EMfield[4] = Eyq;

  // horizontal
  EMfield[3] = Exq*Point[0]/r;
  EMfield[5] = Exq*Point[2]/r;
}

void gm2ringsim::SimpleOuterImpl::GetStorageFieldValue(const double *Point,
					   double *EMfield) const {
  double const r = hypot(Point[0],Point[2]);
  double const xq = r - R_magic();
  double const yq = Point[1];

  double Exq = 0., Eyq = 0.;

  /** @bug Need symbolic indices here, too. */
  if( std::abs(xq) > rmax ){
    int const idx = xq > 0 ? 1 : 0; // OUT/IN
    G4double const v = -storage_tb_volts_;
    Exq = copysign(1.,xq)*v/dist[idx];
  } else { // abs(yq) > 0
    int const idx = yq > 0 ? 2 : 3 ;// TOP/BOTTOM
    G4double const v = storage_tb_volts_;
    Eyq = copysign(1.,yq)*v/dist[idx];
  }

  // vertical
  EMfield[4] = Eyq;

  // horizontal
  EMfield[3] = Exq*Point[0]/r;
  EMfield[5] = Exq*Point[2]/r;
}



// QuadFieldFactory

// builds the default implementation fields
gm2ringsim::QuadFieldFactory::QuadFieldFactory(bool DoScraping, double ScrapeHV, double StoreHV, int Charge){
  DoScraping_ = DoScraping;
  ScrapeHV_ = ScrapeHV;
  StoreHV_ = StoreHV;
  Charge_ = Charge;
  for(int i=0; i!=4; ++i){
    for(int j=0; j!=2; ++j){
      ifi_[i][j] = innerFromType(i,j,SIMPLE_INNER);
      ofi_[i][j] = outerFromType(i,j,SIMPLE_OUTER);
    }
  }
}

gm2ringsim::QuadFieldFactory::~QuadFieldFactory(){
  for(int i=0; i!=4; ++i){
    for(int j=0; j!=2; ++j){
      // FIXME:  gm2ringsim COMPLAINS if i delete an object
      // of an abstract class (gm2ringsim::InnerFieldImpl )
      //delete ifi_[i][j];
      //delete ofi_[i][j];
    }
  }
}



gm2ringsim::QuadField* gm2ringsim::QuadFieldFactory::buildQuadField(int quadNumber, int quadSection){
  return new QuadField(quadNumber, quadSection, ifi_[quadNumber][quadSection], ofi_[quadNumber][quadSection], DoScraping_, ScrapeHV_, StoreHV_, Charge_);
}


gm2ringsim::InnerFieldImpl* gm2ringsim::QuadFieldFactory::innerFromType(int quadNumber, int /*quadSection*/,
									inner_field_impl_type type){
  switch(type){
  case VANISHING_INNER:
    return new VanishingInnerImpl;
  case SIMPLE_INNER:
    {
      double tb_volts;
      double dvolts[4];

      //----------------------
      // Assumes negative beam
      //----------------------      
      if ( StoreHV_ == 40*kilovolt ) {
	tb_volts = volts_n18[quadNumber].v[TOPPLATE].steady_state;
       	dvolts[0] = -tb_volts-volts_n18[quadNumber].v[INNERPLATE].scraping;
      	dvolts[1] = -tb_volts-volts_n18[quadNumber].v[OUTERPLATE].scraping;
	dvolts[2] = tb_volts-volts_n18[quadNumber].v[TOPPLATE].scraping;
	dvolts[3] = tb_volts-volts_n18[quadNumber].v[BOTTOMPLATE].scraping;
      }
      else {
	tb_volts  = volts[quadNumber].v[TOPPLATE].steady_state;
       	dvolts[0] = -tb_volts-volts[quadNumber].v[INNERPLATE].scraping;
      	dvolts[1] = -tb_volts-volts[quadNumber].v[OUTERPLATE].scraping;
       	dvolts[2] = tb_volts-volts[quadNumber].v[TOPPLATE].scraping;
       	dvolts[3] = tb_volts-volts[quadNumber].v[BOTTOMPLATE].scraping;
      }

      //------------------------
      // Negate if positive beam
      //------------------------
      if ( Charge_ == 1 ) {
	tb_volts *= -1;
	for ( int i = 0; i < 4; i++ ) { dvolts[i] *= -1; }
      }

      return new SimpleInnerImpl(tb_volts, dvolts, DoScraping_);
    }
  case MAPPED_INNER:
  default:
    throw unknown_inner_field_impl_type();
  }
}


gm2ringsim::OuterFieldImpl* 
gm2ringsim::QuadFieldFactory::outerFromType(int quadNumber, int /*quadSection*/,
					    outer_field_impl_type type){
  
  switch(type){
  case VANISHING_OUTER:
    return new VanishingOuterImpl;
  case SIMPLE_OUTER:
    {
      double tb_volts;
      double dvolts[4];
      
      //----------------------
      // Assumes negative beam
      //----------------------      
      if ( StoreHV_ == 40*kilovolt ) {
	tb_volts = volts_n18[quadNumber].v[TOPPLATE].steady_state;
       	dvolts[0] = -tb_volts-volts_n18[quadNumber].v[INNERPLATE].scraping;
      	dvolts[1] = -tb_volts-volts_n18[quadNumber].v[OUTERPLATE].scraping;
       	dvolts[2] = tb_volts-volts_n18[quadNumber].v[TOPPLATE].scraping;
       	dvolts[3] = tb_volts-volts_n18[quadNumber].v[BOTTOMPLATE].scraping;
      }
      else {
	tb_volts  = volts[quadNumber].v[TOPPLATE].steady_state;
       	dvolts[0] = -tb_volts-volts[quadNumber].v[INNERPLATE].scraping;
      	dvolts[1] = -tb_volts-volts[quadNumber].v[OUTERPLATE].scraping;
       	dvolts[2] = tb_volts-volts[quadNumber].v[TOPPLATE].scraping;
       	dvolts[3] = tb_volts-volts[quadNumber].v[BOTTOMPLATE].scraping;
      }

      //------------------------
      // Negate if positive beam
      //------------------------
      if ( Charge_ == 1 ) {
	tb_volts *= -1;
	for ( int i = 0; i < 4; i++ ) { dvolts[i] *= -1; }
      }

      return new SimpleOuterImpl(tb_volts, dvolts, DoScraping_);
    }
  default:
    throw unknown_outer_field_impl_type();
  }
}
