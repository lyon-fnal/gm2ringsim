/** @file stationConstruction.cc

    Implements the detector station build/rebuild code.

    @author Kevin Lynch
    @date 2011
*/

#include "stationConstruction.hh"
#include "stationParameters.hh"
#include "stationMessenger.hh"

#include "messageFacility/MessageLogger/MessageLogger.h"

#include "constructionMaterials.hh"
#include "SDHandleOwner.hh"
#include "geometryHelpers.hh"

#include "G4String.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"

#include <sstream>
#include <iomanip>
#include <cmath>

struct index_t {
  int arc, pos;
};

#ifdef TESTBEAM
static int kNumberStations = 1;
#else
static int kNumberStations = 24;
#endif

namespace {

  index_t const indices[] = {
    {0 ,0}, {0 ,1}, 
    {1 ,0}, {1 ,1}, 
    {2 ,0}, {2 ,1}, 
    {3 ,0}, {3 ,1}, 
    {4 ,0}, {4 ,1},
    {5 ,0}, {5 ,1}, 
    {6 ,0}, {6 ,1}, 
    {7 ,0}, {7 ,1}, 
    {8 ,0}, {8 ,1}, 
    {9 ,0}, {9 ,1}, 
    {10,0}, {10,1}, 
    {11,0}, {11,1}, 
  };

  /** @bug these parameters shouldn't be hard coded here ... they
      should be drawn from a database "somewhere" ... mostly because
      they are - to varying degrees - inconsistent from file to
      file. */

  // first scallop angle
  // second one is just 15 degrees further on, by symmetry
  // not sure these are right, but the images look great!
  G4double const theta_out[] = { 11.96*degree, (11.96+15.)*degree },
    theta_in[] = { theta_out[0]-0.12*degree, theta_out[1]-0.12*degree },
    theta_c[] = { (theta_in[0]+theta_out[0])/2., 
		  (theta_in[1]+theta_out[1])/2.};
  
  /** @bug I should add "inch" as a generic unit to be available
      everywhere. */
  G4double const in = 25.4*mm;
  
  // window radii ... not _exactly_ sure anymore where these come
  // from; I THINK they come from vacChamberConstruction at
  // pt_c[wallRegion] and then one has to consider wall thickness.
  /** @bug are these right? */
  G4double const r_in = 268.873*in,
    r_out = 276.149*in, 
    r_c = (r_in+r_out)/2.;
  
  // window angle _FROM_THE_RADIAL_!!
  G4double const window_angle = 3.53*degree;

   G4int hackint = 0;
}

stationConstruction::stationConstruction() :
  params_(new stationParameters(this)),
  mess_(new stationMessenger(params_))
{
  station_P_.assign(0);
}  


stationConstruction::~stationConstruction(){
  for(int i=0; i!=kNumberStations; ++i)
    destroyStation(i);
}

void stationConstruction::
ConstructStations(std::vector<G4VPhysicalVolume *> const & arcs){
  arcs_ = arcs;
  for(int i=0; i!=kNumberStations; ++i){
    buildStation(i);
    //std::cout << "Station " << i << " has been built." << std::endl;
  }
}

void stationConstruction::rebuildAll() {
  for(int i=0; i!=kNumberStations; ++i){
    destroyStation(i);
    buildStation(i);
  }
  doRefresh();
}

void stationConstruction::rebuildStation(G4int station){
  destroyStation(station);
  buildStation(station);
  doRefresh();
}

void stationConstruction::doRefresh(){
  G4UImanager::GetUIpointer() -> ApplyCommand("/vis/viewer/refresh");      
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}

void stationConstruction::buildStation(G4int s/*tation*/){
  // NOTE ON COORDINATE SYSTEM: Things are really messed up here
  // ... that's because we're dealing with a number of interrelated
  // coordinate systems: the global coordinate system(s) (in which we
  // know all the positions), the arc section coordinate system (in
  // which we must do the placement of the station), the station
  // coordinate system, in which we place the subobjects, and finally
  // the coordinate system of the outputs.  Ick.  While I should be
  // able to predict/postdict the correct values and signs, much of
  // this has by necessity been figured out by trial and (lots of)
  // error. 

  // In the ARC coordinate system, the order is "r,t,v" for "ring
  // radial, ring beam direction (thickness), ring vertical"

  hackint = 0;
  //---------------------tdr38 add---------------------
  // get all of this station's parameters.
  station_params const& this_p = params_->get_params(s);


  bool tapered = this_p.calorimeter.tapered ;
  double taperedoffset = 0;
  if(tapered)
    taperedoffset = 15;
  
  G4double const r_2 = params_->radial(s)/2.;
  G4double const t_2 = params_->thickness(s)/2. + taperedoffset/2;
  G4double const v_2 = params_->vertical(s)/2.;
  //---------------end tdr38 add----------------------

  G4VSolid *solid = new G4Box("station_S", r_2, t_2, v_2);

  G4LogicalVolume *log =
    new G4LogicalVolume(solid,
			constructionMaterials::Vacuum(),
			"station_L");
  
  std::ostringstream oss;
  oss << "DetectorStation[" << std::setfill('0') << std::setw(2)
      << s+1 << ']';
  G4String stationname = oss.str();

  // vacuum window window outer edge
  G4ThreeVector window_edge(r_out*std::cos(theta_out[indices[s].pos]), 
			    r_out*std::sin(theta_out[indices[s].pos]),
			    0.);
  
  // get the unit vectors in along the window
  G4ThreeVector 
    unit_along(std::cos(theta_in[indices[s].pos]+window_angle),
	       std::sin(theta_in[indices[s].pos]+window_angle),
	       0.); // OK
  G4ThreeVector unit_vertical(0,0,-1.); // OK ... yes, up :-(
  G4ThreeVector unit_normal = unit_along.cross(unit_vertical); // OK
							       // beamlike 
  station_t const& this_station= params_->get_params(s).station;

  // Calculate the necessary shifts induced by the user defined
  // rotations.  
  /** @bug Only include the user rotations around the vertical
      axis. */
  G4double const vrots = std::sin( std::abs(this_station.v_rotation) );
  G4double const vrotc = std::cos(this_station.v_rotation);
  G4double const correction_along = (vrotc-1.)*r_2 + vrots*t_2;
  G4double const correction_normal = vrots*r_2 + (vrotc-1.)*t_2;

  // this also works as expected (+r, +t, -v)
#ifdef TESTBEAM
  // for test beam we only have the calorimeter in this volume.
  //  This can be modified if we want to simulated the tracking, trigger hodo, etc.
  G4ThreeVector pos(0,0,params_->get_params(s).calorimeter.thickness/2); 
#else
  // for the real g-2 station
  G4ThreeVector pos = 
    // center the station on the center of the outer window edge
    window_edge 
    // move it in by the radial half width
    - r_2*unit_along 
    // move it downstream by the thickness half width
    + t_2*unit_normal
    // We have to move the station around so that it doesn't "whack"
    // (technical term, that) into the outside of the vacuum walls
    // when it's rotated 
    /** @bug doesn't include the r_rotation and t_rotation corrections
	... should probably get this by multiplying appropriate
	rotation matrices, but this will take some thought... */ 
    - correction_along * unit_along
    + correction_normal * unit_normal
    // finally, let's apply the user defined offsets
    +this_station.r_offset*unit_along
    +this_station.t_offset*unit_normal
    +this_station.v_offset*unit_vertical;
#endif

  // counterclockwise rotation around the (?,?,-v) direction
  /** @bug this does not include the optional user rotation about the
      r and t axes */
#ifdef TESTBEAM
    G4RotationMatrix *rot = new G4RotationMatrix(0,0,0);
#else
    G4RotationMatrix *rot = 
    new G4RotationMatrix(0,0,
			 theta_in[indices[s].pos] +window_angle
			 -params_->get_params(s).station.v_rotation);
#endif

  // Now we place the station
  station_P_[s] = 
    new G4PVPlacement(rot, pos, log, stationname, 
		      arcs_[indices[s].arc]->GetLogicalVolume(),
		      false, 0);

  G4VisAttributes *stationVisAtt = 
    new G4VisAttributes(G4Colour(1.,1.,1.,1.));
  stationVisAtt->SetForceSolid(0);
  log->SetVisAttributes(stationVisAtt);

  // Next, place the necessary objects _within_ the station ... we
  // don't need to keep track of these guys, since we can walk the
  // Logical volume hierarchy to find the S,V,P objects and delete
  // them.  

  // this will keep track of where the center of the next object
  // should be in the "thickness" direction.
#ifdef TESTBEAM
  double along = 0;
#else
  double along = -t_2;
#endif


#ifdef TESTBEAM
  G4String name;
#else
  G4String name = stationname+".front_plate";
  //  G4cout<<name<<G4endl;
  build_plate(name, s, this_p.front_plate, along);
  name = stationname+".hodoscope_front_plane";
  build_hodo(name, s, this_p.front_plane, along);
  name = stationname+".mid_plate";
  //  G4cout<<name<<G4endl;
  build_plate(name, s, this_p.mid_plate, along);
  name = stationname+".hodoscope_rear_plane";
  build_hodo(name, s, this_p.rear_plane, along);
  name = stationname+".back_plate";
  //  G4cout<<name<<G4endl;
  build_plate(name, s, this_p.back_plate, along);
  name = stationname+".gap";
  //  G4cout<<name<<G4endl;
  build_plate(name, s, this_p.gap, along);
#endif

  /** @bug this should all get replaced with user selectable classes
      derived from a calorimeter base class. */
  /* ..... CALORIMETER START ..... */
  // Let's put the calorimeter inside the station...
  // Make it just a bit larger than necessary so that we can see 
  // particles entering it before entering the PbF2
  G4Box *calo_S = new G4Box("calo_S",
			    (this_p.calorimeter.radial+this_p.calorimeter.epsilon)/2,			    
			    (this_p.calorimeter.vertical+this_p.calorimeter.epsilon)/2,
			    (this_p.calorimeter.thickness+taperedoffset+this_p.calorimeter.epsilon)/2);
  G4LogicalVolume *calo_L = 
    new G4LogicalVolume(calo_S,
			constructionMaterials::Vacuum1(),
			"calo_L");
#ifdef TESTBEAM
// intentionally doing nothing here...
#else
  along += (this_p.calorimeter.thickness+this_p.calorimeter.epsilon+taperedoffset)/2.;
#endif
  pos = G4ThreeVector(0,along,0);
  along += (this_p.calorimeter.thickness+this_p.calorimeter.epsilon+taperedoffset)/2.;

  name=stationname+".calorimeter";
  //  G4cout << name << G4endl;
#ifdef TESTBEAM
  G4PVPlacement* calo_P = new G4PVPlacement(new G4RotationMatrix(0, pi/2, 0), 
					    pos, calo_L, name,
					    station_P_[s]->GetLogicalVolume(), false, 0);
#else
  G4RotationMatrix* caloRot = new G4RotationMatrix;
  caloRot->rotateX(M_PI/2*rad); //Rotates by pi/2 radians around x-axis
  G4PVPlacement* calo_P = new G4PVPlacement(caloRot, 
					    pos, calo_L, name,
					    station_P_[s]->GetLogicalVolume(), false, 0);
#endif

  G4VisAttributes *caloVisAtt = 
    new G4VisAttributes(G4Colour(1.,0.,0.,1.));
  // caloVisAtt->SetForceSolid(1);
  calo_L->SetVisAttributes(caloVisAtt);

  /** @bug need compositing SD here) */
  calorimeterSD *caloSD = SDHandleOwner::getInstance().getCalorimeterSD();
  calo_L->SetSensitiveDetector( caloSD );
  
  // now build a volume that bounds tightly to the PbF2 that let's us
  // define surfaces
  // Coordinates are weird: +y is the vertical direction, +z is into calo,
  // +x is still radially out  
  G4Box *caloBound_S = new G4Box("caloBound_S",
				 this_p.calorimeter.radial/2,				 
				 this_p.calorimeter.vertical/2,
				 (this_p.calorimeter.thickness+taperedoffset)/2);
  G4LogicalVolume *caloBound_L = 
  new G4LogicalVolume(caloBound_S,
                      constructionMaterials::Vacuum1(),
                      "caloBound_L");
  pos = G4ThreeVector(0,0,0);
  name=stationname+".PbF2Bounding";
  //  G4cout << name << G4endl;
  G4PVPlacement* caloBound_P = new G4PVPlacement(0, pos, caloBound_L, name,
                                            calo_L, false, 0);
  

  /* ..... CALORIMETER END ..... */

#ifdef TESTBEAM
// intentionally doing nothing here...
#else
  name = stationname+".final_plate";
  //  G4cout<<name<<G4endl;
  build_plate(name, s, this_p.final_plate, along);
#endif


  // ~~~~~ wsun: add array of PbF2 crystals ~~~~~
  double epsilon = this_p.calorimeter.epsilon ;
  double xtalWidth = this_p.calorimeter.xtalWidth ;
  double xtalHeight = this_p.calorimeter.xtalHeight ;
  double xtalDepth = this_p.calorimeter.xtalDepth ;
  double epoxyDepth = this_p.calorimeter.epoxyDepth ;
  double pmtDepth = this_p.calorimeter.pmtDepth ;
  G4String photodetectorShape = this_p.calorimeter.photodetectorShape ;
  double photodetectorSize = this_p.calorimeter.photodetectorSize ;

  G4ThreeVector xhat( 1., 0., 0. ) ;
  G4ThreeVector yhat( 0., 1., 0. ) ;
  G4ThreeVector zhat( 0., 0., 1. ) ;
    
  double wrappingDepth = epsilon;
  double totalDepth = wrappingDepth + xtalDepth + epoxyDepth + pmtDepth 
    + taperedoffset;
  if( totalDepth > this_p.calorimeter.thickness + taperedoffset )
     mf::LogWarning("StationConstruction") << "WARNING: xtal + epoxy + pmt longer than calo mother volume!"
	       << std::endl ;

  double wrappingDepthCenter = ( this_p.calorimeter.thickness + taperedoffset - wrappingDepth ) / 2. ;
  G4ThreeVector wrappingOrigin( 0., 0., -wrappingDepthCenter );
  double xtalDepthCenter = wrappingDepthCenter - ( wrappingDepth + xtalDepth ) / 2. ;
  G4ThreeVector xtalOrigin( 0., 0., -xtalDepthCenter ) ;
  double taperedDepthCenter = xtalDepthCenter - ( xtalDepth + taperedoffset ) / 2 ;
  G4ThreeVector taperedOrigin(0., 0., -taperedDepthCenter ) ;
  double epoxyDepthCenter = taperedDepthCenter - ( taperedoffset + epoxyDepth ) / 2. ;
  G4ThreeVector epoxyOrigin( 0., 0., -epoxyDepthCenter ) ;
  double pmtDepthCenter = epoxyDepthCenter - ( epoxyDepth + pmtDepth ) / 2. ;
  G4ThreeVector pmtOrigin( 0., 0., -pmtDepthCenter ) ;
    
  // Xtal reflecting surface: for now assume clad in millipore
//  G4OpticalSurface* xtalSurface = constructionMaterials::Millipore();
  G4String frontOpticalMaterial = this_p.calorimeter.frontWrapping;
  G4String sideOpticalMaterial = this_p.calorimeter.sideWrapping;
  G4String backOpticalMaterial = this_p.calorimeter.backWrapping;
 
  // Define volumes for the front and back wrappings:
  // Coordinates are weird: +y is the vertical direction, +z is into calo,
  // +x is still radially out

  // The front wrapping volume covers the entire front edge of the 
  // caloBound volume and is wrapping depth deep:
  G4Box* frontWrapping_S = new G4Box("frontWrapping_S",
				     this_p.calorimeter.radial/2,				     
				     this_p.calorimeter.vertical/2,
				     wrappingDepth/2);
  G4LogicalVolume* frontWrapping_L = 
    new G4LogicalVolume( frontWrapping_S,
			 constructionMaterials::Vacuum1(),
			 "frontWrapping_L" );
  // Move the front wrapping volume to the front of the caloBound volume:
  G4PVPlacement* frontWrapping_P = 
    new G4PVPlacement( 0,
		       wrappingOrigin,
		       frontWrapping_L,
		       "frontWrapping_P",
		       caloBound_L,
		       false,
		       0);

  // The back wrapping volume will contain the epoxy volumes, but not the
  // photodetector volumes, and will cover the entire back edge of caloBound:
  G4Box* backWrapping_S = new G4Box("backWrapping_S",
				    this_p.calorimeter.radial/2,				    
				    this_p.calorimeter.vertical/2,
				    epoxyDepth/2);
  G4LogicalVolume* backWrapping_L =
    new G4LogicalVolume( backWrapping_S,
			 constructionMaterials::Vacuum1(),
			 "backWrapping_L" );
  // Move the back wrapping volume to where the epoxy volumes will be:
  G4PVPlacement* backWrapping_P =
    new G4PVPlacement( 0,
		       epoxyOrigin,
		       backWrapping_L,
		       "backWrapping_P",
		       caloBound_L,
		       false,
		       0);		      
  // Color the wrapping volumes cyan to ensure they reached the right spot:
  G4VisAttributes *wrappingVisAtt = 
    new G4VisAttributes(G4Colour(0.,1.,1.,1.));
  frontWrapping_L->SetVisAttributes(wrappingVisAtt);
  backWrapping_L->SetVisAttributes(wrappingVisAtt);  

  int xtalCount = 0 ;
  int nXtalRows = this_p.calorimeter.nXtalRows;
#ifdef TESTBEAM
  nXtalRows = 3;
#endif
  for( int irow = 0 ; irow < nXtalRows ; ++irow )
  {
     int nXtalCols = this_p.calorimeter.nXtalCols;
#ifdef TESTBEAM
     nXtalCols = 3;
#endif
     G4ThreeVector offset = -( double( nXtalCols - 1 ) ) / 2.
	* xtalWidth * xhat
	+ ( double( nXtalRows - 1 ) / 2. - double( irow ) )
        * xtalHeight * yhat ;

#ifdef TESTBEAM
     if ( irow != 1 ) {
	--nXtalCols;
	G4ThreeVector halfWidthShift = 0.5 * xtalWidth * xhat;
	offset += halfWidthShift;
     }
#endif

     G4ThreeVector xtalPos = xtalOrigin + offset ;
     G4ThreeVector taperPos = taperedOrigin + offset;
     G4ThreeVector epoxyPos = offset ;
     G4ThreeVector pmtPos = pmtOrigin + offset ;

     for( int icol = 0 ; icol < nXtalCols; ++icol, ++xtalCount )
       {
	 // Coordinates are weird: +y is the vertical direction, +z is into calo,
	 // +x is still radially out, so y and z are switched in many places
    
	 // Xtal volume -- need 2*epsilon gap on the sides for reflective surface
	 G4Box* xtal_S = new G4Box( "xtal_S",
				    ( xtalWidth - epsilon ) / 2.,				    
				    ( xtalHeight - epsilon ) / 2.,
				    xtalDepth / 2.) ;
	 G4LogicalVolume* xtal_L = new G4LogicalVolume( xtal_S,
							constructionMaterials::PbF2(),
							"xtal_L" ) ;
	 G4VisAttributes *xtalVisAtt = 
	   new G4VisAttributes(G4Colour(0.,1.,0.,1.));
	 xtal_L->SetVisAttributes( xtalVisAtt ) ;
       

	 G4LogicalVolume* taper_L;
	 if ( tapered ) {
	   //Tapered volumes for crystal ends:
	   G4Trd* taper_S = new G4Trd( "taper_S",
				       ( xtalWidth - epsilon ) / 2,
				       16/2,    // smaller end is 16 mm on a side
				       ( xtalWidth - epsilon ) / 2,
				       16/2,    // smaller end is 16 mm on a side
				       taperedoffset / 2);
	   taper_L = new G4LogicalVolume( taper_S,
					  constructionMaterials::PbF2(),
					  "taper_L") ;
	   taper_L->SetVisAttributes( xtalVisAtt ) ;
	 }
       
	 /*new G4OpticalSurface( "PbF2Surface" ) ;
	   xtalSurface->SetType( dielectric_metal ) ;
	   xtalSurface->SetFinish( polished ) ;
	   xtalSurface->SetModel( unified ) ;
	 */

	 // Declare variables to avoid them going out of scope after the if/else:
	 G4LogicalVolume* epoxy_L ;
	 G4LogicalVolume* pmt_L ;
       
	 // PMT and Epoxy volumes:
	 // For a square pmt, the epoxy will also be square, with the
	 // length of a side given by photodetectorSize:
	 if( photodetectorShape.compare("Square") == 0 ) {
	   G4Box* epoxy_S = new G4Box( "epoxy_S",
				       photodetectorSize / 2.,
				       photodetectorSize / 2.,
				       epoxyDepth / 2. ) ;
	   epoxy_L = new G4LogicalVolume( epoxy_S,
					  constructionMaterials::BicronBC630(),    // Epoxy(),
					  "epoxy_L" ) ;
	   G4VisAttributes *epoxyVisAtt = 
	     new G4VisAttributes(G4Colour(1.,0.,0.,1.));
	   epoxy_L->SetVisAttributes( epoxyVisAtt ) ;
	 
	   // PMT or SiPM volume
	   G4Box* pmt_S = new G4Box( "pmt_S",
				     photodetectorSize / 2.,
				     photodetectorSize / 2.,
				     pmtDepth / 2. ) ;
	   pmt_L = new G4LogicalVolume( pmt_S,
					constructionMaterials::Borosilicate(),
					"pmt_L" ) ;
	   G4VisAttributes *pmtVisAtt = 
	     new G4VisAttributes(G4Colour(0.,0.,1.,1.));
	   pmt_L->SetVisAttributes( pmtVisAtt ) ;

	   // Place the epoxies and photodetectors appropriately:
	   std::ostringstream epoxyName;
	   epoxyName << "epoxy[" << std::setfill('0') << std::setw(2) << s+1 << ']';
	   G4PVPlacement* epoxy_P =
	     new G4PVPlacement( 0,
				epoxyPos,
				epoxy_L,
				epoxyName.str(),
				backWrapping_L,
				false,
				xtalCount ) ;

	   std::ostringstream pmtName;
	   pmtName << "pmt[" << std::setfill('0') << std::setw(2) << s+1 << ']';
	   G4PVPlacement* pmt_P =
	     new G4PVPlacement( 0,
				pmtPos,
				pmt_L,
				pmtName.str(),
				caloBound_L,
				false,
				xtalCount ) ;         
	 }

	 // If the pmt is not square, it and the epoxy are circular, with 
	 // a diameter given by the photodetectorSize:
	 else if( photodetectorShape.compare("Circle") == 0 ) {
	   G4Tubs* epoxy_S = new G4Tubs( "epoxy_S",
					 0, photodetectorSize / 2,
					 epoxyDepth / 2,
					 0, 360. * deg);
	   epoxy_L = new G4LogicalVolume( epoxy_S,
					  constructionMaterials::BicronBC630(),
					  "epoxy_L");
	   G4VisAttributes *epoxyVisAtt = 
	     new G4VisAttributes(G4Colour(1.,0.,0.,1.));
	   epoxy_L->SetVisAttributes( epoxyVisAtt ) ;

	   G4Tubs* pmt_S = new G4Tubs( "pmt_S",
				       0, photodetectorSize / 2,
				       pmtDepth / 2,
				       0, 360. * deg);
	   pmt_L = new G4LogicalVolume( pmt_S,
					constructionMaterials::Borosilicate(),
					"pmt_L" ) ;
	   G4VisAttributes *pmtVisAtt = 
	     new G4VisAttributes(G4Colour(0.,0.,1.,1.));
	   pmt_L->SetVisAttributes( pmtVisAtt ) ;

	   // Place the epoxies and photodetectors appropriately:
	   // Define the rotation matrix so the circular face of the cylinder
	   // is up against the end of the crystal:

	   std::ostringstream epoxyName;
	   epoxyName << "epoxy[" << std::setfill('0') << std::setw(2) << s+1 << ']';
	   G4PVPlacement* epoxy_P = 
	     new G4PVPlacement( 0,
				epoxyPos,
				epoxy_L,
				epoxyName.str(),
				backWrapping_L,
				false,
				xtalCount);

	   std::ostringstream pmtName;
	   pmtName << "pmt[" << std::setfill('0') << std::setw(2) << s+1 << ']';
	   G4PVPlacement* pmt_P = 
	     new G4PVPlacement( 0,
				pmtPos,
				pmt_L,
				pmtName.str(),
				caloBound_L,
				false,
				xtalCount);	   

	 }

	 else {
	   std::cout << "SOMETHING WRONG WITH PHOTODETECTOR SHAPE! " 
		     << "DOUBLE CHECK .JSON FILE AND RESTART RUN!" << std::endl;
	 }
	 
	 // Increment the positions to the end of the next crystal:
         epoxyPos += xtalWidth * xhat ;
	 pmtPos += xtalWidth * xhat ;

         // Set sensitive detectors
         xtalSD *xtal = SDHandleOwner::getInstance().getXtalSD();
         xtal_L->SetSensitiveDetector( xtal ) ;
	 epoxy_L->SetSensitiveDetector( xtal );
	 taper_L->SetSensitiveDetector( xtal );

         pmtSD *pmt = SDHandleOwner::getInstance().getPMTSD() ;
         pmt_L->SetSensitiveDetector( pmt ) ;

         std::ostringstream xtalName;
         xtalName << "xtal[" << std::setfill('0') << std::setw(2) << s+1 << ']';
         G4PVPlacement* xtal_P =
	   new G4PVPlacement( 0,
			      xtalPos,
			      xtal_L,
			      xtalName.str(),
			      caloBound_L,
			      false,
			      xtalCount ) ;
	 G4PVPlacement* taper_P;
	 if ( tapered ) {
	   std::ostringstream taperName;
	   taperName << "taper[" << std::setfill('0') << std::setw(2) << s+1 << ']';	 
	   taper_P =
	     new G4PVPlacement( 0,
				taperPos,
				taper_L,
				taperName.str(),
				caloBound_L,
				false,
				xtalCount ) ;
	 }
         xtalPos += xtalWidth * xhat ;
	 taperPos += xtalWidth * xhat ;
         // Wrapping is on front surface and all of the long sides.
         // The small gaps between the crystals means that all the long faces
         // interface with the calorimeter mother volume.  If we want a wrapped
         // front face, then move the crystals in by epsilon (wrappingDepth).
         // For an open front face, keep the front face right against 
	 // calorimeter volume boundary.
         // Epoxy against the back face to prevents reflection on ends.
	 
	 // Wrap the crystals appropriately:
	 // Define the optical surface for the side wrappings:
	 if( sideOpticalMaterial.compare("None") != 0 ) {
	   G4OpticalSurface* sideXtalSurface = 
	     constructionMaterials::findOpticalByName(sideOpticalMaterial);
	   G4LogicalBorderSurface* sideXtalSurface_P =
	     new G4LogicalBorderSurface( "PbF2SideSurface",
					 xtal_P,
					 caloBound_P,
					 sideXtalSurface);
	   if ( tapered ) {
	     G4LogicalBorderSurface* taperSurface_P =
	       new G4LogicalBorderSurface( "PbF2TaperSurface",
					   taper_P,
					   caloBound_P,
					   sideXtalSurface);
	   }
	 }
	 // Define the optical surface for the front wrapping:
	 if( frontOpticalMaterial.compare("None") != 0 ) {
	   G4OpticalSurface* frontXtalSurface = 
	     constructionMaterials::findOpticalByName(frontOpticalMaterial);
	   G4LogicalBorderSurface* frontXtalSurface_P =
	     new G4LogicalBorderSurface( "PbF2FrontSurface",
					 xtal_P,
					 frontWrapping_P,
					 frontXtalSurface);
	 }
	 // Define the optical surface for the back wrapping:
	 if( backOpticalMaterial.compare("None") != 0 ) {
	   G4OpticalSurface* backXtalSurface = 
	     constructionMaterials::findOpticalByName(backOpticalMaterial);
	   G4LogicalBorderSurface* backXtalSurface_P = 
	     new G4LogicalBorderSurface( "PbF2BackSurface",
					 xtal_P,
					 backWrapping_P,
					 backXtalSurface);
	 }
       }
  }
    
  // ~~~~~ end wsun add ~~~~~

}

void stationConstruction::
build_plate(G4String pv_name, G4int station,
	    plate_t const& plate, G4double& along){
  if( !plate.create )
    return;
  std::ostringstream oss(pv_name);
  oss << "_S";
  G4Box *plate_S = new G4Box(oss.str(), 
			     params_->radial(station)/2.,
			     plate.thickness/2.,
			     params_->vertical(station)/2.);
  oss.str(pv_name);
  oss << "_L";
  G4LogicalVolume *plate_L = 
    new G4LogicalVolume(plate_S,
			constructionMaterials::findByName(plate.material),
			oss.str());
  
  // In the station coordinates, this works right vis-a-vis the global
  // coordinate system: (+r,+t,-v)
  along += plate.thickness/2.;
  G4ThreeVector pos = G4ThreeVector(0,along,0);
  along += plate.thickness/2.;
  
  //G4VPhysicalVolume *plate_P =
  new G4PVPlacement(0, pos, plate_L, pv_name,
  		    station_P_[station]->GetLogicalVolume(), false, 0);
  /** @bug this color manipulation will crash or do some other funky
      thing if we add more components .... */ 
  double red = hackint*0.1;
  double green = 1.0-hackint*0.1;
  ++hackint;
  G4VisAttributes *plate_VisAtt =
    new G4VisAttributes(G4Colour(red,green,0.,1.));
  plate_VisAtt->SetForceSolid(1);
  plate_L -> SetVisAttributes(plate_VisAtt);

  ringSD *ring = SDHandleOwner::getInstance().getRingSD();
  plate_L->SetSensitiveDetector( ring );
}

void stationConstruction::
build_hodo(G4String pv_name, G4int station, 
	   hodoscope_t const& hodo, G4double& along){
  if( !hodo.create )
    return;
  double rwide, vwide;
  double const rsize = params_->radial(station);
  double const vsize = params_->vertical(station);
  if( hodo.is_vertical ){
    rwide = rsize/hodo.staves;
    vwide = vsize;
  } else {
    rwide = rsize;
    vwide = vsize/hodo.staves;
  }

  along += hodo.thickness/2.;
  double inner_along = (hodo.is_vertical ? -rsize : vsize)/2.;
  double d_inner_along = (hodo.is_vertical ? rwide : -vwide)/2.;

  std::ostringstream pvn_ss, oss;
  for(int i=0; i!= hodo.staves; ++i){
    pvn_ss.str("");
    pvn_ss << pv_name << '[' 
	   << std::setfill('0') << std::setw(2) << i+1 << ']';
    //    G4cout << pvn_ss.str() << G4endl;
    inner_along += d_inner_along;
    oss.str("");
    oss << pvn_ss.str() << "_S";
    G4Box *hodo_S = new G4Box(oss.str(),
			      rwide/2.,
			      hodo.thickness/2.,
			      vwide/2.);
    oss.str("");
    oss << pvn_ss.str() << "_L";
    G4LogicalVolume *hodo_L = 
      new G4LogicalVolume(hodo_S,
			  constructionMaterials::findByName(hodo.material),
			  oss.str());
    G4ThreeVector pos = G4ThreeVector( (hodo.is_vertical ? inner_along : 0), 
				       along,
				       (hodo.is_vertical ? 0 : inner_along) );
    new G4PVPlacement(0, pos, hodo_L, pvn_ss.str(),
		      station_P_[station]->GetLogicalVolume(), false, 0);
    G4VisAttributes *hodo_VisAtt =
      new G4VisAttributes(G4Colour(0.,0.,0.5+0.5*(i%2)));
    hodo_VisAtt->SetForceSolid(1);
    hodo_L -> SetVisAttributes(hodo_VisAtt);

    //    ringSD *ring = SDHandleOwner::getInstance().getRingSD();
    //    hodo_L->SetSensitiveDetector( ring );

    hodoscopeSD *hSD = SDHandleOwner::getInstance().getHodoscopeSD();
    hodo_L->SetSensitiveDetector( hSD );

    inner_along += d_inner_along;
  } // loop over staves
  along += hodo.thickness/2.;
}

void stationConstruction::destroyStation(G4int station){
  //  G4cout << "stationConstruction::destroyStation(" 
  //    <<  station << "):\n";

  if( station_P_[station] == 0 ){
    //    G4cout << "\tNo station to destroy!\n";
    return;
  }

  /** @bug This may not play well with the derivable calorimeter that
      I plan to implement.  That may have to be special cased before
      the call to destroy */
  geometryHelpers::destroyPhysical(station_P_[station]);

  //  G4cout << "\tDestroyed\n";
}
