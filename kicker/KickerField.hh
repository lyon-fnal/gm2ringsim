#ifndef kickerField_hh
#define kickerField_hh

/** @file kickerField.hh

    Provides declarations for a number of classes that provide various
    kicker field implementations and modifications, including abstract
    base classes KickModifier and KickField, derived modifiers
    NoModifier and MorsetModifier, and derived fields NoKickField,
    LCRKickField and SquareKickField. 

    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
    
    Artified by
    @author Brendan Kiburg
    @date Dec 2012
*/

#include "Geant4/G4MagneticField.hh"

#include "Geant4/globals.hh"

#include "gm2ringsim/external_libs/libkdtree++/kdtree++/kdtree.hpp"
#include "gm2ringsim/fields/field_point.hh"

#include "Geant4/G4ThreeVector.hh"

#include "gm2ringsim/external_libs/interpolation/interpolators1D.hh"

#include "TH2F.h"

namespace gm2ringsim {
  /** Abstract base class for field modifier classes. */
  class KickModifier {
  public:
    virtual void ModifyKickField(G4double const Point[4],
				 G4double Bfield[3]) const = 0;
  };
  
  /** Derived field modifier that does nothing. */
  class NoModifier : public KickModifier {
  public:
    void ModifyKickField(G4double const /*Point*/[4],
			 G4double /*Bfield*/[3]) const {}
  };
  
  /** Provides a vertically graded field strength modifier calculated by
      Bill Morse. */
  class MorseModifier : public KickModifier {
  public:
    MorseModifier(G4String filename);
    ~MorseModifier();
    void ModifyKickField(G4double const Point[4],
			 G4double Bfield[3]) const;
  private:
    G4String fname_;
    void parse_file();
    
    uniform_spline_interpolator<double> *interp;
  };
  
  /** Abstract base class for the field itself; "owns" the field
      modifier. 
      
      @bug There's something wrong with KM* ownership here that I need
      to fix .... worry about it later, and be careful in the interim. 
  */
  class KickField : public G4MagneticField {
  public:
    KickField(KickModifier* mod, int Charge, int StorageFieldType) : mod_(mod), Charge_(Charge), StorageFieldType_(StorageFieldType) {}
    virtual void KickFieldValue(G4double const Point[4],
				G4double Bfield[3]) const =0;
    void GetFieldValue(G4double const Point[4],
		       G4double Bfield[3]) const;
    
  protected:
    KickModifier* mod_;
    int Charge_;
    int StorageFieldType_;
  };

  
  
  /** Provides an LCR pulse model for the kicker vertical field. */
  class CornellKickField : public KickField {
  public: 
    CornellKickField(G4double kFieldMag,
		     KickModifier* mod,
		     int Charge, int StorageFieldType);
    
    void KickFieldValue(G4double const Point[4],
			G4double Bfield[3]) const;
    
  private:
    G4String fname_;

    double kFieldMag_;
    int Charge_;
    int StorageFieldType_;

    TH2F *hBx, *hBy;
    
    typedef field_point<G4ThreeVector,G4ThreeVector> data_point;
    typedef std::vector<data_point> data_vec;
    typedef KDTree::KDTree<3, data_point> data_tree;
    
    // the coordinates of the storage field data in the magnet_tree_ are
    // the global coordinates of the simulation.
    data_tree kicker_tree_;

    data_vec parse_file();
        
    G4ThreeVector shepard_interpolate(data_point const& dp, data_vec const& dv) const;

    inline G4double distance(G4ThreeVector const& left,G4ThreeVector const& right){
      return (left-right).mag();
    }

  };
  
  /** Provides an LCR pulse model for the kicker vertical field. */
  class LCRKickField : public KickField {
    
  public: 
    LCRKickField(G4double kickerHV,
		 G4double kickerOffsetTime, G4double circuitC,
		 G4double circuitL, G4double circuitR,
		 KickModifier* mod,
		 int Charge, int StorageFieldType);
    
    void KickFieldValue(G4double const Point[4],
			G4double Bfield[3]) const;
    
  private:
    G4double HV;
    G4double const offsetTime;
    G4double const C;
    G4double const L;
    G4double const R;
    
    G4double const omega, X;
    G4double i0;

    int Charge_;
    int StorageFieldType_;
  };
  
  
  /** Provides an idealized square kick that acts only on the first
      turn. */
  class SquareKickField : public KickField {
    
  public: 
    SquareKickField(G4double kickSquareField, KickModifier* mod, int Charge, int StorageFieldType);
    
    void KickFieldValue(G4double const Point[4],
			G4double Kfield[3]) const;
    
  private:
    G4double squareField;
    int Charge_;
    int StorageFieldType_;
  };
  
  /** Provides no kick whatever.  Useful when studying late time, static
      behavior. */ 
  class NoKickField : public KickField {
    
  public:
    NoKickField(KickModifier* mod, int Charge, int StorageFieldType);
    
    void KickFieldValue(G4double const Point[4],
			G4double Kfield[3]) const;

  private:
    int Charge_;
    int StorageFieldType_;
  };
  
} //namespace gm2ringsim
#endif
