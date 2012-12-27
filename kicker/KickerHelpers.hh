#ifndef G2MIGTRACE_KICKERHELPERS_HH
#define G2MIGTRACE_KICKERHELPERS_HH

/** @file kickerHelpers.hh

    Provides a number of inline helper functions in namespace
    kicker_helpers to calculate LCR kicker pulse shape and timing
    information.

    Since they're inline, they can be included in all translation
    units. 

    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
*/

/** Holds a number of inline helper functions for LCR kicker pulse
    information. */
namespace gm2ringsim {
  namespace kicker_helpers {

  inline G4double getX(G4double L, G4double R){
    return R/(2.*L);
  }
  
  inline G4double getOmega(G4double L, G4double C, G4double R){
    G4double x = getX(L,R);
    return std::sqrt(1/(L*C) - x*x);
  }
  
  inline G4double getProperOffsetTime(G4double L, G4double C, G4double R){
    // The "correct"  offset time can be calculated directly from
    // L,C,R: Find the peak of the current profile as the first
    // local extremum.  Using the first derivative, this time can be
    // found _exactly_.  The offset is then the difference between the
    // muon arrival time and the unshifted peak time.
    G4double x = getX(L,R);
    G4double w = getOmega(L,C,R);
    G4double peaktime = std::atan2(w,x)/w;


    // Calculate time for trip around the storage orbit
    G4double cyclotronPeriod = (2*pi*7.112*m) / c_light;

    // Total angular distance from inflector aperture (-6 degrees) to
    // center of kicker plate 2 (+82.91)
    //   G4double apertureToCenterK2 = 88.91; // degrees
    // from BeamChbrPln.dwg
    G4double apertureToCenterK2 = 87.87;

    // Get fraction of azimuth (relative to full 360 orbit) and
    // multiply by cyclotron period to get time of flight from
    // aperture to center kicker plate 2
    G4double angularTime = (apertureToCenterK2/360.)*cyclotronPeriod;

    // The muon gaussian peak (in time) is centered in global time at
    // 100 ns by default so adding angularTime to this number gives
    // the time (in Geant4 global time) that the center of the muon
    // bunch is sitting in the middle of kicker plate 2
    G4double muArrival = 100.*ns + angularTime;

    // 18 OCT 08 - WTF??  This make-a-no-sense.  I have reimplemented
    // this above in a way that should be correct.  Leave this here
    // for now....
    //G4double muarrival = 100.*ns + ((96.+81.82)/2+6)/360.*150.*ns;
    
    return muArrival-peaktime;
  }
  
  } //namespace kicker_helpers
} //namespace gm2ringsim

//using namespace gm2ringsim::kicker_helpers;

#endif // G2MIGTRACE_KICKERHELPERS_HH
