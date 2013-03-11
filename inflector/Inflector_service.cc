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
#include "artg4/util/util.hh"

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
#include "Geant4/G4SDManager.hh"


#include "gm2ringsim/inflector/InflectorField.hh"
#include "gm2ringsim/inflector/inflectorGeometry.hh"
#include "gm2ringsim/inflector/InflectorSD.hh"
#include "gm2ringsim/inflector/InflectorArtRecord.hh"
#include "gm2ringsim/inflector/InflectorHit.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

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
  spin_tracking_(sts_.spinTrackingEnabled),
  inflectorSDname_("InflectorSD"),
  ringSDname_("RingSD"),
  inflectorSD_(0), // will set below
  ringSD_(0)
{
  printf("In the Inflector service constructor\n");
  
  // Let's prepare the sensitive detector, no registration with G4SDManager necessary as 
  // this is done in FiberHarpSD constructor

  inflectorSD_ = artg4::getSensitiveDetector<InflectorSD>(inflectorSDname_);
  ringSD_ = artg4::getSensitiveDetector<RingSD>(ringSDname_);

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
  buildCore_SandL();
  buildPeripherals_SandL();
  // Build the inflector cryostat walls.  These are essentially
  // volumes that separate the "inflector chamber" from the storage
  // vacuum chamber.
  buildCryostatWalls_SandL();

  // Build the sensitive detectors for all logical volumes created above
  buildSensitiveDetectors();
  
  printf("\n\n\n*INFLECTOR_SERVICE*************************************\n");
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->ListTree();
  printf("\n +++INFLECTOR_SERVICE+++++++++++++++++++++++++++++++++++\n\n\n\n");
  
  sts_.print();
  infGeom_.print();
  getInflectorInfo();
  mf::LogInfo("Inflector_Service") <<"spin tracking enabled is :"<<spin_tracking_;

  //FIXME: Is this really what we want to do??
  std::vector<G4LogicalVolume *> l_inflector;
  return l_inflector;

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Inflector::doPlaceToPVs( std::vector<G4LogicalVolume*> vacLV) {
  // 
  vacPTR_ = vacLV.at(vacuumInflectorSection_);
  
  // Build the inflector physical volumes
  buildInflector() ;
  buildCryostatWalls();
  
  // Build volumes for tracking in the aperture
  buildTrackingVolumes();

  // Automatically create the macros used by the g2GeneralParticleSource
  generateGPSMacros();

  // Build the field
  buildInflectorField();

  //FIXME: Is this really what we want to do??
  std::vector<G4VPhysicalVolume *> p_inflector;
  return p_inflector;

}

void gm2ringsim::Inflector::buildCore_SandL() {
  
  G4VSolid *inflector_S = new G4Box("inflector_S",
				     infGeom_.inflector_X,
				     infGeom_.inflector_Y,
				     infGeom_.inflector_Z);
  
  inflector_L_ = new G4LogicalVolume(inflector_S,
				     artg4Materials::Vacuum(),
				     "inflector_L");
  
  // Create the long rectangular mandrel solid
  G4VSolid *inflectorMandrel_S = new G4Box("inflectorMandrel_S",
					  infGeom_.inflectorMandrel_X,
					  infGeom_.inflectorMandrel_Y,
					  infGeom_.inflectorMandrel_Z);

  inflectorMandrel_L_ = new G4LogicalVolume(inflectorMandrel_S,
					    artg4Materials::Al6061(),
					   "inflectorMandrel_L");


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
				      "beamChannel_L");
  
  G4UserLimits *stepLimiter = new G4UserLimits(maxStepLength_);
  beamChannel_L_ -> SetUserLimits(stepLimiter);

  
  G4VisAttributes *channelVisAtt = 
    new G4VisAttributes(G4Colour(0.0,1.0,0.0,1.0));
  channelVisAtt -> SetForceSolid(1);
  beamChannel_L_ -> SetVisAttributes(channelVisAtt);
  
  G4VisAttributes *mandrelVisAtt = 
    new G4VisAttributes(G4Colour(0.0,0.7,1.0,1.0));
  mandrelVisAtt -> SetForceWireframe(1);
  inflectorMandrel_L_ -> SetVisAttributes(mandrelVisAtt);
  
  beamChannel_L_->SetSensitiveDetector(ringSD_);
  inflectorMandrel_L_->SetSensitiveDetector(ringSD_);


}                                      

// Create the solids and logicals that physically bracket the
// inflector mandrel upstream (US) and downstream (DS)
void gm2ringsim::Inflector::buildPeripherals_SandL(){

  G4VSolid *window_S =  new G4Box("window_S", 
				  infGeom_.window_X, 
				  infGeom_.window_Y, 
				  infGeom_.window_Z);
  
  window_DS_L_ = new G4LogicalVolume(window_S,
				    artg4Materials::Al6061(),
				    "window_DS_L",
				    0,
				    0,
				    0);

  window_DS_L_->SetSensitiveDetector(ringSD_);
     

  window_US_L_ = new G4LogicalVolume(window_S,
				    artg4Materials::Al6061(),
				    "window_US_L",
				    0,
				    0,
				    0);

  window_US_L_->SetSensitiveDetector(ringSD_);


  G4VSolid *equivalentAl_S = new G4Box("equivalentAl_S",
				      infGeom_.equivalentPlate_X,
				      (infGeom_.equivalentAlWidth / 2),
				      infGeom_.equivalentPlate_Z);
  
  G4VSolid *equivalentCu_S = new G4Box("equivalentCu_S",
				      infGeom_.equivalentPlate_X,
				      (infGeom_.equivalentCuWidth / 2),
				      infGeom_.equivalentPlate_Z);
  
  G4VSolid *equivalentNbTi_S = new G4Box("equivalentNbTi_S",
					infGeom_.equivalentPlate_X,
					(infGeom_.equivalentNbTiWidth / 2),
					infGeom_.equivalentPlate_Z);
  
  equivalentAl_DS_L_ = 
    new G4LogicalVolume(equivalentAl_S,
			artg4Materials::Al(),
			"equivalentAl_DS_L",
			0,
			0,
			0);
  
  equivalentAl_DS_L_->SetSensitiveDetector(ringSD_);
  
  equivalentAl_US_L_ = 
    new G4LogicalVolume(equivalentAl_S,
			artg4Materials::Al(),
			"equivalentAl_US_L",
			0,
			0,
			0);
  
  equivalentAl_US_L_->SetSensitiveDetector(ringSD_);
  
  equivalentCu_DS_L_ = 
    new G4LogicalVolume(equivalentCu_S,
			artg4Materials::Cu(),
			"equivalentCu_DS_L",
			0,
			0,
			0);
  
  equivalentCu_DS_L_->SetSensitiveDetector(ringSD_);
  
  equivalentCu_US_L_ = 
    new G4LogicalVolume(equivalentCu_S,
			artg4Materials::Cu(),
			"equivalentCu_US_L",
			0,
			0,
			0);
  
  equivalentCu_US_L_->SetSensitiveDetector(ringSD_);
  
  equivalentNbTi_DS_L_ =
    new G4LogicalVolume(equivalentNbTi_S,
			artg4Materials::Cu(),
			"equivalentNbTi_DS_L",
			0,
			0,
			0);
  
  equivalentNbTi_DS_L_->SetSensitiveDetector(ringSD_);
  
  equivalentNbTi_US_L_ =
    new G4LogicalVolume(equivalentNbTi_S,
			artg4Materials::Cu(),
			"equivalentNbTi_US_L",
			0,
			0,
			0);
  

  equivalentNbTi_US_L_->SetSensitiveDetector(ringSD_);

  G4VSolid *flange_S = new G4Box("flange_S", infGeom_.flange_X, 
				 infGeom_.flange_Y, infGeom_.flange_Z);

  endFlange_DS_L_ = 
    new G4LogicalVolume(flange_S,
			artg4Materials::Al6061(),
			"endFlange_DS_L",
			0,
			0,
			0);
  
  endFlange_DS_L_->SetSensitiveDetector(ringSD_);
  
  endFlange_US_L_ = 
    new G4LogicalVolume(flange_S,
			artg4Materials::Al6061(),
			"endFlange_US_L",
			0,
			0,
			0);
  
  endFlange_US_L_->SetSensitiveDetector(ringSD_);

  G4VSolid *launchRegion_S = new G4Box("lanchRegion_S", infGeom_.launch_X,
				       infGeom_.launch_Y, infGeom_.launch_Z);
  

  //  In order to ensure that particle launched from the upstream
  //  vertex do not see any of the storage field, thus ruining their
  //  initial trajectory through the inflector, we need to create a
  //  zero-field region upstream.  This is accomplished via the
  //  "lanchRegion".
  // 
  //  Note: the implementation of the full inflector field may change
  //  the need for this approach.  ZSH - 08 NOV 08
  /** @bug May need the same rigamarole here for spin/not spin as well
      ... still don't know if this can just be dispensed with, per
      ZSH's comment. */

  G4UniformMagField* launchField
    = new G4UniformMagField(G4ThreeVector(0., 0., 0.));
  iEquation_ = new G4Mag_UsualEqRhs(launchField);
  iStepper_ = new G4ClassicalRK4(iEquation_);
  iChordFinder_ = new G4ChordFinder(launchField,1*mm,iStepper_);
  launchFieldManager_ = new G4FieldManager(launchField, iChordFinder_);
  
  launchRegion_L_ = new G4LogicalVolume(launchRegion_S,
					artg4Materials::Vacuum(),
					"launchRegion_L",
					launchFieldManager_,
					0,
					0);
  
  G4VisAttributes *windowVisAtt = 
    new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.0));
  windowVisAtt -> SetForceSolid(1);
  window_US_L_ -> SetVisAttributes(windowVisAtt);
  window_DS_L_ -> SetVisAttributes(windowVisAtt);
  
  G4VisAttributes *alumEqVisAtt = 
    new G4VisAttributes(G4Colour(0.5,0.5,0.5,1.0));
  alumEqVisAtt -> SetForceWireframe(1);
  equivalentAl_US_L_ -> SetVisAttributes(alumEqVisAtt);
  equivalentAl_DS_L_ -> SetVisAttributes(alumEqVisAtt);

  G4VisAttributes *copperEqVisAtt = 
    new G4VisAttributes(G4Colour(1.0,0.6,0.0,1.0));
  copperEqVisAtt -> SetForceSolid(1);
  equivalentCu_US_L_ -> SetVisAttributes(copperEqVisAtt);
  equivalentCu_DS_L_ -> SetVisAttributes(copperEqVisAtt);
  
  G4VisAttributes *NbTiEqVisAtt = 
    new G4VisAttributes(G4Colour(0.0,1.0,1.0,1.0));
  NbTiEqVisAtt -> SetForceWireframe(1);
  equivalentNbTi_US_L_ -> SetVisAttributes(NbTiEqVisAtt);
  equivalentNbTi_DS_L_ -> SetVisAttributes(NbTiEqVisAtt);
  
  G4VisAttributes *endFlangeVisAtt = 
    new G4VisAttributes(G4Colour(1.0,0.0,1.0,1.0));
  endFlangeVisAtt -> SetForceSolid(1);
  endFlange_US_L_ -> SetVisAttributes(endFlangeVisAtt);
  endFlange_DS_L_ -> SetVisAttributes(endFlangeVisAtt);

  G4VisAttributes *launchVisAtt =
    new G4VisAttributes(G4Colour(0.0,1.,0.,1.0));
  launchVisAtt -> SetForceWireframe(1);
  launchRegion_L_ -> SetVisAttributes(launchVisAtt);
    
}

void gm2ringsim::Inflector::buildInflector( ) { 
  
  // "inflector_S/L/P" volumes act as an imaginary "wrapper" for all
  // the daughter volumes that make up the inflector.  It's dimensions
  // are ~equal to the maximum dimensions of the inflector (so as to
  // contain the subvolumes but nothing else) and it's material is
  // vacuum.  This is done so that position and rotation calculations
  // and implementation are done for only this single volume.  Since
  // all subvolumes are daughters of this mother volume, they simply
  // move with the mother automatically.
  inflector_P_ = new G4PVPlacement(calc_rotation(),
				   calc_position(),
				   inflector_L_,
				   "Inflector",
				   vacPTR_ , //vacptr is LV in ART now -> GetLogicalVolume(),
				   false,
				   0);
  
  inflectorMandrel_P_ = new G4PVPlacement(0,
					  G4ThreeVector(0., 0., 0.),
					  inflectorMandrel_L_,
					  "InflectorMandrel",
					  inflector_L_,
					  false,
					  0);
  
  beamChannel_P_ = new G4PVPlacement(0,
				     G4ThreeVector(0., 0., infGeom_.beamChannel_offset),
				     beamChannel_L_,
				     "BeamChannel",
				     inflectorMandrel_L_,
				     false,
				     0);
  
  if(useUpstreamWindow_)
    window_US_P_ = new G4PVPlacement(0,
				    G4ThreeVector(0., infGeom_.windowPlacement, 0.),
				    window_US_L_,
				    "UpstreamWindow",
				    inflector_L_,
				    false,
				    0);

  if(useUpstreamConductor_){
    if(useConductorEquivalent_){
      equivalentAl_US_P_ = new G4PVPlacement(0,
					    G4ThreeVector(0., infGeom_.eqAlPlacement, 0.),
					    equivalentAl_US_L_,
					    "UpstreamEquivalentAl",
					    inflector_L_,
					    false,
					    0);
      
      equivalentCu_US_P_ = new G4PVPlacement(0,
					    G4ThreeVector(0., infGeom_.eqCuPlacement, 0.),
					    equivalentCu_US_L_,
					    "UpstreamEquivalentCu",
					    inflector_L_,
					    false,
					    0);
      
      equivalentNbTi_US_P_ = new G4PVPlacement(0,
					      G4ThreeVector(0., infGeom_.eqNbTiPlacement, 0.),
					      equivalentNbTi_US_L_,
					      "UpstreamEquivalentNbTi",
					      inflector_L_,
					      false,
					      0);
    }
    else{/* NOT YET IMPLEMENTED -> FOR PHYSICAL CONDUCTOR WINDINGS*/}
  }
  


  
  if(useUpstreamEndFlange_)
    endFlange_US_P_ = new G4PVPlacement(0,
				       G4ThreeVector(0., infGeom_.flangePlacement, 0.),
				       endFlange_US_L_,
				       "UpstreamEndFlange",
				       inflector_L_,
				       false,
				       0);
  
  if(useDownstreamWindow_)
    window_DS_P_ = new G4PVPlacement(0,
				    G4ThreeVector(0., -infGeom_.windowPlacement, 0.),
				    window_DS_L_,
				    "DownstreamWindow",
				    inflector_L_,
				    false,
				    0);

  
 if(useDownstreamConductor_){
    if(useConductorEquivalent_){
      equivalentAl_DS_P_ = new G4PVPlacement(0,
					    G4ThreeVector(0., -infGeom_.eqAlPlacement, 0.),
					    equivalentAl_DS_L_,
					    "DownstreamEquivalentAl",
					    inflector_L_,
					    false,
					    0);
      
      equivalentCu_DS_P_ = new G4PVPlacement(0,
					    G4ThreeVector(0., -infGeom_.eqCuPlacement, 0.),
					    equivalentCu_DS_L_,
					    "DownstreamEquivalentCu",
					    inflector_L_,
					    false,
					    0);
      
      equivalentNbTi_DS_P_ = new G4PVPlacement(0,
					      G4ThreeVector(0., -infGeom_.eqNbTiPlacement, 0.),
					      equivalentNbTi_DS_L_,
					      "DownstreamEquivalentNbTi",
					      inflector_L_,
					      false,
					      0);
    }
    else{/* NOT YET IMPLEMENTED -> FOR PHYSICAL CONDUCTOR WINDINGS */}
  }
  
 
  if(useDownstreamEndFlange_)
    endFlange_DS_P_ = new G4PVPlacement(0,
				       G4ThreeVector(0., -infGeom_.flangePlacement, 0.),
				       endFlange_DS_L_,
				       "DownstreamEndFlange",
				       inflector_L_,
				       false,
				       0);

  launchRegion_P_ = new G4PVPlacement(0,
				     G4ThreeVector(0, infGeom_.launchPlacement, 0),
				     launchRegion_L_,
				     "LaunchRegion",
				     inflector_L_,
				     false,
				     0);



  
} //Inflector::buildInflector() 

void gm2ringsim::Inflector::buildCryostatWalls_SandL(){
  G4VSolid *parallelCryoWall_S = new G4Box("parallelCryoWall_S",
					   infGeom_.parWall_X,
					   infGeom_.parWall_Y,
					   infGeom_.parWall_Z);
  
  parallelCryoWall_L_ = new G4LogicalVolume(parallelCryoWall_S,
					    artg4Materials::Al(),
					    "parallelCryoWall_L",
					    0,
					    0,
					    0);
  
  G4VSolid *perpCryoWall_S = new G4Box("perpCryoWall_S",
				       infGeom_.perpWall_X,
				       infGeom_.perpWall_Y,
				       infGeom_.perpWall_Z);
  
  G4VSolid *cryoWindowCutout_S = new G4Box("cryoWindowCutout_S",
					   infGeom_.cryoWindow_X,
					   infGeom_.cryoWindow_Y,
					   infGeom_.cryoWindow_Z);
  
  G4Transform3D cryoWindowTransform(G4RotationMatrix(),
				    G4ThreeVector(infGeom_.cryoWindow_posX,
						  infGeom_.cryoWindow_posY,
						  infGeom_.cryoWindow_posZ));
  
  G4VSolid *perpCryoWall_SS = new G4SubtractionSolid("perpCryoWall_SS",
						     perpCryoWall_S,
						     cryoWindowCutout_S,
						     cryoWindowTransform);
  
  perpCryoWall_L_ = new G4LogicalVolume(perpCryoWall_SS,
					artg4Materials::Al(),
					"perpCryoWall_L",
					0,
					0,
					0);
  
  G4VisAttributes *cryoVisAtt =
    new G4VisAttributes(G4Colour(0.8,0.8,0.8,1.0));
  cryoVisAtt -> SetForceSolid(1);
  
  parallelCryoWall_L_->SetVisAttributes(cryoVisAtt);
  perpCryoWall_L_->SetVisAttributes(cryoVisAtt);
  
  parallelCryoWall_L_->SetSensitiveDetector(ringSD_);
  perpCryoWall_L_->SetSensitiveDetector(ringSD_);
  
} //Inflector::buildCryostatWalls_SandL() 

void gm2ringsim::Inflector::buildSensitiveDetectors(){
  /*  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->ListTree();
  
  
  G4String inflectorSDname = "InflectorSD";
  InflectorSD* anInflectorSD = new InflectorSD(inflectorSDname);
  
  SDman->AddNewDetector( anInflectorSD );
  */
  
  /*
  G4String trackerChamberSDname = "ExN02/TrackerChamberSD";
  ExN02TrackerSD* aTrackerSD = new ExN02TrackerSD( trackerChamberSDname );
  

  SDman->AddNewDetector( aTrackerSD );
  logicChamber->SetSensitiveDetector( aTrackerSD );
  */
} //Inflector::buildSensitiveDetectors()


// Build the Cryostat Physical Volumes
void gm2ringsim::Inflector::buildCryostatWalls(){
  mf::LogInfo("Inflector_Service")<<"About to try to build the CryostatWalls";
  parallelCryoWall_P_ = new G4PVPlacement(new G4RotationMatrix(infGeom_.parWall_alpha,      
					  		       infGeom_.parWall_beta,
					  		       infGeom_.parWall_gamma),
					  G4ThreeVector(infGeom_.parWall_posX, 
					  		infGeom_.parWall_posY,
					  		infGeom_.parWall_posZ),
					  parallelCryoWall_L_,
					  "ParallelCryostatWall",
					  vacPTR_, 
					  false,
					  0);
  
  perpCryoWall_P_ = new G4PVPlacement(new G4RotationMatrix(infGeom_.perpWall_alpha,
							   infGeom_.perpWall_beta,
							   infGeom_.perpWall_gamma),
				      G4ThreeVector(infGeom_.perpWall_posX,
						    infGeom_.perpWall_posY,
						    infGeom_.perpWall_posZ),
				      perpCryoWall_L_,
				      "PerpendicularCryostatWall",
				      vacPTR_, 
				      false,
				      0);
  
} // Inflector::buildCryostatWalls()

void gm2ringsim::Inflector::buildTrackingVolumes(){
  if(num_trackers_ < 2)
    return;
  G4Transform3D const 
    joinBeamChannels(G4RotationMatrix(0,0,0),
		     G4ThreeVector(0,0,-infGeom_.beamChannel2_offset));
  
  std::ostringstream name;
  
  G4VisAttributes *tcbVisAtt = 
    new G4VisAttributes(G4Colour(1,0,0,1.0));
  tcbVisAtt -> SetForceSolid(1);  
  
  for(int i=0; i!=num_trackers_; ++i){
    name.str("");
    name << "InflectorTracker1_S["
	 << std::setfill('0') 
	 << std::setw(2)
	 << i
	 << "]";
    G4Box *tbox = new G4Box(name.str().c_str(),
			    infGeom_.beamChannel1_X,
			    infGeom_.trackerThickness/2,
			    infGeom_.beamChannel1_Z);
    
    
    name.str(""); 
    name << "InflectorTracker2_S["
	 << std::setfill('0') 
	 << std::setw(2)
	 << i
	 << "]";
    G4Trd *ttrd = new G4Trd(name.str().c_str(),
			    infGeom_.beamChannel2_X1, 
			    infGeom_.beamChannel2_X2,
			    infGeom_.trackerThickness/2,
			    infGeom_.trackerThickness/2,
			    infGeom_.beamChannel2_Z);
        
    name.str("");
    name << "InflectorTracker_S["
	 << std::setfill('0') 
	 << std::setw(2)
	 << i 
	 << "]";
    G4UnionSolid *sol = new G4UnionSolid(name.str().c_str(),
					 tbox,
					 ttrd,
					 joinBeamChannels);

    name.str("");
    name << "InflectorTracker_L["
	 << std::setfill('0') 
	 << std::setw(2)
	 << i
	 << "]";

    G4LogicalVolume *log = 
      new G4LogicalVolume(sol,
			  artg4Materials::Vacuum(),
			  name.str().c_str(),
			  inflectorFieldManager_,
			  0,
			  0);
    log -> SetVisAttributes(tcbVisAtt);

    log -> SetSensitiveDetector(inflectorSD_);

    name.str("");
    name << "InflectorTracker["
	 << std::setfill('0') 
	 << std::setw(2)
	 << i
	 << "]";
    G4double const tIML = infGeom_.ig.mandrel_length() - infGeom_.trackerThickness;

    // Don't create the variable place if we aren't going to use it below
    //    G4PVPlacement *place =
    new G4PVPlacement(0,
		      G4ThreeVector(0.,
				    // i*tIML/(num_trackers-1) -tIML/2., 
				    - i*tIML/(infGeom_.num_trackers-1) 
				    + tIML/2., 
				    0),
		      log,
		      name.str().c_str(),
		      beamChannel_L_,
		      false,
		      0);
    
    // The tracker_physicals only need to be stored IF you intend
    // to delete and rebuild the inflector (see: g2migtrace implementation of DeleteInflector


  }
  
 }

void gm2ringsim::Inflector::buildInflectorField(){
  // There are THREE things we have to be concerned about here:
  // 1) If we change something that affects the field (geometry, field
  // normalization, etc.) we have to rebuild the Field instances, and
  // hence the EOM, and reassign the FieldManager
  // 2) If we change GEOMETRY, we will have to update the field, but
  // we may NOT want to rebuild the underlying Field code (think of
  // the mappedField implementation, which has to load around 30M of
  // data files...
  // 3) If we change ONLY the spin evolution, we have to rebuild the
  // EOM and reassign the FieldManager
  
  rebuildFieldImpl();
  rebuildEOM();
  assignFieldManager();

 }

/** @bug There should be some more indirection to enable simple
    rebuild of the field without reload of the field maps, which are
    f'ing big text files. */
void gm2ringsim::Inflector::rebuildFieldImpl(){
  if( inflectorMagField_ ){
    delete inflectorMagField_;
  }
  
  switch( mag_field_type_ ){
  case VANISHING_FIELD:
    inflectorMagField_ = new VanishingInflectorField;
    break;
  case SIMPLE_FIELD:
    inflectorMagField_ = new SimpleInflectorField(fieldNormConst_);
    break;
  case MAPPED_FIELD:
    inflectorMagField_ = new MappedInflectorField();
    break;
  default:
    G4cout << "An improper Inflector Field was set in Inflector::rebuildFieldImpl()!!\n";
  }
  
 }


//FIXME: Do we really need to rebuild? Let's make spin_tracking_ a const and 
//        avoid all of these rebuilds...
void gm2ringsim::Inflector::rebuildEOM(){

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
  
} // END rebuildEOM()

void gm2ringsim::Inflector::assignFieldManager(){
  inflectorFieldManager_ -> SetDetectorField(inflectorMagField_);
  inflectorFieldManager_ -> SetChordFinder(iChordFinder_);
  inflector_L_ ->SetFieldManager(inflectorFieldManager_, true);
  launchRegion_L_ ->SetFieldManager(launchFieldManager_, true);
}


/** Prints inflector status information. */
void gm2ringsim::Inflector::getInflectorInfo(){
  
  
  G4cout << "\n\n\t\t        Inflector Information\n"
	 << "    -------------------------------------------------------------\n\n";

  G4cout<< std::setw(40) << "The Upstream Inflector End is:"
	<< std::setw(20);
  if(useUpstreamWindow_ && useUpstreamConductor_ && useUpstreamEndFlange_) 
    G4cout << "COMPLETELY CLOSED\n";
  else if(!useUpstreamWindow_ && !useUpstreamConductor_ && !useUpstreamEndFlange_) 
    G4cout << "COMPLETELY OPEN\n";
  else
    G4cout << "PARTIALLY CLOSED\n";

  G4cout<< std::setw(40) << "The Aluminum Window is:"
	<< std::setw(20);
  if(useUpstreamWindow_) 
    G4cout << "ON\n";
  else 
    G4cout << "OFF\n";

  G4cout << std::setw(40) << "The Superconductor is:"
	 << std::setw(20);
  if(useUpstreamConductor_)
    G4cout << "ON\n";
  else 
    G4cout << "OFF\n";

 G4cout << std::setw(40) << "The Aluminum End Flange is:"
	 << std::setw(20);
  if(useUpstreamEndFlange_)
    G4cout << "ON\n";
  else 
    G4cout << "OFF\n";


  G4cout << "\n";


  G4cout<< std::setw(40) << "The Downstream Inflector End is:"
	<< std::setw(20);
  if(useUpstreamWindow_ && useUpstreamConductor_ && useUpstreamEndFlange_) 
    G4cout << "COMPLETELY CLOSED\n";
  else if(!useUpstreamWindow_ && !useUpstreamConductor_ && !useUpstreamEndFlange_) 
    G4cout << "COMPLETELY OPEN\n";
  else
    G4cout << "PARTIALLY CLOSED\n";

  G4cout << std::setw(40) << "The Aluminum Window is:"
	 << std::setw(20);
  if(useDownstreamWindow_) 
    G4cout << "ON\n";
  else 
    G4cout << "OFF\n";

  G4cout << std::setw(40) << "The Superconductor is:"
	 << std::setw(20);
  if(useDownstreamConductor_)
    G4cout << "ON\n";
  else 
    G4cout << "OFF\n";
  
  G4cout << std::setw(40) << "The Aluminum End Flange is:"
	 << std::setw(20);
  if(useDownstreamEndFlange_)
    G4cout << "ON\n";
  else 
    G4cout << "OFF\n";

  G4cout << "\n";
  
  G4cout << std::setw(40) << "The Superconducting Windings are:"
	 << std::setw(30);
  if(useConductorEquivalent_)
    G4cout << "EQUIVALENT MATERIAL PLATES\n";
  else 
    G4cout << "PHYSICALLY REALISTIC COIL WINDINGS\n";
    
  G4cout << "\n";

  inflectorGeometry const& ig = infGeom_.ig ;//inflectorGeometry::getInstance();

  G4cout << std::setw(40) << "Azimuthal Aperture Position:"
	 << std::setw(15) << G4BestUnit(ig.delta(),"Angle");
  if(ig.delta() == 6.*degree)
    G4cout << "  (DEFAULT)";

  G4cout << "\n";
  
  G4cout << std::setw(40) << "Radial Aperture Distance:"
	 << std::setw(15) << G4BestUnit(ig.aperture_off(),"Length");
  if(ig.aperture_off() == 77*mm)
    G4cout << "  (DEFAULT)";

  G4cout << "\n";

  G4cout << std::setw(40) << "Tilt Angle:"
	 << std::setw(15) << G4BestUnit(ig.zeta(),"Angle");
  if(ig.zeta() == 0.*degree)
    G4cout << "  (DEFAULT)";
  
  G4cout << "\n";
  
  G4cout << std::setw(40) << "Swing Angle:"
	 << std::setw(15) << G4BestUnit(ig.gamma(),"Angle");
  if(ig.gamma() == 2.4*milliradian)
    G4cout << "  (DEFAULT)";

  G4cout << "\n";


  G4cout << "Inflector Field Type is ";
  switch( mag_field_type_ ){
  case VANISHING_FIELD:
    G4cout << "vanishing\n";
    break;
  case SIMPLE_FIELD:
    G4cout << "simple\n";
    break;
  case MAPPED_FIELD:
    G4cout << "mapped\n";
    break;
  default:
    G4cout << "failure!!!!\n";
  }

  G4cout << "There are currently " << num_trackers_ << " active tracking volumes.\n"; 

  G4cout << "G2GPS Launch angles are:\n";
  G4cout << "\tIn-plane: " << G4BestUnit(ig.inplane_angle(),"Angle") << '\n';
  G4cout << "\tOut-of-plane: " << G4BestUnit(ig.outplane_angle(),"Angle") << '\n';

  G4cout << '\n';    

 }

/** Sets the number of inflector beam aperture tracking volumes. */
void gm2ringsim::Inflector::setNumTrackers(G4int //n
					   ){ }

/** Gets the number of inflector beam aperture tracking volumes. */
G4int gm2ringsim::Inflector::getNumTrackers() const {
  return num_trackers_; 
}

/** Sets the magnetic field model.
    Options are "vanishing/none", "simple" or uniform, and "mapped"
    from Wuzheng Meng's detailed field maps.                                
*/
void gm2ringsim::Inflector::setMagFieldType(G4String){ }

/** Regenerates the GPS Macros when the inflector geometry is
    rebuilt. */

void gm2ringsim::Inflector::generateGPSMacros(){
  // This will need to generate a fcl instead of a g2migtrace input file
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
void gm2ringsim::Inflector::doCallArtProduces(art::EDProducer * producer) {
  producer->produces<InflectorArtRecordCollection>(category());
}

// Actually add the data to the event
void gm2ringsim::Inflector::doFillEventWithArtHits(G4HCofThisEvent *hc) {
   
  std::unique_ptr<InflectorArtRecordCollection> myArtHits(new InflectorArtRecordCollection);

  // Find the collection ID for the hits
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();

  // The string here is unfortunately a magic constant. It's the string used       
  // by the sensitive detector to identify the collection of hits.                 
  G4int collectionID = fSDM->GetCollectionID(inflectorSDname_);
  
  InflectorHitsCollection* myCollection =
    static_cast<InflectorHitsCollection*>(hc->GetHC(collectionID));
  // Check whether the collection exists                                           
  if (NULL != myCollection) {
    std::vector<InflectorHit*> geantHits = *(myCollection->GetVector());
    
    
    for ( auto e : geantHits ) {
      e->Print();
      // Copy this hit into the Art hit                                         

      myArtHits->push_back( convert(e));
      /*
      myArtHits->emplace_back( e->position.x(),e->position.y(),e->position.z(),
			       e->local_position.x(),e->local_position.y(),
			       e->local_position.z(),
			       e->momentum.x(),e->momentum.y(),e->momentum.z(),
			       e->local_momentum.x(),e->local_momentum.y(),
			       e->local_momentum.z(),
			       e->time,
			       e->trackID,
			       e->volumeUID);
      */

    } //loop over geantHits
  } //if we have a myCollection

  else {
    throw cet::exception("Inflector") << "Null collection of Geant tracker hits"
				      << ", aborting!" << std::endl;
  }
  // Now that we have our collection of artized hits, add them to the event.
  // Get the event from the detector holder service                                
  art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
  art::Event & e = detectorHolder -> getCurrArtEvent();

  // Put the hits into the event                                                   
  e.put(std::move(myArtHits), category());
}

//This came from the RootStorageManager code
// inflectorRecord convert (InflectorHit* pih)
gm2ringsim::InflectorArtRecord gm2ringsim::Inflector::convert(InflectorHit* e){
  InflectorArtRecord ir;
  inflectorGeometry const& ig = infGeom_.ig;

  
  ir.time = e->time;
  ir.trackID = e->trackID;
  ir.volumeUID = e->volumeUID;
   // hard stuff
    // inflector coordinates:
    // x_inf is the offset from the middle of the aperture in the
    // thinner direction, the "horizontal" or most radial direction
    // z_inf is the longitudinal offset from the upstream end of the
    // inflector
    // y_inf is the offset from the middle of the aperture in the
    // thicker direction, the "vertical" or most axial direction

   G4ThreeVector xformed = e->position;
    // undo the transformations in inflectorContruction.cc
    xformed.rotateY(-ig.delta());
    xformed -= G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
    xformed.rotateY(-ig.gamma());
    xformed.rotateX(ig.zeta());
    xformed += G4ThreeVector(0.,0.,ig.length());

    //    G4cout << "xformed: " << xformed << '\n';

    ir.x_inf = xformed.x();
    ir.y_inf = xformed.y();
    ir.z_inf = xformed.z();


    ir.x_loc  = e->local_position.x();
    ir.y_loc  = e->local_position.y();
    ir.z_loc  = e->local_position.z();

    ir.px_inf = e->momentum.x();
    ir.py_inf = e->momentum.y();
    ir.pz_inf = e->momentum.z();

    ir.px_loc = e->local_momentum.x();
    ir.py_loc = e->local_momentum.y();
    ir.pz_loc = e->local_momentum.z();

    return ir;

}



G4ThreeVector gm2ringsim::Inflector::calc_position() const  {
  //FIXME: removed const temporarily //const {
  // Calculate the EFFECTIVE aperture radius not the absolute radius
  // (which would simply be 7.189 m).  We must ADD the distance that
  // the beam channel is offset from the center of the inflector
  // assembly in order to ensure that it is the BEAM CHANNEL center
  // that is placed 77 mm from the storage orbit, and *not* the
  // INFLECTOR ASSEMBLY center.

  // This is subtle.  Think trigonometry with a side view of the
  // inflector.  Since the inflector is rotated around it central
  // point when "tilted", it must necessarily by translated so
  // that the aperture ends up in it's original and correct
  // location!   

  // g2MIGTRACE:  inflectorGeometry const& ig = inflectorGeometry::getInstance();
  inflectorGeometry const& ig = infGeom_.ig;
  G4double const bco = infGeom_.beamChannel_offset - infGeom_.beamChannel1_Z;  
  G4double const apRad = (R_magic() + ig.aperture_off()) + bco;
  
  // X,Y,Z here are not g2MIGTRACE global coordinates, because of the
  // rotation in calc_rotation!

  // FIXME : Make Inflector_option zeta_free: true
  
  G4double posX = 0;
  G4double posY = 0;
  G4double posZ = 0;

  if (0) // GetfromFcl
    {
      posX =  apRad*cos(epsilon_ - ig.delta()) + 
	ig.mandrel_half_length()*sin(epsilon_ - ig.delta() - ig.gamma());
      
      posY = apRad*sin(epsilon_ - ig.delta()) -
	ig.mandrel_half_length()*cos(epsilon_ - ig.delta() - ig.gamma());
      
      posZ = 0.;
    }
  //GetXYZ_zetaFree(posX,posY,posZ);
  
  //FIXME: Zach rotation
  if (0) 
    {
      // Calculate the position of the completly assembled inflector 
      // what Zach wrote....
      G4double changeInXandY = ig.mandrel_half_length()*(1. - cos(ig.zeta()));
      G4double changeInZ = ig.mandrel_half_length() * sin(ig.zeta());
      
      posX = ( apRad*cos(epsilon_ - ig.delta()) ) 
    + (ig.mandrel_half_length() - changeInXandY)*sin(epsilon_ - ig.delta() - ig.gamma());
      posY = ( apRad*sin(epsilon_ - ig.delta()) ) 
    - (ig.mandrel_half_length() - changeInXandY)*cos(epsilon_ - ig.delta() - ig.gamma());
      posZ = changeInZ;
    }

  if(1){
  // we need to rotate not around the upstream center of the
  // inflector, but around the upstream BEAM CHANNEL.  So, we have to
  // not only rotate, but we have to make a small shift to account for
  // the offset rotation calculation.
  G4double const radial_fixup = -bco*(1.-cos(ig.gamma()));
  G4double const tangent_fixup = bco*sin(ig.gamma());

  //G4double// const 
    posX = apRad*cos(epsilon_ - ig.delta())
    + ig.mandrel_half_length()*cos(ig.zeta())*sin(epsilon_ - ig.delta()- ig.gamma())
    // fixups
    +radial_fixup*cos(epsilon_) + tangent_fixup*sin(epsilon_)
    ;
  //G4double //const
    posY =  apRad*sin(epsilon_ - ig.delta())
    - ig.mandrel_half_length()*cos(ig.zeta())*cos(epsilon_ - ig.delta() - ig.gamma())
    // fixups
    +radial_fixup*sin(epsilon_)-tangent_fixup*cos(epsilon_)
    ;
  // G4double// const
    posZ = ig.mandrel_half_length()*sin(ig.zeta());
  }


  G4ThreeVector const center(posX, posY, posZ);

  return center;
} // Inflector::calc_postiion()
 
/*
void gm2ringsim::Inflector::GetXYZ_zetaFree(G4double &posX, G4double &posY, G4double &posZ)  {
 inflectorGeometry const& ig = infGeom_.ig;
 G4double const bco = infGeom_.beamChannel_offset - infGeom_.beamChannel1_Z;
 G4double const apRad = (R_magic() + ig.aperture_off()) + bco;
  
  posX =  apRad*cos(epsilon_ - ig.delta()) + 
    ig.mandrel_half_length()*sin(epsilon_ - ig.delta() - ig.gamma());
  
  posY = apRad*sin(epsilon_ - ig.delta()) -
    ig.mandrel_half_length()*cos(epsilon_ - ig.delta() - ig.gamma());
  
  posZ = 0.;
}
*/



#include <utility>
G4RotationMatrix* gm2ringsim::Inflector::calc_rotation() { 
  // Calculate the rotation of the completely assembled inflector.  By
  // default, the inflector is instantiated such that the "vertical"
  // direction of the beam channel is parallel to the storage orbit.
  // The three rotations below not only rotate the inflector correctly
  // but also ensure that the beam channel is in its correct
  // orientation relative to the storage orbit (ie, if you are looking
  // upstream at the inflector, the channel looks like a "D")

  // theta1: first rotate inflector to be *perpendicular* to a tangent 
  //         to the storage orbit at desired inflector position 
  // phi: rotate 90 degrees out of storage orbit plane
  // theta2: Rotate 90 degree back into the plane, with beam channel now 
  //         in correct the correct orientation
  
  // g2MIGTRACE:  inflectorGeometry const& ig = inflectorGeometry::getInstance();
  // ART : 
  inflectorGeometry const& ig = infGeom_.ig;
  
  G4double theta1 = (90*degree + epsilon_) - ig.delta() - ig.gamma();
  G4double phi = -90*degree;
  G4double theta2 = 90*degree + ig.zeta();

  G4RotationMatrix temp(theta1, phi, theta2);

  std::swap(temp, *inflectorRotation_);

  return inflectorRotation_;
}


/* These function were previously implemented in g2MIGTRACE but were 
   not ported due to the fact that they only existed in order to 
   update the inflector if a geometry change occurred at run time.
   This is no longer supported in the ARTized version
   
   void gm2ringsim::Inflector::enable_spintracking(bool e){
      if( spin_tracking_ == e )
      return;
  
      spin_tracking_ = e;
      rebuildEOM();
      assignFieldManager();
    }

void gm2ringsim::Inflector::rebuildInflector(){ }
void gm2ringsim::Inflector::DeleteInflector(){ }


void gm2ringsim::Inflector::setAperturePosition(G4double){ }
void gm2ringsim::Inflector::setApertureDistance(G4double){ }
void gm2ringsim::Inflector::setInflectorTiltAngle(G4double){ }
void gm2ringsim::Inflector::setInflectorSwingAngle(G4double){ }
void gm2ringsim::Inflector::setConductorModelType(G4String){ }
void gm2ringsim::Inflector::setUpstreamEndType(G4String){ }
void gm2ringsim::Inflector::setDownstreamEndType(G4String){ }
void gm2ringsim::Inflector::setUpstreamWindow(G4String){ }
void gm2ringsim::Inflector::setUpstreamConductor(G4String){ }
void gm2ringsim::Inflector::setUpstreamEndFlange(G4String){ }
void gm2ringsim::Inflector::setDownstreamWindow(G4String){ }
void gm2ringsim::Inflector::setDownstreamConductor(G4String){ }
void gm2ringsim::Inflector::setDownstreamEndFlange(G4String){ }
void gm2ringsim::Inflector::setConductorCurrent(G4double){ }
void gm2ringsim::Inflector::setFieldNormalizationConstant(G4double){ }
*/






using gm2ringsim::Inflector;
DEFINE_ART_SERVICE(Inflector)
