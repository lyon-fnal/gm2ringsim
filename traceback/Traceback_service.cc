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
                   p.get<std::string>("mother_category", "vac"))
{}
G4LogicalVolume* gm2ringsim::Traceback::makeATracebackLV(gm2ringsim::TracebackGeometry const & tg) {
  
  G4double half_zmax = 150.0/2.0;
  G4double half_thetamax = 60.0/2.0;
  
  G4double tan3 = 0.05240773;
    
  G4double new_theta = 0.0;
  
  G4VSolid *s1 = new G4Box("traceback_S1", tg.traceback_radial[0]/2, half_thetamax, half_zmax);
  G4VSolid *s2 = new G4Box("traceback_S2", tg.traceback_radial[1]/2, half_thetamax, half_zmax);

  new_theta = new_theta + 60.0;

  G4double new_r = (tg.traceback_radial[0]/2-tg.traceback_radial[1]/2)+ new_theta*tan3;
    
  G4ThreeVector off2 (-new_r, -new_theta, 0.0);
  
  G4UnionSolid *solid = new G4UnionSolid("traceback_S",s1, s2, 0, off2);
  
  for(int i = 2; i!= 22; ++i){
    
    G4VSolid *s_this = new G4Box("traceback_this", tg.traceback_radial[i]/2, half_thetamax, half_zmax);
    
    new_theta = new_theta + 60.0;
    
    new_r = (tg.traceback_radial[0]/2-tg.traceback_radial[i]/2)+ new_theta*tan3;
    
    G4ThreeVector off_this (-new_r, -new_theta, 0.0);
    
    G4UnionSolid *solid_this = new G4UnionSolid("traceback_S",solid, s_this, 0, off_this);
    
    solid = solid_this;
    
  }
  
  G4LogicalVolume *traceback_L = new G4LogicalVolume(solid,
                                                     artg4Materials::Vacuum(),
                                                     "traceback_L");
   
  artg4::setVisAtts(traceback_L, tg.displayTraceback, tg.tracebackColor);
  return traceback_L;

}
// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Traceback::doBuildLVs() {
  TracebackGeometry tg(myName());
  tg.print();
  
  // Create the vector of logical volumes
  std::vector<G4LogicalVolume*> tracebackLVs;
  
  // Build the logical volumes
  for ( unsigned int tracebackNumber = 0; tracebackNumber != 24; ++tracebackNumber ) {
    // Push this into the vector
    tracebackLVs.push_back( makeATracebackLV(tg));
  }
  return tracebackLVs;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Traceback::doPlaceToPVs( std::vector<G4LogicalVolume*> vacs) {
  TracebackGeometry tg(myName());
  
  std::vector<G4VPhysicalVolume*> tracebackPVs;
  tracebackPVs.resize(lvs().size());
  
  G4double const r_2 = tg.r/2.;
  G4double const t_2 = tg.t/2.;
  
  //loop over the logical volumes
  unsigned int tracebackNum = 0;
  for ( auto aTracebackLV : lvs() ) {
    
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    std::string tracebackLabel( boost::str( boost::format("TracebackNumber[%02d]") % tracebackNum ));
    int arc_position = tracebackNum % 2;
    
    G4ThreeVector window_edge(tg.r_out*std::cos(tg.theta_out[ arc_position ]),
                              tg.r_out*std::sin(tg.theta_out[ arc_position ]),
                              0.);

    
    G4ThreeVector unit_along(std::cos(tg.theta_in[ arc_position ] + tg.window_angle),
                             std::sin(tg.theta_in[ arc_position ] + tg.window_angle),
                             0.);
    
    G4ThreeVector unit_vertical(0,0,-1.); // OK ... yes, up :-(
    
    G4ThreeVector unit_normal = unit_along.cross(unit_vertical); // OK
    
    G4double const vrots = std::sin( std::abs(tg.v_rotation) );
    G4double const vrotc = std::cos(tg.v_rotation);
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
    +tg.r_offset * unit_along
    +tg.t_offset * unit_normal
    +tg.v_offset * unit_vertical;
    
    G4RotationMatrix *rot = new G4RotationMatrix(0,
                                                 0,
                                                 tg.theta_in[ arc_position ] + tg.window_angle - tg.v_rotation);
    
    int arc_number = floor(tracebackNum/2);
    tracebackPVs.push_back(
                         new G4PVPlacement(
                                           rot,
                                           pos,
                                           aTracebackLV,
                                           tracebackLabel,
                                           vacs[ arc_number ],
                                           false,
                                           0, true
                                           )
                         );
    
    
    tracebackNum++;
  }
  return tracebackPVs;


}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Traceback::doCallArtProduces(art::EDProducer * producer) {

//}

// Actually add the data to the event
//void gm2ringsim::Traceback::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
//}

using gm2ringsim::Traceback;
DEFINE_ART_SERVICE(Traceback)
