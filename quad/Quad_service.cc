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
#include "messagefacility/MessageLogger/MessageLogger.h"

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
#include "Geant4/G4EqEMFieldWithEDM.hh"
#include "Geant4/G4ClassicalRK4.hh"
#include "Geant4/G4UserLimits.hh"

#include "gm2ringsim/fields/g2EqEMFieldWithSpin.hh"
#include "gm2ringsim/fields/g2EqEMFieldWithEDM.hh"

#include <sstream>

// Constructor for the service 
gm2ringsim::Quad::Quad(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "quad"),
	       p.get<std::string>("category", "quad"),
	       p.get<std::string>("mother_category", "vac")),
  sts_("SpinTracking"),
  qg_(myName()), //QuadGeometry
  qff_(qg_.DoScraping, qg_.ScrapeHV, qg_.StoreHV, sts_.GetCharge()),
  nospin_tracking_(true),
  spin_tracking_(sts_.spinTrackingEnabled),
  edm_tracking_(sts_.edmTrackingEnabled)
  // The rest of the internal variables are things like pointers
  // and structures that get created/assigned below
{
  if ( spin_tracking_ || edm_tracking_ ) { nospin_tracking_ = false; }

  if ( qg_.SupportMaterial == "Macor" || qg_.SupportMaterial == "MACOR" || qg_.SupportMaterial == "MacorCeramic" ) {
    support_material = artg4Materials::MacorCeramic();
  }
  if ( qg_.SupportMaterial == "Beryl" || qg_.SupportMaterial == "Be" || qg_.SupportMaterial == "Beryllium" ) {
    support_material = artg4Materials::Be();
  }
  if ( qg_.SupportMaterial == "Massless" || qg_.SupportMaterial == "Vacuum" || qg_.SupportMaterial == "None" ) {
    support_material = artg4Materials::Vacuum();
  }
  if ( qg_.PlateMaterial == "Al" || qg_.PlateMaterial == "Aluminum" ) {
    plate_material = artg4Materials::Al();
  }
  if ( qg_.PlateMaterial == "Massless" || qg_.PlateMaterial == "Vacuum" || qg_.PlateMaterial == "None" ) {
    plate_material = artg4Materials::Vacuum();
  }

  G4cout << "=========== Quad ===========" << G4endl;
  G4cout << "| Beam Charge      = " << sts_.GetCharge() << G4endl;
  G4cout << "| DoScraping       = " << qg_.DoScraping << G4endl;
  G4cout << "| Spin Tracking    = " << spin_tracking_ << G4endl;
  G4cout << "| EDM Tracking     = " << edm_tracking_ << G4endl;
  G4cout << "| Support Material = " << qg_.SupportMaterial << G4endl;
  G4cout << "| Plate Material   = " << qg_.PlateMaterial << G4endl;
  G4cout << "| Q1 offset        = " << qg_.outerQ1offset << G4endl;
  if ( qg_.StoreHV == 40*kilovolt ) {
    G4cout << "| Running w/ HV    =  40 kV" << G4endl;
    G4cout << "| Running w/ HV    =  34 (scraping) kV" << G4endl;
  }
  else {
    G4cout << "| Running w/ HV    =  24 kV" << G4endl;
    G4cout << "| Running w/ HV    =  17 (scraping) kV" << G4endl;
  }
  G4cout << "============================" << G4endl;
  //printf("In the Quad constructor \n");
  
  //Create 2D array from vectors passed in
  for (int i=0;i<6;++i){
    angSupportPos_[0][i]=qg_.angSupportPos0[i];
    angSupportPos_[1][i]=qg_.angSupportPos1[i];
  }
  
  
  //  G4cout << "call init_plate_params()\n";
  init_plate_params();
  //  G4cout << "call init_curl_params()\n";
  init_curl_params();

  //Print the Geometry as a check
  qg_.print();
}

  // Build the shape and logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Quad::doBuildLVs() {
  buildQuadsSandL();
  
  //FIXME: Decide which logical volumes we want to store in ART
  std::vector<G4LogicalVolume *> l_quads;
  return l_quads;
  
  printf("done with doBuildLVs()\n");
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Quad::doPlaceToPVs( std::vector<G4LogicalVolume*> vacLV) {

  buildQuads(vacLV); // PhysicalVolumes
  
  //FIXME: Decide which physical volumes we want to store in ART
  std::vector<G4VPhysicalVolume *> p_quads;
  return p_quads;
}

void gm2ringsim::Quad::buildQuads(std::vector<G4LogicalVolume*>& vacLV){
 // Loop over the 4 quad regions
  for(G4int quadRegion = 0; quadRegion < qg_.numQuadRegions; quadRegion++)
    // Loop over the 2 sections of each region
    for(G4int sectionType = 0; sectionType < qg_.numQuadSections; sectionType++)
      {
	buildRegion(vacLV,quadRegion, sectionType);
	buildPlates(quadRegion, sectionType);
	buildInnerOuterSupports(quadRegion, sectionType);
	buildTopBottomSupports(quadRegion, sectionType);
      }
}

void gm2ringsim::Quad::buildQuadsSandL(){
  // Loop over the 4 quad regions
  for(G4int quadRegion = 0; quadRegion < qg_.numQuadRegions; quadRegion++)
    // Loop over the 2 sections of each region
    for(G4int sectionType = 0; sectionType < qg_.numQuadSections; sectionType++)
      {
	buildRegionSandL(quadRegion, sectionType);
	buildPlatesSandL(quadRegion, sectionType);
	buildInnerOuterSupportsSandL(quadRegion, sectionType);
	buildTopBottomSupportsSandL(quadRegion, sectionType);
	buildFieldManagers(quadRegion, sectionType);
      }

  // Field managers get assigned to the LVs so we can do this now or wait til the PVs are built
  assignFieldManagers();
}

void gm2ringsim::Quad::buildRegionSandL(G4int quadRegion, G4int sectionType){
  // Each "Region" will contain the plates, including their curvy
  // bits, and the fields.  The region fills the space between the
  // innermost radius of the vacRegion that could be contained in a
  // torus, and the outermost toroid radius of the vacTregion

  G4Tubs *tubs = new G4Tubs("quadRegion_S",
			    qg_.QFR_rmin, qg_.QFR_rmax, qg_.QFR_z,
			    qg_.quad_Sphi[sectionType], qg_.quad_Dphi[sectionType]);

  //  G4LogicalVolume *log =
  genericQuadRegion_L_[quadRegion][sectionType] =
    new G4LogicalVolume(tubs,
			artg4Materials::Vacuum(),
			"quadRegion_L");

    artg4::setVisAtts( genericQuadRegion_L_[quadRegion][sectionType],
		     qg_.displayQFR, qg_.qfrColor,
		     //wallLV, g.displayWall, g.wallColor,
		     [] (G4VisAttributes* att) {
		       att->SetForceSolid(0);
		     } ); 

}



void gm2ringsim::Quad::buildRegion(std::vector<G4LogicalVolume*>& vacLV, G4int quadRegion, G4int sectionType){
  
  std::ostringstream regionName;
  regionName << "QuadFieldRegion[" << quadRegion << "][" 
	     << sectionType << ']';

  //vacs[ arcNumber ]->GetDaughter(0)->GetLogicalVolume(),
  G4LogicalVolume *vac = vacLV[qVacWallArray_[quadRegion][sectionType]];//->GetDaughter(0)->GetLogicalVolume();
  genericQuadRegion_P_[quadRegion][sectionType] =
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,0),
		      genericQuadRegion_L_[quadRegion][sectionType], //log,
		      regionName.str(),
		      vac,
		      false,
		      0);

} 

void gm2ringsim::Quad::buildPlates(G4int quadRegion, G4int sectionType){ 

  for(G4int plateType=0; plateType!=qg_.numPlateTypes; plateType++){
    // Create a unique physical volume name
    std::ostringstream plateName;
    plateName << qg_.plateNames[plateType]
	      << "[" << quadRegion << "]" << "[" << sectionType << "]";
    
    plate_params p = get_plate_params(quadRegion, sectionType, plateType);
    
    genericQuadPlate_P_[quadRegion][sectionType][plateType] = 
      new G4PVPlacement(0,
			G4ThreeVector(0, 0.*m, p.zoff),
			genericQuadPlate_L_[quadRegion][sectionType][plateType],
			plateName.str(),
			genericQuadRegion_P_[quadRegion][sectionType]->GetLogicalVolume(),
			false,
			0);
    
  } // loop over plateTypes
}


void gm2ringsim::Quad::buildPlatesSandL(G4int quadRegion, G4int sectionType){
  // Each of the four plates that compose a quad section are built
  // within this loop as "genericQuadPlate"; the correct dimensions
  // for each plate type are obtained by accessing the dimensional
  // arrays with the loop index...clever!  Saves a lot of code.

  // This string is important for looking up the RingSD from the SDManager
  RingSD* ringSD = artg4::getSensitiveDetector<RingSD>("RingSD"); 

  
  //printf("about to buildPlatesSandL\n");
  for(G4int plateType=0; plateType!=qg_.numPlateTypes; plateType++){
    //printf("Getting plate parameters for q=%d,s=%d,p=%d\n",quadRegion,sectionType,plateType);
    plate_params p = get_plate_params(quadRegion, sectionType, plateType);
    p.print();
    mf::LogDebug("Quad_service") << p.rmin << ' ' << p.rmax << ' '
    	   << p.zby2 << ' ' << p.sphi << ' '
    	   << p.dphi << '\n';
    G4Tubs *mainPlate_S = new G4Tubs("mainPlate_S", 
				     p.rmin, p.rmax, p.zby2, p.sphi, p.dphi);
    
    // The inner and outer plates have curled edges that are attached
    // to the ends of the main plate via G4UnionSolid methods
    curl_params c = get_curl_params(quadRegion, sectionType, plateType);
    
    G4VSolid *plate_S;
    
    /** @bug The geometry manipulations needed to add the curled plate
	edges seem to cause Geant in digestion ... disable them for
	now.  Revisit this in a later version of the distribution. */
    
    //    if( c.is_curled ){
    if( false ){
      G4cout << "curled!\n";
      G4Torus *curledEdge_S = new G4Torus("curledEdge_S",
					  c.rmin, c.rmax, 
					  p.rmin+c.sign*c.rmax,
					  p.sphi, p.dphi);
      
      G4Transform3D topCurlTransform(G4RotationMatrix(),
				     G4ThreeVector(0., 0., -p.zby2));
      G4UnionSolid *plateWithCurledEdges_US = new G4UnionSolid("plateWithCurledEdges",
							       mainPlate_S,
							       curledEdge_S,
							       topCurlTransform);
      
      G4Transform3D bottomCurlTransform(G4RotationMatrix(),
					G4ThreeVector(0., 0., p.zby2));
      
      plateWithCurledEdges_US = new G4UnionSolid("plateWithCurledEdges",
						 plateWithCurledEdges_US,
						 curledEdge_S,
						 bottomCurlTransform);
      
      plate_S = plateWithCurledEdges_US;
    }
    else //c is not curled
      plate_S = mainPlate_S;
    
    //G4LogicalVolume *
    genericQuadPlate_L_[quadRegion][sectionType][plateType] =
      new G4LogicalVolume(plate_S,
			  plate_material,
			  "genericQuadPlate_L");
    
    artg4::setVisAtts( genericQuadPlate_L_[quadRegion][sectionType][plateType],
		       qg_.displayPlates, qg_.platesColor,
		       [] (G4VisAttributes* att) {
			 att->SetForceSolid(1);
		       }
		       ); 
    
    genericQuadPlate_L_[quadRegion][sectionType][plateType]->SetSensitiveDetector( ringSD );
  } //plateType
}

void gm2ringsim::Quad::buildInnerOuterSupports(G4int quadRegion, G4int sectionType) {
  
  G4int const supportPairsPerSection = qg_.numSupportPairsPerSection[sectionType];
  for(G4int sPair=0; sPair<supportPairsPerSection; sPair++){
    
    // A unique name for the inner support physical volume
    std::ostringstream supportName;
    supportName << "QuadInnerSupport[" << quadRegion << "]"
		<< "[" << sectionType << "]" << "[" << sPair << "]";
    
    G4VPhysicalVolume *ptr = genericQuadRegion_P_[quadRegion][sectionType];
    genericInnerSupport_P_[quadRegion][sectionType][sPair] 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,0),
			  genericInnerSupport_L_[quadRegion][sectionType][sPair],
			  supportName.str(),
			  ptr -> GetLogicalVolume(),
			  false,
			  0);


    
    // A unique name for the outer support physical volume
    supportName.str("");
    supportName << "QuadOuterSupport[" << quadRegion << "]"
		<< "[" << sectionType << "]" << "[" << sPair << "]";
    


    ptr = genericQuadRegion_P_[quadRegion][sectionType];
    genericOuterSupport_P_[quadRegion][sectionType][sPair] 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,0),
			  genericOuterSupport_L_[quadRegion][sectionType][sPair],
			  supportName.str(),
			  ptr -> GetLogicalVolume(),
			  false,
			  0);
   }
}

void gm2ringsim::Quad::buildInnerOuterSupportsSandL(G4int quadRegion, G4int sectionType) { 
  // All supports are again built using 'for' loops.  Here, determine
  // the number of pairs for the current sectionType
  G4int const supportPairsPerSection = qg_.numSupportPairsPerSection[sectionType];
  
  // This string is important for looking up the RingSD from the SDManager
  RingSD* ringSD = artg4::getSensitiveDetector<RingSD>("RingSD"); 


  // convert vectors to arrays
  const int nInd = 7;//qg_.support_nPlanes;
  double sz[nInd]; for (int i=0;i<nInd;++i){ sz[i]=qg_.support_z[i];}
  double srI[nInd]; for (int i=0;i<nInd;++i){ srI[i]=qg_.support_rInner[i];}
  double srO[nInd]; for (int i=0;i<nInd;++i){ srO[i]=qg_.support_rOuter[i];}
  
  for(G4int sPair=0; sPair<supportPairsPerSection; sPair++){
    
    // An extra long support but with the correct shape
    G4Polycone *support_S = new G4Polycone("support_S",
					   qg_.support_Sphi,
					   qg_.support_Dphi,
					   qg_.support_nPlanes,
					   sz,
					   srI,
					   srO);
    
    // Volumes that are used to "machine" the support_S solid into the
    // correct length and position in one step for each support type
    G4Tubs *innerSupportMachining_S = new G4Tubs("innerSupportMachining_S",
						 qg_.innerSM_rMin,
						 qg_.innerSM_rMax,
						 qg_.machining_z,
						 qg_.machining_Sphi,
						 qg_.machining_Dphi);
    
    G4Tubs *outerSupportMachining_S = new G4Tubs("outerSupportMachining_S",
						 qg_.outerSM_rMin,
						 qg_.outerSM_rMax,
						 qg_.machining_z,
						 qg_.machining_Sphi,
						 qg_.machining_Dphi);
    
    // Calculate the position and rotation of the current inner
    // support *as it will be placed in the storage ring*.
    G4double posX = qg_.ISupportRad*cos(angSupportPos_[sectionType][sPair]);
    G4double posY = qg_.ISupportRad*sin(angSupportPos_[sectionType][sPair]);

    
    G4Transform3D ISMTransform(G4RotationMatrix(0.*degree, 
						90.*degree, 
						90.*degree-angSupportPos_[sectionType][sPair]),
			       G4ThreeVector(posX, posY, 0.));
    
    // "Machine" the support via intersection of the support solid
    // with the machining volume.  After this step, the support is
    // perfectly placed and rotated 
    G4IntersectionSolid *genericInnerSupport_IS = 
      new G4IntersectionSolid("genericInnerSupport_IS",
			      innerSupportMachining_S,
			      support_S,
			      ISMTransform);
    
    
    // Repeat the above procedure for the outer support, which is the
    // same except for slight changes to rotation and translation
    posX = qg_.OSupportRad*cos(angSupportPos_[sectionType][sPair]);
    posY = qg_.OSupportRad*sin(angSupportPos_[sectionType][sPair]);
    
    G4Transform3D OSMTransform(G4RotationMatrix(0.*degree, 
						90.*degree, 
						270.*degree-angSupportPos_[sectionType][sPair]),
			       G4ThreeVector(posX, posY, 0.));
    
    G4IntersectionSolid *genericOuterSupport_IS = 
      new G4IntersectionSolid("genericOuterSupport_IS",
			      outerSupportMachining_S,
			      support_S,
			      OSMTransform);
    
      

      //      G4LogicalVolume *genericInnerSupport_L = 
      genericInnerSupport_L_[quadRegion][sectionType][sPair] =
	new G4LogicalVolume(genericInnerSupport_IS,
			    support_material,
			    "genericInnerSupport_L");
      
    
      
      //      G4LogicalVolume *genericOuterSupport_L = 
      genericOuterSupport_L_[quadRegion][sectionType][sPair] =
	new G4LogicalVolume(genericOuterSupport_IS,
			    support_material,
			    "genericOuterSupport_L");
      

  
      artg4::setVisAtts( genericInnerSupport_L_[quadRegion][sectionType][sPair],
			 qg_.displaySupports, qg_.supportsColor,
			 [] (G4VisAttributes* att) {
			   att->SetForceSolid(1);
			 }
			 );     

      artg4::setVisAtts( genericOuterSupport_L_[quadRegion][sectionType][sPair],
			 qg_.displaySupports, qg_.supportsColor,
			 [] (G4VisAttributes* att) {
			   att->SetForceSolid(1);
			 }
			 );     
      genericInnerSupport_L_[quadRegion][sectionType][sPair]->SetSensitiveDetector( ringSD );
      genericOuterSupport_L_[quadRegion][sectionType][sPair]->SetSensitiveDetector( ringSD );
      
  } // Loop overs supportPairs
}

// Placeholder to possible implement the top/bottom quadrupole plate
// support.  These supports are probably completely irrelevant as far
// as storage efficiency goes so including them in the simulation is
// not critical.  ZSH 08 AUG 09.
void gm2ringsim::Quad::buildTopBottomSupportsSandL(G4int /*quadRegion*/, G4int /*sectionType*/)
{}

void gm2ringsim::Quad::buildTopBottomSupports(G4int /*quadRegion*/, G4int /*sectionType*/)
{}

void gm2ringsim::Quad::assignFieldManagers(){

  for( int i=0; i!=qg_.numQuadRegions; ++i) {
    for( int j=0; j!=qg_.numQuadSections; ++j) {
      if( nospin_tracking_ ){
        genericQuadRegion_L_[i][j]->SetFieldManager(withoutSpin_[i][j], true);
      }
      else if ( spin_tracking_ ) {
	genericQuadRegion_L_[i][j]->SetFieldManager(withSpin_[i][j], true);
      }
      else if ( edm_tracking_ ) {
	genericQuadRegion_L_[i][j]->SetFieldManager(withEDM_[i][j], true);
      }
    }
  }
}

void gm2ringsim::Quad::buildFieldManagers(G4int quadRegion, G4int sectionType) {
  
  bool myspin = false;
  bool myedm = false;

  //G4cout << "buildFieldManagers: " << quadRegion << ' ' << sectionType << '\n';
  
  G4ElectroMagneticField *field;
  G4EquationOfMotion *equation;
  G4MagIntegratorStepper *stepper;
  G4MagInt_Driver *driver;
  G4ChordFinder *chord;

  
  // first, the quad field regions
  // shared
  field = qff_.buildQuadField(quadRegion, sectionType);



  //----------------------------------------
  // build the spin ignoring field equations
  //----------------------------------------
  if ( nospin_tracking_ ) {
    equation = new G4EqMagElectricField(field);
    stepper = new G4ClassicalRK4(equation, 8); // modifies energy, so 8
    driver = new G4MagInt_Driver(0.01*mm, stepper, stepper->GetNumberOfVariables());
    chord = new G4ChordFinder(driver);
    withoutSpin_[quadRegion][sectionType] = new G4FieldManager(field,chord,true);
  }

  //----------------------------------------
  // build the spin evolving field equations
  //----------------------------------------
  if ( spin_tracking_ ) {
    if ( myspin ) {
      equation = new g2EqEMFieldWithSpin(field);
    }
    else {
      equation = new G4EqEMFieldWithSpin(field);
    }
    stepper = new G4ClassicalRK4(equation, 12); // modifies spin, so 12
    driver = new G4MagInt_Driver(0.01*mm, stepper, stepper->GetNumberOfVariables());
    chord = new G4ChordFinder(driver);
    withSpin_[quadRegion][sectionType] = new G4FieldManager(field,chord,true);
  }
 
  if ( edm_tracking_ ) {
    if ( myedm ) {
      g2EqEMFieldWithEDM *equation2 = new g2EqEMFieldWithEDM(field);
      equation2->SetEta(sts_.GetEta());
      if ( sts_.GetGm2() >= 0 ) { equation2->SetAnomaly(sts_.GetGm2()); }
      stepper = new G4ClassicalRK4(equation2,12);
    }
    else {
      G4EqEMFieldWithEDM *equation2 = new G4EqEMFieldWithEDM(field);
      equation2->SetEta(sts_.GetEta());
      if ( sts_.GetGm2() >= 0 ) { equation2->SetAnomaly(sts_.GetGm2()); }
      stepper = new G4ClassicalRK4(equation2,12);
    }
    G4MagInt_Driver *driver = new G4MagInt_Driver(0.01*mm, stepper, stepper->GetNumberOfVariables());
    chord = new G4ChordFinder(driver);
    withEDM_[quadRegion][sectionType] = new G4FieldManager(field, chord, true);
  }
  
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing

gm2ringsim::Quad::plate_params::
plate_params() :
  rmin(0), rmax(0), zby2(0), sphi(0), dphi(0), zoff(0)
{}

gm2ringsim::Quad::plate_params::
plate_params(G4double rmin_, G4double rmax_, G4double zby2_, 
	     G4double sphi_, G4double dphi_,
	     G4double zoff_) :
  rmin(rmin_), rmax(rmax_), zby2(zby2_), 
  sphi(sphi_), dphi(dphi_), zoff(zoff_)
{}

void gm2ringsim::Quad::plate_params::
print() {
  std::ostringstream oss;
  oss << "Plate Params:\n";
  oss << "\t rmin: "<<rmin<<"\n";
  oss << "\t rmax: "<<rmax<<"\n";
  oss << "\t zby2: "<<zby2<<"\n";
  oss << "\t sphi: "<<sphi<<"\n";
  oss << "\t dphi: "<<dphi<<"\n";
  oss << "\t zoff: "<<zoff<<"\n";
  oss << "\t\t End of Plate Params\n";
  mf::LogInfo("PlateParams") << oss.str();
}


gm2ringsim::Quad::curl_params::
curl_params() :
  is_curled(false), rmin(0), rmax(0), sign(1.)
{}  

gm2ringsim::Quad::curl_params::
curl_params(G4double rmin_, G4double rmax_, G4double sign_) :
  is_curled(true), rmin(rmin_), rmax(rmax_), sign(sign_)
{}

void gm2ringsim::Quad::curl_params::
print() {
  mf::LogInfo("Quad_service") << "\n"
  <<"Curl Params:"
  <<"\n\t rmin: "<<rmin
  <<"\n\t rmax: "<<rmax
  <<"\n\t sign: "<<sign
  <<"\n\t is_curled:"<<is_curled
  <<"\n\t\t End of Curl Params" << "\n";
}


void gm2ringsim::Quad::init_curl_params(){
  G4int idx;
  curl_params c(qg_.curlInnerRadius, qg_.curlOuterRadius, 1.);
  c.is_curled = true;
  for(int i=0; i!=qg_.numQuadSections; ++i){
    // INNERPLATE
    idx = get_index(i, INNERPLATE);
    c.sign = -1.;
    curl_map_[idx] = c;
    // OUTERPLATE
    idx = get_index(i, OUTERPLATE);
    curl_map_[idx] = c;
  }
}


void gm2ringsim::Quad::init_plate_params(){
  G4int idx;
  for( int i=0; i!=qg_.numQuadSections; ++i ){

    //-------------
    // INNERPLATE
    //-------------
    idx = get_index(i, INNERPLATE);
    plate_map_[idx] = 
      plate_params(R_magic()-qg_.plateSep/2.-qg_.innerOuterThickness, R_magic()-qg_.plateSep/2., 
		   qg_.innerOuterWidth/2., qg_.quad_Sphi[i], qg_.quad_Dphi[i],
		   0. );

    //-------------
    // OUTERPLATE
    //-------------
    idx = get_index(i, OUTERPLATE);
    plate_map_[idx] = 
      plate_params(R_magic()+qg_.plateSep/2., R_magic()+qg_.plateSep/2.+qg_.innerOuterThickness,
		   qg_.innerOuterWidth/2., qg_.quad_Sphi[i], qg_.quad_Dphi[i],
		   0.);

    //-------------
    // TOPPLATE
    //-------------
    idx = get_index(i, TOPPLATE);
    plate_map_[idx] = 
      plate_params(R_magic()-qg_.topBottomWidth/2., R_magic()+qg_.topBottomWidth/2.,
		   qg_.topBottomThickness/2., qg_.quad_Sphi[i], qg_.quad_Dphi[i],
		   qg_.plateSep/2.+qg_.topBottomThickness/2.);

    //-------------
    // BOTTOMPLATE
    //-------------
    idx = get_index(i, BOTTOMPLATE);
    plate_map_[idx] =
      plate_params(R_magic()-qg_.topBottomWidth/2., R_magic()+qg_.topBottomWidth/2.,
		   qg_.topBottomThickness/2., qg_.quad_Sphi[i], qg_.quad_Dphi[i],
		   -qg_.plateSep/2.-qg_.topBottomThickness/2.);

  }

  
  //--------------------------------------
  // We have to fix up the Q1 OUTER plates
  //--------------------------------------
  int quadRegion, sectionType;
  idx = get_index(quadRegion=0, sectionType=SECTION13, OUTERPLATE);
  plate_map_[idx] = 
    plate_params(R_magic() + qg_.plateSep/2.0 + qg_.outerQ1offset, R_magic() + qg_.plateSep/2.0 + qg_.outerThickness_Q1 + qg_.outerQ1offset,
		 qg_.innerOuterWidth/2., qg_.quad_Sphi[SECTION13], qg_.quad_Dphi[SECTION13],
		 0.);
  idx = get_index(quadRegion=0, sectionType=SECTION26, OUTERPLATE);
  plate_map_[idx] = 
    plate_params(R_magic() + qg_.plateSep/2.0 + qg_.outerQ1offset, R_magic() + qg_.plateSep/2.0 + qg_.outerThickness_Q1 + qg_.outerQ1offset,
		 qg_.innerOuterWidth/2., qg_.quad_Sphi[SECTION26], qg_.quad_Dphi[SECTION26],
		 0.);

  //  G4cout << "plate_map.size() " << plate_map.size() << '\n';
}


G4int gm2ringsim::Quad::get_index(G4int sectionType, G4int plateType){
  return (sectionType+1)*100 + (plateType+1);
}

G4int gm2ringsim::Quad::get_index(G4int quadRegion, G4int sectionType, G4int plateType){
  return (quadRegion+1)*10000 + (sectionType+1)*100 + (plateType+1);
}




gm2ringsim::Quad::curl_params 
gm2ringsim::Quad::get_curl_params(G4int, //quadRegion
				 G4int sectionType,G4int plateType){
  curl_map_t::iterator found;
  found = curl_map_.find(get_index(sectionType, plateType));
  if( found != curl_map_.end() ){
    //    G4cout << "Found!\n";
    return found->second;
  }
  return curl_params();
}

gm2ringsim::Quad::plate_params 
gm2ringsim::Quad::get_plate_params(G4int quadRegion, G4int sectionType, G4int plateType){
  plate_map_t::iterator found;
  found = plate_map_.find(get_index(quadRegion, sectionType, plateType));
  if( found != plate_map_.end() ){
    //    G4cout << "Found a special one! " 
    //	   << get_index(quadRegion, sectionType, plateType) << '\n';
    return found->second;
  }
  found = plate_map_.find(get_index(sectionType, plateType));
  //    G4cout << "Found a regular one! " 
  //	   << get_index(sectionType, plateType) << '\n';
  return found->second;
}


using gm2ringsim::Quad;
DEFINE_ART_SERVICE(Quad)
