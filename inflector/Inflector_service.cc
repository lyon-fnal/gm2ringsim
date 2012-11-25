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

#include "gm2ringsim/actions/PGA/g2PreciseValues.hh"

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
  infGeom_(myName())
{
  printf("In the Inflector service contsructor\n");
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
