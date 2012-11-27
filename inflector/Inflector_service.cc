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
	       //	       p.get<std::string>("mother_category", "vac")),
	       p.get<std::string>("mother_category", "world")),
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
  vacuumInflectorSection_(11), //FIXME: move to fhicl
  // where we are in the construction timeline                                  
  initialBuild_(true),
  // Set the maximum step length via G4UserLimits.  This is currently           
  // used to limiting the muons step size in the beam channel                   
  maxStepLength_(5*mm),
    // Set the defaults which control what components will be put into            
  // the inflector assembly.  By default, both ends of the inflector            
  // are completely closed, as in the real inflector.                           
  useConductorEquivalent_(true),
  useUpstreamWindow_(true),
  useDownstreamWindow_(true),
  useUpstreamConductor_(true),
  useDownstreamConductor_(true),
  useUpstreamEndFlange_(true),
  useDownstreamEndFlange_(true),
  mag_field_type_(VANISHING_FIELD),
/** @bug The following fields should be moved to the field                    
    implementation classes.                                                   
    
    The defaults necessary for the inflector field.  The values were          
    obtained from an email from Wuzheng that can be found in                  
    reference section.  They are apparently the values for the                
    '00-'01 runs. */
  conductorCurrent_(2724.*ampere),
  fieldNormConst_(14246.5*gauss),
  currentToMagFieldConversion_((5.23*gauss) / (1.0*ampere))
{
  printf("In the Inflector service constructor\n");
  
  std::tr1::function<void(bool)> f =
    std::tr1::bind(&Inflector::enable_spintracking, this,
                   std::tr1::placeholders::_1);
  
  conn_ =
    spinController::getInstance().connect(e_arc_is_parent, f);
  
}

void gm2ringsim::Inflector::enable_spintracking(bool e){
  if( spin_tracking_ == e )
    return;
  
  spin_tracking_ = e;
  
  //FIXME: Add these 2 functions back in
  RebuildEOM();
  AssignFieldManager();
}

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

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Inflector::doBuildLVs() {
  
  infGeom_.print();
  
  std::vector<G4LogicalVolume *> l_inflector;
  return l_inflector;

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Inflector::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  
  std::vector<G4VPhysicalVolume *> p_inflector;
  return p_inflector;

}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Inflector::doCallArtProduces(art::EDProducer * producer) {
//
//}

// Actually add the data to the event
//void gm2ringsim::Inflector::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}

using gm2ringsim::Inflector;
DEFINE_ART_SERVICE(Inflector)
