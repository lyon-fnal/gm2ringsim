// Implementation of Traceback


#include "gm2ringsim/traceback/Traceback_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "gm2ringsim/traceback/TracebackGeometry.hh"

#include "boost/format.hpp"

#include "Geant4/G4Box.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4UnionSolid.hh"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Traceback::Traceback(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
                   p.get<std::string>("name", "traceback"),
                   p.get<std::string>("category", "traceback"),
                   p.get<std::string>("mother_category", "vac")),
  geom_(myName()),
  strawSDName_("straw"),
  strawSD_(0)
{
  strawSD_ = artg4::getSensitiveDetector<StrawSD>(strawSDName_);
}
G4LogicalVolume* gm2ringsim::Traceback::makeATracebackLV() {
  
  G4double move_theta;
  G4double move_r;

  G4VSolid *solid = new G4Box("traceback_S1", geom_.traceback_radial_half[0], geom_.traceback_theta_half, geom_.traceback_z_half);

  for(int i = 1; i!= 22; ++i){
       
    G4VSolid *s = new G4Box("traceback_this", geom_.traceback_radial_half[i], geom_.traceback_theta_half, geom_.traceback_z_half);
    
    move_theta = geom_.traceback_theta + geom_.traceback_theta*(i-1);
    
    move_r = (geom_.traceback_radial_half[0]-geom_.traceback_radial_half[i])+ move_theta*geom_.tan_traceback_radial_shift_angle;
    
    G4ThreeVector move_this (-move_r, -move_theta, 0.0);
    G4UnionSolid *solid_this = new G4UnionSolid("traceback_S",solid, s, 0, move_this);
    solid = solid_this;
    
  }

  G4LogicalVolume *traceback_L = new G4LogicalVolume(solid,
                                                     artg4Materials::Vacuum(),
                                                     "traceback_L");
   
  artg4::setVisAtts(traceback_L, geom_.displayTraceback, geom_.tracebackColor);
  return traceback_L;

}
// Build the logical volumes
void gm2ringsim::Traceback::makeTracebackLVs(std::vector<G4LogicalVolume*>& tracebacks) {
  
  // Create the vector of logical volumes
  //std::vector<G4LogicalVolume*> tracebackLVs;
  
  // Build the logical volumes
  for ( unsigned int tracebackNumber = 0; tracebackNumber != 24; ++tracebackNumber ) {
    // Push this into the vector
    tracebacks.push_back( makeATracebackLV());
  }
}

void gm2ringsim::Traceback::makeStrawDetectors(std::vector<G4VPhysicalVolume*>& straws,std::vector<G4LogicalVolume*>& tracebacks){
  for (int tb = 0; tb<24 ;tb++){
    for (unsigned int sc =0 ; sc<geom_.strawLocation.size(); sc++){
    
      G4double move_theta;
      G4double move_r;
    
      G4VSolid *strawSystem = new G4Box("strawSystem", geom_.traceback_radial_half[geom_.strawLocation[sc]]-50, geom_.traceback_theta_half-50, geom_.traceback_z_half-50);
    
      std::string strawLVName = artg4::addNumberToName("StrawChamberLV", sc);

      G4LogicalVolume* strawLV = new G4LogicalVolume(
                                                   strawSystem,
                                                   artg4Materials::Vacuum(),
                                                   strawLVName,
                                                   0,
                                                   0);
    
      artg4::setVisAtts( strawLV, geom_.displayStraw, geom_.strawColor,
                      [] (G4VisAttributes* att) {
                        att->SetForceSolid(1);
                        att->SetVisibility(1);
                      }
                      );

      std::string pvName = artg4::addNumberToName("StrawChamberPV", sc);
      move_r =  geom_.traceback_radial_half[geom_.strawLocation[0]]-geom_.traceback_radial_half[geom_.strawLocation[sc]]
                + move_theta*geom_.tan_traceback_radial_shift_angle;
      
      move_theta = geom_.traceback_theta*(geom_.strawLocation[sc]);

      G4ThreeVector position (-move_r, -move_theta, 0.0);
      std::cout<<"Straw Chamber: "<<sc<<std::endl;
      std::cout<<"Position: "<<position <<std::endl;
      // We can make the physical volumes here
      straws.push_back( new G4PVPlacement(0,
                                            position,
                                            strawLV,
                                            pvName.c_str(),
                                            tracebacks[tb],
                                            false,
                                            0)
                   );
    }

  }
}


//Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Traceback::doBuildLVs() {
  geom_.print();
  
  std::vector<G4LogicalVolume*> tracebacks;
  makeTracebackLVs(tracebacks);
  std::vector<G4VPhysicalVolume*> straws;

  makeStrawDetectors(straws, tracebacks);
  return tracebacks;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Traceback::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  //TracebackGeometry tg(myName());
  
  std::vector<G4VPhysicalVolume*> tracebackPVs;
  tracebackPVs.resize(lvs().size());
  
  G4double const r_2 = geom_.r/2.;
  G4double const t_2 = geom_.t/2.;
  
  //loop over the logical volumes
  unsigned int tracebackNum = 0;
  for ( auto aTracebackLV : lvs() ) {
    
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    std::string tracebackLabel( boost::str( boost::format("TracebackNumber[%02d]") % tracebackNum ));
    int arc_position = tracebackNum % 2;
    
    G4ThreeVector window_edge(geom_.r_out*std::cos(geom_.theta_out[ arc_position ]),
                              geom_.r_out*std::sin(geom_.theta_out[ arc_position ]),
                              0.);

    G4ThreeVector unit_along(std::cos(geom_.theta_in[ arc_position ] + geom_.window_angle),
                             std::sin(geom_.theta_in[ arc_position ] + geom_.window_angle),
                             0.);
    
    G4ThreeVector unit_vertical(0,0,-1.); // OK ... yes, up :-(
    
    G4ThreeVector unit_normal = unit_along.cross(unit_vertical); // OK
    
    G4double const vrots = std::sin( std::abs(geom_.v_rotation) );
    G4double const vrotc = std::cos(geom_.v_rotation);
    G4double const correction_along = (vrotc-1.)*r_2 + vrots*t_2;
    G4double const correction_normal = vrots*r_2 + (vrotc-1.)*t_2;
    // beamlike
    
    // for the real g-2 station
    G4ThreeVector pos =
    // center the station on the center of the outer window edge
    window_edge
    // move it in by the radial half width
    - r_2 * unit_along
    // move it downstream by the thickness half width
    + t_2 * unit_normal
    // We have to move the station around so that it doesn't "whack"
    // (technical term, that) into the outside of the vacuum walls
    // when it's rotated
    /** @bug doesn't include the r_rotation and t_rotation corrections
     ... should probably get this by multiplying appropriate
     rotation matrices, but this will take some thought... */
    - correction_along * unit_along
    + correction_normal * unit_normal
    // finally, let's apply the user defined offsets
    +geom_.r_offset * unit_along
    +geom_.t_offset * unit_normal
    +geom_.v_offset * unit_vertical;
    
    G4RotationMatrix *rot = new G4RotationMatrix(0,
                                                 0,
                                                 geom_.theta_in[ arc_position ] + geom_.window_angle - geom_.v_rotation);
    
    int arc_number = floor(tracebackNum/2);

    tracebackPVs.push_back(
                           new G4PVPlacement(
                                             rot,
                                             pos,
                                             aTracebackLV,
                                             tracebackLabel,
                                             vacs[ arc_number ]->GetDaughter(0)->GetLogicalVolume(),
                                             false,
                                             0, true
                                             )
                         );
    
    tracebackNum++;
  }
  return tracebackPVs;


}
//i want to take a vector of traceback locations [1,3,5] for example.
//for each of those locations I want to put two straw chambers (planes for now)
//those straw chambers should be at 20mm on either side of the edges of the box
//they should be 5mm thick

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Traceback::doCallArtProduces(art::EDProducer * producer) {

//}

// Actually add the data to the event
//void gm2ringsim::Traceback::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
//}

using gm2ringsim::Traceback;
DEFINE_ART_SERVICE(Traceback)
