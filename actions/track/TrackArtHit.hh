// TrackArtHit provides an artized hit class which can safely be added to
// art events. Its members are all public and are modified in its 
// initialization in TrackDetector.

// Include guard
#ifndef TRACK_ART_HIT_HH
#define TRACK_ART_HIT_HH

// Includes
#include <vector>

// Everything goes in the Art G4 namespace
namespace gm2ringsim {
  // Begin struct definition
  struct TrackArtHit {
  public: 
    // Default constructor sets everything to zero
    TrackArtHit() :
      trackType(),
      trackID(0),
      parentTrackID(0),
      turn(0),
      volumeUID(0),
      rhat(0),
      vhat(0),
      theta(0),
      time(0),
      t0(0),
      p(0),
      e(0),
      prhat(0),
      pvhat(0)
    {}
    
    // Members of this struct are all public, and are modified in the 
    // creation of a TrackArtHit by TrackDetector.

    // Usually the particle's name
    std::string trackType;

    // GEANT track ID of the particle
    int trackID;

    // GEANT track ID of the particle's parent (itself for the primary)
    int parentTrackID;

    // Orbit number at particle birth
    int turn;

    // The physical volume UID the track was born in
    int volumeUID;

    // Radial offset from idealized storage orbit at particle birth
    double rhat;

    // Vertical offset from idealized storage orbit at particle birth
    double vhat;

    // Azimuthal angle downstream from global zero at particle's birth.
    // Global zero is a few degrees downstream of the inflection point.
    double theta;

    // Global time at the particle's birth
    double time;

    double t0;

    // Total momentum of the particle at birth
    double p;

    double e;

    // Fraction of the particle's momentum oriented radially at birth
    double prhat;

    // Fraction of the particle's momentum oriented vertically at birth
    double pvhat;
  };

  // Define a collection of these hits
  typedef std::vector<TrackArtHit> TrackArtHitCollection;

}

#endif
