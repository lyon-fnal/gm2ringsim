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

#include "gm2ringsim/external_libs/interpolation/interpolators1D.hh"

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
    KickField(KickModifier* mod) : mod_(mod) {}
    virtual void KickFieldValue(G4double const Point[4],
				G4double Bfield[3]) const =0;
    void GetFieldValue(G4double const Point[4],
		       G4double Bfield[3]) const;
    
  protected:
    KickModifier* mod_;
  };
  
  /** Provides an LCR pulse model for the kicker vertical field. */
  class LCRKickField : public KickField {
    
  public: 
    LCRKickField(G4double kickerHV,
		 G4double kickerOffsetTime, G4double circuitC,
		 G4double circuitL, G4double circuitR,
		 KickModifier* mod);
    
    void KickFieldValue(G4double const Point[4],
			G4double Bfield[3]) const;
    
  private:
    G4double const HV;
    G4double const offsetTime;
    G4double const C;
    G4double const L;
    G4double const R;
    
    G4double const omega, X, i0;
  };
  
  
  /** Provides an idealized square kick that acts only on the first
      turn. */
  class SquareKickField : public KickField {
    
  public: 
    SquareKickField(G4double kickSquareField, KickModifier* mod);
    
    void KickFieldValue(G4double const Point[4],
			G4double Kfield[3]) const;
    
  private:
    G4double const squareField;
  };
  
  /** Provides no kick whatever.  Useful when studying late time, static
      behavior. */ 
  class NoKickField : public KickField {
    
  public:
    NoKickField(KickModifier* mod);
    
    void KickFieldValue(G4double const Point[4],
			G4double Kfield[3]) const;
  };
  
} //namespace gm2ringsim
#endif
