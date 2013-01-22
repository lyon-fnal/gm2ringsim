#ifndef STORAGERINGFIELD_HH
#define STORAGERINGFIELD_HH

/** @file storageRingField.hh

    Provides declarations of a number of classes related to the
    "uniform" storage ring field

    @sa storageRingField
    @sa storageFieldImpl
    @sa uniformStorageImpl
    @sa fringeStorageImpl
    @sa detailedMultipoleStorageImpl

    @sa storageFieldController

    @sa storageFieldMessenger
    @sa fringeStorageMessenger
    @sa detailedMultipoleMessenger

    - Zach Hartwig 2005
    - Kevin Lynch 2009
*/

#include <string>
#include <tr1/memory>
#include <utility>

#include "Geant4/G4MagneticField.hh"
#include "Geant4/G4ThreeVector.hh"

#include "Geant4/globals.hh"

#include "gm2ringsim/external_libs/interpolation/interpolators2D.hh"

namespace gm2ringsim {

  /** The concrete interface to the storage field */
  class storageRingField : public G4MagneticField
  {
  public:
    void GetFieldValue( const double Point[3],
			double *Bfield ) const;
    
  };
  
  /** Abstract base class for the internal field implementation classes */
  class storageFieldImpl {
  public:
    storageFieldImpl(double central) : central_(central) {}
    virtual ~storageFieldImpl(){}
    virtual void GetFieldValue( const double Point[3],
				double *Bfield ) const = 0;
    virtual void set_central(double central){ central_ = central; }
  protected:
    double central_;
  };
  
  class storageFieldMessenger; // defined below
  
  /** A Meyer's singleton which provides an interface that maintains and
      controls the current state of the storage ring field implementations 
  */
  class storageFieldController {
  public:
    
    static storageFieldController const& getInstance();
    void GetFieldValue( const double Point[3],
			double *Bfield ) const;  
    
    void setFieldType(G4String);
    
    std::string setCentralFieldImpl(std::string const& val);
    std::string getCentralFieldImpl() const { return centralname_; }
    
    std::string setFringeFieldImpl(std::string const& val);
    std::string getFringeFieldImpl() const { return fringename_; }
    
    double central_field(double central);
    double central_field() const { return central_; }
    
  private:
    storageFieldController();
    ~storageFieldController(){}
    
    double central_;
    std::tr1::shared_ptr<storageFieldImpl> centralimpl_, fringeimpl_;
    
    std::tr1::shared_ptr<storageFieldMessenger> sfm_;
    
    std::string centralname_, fringename_;
    
    // don't implement! these!
    storageFieldController(storageFieldController const&); 
    storageFieldController& operator=(storageFieldController const&); 
    
  }; // class storageFieldControlloer


  
  
  // implementations
  
  /** Provides a purely vertical, uniform field. */
  class uniformStorageImpl : public storageFieldImpl {
  public:
    uniformStorageImpl(double central);
    void GetFieldValue( const double Point[3],
			double *Bfield ) const;
  private:
    double Field[3];
  };
  
  // fwd dcl
  class fringeStorageMessenger;
  
  /** Provides a field map based implementation of the storage field,
      including the fringe region well inside and outside the storage
      orbit.
  */
  class fringeStorageImpl : public storageFieldImpl {
  public:
    fringeStorageImpl(double central);
    void GetFieldValue( const double Point[3],
			double *Bfield ) const;
    
    std::string fringe_map_name() const { return fringe_map_name_; }
    std::string fringe_map_name(std::string const& new_name);
    
    void set_central(double central){ central_ = central; set_scale(); }
  private:
    bool load_fringe_map();
    std::string fringe_map_name_;
    
    std::tr1::shared_ptr<twoD_interpolator<G4ThreeVector> > map_;
    double scale_;
    void set_scale();
    
    std::tr1::shared_ptr<fringeStorageMessenger> mess_;
  }; // class fringStorageImpl
  
  // fwd dcl
  class detailedMultipoleMessenger;
  
  /** Provides a hybrid field map and multipole expanded field
      implementation.
      
      The mapping here provides a set of multipole coefficients measured
      at a large number of (mapped) points around the ring azimuth.
      Data from a g2track database.
      
      @bug How do we set the scale here in the case of user change of
      central field?  Does that even make sense here?
  */
  class detailedMultipoleStorageImpl : public storageFieldImpl {
  public:
    detailedMultipoleStorageImpl(double central);
    void GetFieldValue( const double Point[3],
			double *Bfield ) const;
    
    std::string multipole_map_name() const { return multipole_map_name_; }
    std::string multipole_map_name(std::string const& new_name);
    
    double theta_offset(double to);
    double theta_offset() const { return theta_offset_; }
    
  private:
    bool load_multipole_map();
    std::string multipole_map_name_;
    
    struct data_holder {
      double By0;
      std::vector<std::pair<double,double> > ab;
      G4ThreeVector calcB(double r, double phi) const;
    };
    
    std::vector<data_holder> data_;
    double theta_offset_; // angular position of the inflector aperture
    // in radians in the global coordinate system 
    std::tr1::shared_ptr<detailedMultipoleMessenger> mess_;
  };
  
} //namespace gm2ringsim

#include "Geant4/G4UImessenger.hh"
#include "Geant4/G4UIdirectory.hh"
#include "Geant4/G4UIcmdWithAString.hh"
#include "Geant4/G4UIcmdWithADoubleAndUnit.hh"
#include "Geant4/G4UIcmdWithoutParameter.hh"


namespace gm2ringsim {
  /** Provides a G4UImessenger interface to set details of the storage
      field.
      
      Provides the following commands:
      - /g2MIGTRACE/storageField/centralField
      - /g2MIGTRACE/storageField/fringeField
      - /g2MIGTRACE/storageField/setCentralField
      - /g2MIGTRACE/storageField/getCentralField
  */
  class storageFieldMessenger : public G4UImessenger {
  public:
    storageFieldMessenger(storageFieldController *);
    ~storageFieldMessenger();
    
    void SetNewValue(G4UIcommand*,G4String);
    
  private:
    storageFieldController *controller_;
    
    G4UIdirectory *storageFieldDir_;
    
    G4UIcmdWithAString *centralFieldCmd_, *fringeFieldCmd_;
    
    G4UIcmdWithADoubleAndUnit *centralValueCmd_;
    G4UIcmdWithoutParameter *getCentralValueCmd_;
  };
  
  /** Provides a G4UImessenger interface to control the
      detailedMultipoleImpl.
      
      Provides the commands:
      - g2MIGTRACE/storageField/detailedMultipoleFieldImpl/detailedMapName
      - g2MIGTRACE/storageField/detailedMultipoleFieldImpl/setThetaOffset
      - g2MIGTRACE/storageField/detailedMultipoleFieldImpl/getThetaOffset
  */
  class detailedMultipoleMessenger : public G4UImessenger {
  public:
    detailedMultipoleMessenger(detailedMultipoleStorageImpl*);
    ~detailedMultipoleMessenger();
    
    void SetNewValue(G4UIcommand*,G4String);
  private:
    detailedMultipoleStorageImpl *dmsi_;
    G4UIdirectory *detailedMultipoleImplDir_;
    G4UIcmdWithAString *detailedMapCmd_;
    G4UIcmdWithADoubleAndUnit *setThetaCmd_;
    G4UIcmdWithoutParameter *getThetaCmd_;
  };
  
  /** Provides a G4UImessenger interface to control the
      detailedMultipoleImpl.
      
      Provides the commands:
      - g2MIGTRACE/storageField/fringeFieldImpl/fringeMapName
  */
  class fringeStorageMessenger : public G4UImessenger {
  public:
    fringeStorageMessenger(fringeStorageImpl*);
    ~fringeStorageMessenger();
    void SetNewValue(G4UIcommand*,G4String);
  private:
    fringeStorageImpl *fsi_;
    G4UIdirectory *fringeImplDir_;
    G4UIcmdWithAString *fringeMapCmd_;
  }; // 

} //namespace gm2ringsim

#endif // STORAGERINGFIELD_HH

