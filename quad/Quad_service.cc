// Implementation of Quad
/** @file Quad_service.cc
    
    Implements the quadrupole field and plate volumes in class
    quadConstruction. 
    
    The dimensional measurements were taken from:
     -# The Quadrupole NIM Paper:  NIM A 504 (2003) 458-484
     -# The AutoCad "as built" drawing: quad-frameendview.dwg
     -# The AutoCad "as built" drawing: qaud-frameplanview2.dwg
     
     This parameter file represents a completely overhauled version of
     the original(s): numbers now reflect the most accurate
     information we have available (the "as built" drawings) and new
     variables have been implemented to complement the overhauled
     construction process.  ZSH(28 MAY 09)
     
     Abbreviations/Conventions used in this file:
     I=Inner, O=Outer, T=Top, B=Bottom (refers to quad plates)
     Q=Quadrupole, QFR=Quad Field Region, LFR=LinearField Regions
     width=refers to long dimension of plate (viewed end on)
     thickness=refers to short dimension of plate (viewed end on)
     
     
     Dimensional measurements directly from CAD drawings
     
     @author Zach Hartwig
     @author Kevin Lynch
     @date 2005-2011
*/

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "gm2ringsim/quad/Quad_service.hh"
#include "gm2ringsim/quad/QuadField.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/ring/RingSD.hh"
#include "gm2ringsim/common/conversions.hh"
#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4Polycone.hh"
#include "Geant4/G4Torus.hh"
#include "Geant4/G4UnionSolid.hh"
#include "Geant4/G4IntersectionSolid.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4FieldManager.hh"
#include "Geant4/G4MagIntegratorStepper.hh"
#include "Geant4/G4MagIntegratorDriver.hh"
#include "Geant4/G4ChordFinder.hh"
#include "Geant4/G4EqMagElectricField.hh"
#include "Geant4/G4EqEMFieldWithSpin.hh"
#include "Geant4/G4ClassicalRK4.hh"
#include "Geant4/G4UserLimits.hh"


// Constructor for the service 
gm2ringsim::Quad::Quad(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
		 p.get<std::string>("name", "quad"),
		 p.get<std::string>("category", "quad"),
		 //FIXME
		 p.get<std::string>("mother_category", "vac")),
    qg_(myName()) //QuadGeometry
{
  qg_.print();

}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Quad::doBuildLVs() {
  
  //FIXME
  std::vector<G4LogicalVolume *> l_inflector;
  return l_inflector;

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Quad::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  //FIXME
  std::vector<G4VPhysicalVolume *> p_inflector;
  return p_inflector;
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Quad::doCallArtProduces(art::EDProducer * producer) {

//}

// Actually add the data to the event
//void gm2ringsim::Quad::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
//}


gm2ringsim::Quad::curl_params::
curl_params() :
  is_curled(false), rmin(0), rmax(0), sign(1.)
{}  

gm2ringsim::Quad::curl_params::
curl_params(G4double rmin_, G4double rmax_, G4double sign_) :
  is_curled(true), rmin(rmin_), rmax(rmax_), sign(sign_)
{}


/*
  gm2ringsim::Quad::curl_params gm2ringsim::Quad::get_curl_params(G4int //quadRegion
  , G4int sectionType, G4int plateType){
  curl_map_t::iterator found;
  found = curl_map_.find(get_index(sectionType, plateType));
  if( found != curl_map_.end() ){
    //    G4cout << "Found!\n";
    return found->second;
  }
  return curl_params();

}*/





using gm2ringsim::Quad;
DEFINE_ART_SERVICE(Quad)
