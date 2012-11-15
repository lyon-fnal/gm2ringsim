// Implementation of World

#include "gm2ringsim/tmp/World_service.hh"

// Hacked up geometry parameters class (may replace later)
//#include "artg4geantn02/detectors/GeometryStuff.hh"

// Geant stuff
#include "Geant4/G4Material.hh"
#include "Geant4/G4Box.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4GeometryTolerance.hh"
#include "Geant4/G4GeometryManager.hh"
#include "Geant4/G4UserLimits.hh"

#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4Colour.hh"

//#include CHANGE_ME: Add include for detector Art hit header

#include "gm2ringsim/geometry/BKGeom.hh"

// Constructor for the service (we'll hard code all parameters at first)
gm2ringsim::World::World(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "world"),
                   p.get<std::string>("category", "world"),
                   p.get<std::string>("mother_category", "")),
    magFieldStrength_( p.get<double>("magFieldStrength", 0.0)*tesla ),
    //magField_(),
    bla_( p.get<fhicl::ParameterSet>("bla") ),
    p_( p ),
    g_()
{

  std::cout<<"Geometry was built in constructor with values from fhicl:\n\t\t bottles:"<<g_.bottles<<"\n\t\t mybot"<<g_.mybot<<std::endl;


  // Looking at ExN02MagneticField, I see that calling SetMagFieldValue does more than what is 
  // in the constructor if we passed in the strength directly. So we make a zero field at first, and then 
  // set the field here. Would be nicer if we could rely on setting it from the constructor
  //  magField_.SetMagFieldValue(magFieldStrength_);
    
}


void gm2ringsim::World::initialize() {

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(100);//g.fWorldLength);
  G4cout << "Computed tolerance = "
  << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
  << " mm" << G4endl;
  
  int fun = bla_.get<int>("fun");
  fun++;
  
  fhicl::ParameterSet bla( p_.get<fhicl::ParameterSet>("bla") );
  int ffun = bla.get<int>("fun");
  ffun++;
  
  G4cout << "ffun " << ffun << G4endl;
  


}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::World::doBuildLVs() {

  G4double a, z;
  G4double density;
  G4int nel;

  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
   
  G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  //  GeometryStuff g;

  G4double HalfWorldLength = 0.5*100;//g.fWorldLength;

  G4Box* solidWorld= new G4Box("world",HalfWorldLength,HalfWorldLength,HalfWorldLength);
  G4LogicalVolume* logicWorld= new G4LogicalVolume( solidWorld, Air, "World", 0, 0, 0);

  // Set attributes
  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  logicWorld->SetVisAttributes(boxVisAtt);
  
  return std::vector<G4LogicalVolume*> {logicWorld};
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::World::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
    return std::vector<G4VPhysicalVolume*> {
        new G4PVPlacement(  0,               // no rotation
                            G4ThreeVector(), // at (0,0,0)
                            lvs()[0],      // its logical volume
                            "World",         // its name
                            0,               // its mother  volume
                            false,           // no boolean operations
                            0 )              // copy number
        };
}

using gm2ringsim::World;
DEFINE_ART_SERVICE(World)
