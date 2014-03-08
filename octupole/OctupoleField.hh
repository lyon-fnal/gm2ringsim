#ifndef G2MIGTRACE_OCTUPOLEFIELD_HH
#define G2MIGTRACE_OCTUPOLEFIELD_HH

/** @file octupoleField.hh
    octupoleField.hh contains declarations for the fields created by
    the PNPI pulsed magnetic octupole CBO mixing devices.

    @author Kevin Lynch,
    @date 2009
    ARTized by 
    @author Brendan Kiburg
    @date 2013

 */

#include "Geant4/G4MagneticField.hh"
#include "gm2ringsim/octupole/OctGeometry.hh"

#include <utility>

namespace gm2ringsim {
  
  /** Magnetic field inside the octupole CBO mixing device.
      
      A G4Field class that defines the time dependent magnetic field
      inside the PNPI pulsed octupole CBO mixing device.
  */
  class OctupoleField : public G4MagneticField {
  public:
    
    /** Should only be called by octupoleConstruction members */
    OctupoleField(int Charge, int StorageFieldType);
    
    //Always include a virtual destructor
    virtual ~OctupoleField(){}

    /** Provides the field value given the position 4-vector.
	
	Also calls the storageFieldController to get the storage ring
	field.
    */ 
    // Must be capitalized because it is overriding 
    // a virtual G4MagneticField function
    void GetFieldValue( double const Point[4],
			double Bfield[3] ) const;
    
    double period(double p) { std::swap(period_, p); return p; }
    double period() const { return period_; }
    
    double offset(double o) { std::swap(offset_, o); return o; }
    double offset() const { return offset_; }
    
    double gradient(double g) { std::swap(gradient_, g); return g; }
    double gradient() const { return gradient_; }
    
  private:
    OctGeometry octGeom_;
    double period_, offset_, gradient_;
    int Charge_;
    int StorageFieldType_;
  }; // class OctupoleField
  
} // namespace gm2ringsim

#endif // G2MIGTRACE_OCTUPOLEFIELD_HH
