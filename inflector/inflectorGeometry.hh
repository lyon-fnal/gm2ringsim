#ifndef INFLECTORGEOMETRY_HH
#define INFLECTORGEOMETRY_HH

/** @file inflectorGeometry.hh

    Declares a Meyer's singleton, inflectorGeometry, to hold globally
    consistent values of various inflector parameters. 

    @author Kevin Lynch
    @date 2009
*/


#include "Geant4/G4Types.hh"

/** A Meyer's singleton that stores the inflector geometry that is
    needed by multiple classes: inflectorConstruction, inflectorField,
    inflectorMessenger, g2SingleParticleSource.

    Angles here use the standard Geant4, right handed system, not the
    g2MIGTRACE downstream(clockwise) system.

*/
class inflectorGeometry {
public:
  static inflectorGeometry& getInstance();

  /** Gets the azimuthal position from g2MIGTRACE angle=0.  positive
      moves upstream. */
  G4double delta() const { return delta_; }
  /** Sets the azimuthal position from g2MIGTRACE angle=0.  positive
      moves upstream. */
  G4double delta(G4double d) { G4double t = delta_; delta_ = d; return t; }

  /** Gets the inflector swing angle from tangent ... positive rotates
      beam direction outward. */
  G4double gamma() const { return gamma_; }
  /** Sets the inflector swing angle from tangent ... positive rotates
      beam direction outward. */
  G4double gamma(G4double d) { G4double t = gamma_; gamma_ = d; return t; }

  /** Gets the inflector tilt angle from plane of ring ... moves
      rotates the beam upward out of the plane. */
  G4double zeta() const { return zeta_; }
  /** Sets the inflector tilt angle from plane of ring ... moves
      rotates the beam upward out of the plane. */
  G4double zeta(G4double d) { G4double t = zeta_; zeta_ = d; return t; }

  /** Alias for gamma() */
  G4double swing_angle() const { return gamma(); }
  /** Alias for gamma() */
  G4double swing_angle(G4double d) { return gamma(d); }

  /** Alias for zeta() */
  G4double tilt_angle() const { return zeta(); }
  /** Alias for zeta() */
  G4double tilt_angle(G4double d) { return zeta(d); }

  /** Gets the radial distance from muon orbit to inflector aperture
      center. */
  G4double aperture_off() const { return apertureDist_; }
  /** Sets the radial distance from muon orbit to inflector aperture
      center. */
  G4double aperture_off(G4double d){ 
    G4double t = apertureDist_; 
    apertureDist_ = d; 
    return t;
  }

  /** Gets the projection of the launch angle onto the symmetry plane
      ... same geometrical meaning as gamma(). */
  G4double inplane_angle() const { return ipLaunch_; }
  /** Sets the projection of the launch angle onto the symmetry plane
      ... same geometrical meaning as gamma(). */
  G4double inplane_angle(G4double ipLaunch) 
  { G4double t = ipLaunch_; ipLaunch_ = ipLaunch; return t; }

  /** Gets the projection of the launch angle onto the vertical axis
      ... same geometrical meaning as zeta(). */
  G4double outplane_angle() const { return oopLaunch_; }
  /** Sets the projection of the launch angle onto the vertical axis
      ... same geometrical meaning as zeta(). */
  G4double outplane_angle(G4double oopLaunch) 
  { G4double t = oopLaunch_; oopLaunch_ = oopLaunch; return t; }

  /** Gets the inflector mandrel half length; not modifiable. */
  G4double mandrel_half_length() const { return mL_; }

  /** Gets the inflector mandrel length; not modifiable. */
  G4double mandrel_length() const { return 2.*mL_; }

  /** Gets the full inflector half length; not modifiable. */
  G4double half_length() const { return iL_; }

  /** Gets the full inflector length; not modifiable. */
  G4double length() const { return 2.*iL_; }

  /** Gets the inflector window thickness; not modifiable. */
  G4double window_thickness() const { return windowThickness_; }

  /** Gets the inflector conductor thickness; not modifiable. */
  G4double conductor_thickness() const { return conductorThickness_; }

  /** Gets the inflector cryostat thickness; not modifiable. */
  G4double cryostat_thickness() const { return cryostatThickness_; }

public:
  inflectorGeometry();
  ~inflectorGeometry(){}

  // don't implement these!
  inflectorGeometry(inflectorGeometry const&);
  inflectorGeometry& operator=(inflectorGeometry const&);

private:
  G4double delta_, gamma_, zeta_;
  G4double apertureDist_, ipLaunch_, oopLaunch_;

  // shared, immutable constants

  // thicknesses
  G4double const windowThickness_, conductorThickness_, cryostatThickness_;
  // mandrel and full inflector half lengths
  G4double const mL_, iL_;
};

#endif // INFLECTORGEOMETRY_HH
