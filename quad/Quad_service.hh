// Tracker detector service for 

// Include guards
#ifndef QUAD_HH
#define QUAD_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4FieldManager.hh"

#include <vector>
#include <map>
#include <string>

#include "artg4/material/Materials.hh" 
#include "gm2ringsim/common/spin/SpinController.hh"
#include "gm2ringsim/quad/QuadField.hh"
#include "gm2ringsim/quad/QuadGeometry.hh"
#include "gm2ringsim/actions/SpinTrackingSettings.hh"
#include "gm2ringsim/common/ring/RingSD.hh"

#include "gm2ringsim/common/ring/arthits/RingArtRecord.hh"
#include "gm2ringsim/common/ring/RingHit.hh"

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

// Within a namespace
namespace gm2ringsim {
  
  // The class
  class Quad : public artg4::DetectorBase {
    
  public:
    
    // Constructor
    Quad(fhicl::ParameterSet const &, art::ActivityRegistry & );
    
    // We always need a virtual destructor
    virtual ~Quad() {};
    
    //void ConstructQuads(const std::vector<G4LogicalVolume *>&);
      
      /** Interface to the spinController to enable/disable spin tracking
	  in the field implementation. */
      void enable_spintracking(bool);
      
    private:
      
      // Private overriden methods
      // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
      
      // Create the physical volumes
      virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
      
      // CHANGE_ME: Delete the next two functions if no hits

    
    void buildQuadsSandL();
    void buildQuads(std::vector<G4LogicalVolume*>&);
    
    
    void buildRegionSandL(G4int, G4int);
    void buildRegion(std::vector<G4LogicalVolume*>&,G4int, G4int);
    
    void buildPlatesSandL(G4int, G4int);
    void buildPlates(G4int, G4int);
    
    void buildInnerOuterSupportsSandL(G4int, G4int);
    void buildInnerOuterSupports(G4int, G4int);

    void buildTopBottomSupportsSandL(G4int, G4int);
    void buildTopBottomSupports(G4int, G4int);
    
    void buildFieldManagers(G4int, G4int);
    void assignFieldManagers();
    //  void do_enable_spintracking(bool);
    
    G4int get_index(G4int, G4int);
    struct plate_params{
      G4double rmin, rmax, zby2, sphi, dphi, zoff;
      plate_params();
      plate_params(G4double,G4double,G4double,G4double,G4double,G4double);
      void print();
    };
    friend class plate_params;
    void init_plate_params();
    plate_params get_plate_params(G4int, G4int, G4int);
    
      G4int get_index(G4int, G4int, G4int);
      
      struct curl_params {
	G4bool is_curled;
	G4double rmin, rmax, sign;
	curl_params();
	curl_params(G4double,G4double,G4double);
	void print();
      };
      
    friend class curl_params;
    void init_curl_params();
    curl_params get_curl_params(G4int, G4int, G4int);
    typedef std::map<int, plate_params> plate_map_t;
      typedef std::map<int, curl_params> curl_map_t;
      
      
      enum plate_type {INNERPLATE, OUTERPLATE, TOPPLATE, BOTTOMPLATE, plate_type_end};
      
      enum plate_section {SECTION13, SECTION26, plate_section_end};

    SpinTrackingSettings sts_;
    QuadGeometry qg_;
    QuadFieldFactory qff_;
    
    bool nospin_tracking_;
    const bool spin_tracking_;
    const bool edm_tracking_;

    double ScrapeHV, StoreHV_;

      plate_map_t plate_map_;
      curl_map_t curl_map_;

      // Because the quadrupoles are constructed within 2 'for' loops,
      // (one loop for region (1-4) and one for type(13 or 26 degree) )
      // I found it convenient to specify in which 'arc' section each 
      // belongs by creating the two dimensional array found below. In
      // the beginning of the 'quadConstruction::BuildQuadObjects()' 
      // function, the array is used to locate the correct pointer
      // to the appropriate mother volume by selecting the correct
      // pointer from the 'vacWallPTRS' vector, which is constructed
      // in 'primaryConstruction' as handy tool to keep track of volumes
      
    enum arc_placement {Arc0,Arc1,Arc3=3,Arc4,Arc6=6,Arc7,Arc9=9,Arc10=10};
    
    const G4int qVacWallArray_[4][2] = { {Arc0,Arc1}, 
					 {Arc3,Arc4}, 
					 {Arc6,Arc7}, 
					 {Arc9,Arc10} };
    

    double angSupportPos_[2][6];
    
    // Just changed the functions to pass along the std::vector<G4LogicalVolume*> motherLVS instead
    //std::vector<G4VPhysicalVolume *> vacPTRS_; //Was a PV in g2MIGTRACE  

    /** @bug These explicit array dimension constants should be replaced
	  ... see quadConstruction.cc. */
      
      // There are 4 quadrupole regions in the storage ring
      // A region is divided into 13 and 26 degree sections
      // 4 plates (in,out,top,bot) form the physical quadrupole electrodes
      // There are inner and outer supports
      
      // quad regions, sections
    G4LogicalVolume *genericQuadRegion_L_[4][2];
    G4VPhysicalVolume *genericQuadRegion_P_[4][2];
    
    
    G4Material *plate_material;
    // quad region, section, plate
    G4LogicalVolume *genericQuadPlate_L_[4][2][4];
    G4VPhysicalVolume *genericQuadPlate_P_[4][2][4];

    G4Material *support_material;
    // quad region, section, support
    G4LogicalVolume *genericInnerSupport_L_[4][2][6];
    G4VPhysicalVolume *genericInnerSupport_P_[4][2][6];
    
    G4LogicalVolume* genericOuterSupport_L_[4][2][6];
    G4VPhysicalVolume *genericOuterSupport_P_[4][2][6];
    
    // quad region, section  
    G4FieldManager *withoutSpin_[4][2];
    G4FieldManager *withSpin_[4][2];
    G4FieldManager *withEDM_[4][2];



    //      enum plate_type {INNERPLATE, OUTERPLATE, TOPPLATE, BOTTOMPLATE, plate_type_end};
    
    // enum plate_section {SECTION13, SECTION26, plate_section_end};
    
  };

}

#endif
