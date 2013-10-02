// Implementation of Kicker

#include "gm2ringsim/kicker/Kicker_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4RunManager.hh"

#include "Geant4/G4FieldManager.hh"
#include "fields/g2FieldEqRhs.hh"
#include "Geant4/G4Mag_UsualEqRhs.hh"
#include "Geant4/G4Mag_SpinEqRhs.hh"
#include "Geant4/G4ClassicalRK4.hh"
#include "Geant4/G4EqEMFieldWithSpin.hh"
#include "Geant4/G4EqEMFieldWithEDM.hh"

//#include "kickerMessenger.hh"
#include "kicker/KickerHelpers.hh"
#include "artg4/material/Materials.hh"

#include "Geant4/G4String.hh"

#include "artg4/util/util.hh"
#include "gm2ringsim/common/ring/RingSD.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/fields/g2EqEMFieldWithSpin.hh"
#include "gm2ringsim/fields/g2EqEMFieldWithEDM.hh"

#include <iomanip>
#include <tr1/functional>


//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Kicker::Kicker(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
		 p.get<std::string>("name", "kicker"),
		 p.get<std::string>("category", "kicker"),
		 p.get<std::string>("mother_category", "vac")),
    sts_("SpinTracking"),
    nospin_tracking_(true),
    spin_tracking_(sts_.spinTrackingEnabled),
    edm_tracking_(sts_.edmTrackingEnabled),
    kg_(myName()),
    numKickers_(3),
    numKickerObjects_(3),
    which_modifier_(NO_MODIFIER) 
{
  if ( spin_tracking_ || edm_tracking_ ) { nospin_tracking_ = false; }

  
  G4cout << "=========== Kicker ===========" << G4endl;
  G4cout << "| Beam Charge  = " << sts_.GetCharge() << G4endl;
  G4cout << "| Type of Kick = " << kg_.TypeOfKick << G4endl;
  if ( kg_.TypeOfKick == "LCR" || kg_.TypeOfKick == "E821" ) { KickType_ = KICK_LCR; }
  if ( kg_.TypeOfKick == "Square" || kg_.TypeOfKick == "Perfect" ||
       kg_.TypeOfKick == "SQUARE" || kg_.TypeOfKick == "PERFECT" ) { KickType_ = KICK_SQUARE; }

  if ( KickType_ == KICK_LCR ) {
    G4cout << "| B(y) = " << kg_.kickerHV[0] << " , " << kg_.kickerHV[1] << " , " << kg_.kickerHV[2] << " Gauss" << G4endl;
    if ( kg_.kickerHV[0] <= 0.0 && kg_.kickerHV[0] <= 0.0 && kg_.kickerHV[0] <= 0.0 ) {
      KickType_ = KICK_OTHER;
    }
  }
  else if ( KickType_ == KICK_SQUARE ) {
    G4cout << "| HV = " << kg_.squareMag[0] << " , " << kg_.squareMag[1] << kg_.squareMag[2] << G4endl;
    if ( kg_.squareMag[0] <= 0.0 && kg_.squareMag[0] <= 0.0 && kg_.squareMag[0] <= 0.0 ) {
      KickType_ = KICK_OTHER;
    }
  }
  else {
    G4cout << "| No Kick" << G4endl;
  }
  G4cout << "=============================" << G4endl;
  kg_.print();
  //FIXME??: move to kickerGeometry
  figureProperOffsetTime();
}

//FIXME??: Move this all to KickerGeometry, since all of these things 
// can be known in that constructor

void gm2ringsim::Kicker::figureProperOffsetTime(){
  G4double& L = kg_.circuitL.at(KICKER2);
  G4double& C = kg_.circuitC.at(KICKER2);
  G4double& R = kg_.circuitR.at(KICKER2);
  G4double k2off = kicker_helpers::getProperOffsetTime(L,C,R);
  G4double cyclotronPeriod = (2*pi*R_magic()) / c_light;
  
  // Distance beteen centers of successive kicker plates                        
  G4double cToCDistance = 15.;
  
  // Time of flight between centers of successive kicker plates                 
  G4double cToCTime = (cToCDistance/360.) * cyclotronPeriod;
  
  // Subtract cToCTime since K1 sees bunch that much earlier...                 
  kg_.kickerProperOffsetTime[KICKER1] = k2off-cToCTime;
  
  kg_.kickerProperOffsetTime[KICKER2] = k2off;
  
  // ...and add cToCTime since K3 sees bunch that much later                    
  kg_.kickerProperOffsetTime[KICKER3] = k2off+cToCTime;
} //figureProperOffsetTime

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Kicker::doBuildLVs() {
    
    buildKickerPlatesSAndL();
    buildKickerFields();
    
    //FIXME: Decide which logical volumes we want to store in ART
    std::vector<G4LogicalVolume *> l_kicker;
    return l_kicker;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Kicker::doPlaceToPVs( std::vector<G4LogicalVolume*> vacLVs) {
  
  buildKickerPlates(vacLVs);
  
  //FIXME: Decide which physical volumes we want to store in ART
  std::vector<G4VPhysicalVolume *> p_kicker;
  return p_kicker;
}

void gm2ringsim::Kicker::buildKickerPlatesSAndL() {

  for (int kickerNumber = 0; kickerNumber < numKickers_; ++kickerNumber){
    // For a detailed explanation of how the arrays are ever-so-cleverly          
    // utilized, see 'kickerParameters.hh'                                        
    for(G4int objectType = 0; objectType < numKickerObjects_; objectType++)
      {
// 	if ( objectType == FIELDREGION ) {
// 	  G4cout << kickerNumber << "\tObjectType = FieldRegion" << G4endl;
// 	}
// 	if ( objectType == INNERPLATE ) {
// 	  G4cout << kickerNumber << "\tObjectType = InnerPlate" << G4endl;
// 	}
// 	if ( objectType == OUTERPLATE ) {
// 	  G4cout << kickerNumber << "\tObjectType = OuterPlate" << G4endl;
// 	}
// 	G4cout << "  rMin     = " << kg_.kPlates_rMin[objectType] << G4endl;
// 	G4cout << "  rMax     = " << kg_.kPlates_rMax[objectType] << G4endl;
// 	G4cout << "  z        = " << kg_.kPlates_z[objectType] << G4endl;
// 	G4cout << "  sPhi     = " << 180.0/3.14159*kg_.kPlates_Sphi[kickerNumber] << G4endl;
// 	G4cout << "  dPhi     = " << 180.0/3.15159*kg_.kPlates_Dphi << G4endl << G4endl;
	
	G4Tubs *kicker_S = new G4Tubs("kicker_S",
				      kg_.kPlates_rMin[objectType],
				      kg_.kPlates_rMax[objectType],
				      kg_.kPlates_z[objectType],
				      kg_.kPlates_Sphi[kickerNumber],
				      kg_.kPlates_Dphi);

	// This nicely segments the field regions, which require magnetic             
	// fields, from the plates.  It also allows the fields to be changed          
	// dynamically by user commands, since the code has direct access to          
	// the field manager for each field region volume and can change the          
	// field through this handle.      

	if(objectType == FIELDREGION){
	  kicker_L_[kickerNumber][objectType] =
	    new G4LogicalVolume(kicker_S,
				artg4Materials::Vacuum(),
				"kickerFieldRegion_L"
				//,manager_[kickerNumber]
				);
	  
	}//FIELDREGION
	else {
	  kicker_L_[kickerNumber][objectType] = new G4LogicalVolume(kicker_S,
								    artg4Materials::Al(),
								    "kicker_L");
	  // Set the visualization attributes for non-field regions                   

	  artg4::setVisAtts( kicker_L_[kickerNumber][objectType],
			     kg_.displayPlates, kg_.platesColor,
			     [] (G4VisAttributes* att) {
			       att->SetForceSolid(1);
			     }
			     ); 
	  
	  //Attach sensitive detectors, but not to field region                       
	  RingSD* ringSD = artg4::getSensitiveDetector<RingSD>("RingSD"); 
	  kicker_L_[kickerNumber][objectType]->SetSensitiveDetector( ringSD );
	  
	} // not field region
      } // loop over object types
  } // loop over kickers
} //buildKickerPlatesSandL


void gm2ringsim::Kicker::buildKickerPlates(std::vector<G4LogicalVolume*> const& vacLVs) {

  // ObjectName will be manipulated by within the appropriate 'for' loop        
  // to ensure that each piece gets a unique and informative name               

  // FIXME: let's use something nicer than char name[x]
  char objectName[30];
 // Loop over the 3 kicker section                                             
  for(G4int kickerNumber = 0; kickerNumber < numKickers_; kickerNumber++)
    {
      // The correct G4PhysicalVolume pointer is accessed automatically         
      // from the vacWallPTR vector using the 'for' loop variables              
      // and the 'kVacWallArray' (See 'kickerParameters.hh' for details)        
      //      vacPTR = vacPTRS.at(kVacWallArray[kickerNumber]);                 

      // Loop over the 3 pieces 
      for(G4int objectType = 0; objectType < numKickerObjects_; objectType++)
        {
          if(objectType == FIELDREGION)
	    {
              std::sprintf(objectName, "%s[%02d]","KickerFieldRegion",
                           kickerNumber);
	    } // if FIELDREGION

	  if(objectType == INNERPLATE)
            {
              std::sprintf(objectName, "%s[%02d]","KickerInnerPlate",
                           kickerNumber);
            } // if INNERPLATE

	  if(objectType == OUTERPLATE)
            {
              std::sprintf(objectName, "%s[%02d]","KickerOuterPlate",
                           kickerNumber);
            } // if OUTERPLATE
	  kicker_P_[kickerNumber][objectType] =
	    new G4PVPlacement(0,
			      G4ThreeVector(),
			      kicker_L_[kickerNumber][objectType],
			      objectName,
			      vacLVs.at(kg_.kVacWallArray.at(kickerNumber)),
			      false,
			      0);
	} // end loop over kicker objects
    } // end loop over kicker sections
}//Kicker::buildKickerPlates(...)

void gm2ringsim::Kicker::buildKickerFields(){

  bool myspin = false;
  bool myedm = false;

  for(int i=0; i!=numKickers_; ++i){
    if( which_modifier_ == NO_MODIFIER )
      modifier_[i] = new NoModifier;
    else
      modifier_[i] = new MorseModifier("g2RunTimeFiles/Morse-profile.dat");

    if( KickType_ == KICK_LCR ){
      kickerMagField_[i] = new LCRKickField(kg_.kickerHV[i]*
					    kg_.kickPercent[i]/100.,
					    kg_.kickerProperOffsetTime[i] +
					    kg_.kickerOffsetTime[i],
					    kg_.circuitC[i],
					    kg_.circuitL[i],
					    kg_.circuitR[i],
					    modifier_[i],
					    sts_.GetCharge());
      if( nospin_tracking_ ){
        iEquation_[i] = new g2TimeDepMagField_EqRhs(kickerMagField_[i]);
        iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 8);
	// 8 for time dependence.                                               
      }
      else if ( spin_tracking_ ) {
	if ( myspin ) {
	  iEquation2_[i] = new g2EqEMFieldWithSpin(kickerMagField_[i]);
	  iStepper_[i] = new G4ClassicalRK4(iEquation2_[i], 12);
	}
	else {
	  iEquation_[i] = new g2TimeDepMagField_SpinEqRhs(kickerMagField_[i]);
	  iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 12);
	}
	// 12 for spin dependence       
      }
      else if ( edm_tracking_ ) {
	if ( myedm ) {
	  iEquation3_[i] = new g2EqEMFieldWithEDM(kickerMagField_[i]);
	  iEquation3_[i]->SetEta(sts_.GetEta());
	  if ( sts_.GetGm2() > 0 ) { iEquation3_[i]->SetAnomaly(sts_.GetGm2()); }
	  iStepper_[i] = new G4ClassicalRK4(iEquation3_[i], 12);
	}
	else {
	  iEquation_[i] = new g2TimeDepMagField_SpinEqRhs(kickerMagField_[i]);
	  iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 12);
	}
	// 12 for spin dependence       
      }
    }else if ( KickType_ == KICK_SQUARE ) {
      kickerMagField_[i] = new SquareKickField(kg_.squareMag[i]*
					       kg_.kickPercent[i]/100.,
					       modifier_[i],
					       sts_.GetCharge());

      if( nospin_tracking_ ){
	iEquation_[i] = new g2TimeDepMagField_EqRhs(kickerMagField_[i]);
        iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 8);
      }
      else if ( spin_tracking_ ) {
	if ( myspin ) {
	  iEquation2_[i] = new g2EqEMFieldWithSpin(kickerMagField_[i]);
	  iStepper_[i] = new G4ClassicalRK4(iEquation2_[i], 12);
	}
	else {
	  iEquation_[i] = new g2TimeDepMagField_SpinEqRhs(kickerMagField_[i]);
	  iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 12);
	}
      }
      else if ( edm_tracking_ ) {
	if ( myedm ) {
	  iEquation3_[i] = new g2EqEMFieldWithEDM(kickerMagField_[i]);
	  iEquation3_[i]->SetEta(sts_.GetEta());
	  if ( sts_.GetGm2() > 0 ) { iEquation3_[i]->SetAnomaly(sts_.GetGm2()); }
	  iStepper_[i] = new G4ClassicalRK4(iEquation3_[i], 12);
	}
	else {
	  iEquation_[i] = new g2TimeDepMagField_SpinEqRhs(kickerMagField_[i]);
	  iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 12);
	}
      }
    } else {
      G4cout << "Invalid kick type specified!  Not going to kick!\n";
      kickerMagField_[i] = new NoKickField(modifier_[i], 
					   sts_.GetCharge());
      if( nospin_tracking_ ){
        iEquation_[i] = new G4Mag_UsualEqRhs(kickerMagField_[i]);
	iStepper_[i] = new G4ClassicalRK4(iEquation_[i]);
      }
      else if ( spin_tracking_ ) {
	iEquation_[i] = new G4Mag_SpinEqRhs(kickerMagField_[i]);
	iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 12);
      }
      else if ( edm_tracking_ ) {
	iEquation_[i] = new G4Mag_SpinEqRhs(kickerMagField_[i]);
        iStepper_[i] = new G4ClassicalRK4(iEquation_[i], 12);
      }
    }
    // 4) Create a chord finder ... this one can be shared                      
    iChordFinder_[i] = new G4ChordFinder(kickerMagField_[i], .01*mm, iStepper_[i]);

    // Create the field manager and return it to the logical volume.  We        
    // only want to create it once, while the members above need to be          
    // recreated anytime a kicker magnetic field parameter changes.             
    manager_[i] = new G4FieldManager(kickerMagField_[i], iChordFinder_[i]);

    kicker_L_[i][FIELDREGION]->
      SetFieldManager(manager_[i], true);
  } //loop over numKickers
}//Kicker::buildKickerFields

//void gm2ringsim::Kicker::setSquarePulseMagnitude(


// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Kicker::doCallArtProduces(art::EDProducer * producer) {
//
//}

// Actually add the data to the event
//void gm2ringsim::Kicker::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}

using gm2ringsim::Kicker;
DEFINE_ART_SERVICE(Kicker)
 
