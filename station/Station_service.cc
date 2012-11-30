// Implementation of Station

#include "gm2ringsim/station/Station_service.hh"
#include "artg4/material/Materials.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4Box.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"

#include "boost/format.hpp"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Station::Station(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "station"),
                   p.get<std::string>("category", "station"),
                   p.get<std::string>("mother_category", "arc"))
{}

G4LogicalVolume* gm2ringsim::Station::makeStationLV(const StationGeometry& sg) {
  G4double const r_2 = sg.r/2.;
  G4double const t_2 = sg.t/2.;
  G4double const v_2 = sg.v/2.;
  
  G4VSolid *station_S = new G4Box("station_S", r_2, t_2, v_2);

  G4LogicalVolume *station_L = new G4LogicalVolume(station_S,
                                                   artg4Materials::Vacuum(),
                                                   "station_L"
                                                   );
  auto *StationVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 1.5,1.0));
  station_L -> SetVisAttributes(StationVisAtt);

  return station_L;
  
}
// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Station::doBuildLVs() {
  
  StationGeometry sg(myName());
  sg.print();
  
  // Create the vector of logical volumes
  std::vector<G4LogicalVolume*> stationLVs;
  
  // Build the logical volumes
  for ( unsigned int stationNumber = 0; stationNumber != 24; ++stationNumber ) {
    // Push this into the vector
    stationLVs.push_back( makeStationLV(sg));
  }
  
  return stationLVs;
  
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Station::doPlaceToPVs( std::vector<G4LogicalVolume*> arcs) {
  StationGeometry sg(myName());
  
  std::vector<G4VPhysicalVolume*> stationPVs;
  stationPVs.resize(lvs().size());
  
  G4double const r_2 = sg.r/2.;
  G4double const t_2 = sg.t/2.;

  //loop over the logical volumes
  unsigned int stationNum = 0;
  for ( auto aStationLV : lvs() ) {
    
    // We to name the station including its station number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    std::string stationLabel( boost::str( boost::format("StationNumber[%02d]") % stationNum ));
    G4ThreeVector window_edge(sg.r_out*std::cos(sg.theta_out[ stationNum % 2 ]),
                              sg.r_out*std::sin(sg.theta_out[ stationNum % 2 ]),
                              0.);
    
    std::cout<<"station number: "<<stationNum<<std::endl;
    std::cout<<"theta_out[0]"<<sg.theta_out[0]<<std::endl;
    std::cout<<"theta_in[0]"<<sg.theta_in[0]<<std::endl;
    std::cout<<"theta_in[stationNum%2]: "<<sg.theta_in[ stationNum %2]<<std::endl;
    
    G4ThreeVector unit_along(std::cos(sg.theta_in[ stationNum % 2 ] + sg.window_angle),
                             std::sin(sg.theta_in[ stationNum % 2 ] + sg.window_angle),
                             0.);
    
    G4ThreeVector unit_vertical(0,0,-1.); // OK ... yes, up :-(
    
    G4ThreeVector unit_normal = unit_along.cross(unit_vertical); // OK
    
    G4double const vrots = std::sin( std::abs(sg.v_rotation) );
    G4double const vrotc = std::cos(sg.v_rotation);
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
      +sg.r_offset * unit_along
      +sg.t_offset * unit_normal
      +sg.v_offset * unit_vertical;
    
    G4RotationMatrix *rot = new G4RotationMatrix(0,
                                                 0,
                                                 sg.theta_in[ stationNum % 2 ] + sg.window_angle - sg.v_rotation);
    std::cout<<"which arc? " << floor(stationNum/2)<<std::endl;
    stationPVs.push_back(
                     new G4PVPlacement(
                                       rot,
                                       pos,
                                       aStationLV,
                                       stationLabel,
                                       arcs[ floor(stationNum/2) ],
                                       false,
                                       0
                                       )
                     );


    stationNum++;
  }
  return stationPVs;
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Station::doCallArtProduces(art::EDProducer * producer) {

//}

// Actually add the data to the event
//void gm2ringsim::Station::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
//}

using gm2ringsim::Station;
DEFINE_ART_SERVICE(Station)
