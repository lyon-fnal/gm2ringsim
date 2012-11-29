/** // Implementation of Inflector
    All measurements taken from the g-2 NIM Paper titled "The                 
    Superconducting Inflector for the BNL g-2 experiment" : Reference         
    "A 491 23-40 (2002)"                                                      
                                                                              
    Additional information about the inflector windows, end flanges and       
    conductor windings covering the storage aperture comes from:              
    email/telephone corresponence between Lee Roberts, Wuzheng Meng,          
    and Zach Hartwig (emails can be found in the g2MIGTRACE repository:       
    https://g-2.bu.edu/repos/g2MIGTRACE/ref                                   
                                                                              
    Please see                                                                
    https://g-2.bu.edu/repos/g2MIGTRACE/ref/inflectorMockup.jpg for           
    excellent pictoral discription                                            
                                                                              
    Dimensions and position/rotation information come from the                
    cryostat wall come from "inf_layout.dwg".  ZSH 27 JUL 09.                 
                                                                              
    @author Zach Hartwig                                                      
    @author Kevin Lynch                                                       
    @date 2005-2011                                                           

    @author Brendan Kiburg
    @date Nov 2012 artified
*/

#include "gm2ringsim/inflector/Inflector_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "artg4/material/Materials.hh"

//Geant4
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4Transform3D.hh"
#include "Geant4/G4RotationMatrix.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4ClassicalRK4.hh"
#include "Geant4/G4Mag_UsualEqRhs.hh"
#include "Geant4/G4Mag_SpinEqRhs.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4UImanager.hh"
#include "Geant4/G4UniformMagField.hh"

#include "gm2ringsim/inflector/inflectorField.hh"
#include "gm2ringsim/inflector/inflectorGeometry.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>

#include <tr1/tuple>
#include <tr1/functional>

#include <boost/algorithm/string.hpp>


//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Inflector::Inflector(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "inflector"),
	       p.get<std::string>("category", "inflector"),
	       p.get<std::string>("mother_category", "vac")),
  //p.get<std::string>("mother_category", "world")),
  sts_("SpinTracking"), // This is what goes in the fcl file under the RunSettings service
  infGeom_(myName()),
  num_trackers_(infGeom_.num_trackers),
  inflectorMagField_(0),
  iEquation_(0),
  iStepper_(0),
  iChordFinder_(0),
  inflectorFieldManager_(new G4FieldManager),
  launchFieldManager_(0),
  // need a valid rotation matrix, since we swap them                         
  inflectorRotation_(new G4RotationMatrix()),
  // azimuthal span of one arc section                                        
  epsilon_(infGeom_.epsilon),
  // Set the vacuum section that contains the inflector                       
  vacuumInflectorSection_(infGeom_.vacuumInflectorSection),
  // where we are in the construction timeline                                  
  initialBuild_(true), // hopefully we won't need this anymore since geometry will be fixed
  // Set the maximum step length via G4UserLimits.  This is currently           
  // used to limiting the muons step size in the beam channel                   
  maxStepLength_(infGeom_.maxStepLength),
    // Set the defaults which control what components will be put into            
  // the inflector assembly.  By default, both ends of the inflector            
  // are completely closed, as in the real inflector.                           
  useConductorEquivalent_(infGeom_.useConductorEquivalent),
  useUpstreamWindow_(infGeom_.useUpstreamWindow),
  useDownstreamWindow_(infGeom_.useDownstreamWindow),
  useUpstreamConductor_(infGeom_.useUpstreamConductor),
  useDownstreamConductor_(infGeom_.useDownstreamConductor),
  useUpstreamEndFlange_(infGeom_.useUpstreamEndFlange),
  useDownstreamEndFlange_(infGeom_.useDownstreamEndFlange),
// which type of field
//mag_field_type(MAPPED_FIELD)
  mag_field_type_(VANISHING_FIELD),
/** @bug The following fields should be moved to the field                    
    implementation classes.                                                   
    
    The defaults necessary for the inflector field.  The values were          
    obtained from an email from Wuzheng that can be found in                  
    reference section.  They are apparently the values for the                
    '00-'01 runs. */
  conductorCurrent_(infGeom_.conductorCurrent),
  fieldNormConst_(infGeom_.fieldNormConst),
  currentToMagFieldConversion_(infGeom_.currentToMagFieldConversion),
  spin_tracking_(sts_.spinTrackingEnabled)

{
  printf("In the Inflector service constructor\n");
  

  //FIXME: No need for this binding. We can just grab spintracking from
  //      the master fcl and set the spintracking variable accordingly,once.
  /*  std::tr1::function<void(bool)> f =
    std::tr1::bind(&Inflector::enable_spintracking, this,
                   std::tr1::placeholders::_1);
  
  conn_ =
    spinController::getInstance().connect(e_arc_is_parent, f);
  */
}

gm2ringsim::Inflector::~Inflector(){
  //FIXME deal with the conn_ structure
  //  spinController::getInstance().disconnect(conn_);

  //  delete InflectorMessenger;
  delete inflectorRotation_;
}





// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Inflector::doBuildLVs() {
  // Get the pointer to the vacChamber section that contains the inflector

  //Build all the solids and logicals used in the inflector
  BuildCore_SandL();
  BuildPeripherals_SandL();
  
  
  
  sts_.print();
  infGeom_.print();
  std::cout<<"spin tracking enabled is :"<<spin_tracking_<<std::endl;
  std::vector<G4LogicalVolume *> l_inflector;
  return l_inflector;

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Inflector::doPlaceToPVs( std::vector<G4LogicalVolume*> //mother
								      ) {
  //  vacPTR = VacH.at(vacuumInflectorSection);
  //vacPTR_ = mother.at(vacuumInflectorSection_);
  // FIXME Do I actually need vacPTR_ set before I build one of the logical volumes??
  
  // Build the inflector physical volumes
  BuildInflector();

  // Build the inflector cryostat walls.  These are essentially
  // volumes that separate the "inflector chamber" from the storage
  // vacuum chamber.
  BuildCryostatWalls();

  // Build volumes for tracking in the aperture
  BuildTrackingVolumes();

  // Automatically create the macros used by the g2GeneralParticleSource
  GenerateGPSMacros();

  // Build the field
  BuildInflectorField();

  std::vector<G4VPhysicalVolume *> p_inflector;
  return p_inflector;

}

void gm2ringsim::Inflector::BuildCore_SandL() {
  
  G4VSolid *inflector_S = new G4Box("inflector_S",
				     infGeom_.inflector_X,
				     infGeom_.inflector_Y,
				     infGeom_.inflector_Z);
  
  inflector_L_ = new G4LogicalVolume(inflector_S,
				     artg4Materials::Vacuum(),
				     "inflector_L_");
  
  // Create the long rectangular mandrel solid
  G4VSolid *inflectorMandrel_S = new G4Box("inflectorMandrel_S",
					  infGeom_.inflectorMandrel_X,
					  infGeom_.inflectorMandrel_Y,
					  infGeom_.inflectorMandrel_Z);

  inflectorMandrel_L_ = new G4LogicalVolume(inflectorMandrel_S,
					    artg4Materials::Al6061(),
					   "inflectorMandrel_L_");


   // Create 2 solids that will be G4Union'ed together to create the beam channel
  G4VSolid *beamChannel1_S= new G4Box("beamChannel1_S",
				      infGeom_.beamChannel1_X, 
				      infGeom_.beamChannel1_Y, 
				      infGeom_.beamChannel1_Z);
  
  G4VSolid *beamChannel2_S = new G4Trd("beamChannel2_S",
				      infGeom_.beamChannel2_X1, 
				      infGeom_.beamChannel2_X2,
				      infGeom_.beamChannel2_Y1, 
				      infGeom_.beamChannel2_Y2, 
				      infGeom_.beamChannel2_Z);
  
  G4Transform3D joinBeamChannels(G4RotationMatrix(0,0,0),
				 G4ThreeVector(0,0,- infGeom_.beamChannel2_offset));

    
  G4VSolid *beamChannel_US = new G4UnionSolid("beamChannel_US",
					     beamChannel1_S,
					     beamChannel2_S,
					     joinBeamChannels);

  beamChannel_L_ = new G4LogicalVolume(beamChannel_US,
				       artg4Materials::Vacuum(),
				      "beamChannel_L_");
  
  G4UserLimits *stepLimiter = new G4UserLimits(maxStepLength_);
  beamChannel_L_ -> SetUserLimits(stepLimiter);

  

}                                      

void gm2ringsim::Inflector::BuildPeripherals_SandL(){ }
void gm2ringsim::Inflector::BuildInflector(){ } 
void gm2ringsim::Inflector::BuildCryostatWalls(){ }
void gm2ringsim::Inflector::BuildTrackingVolumes(){ }
void gm2ringsim::Inflector::RebuildInflector(){ }
void gm2ringsim::Inflector::DeleteInflector(){ }

void gm2ringsim::Inflector::BuildInflectorField(){ }
void gm2ringsim::Inflector::RebuildFieldImpl(){ }


//FIXME: Do we really need to rebuild? Let's make spin_tracking_ a const and 
//        avoid all of these rebuilds...
void gm2ringsim::Inflector::RebuildEOM(){

  if( iEquation_ )
    delete iEquation_;
  if( iStepper_ )
    delete iStepper_;
  if( iChordFinder_ )
    delete iChordFinder_;

  // Create the equations of motion                                             
  if( !spin_tracking_ ){
    iEquation_ = new G4Mag_UsualEqRhs(inflectorMagField_);
    iStepper_ = new G4ClassicalRK4(iEquation_);
  } else {
    iEquation_ = new G4Mag_SpinEqRhs(inflectorMagField_);
    iStepper_ = new G4ClassicalRK4(iEquation_, 12);
  }
  // Create the chord finder that calculates curved trajectories                
  iChordFinder_ = new G4ChordFinder(inflectorMagField_,
                                   .01*mm, iStepper_);

} // END RebuildEOM()

void gm2ringsim::Inflector::AssignFieldManager(){
  inflectorFieldManager_ -> SetDetectorField(inflectorMagField_);
  inflectorFieldManager_ -> SetChordFinder(iChordFinder_);
  inflector_L_ ->SetFieldManager(inflectorFieldManager_, true);
  launchRegion_L_ ->SetFieldManager(launchFieldManager_, true);
}

/** Sets the downstream aperture position in the azimuthal direction. */
void gm2ringsim::Inflector::setAperturePosition(G4double){ }
/** Set the radial position of the downstream aperture. */
void gm2ringsim::Inflector::setApertureDistance(G4double){ }
/** Sets the vertical tilt angle of the inflector relative to the
    symmetry plane of the ring. */
void gm2ringsim::Inflector::setInflectorTiltAngle(G4double){ }
/** Sets the horizontal swing angle of the inflector axis relative
    to the ring tangent. */
void gm2ringsim::Inflector::setInflectorSwingAngle(G4double){ }
	     
/** @bug Doesn't actually do anything useful, since we don't model
    anything but an "average" plate.                                        
*/
void gm2ringsim::Inflector::setConductorModelType(G4String){ }
/** Sets the upstream end (window, conductor, and flange)
    open/closed. */
void gm2ringsim::Inflector::setUpstreamEndType(G4String){ }
/** Sets the downstream end (window, conductor, and flange)
    open/closed. */
void gm2ringsim::Inflector::setDownstreamEndType(G4String){ }
/** Sets the presence of the upstream window. */
void gm2ringsim::Inflector::setUpstreamWindow(G4String){ }
/** Sets the presence of the upstream conductor. */
void gm2ringsim::Inflector::setUpstreamConductor(G4String){ }
/** Sets the presence of the upstream end flange. */
void gm2ringsim::Inflector::setUpstreamEndFlange(G4String){ }
/** Sets the presence of the downstream window. */
void gm2ringsim::Inflector::setDownstreamWindow(G4String){ }
/** Sets the presence of the downstream conductor. */
void gm2ringsim::Inflector::setDownstreamConductor(G4String){ }
/** Sets the presence of the downstream end flange. */
void gm2ringsim::Inflector::setDownstreamEndFlange(G4String){ }

/** Sets the field strength by setting the conductor current.
    @bug I don't think this does anything at all.                           
*/
void gm2ringsim::Inflector::setConductorCurrent(G4double){ }
/** Sets the normalization constant that converts between conductor
    current and nominal central field value.
    @bug I don't think this does anything at all.                           
*/
void gm2ringsim::Inflector::setFieldNormalizationConstant(G4double){ }
/** Prints inflector status information. */
void gm2ringsim::Inflector::getInflectorInfo(){ }
/** Sets the number of inflector beam aperture tracking volumes. */
void gm2ringsim::Inflector::NumTrackers(G4int //n
					){ }
/** Gets the number of inflector beam aperture tracking volumes. */
G4int gm2ringsim::Inflector::NumTrackers() const { return num_trackers_; }

/** Sets the magnetic field model.
    Options are "vanishing/none", "simple" or uniform, and "mapped"
    from Wuzheng Meng's detailed field maps.                                
*/
void gm2ringsim::Inflector::setMagFieldType(G4String){ }

/** Regenerates the GPS Macros when the inflector geometry is
    rebuilt. */

void gm2ringsim::Inflector::GenerateGPSMacros(){}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Inflector::doCallArtProduces(art::EDProducer * producer) {
//
//}

// Actually add the data to the event
//void gm2ringsim::Inflector::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}

/* These function were previously implemented in g2MIGTRACE but were 
   not ported due to the fact that they only existed in order to 
   update the inflector if a geometry change occurred at run time.
   This is no longer supported in the ARTized version
   
   void gm2ringsim::Inflector::enable_spintracking(bool e){
      if( spin_tracking_ == e )
      return;
  
      spin_tracking_ = e;
      RebuildEOM();
      AssignFieldManager();
    }



*/



using gm2ringsim::Inflector;
DEFINE_ART_SERVICE(Inflector)
