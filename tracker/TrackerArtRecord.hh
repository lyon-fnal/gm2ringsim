#ifndef TRACKERRECORD_RHH
#define TRACKERRECORD_RHH

/** @file trackerRecord.rhh

    Provides a Root storable class to record in-ring beam tracking
    data. 

    - Kevin Lynch 2009
*/

#include "Rtypes.h"

/** Provides a Root storable class to record in-ring beam tracking
    data.  Uses the data recorded by the trackerSD class. */
struct trackerRecord {

  /** Horizontal offset from the central orbit, mm. */
  Float_t rhat;
  /** Vertical offset from the central orbit, mm. */
  Float_t vhat; 
  /** Global angular offset downstream of the ar0/arc11 boundary (NOT
      the inflection point!). */
  Float_t theta;
  /** Global event time, ns. */
  Float_t time;

  /** Total momentum, MeV. */
  Float_t p; 
  /** Fraction of momentum along the rhat direction. */
  Float_t prhat;
  /** Fraction of momentum along the vhat direction. */
  Float_t pvhat; 

  /** Current orbit number since injection. */
  Int_t turn;
  /** Unique volume ID of the current tracking volume; use with the
      g2UniqueObjectManager for volume identification. */
  Int_t volumeUID;
  /** Current track ID; used with the stored trackRecord for particle
      identification. */
  Int_t trackID; 

  trackerRecord();

};

#endif // TRACKERRECORD_RHH
