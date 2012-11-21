// Implementation of Arc

#include "gm2ringsim/arc/Arc_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "artg4/material/Materials.hh"

#include "gm2ringsim/arc/ArcGeometry.hh"

#include "boost/format.hpp"

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"


// Constructor for the service 
gm2ringsim::Arc::Arc(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "arc"),
                   p.get<std::string>("category", "arc"),
                   p.get<std::string>("mother_category", "world"))
{}

G4LogicalVolume* gm2ringsim::Arc::makeAnArcLV(gm2ringsim::ArcGeometry const & g, unsigned int arcNum) {
  
  double extension = 0.0;
  if ( arcNum == 11 ) extension = g.arc11_rExtension;
  
  // Make the tub (note use of auto)
  G4Tubs* arc_S = new G4Tubs("wall",
                             g.arc_rMin,
                             g.arc_rMax + extension,
                             g.arc_z, g.arc_Sphi, g.arc_Dphi );
  
  // Make the logical volume
  G4LogicalVolume* arc_L = new G4LogicalVolume(arc_S,
                                               artg4Materials::Vacuum(),
                                               "ArcSection"
                                               // ADD SPIN HERE
                                               );
  
  // Set visualization
  auto *ArcVisAtt = new G4VisAttributes(0);
  arc_L -> SetVisAttributes(ArcVisAtt);
  
  return arc_L;
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Arc::doBuildLVs() {
  
  // Get the geometry
  ArcGeometry g(myName());
  
  g.print();
  
  // Create the vector of logical volumes
  std::vector<G4LogicalVolume*> arcLVs;

  // Build the logical volumes
  for ( unsigned int arcNumber = 0; arcNumber != 12; ++arcNumber ) {
    
    // Push this into the vector
    arcLVs.push_back( makeAnArcLV(g, arcNumber) );
  }

  return arcLVs;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Arc::doPlaceToPVs( std::vector<G4LogicalVolume*> motherLV) {
  
  std::vector<G4VPhysicalVolume*> arcPVs;
  arcPVs.resize(lvs().size());
  
  // We need to loop over the logical volumes
  unsigned int arcNum = 0;
  for ( auto anArcLV : lvs() ) {
    
    // We to name the arc including its arc number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    std::string arcLabel(
      boost::str( boost::format("ArcSection[%02d]") % arcNum )
    );
    
    // Make the physical volume
    arcPVs.push_back(
      new G4PVPlacement(
          new G4RotationMatrix(0.0, pi/2, (arcNum*pi)/6),
          G4ThreeVector(),
          anArcLV,
          arcLabel,
          motherLV[0],
          false,
          0
      )
    );
    
    ++arcNum;
  }
  
  return arcPVs;
}

using gm2ringsim::Arc;
DEFINE_ART_SERVICE(Arc)
