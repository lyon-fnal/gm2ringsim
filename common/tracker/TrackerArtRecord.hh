#ifndef TRACKERARTRECORD_RHH
#define TRACKERARTRECORD_RHH

/** @file TrackerArtRecord.hh

    Provides a Root storable class to record in-ring beam tracking
    data. 
    @author  Kevin Lynch
    @date 2009
    @ARTized from trackerRecord.rhh,rcc
    @author Brendan Kiburg
    @data Dec 2012 ARTified
    
*/

//#include "Rtypes.h"

namespace gm2ringsim{
  
  /** Provides a Root storable class to record in-ring beam tracking
      data.  Uses the data recorded by the trackerSD class. */
  
  struct TrackerArtRecord {
    /** Horizontal offset from the central orbit, mm. */
    float rhat;
    /** Vertical offset from the central orbit, mm. */
    float vhat; 
    /** Global angular offset downstream of the ar0/arc11 boundary (NOT
	the inflection point!). */
    float theta;
    /** Global event time, ns. */
    float time;
    
    /** Total momentum, MeV. */
    float p; 
    /** Fraction of momentum along the rhat direction. */
    float prhat;
    /** Fraction of momentum along the vhat direction. */
    float pvhat; 
    
    /** Current orbit number since injection. */
    int turn;
    /** Unique volume ID of the current tracking volume; use with the
	g2UniqueObjectManager for volume identification. */
    int volumeUID;
    /** Current track ID; used with the stored trackRecord for particle
	identification. */
    int trackID; 
    
    TrackerArtRecord() :
      rhat(0), vhat(0), theta(0), time(0), p(0), prhat(0), pvhat(0),
      turn(-1), volumeUID(0), trackID(-1) {}

    ~TrackerArtRecord(){};
    
#ifndef __GCCXML__
    TrackerArtRecord(float rin, float vin, float thetain,
		     float tin, float pin, float prin, float pvin,
		     float turnin,float volin,int trackin) :
      rhat(rin), vhat(vin), theta(thetain),
      time(tin), p(pin), prhat(prin), pvhat(pvin),
      turn(turnin), volumeUID(volin),trackID(trackin)
    {}
    
#endif //__GCCXML__
    
  }; //struct TrackerArtRecord
  
  typedef std::vector<TrackerArtRecord> TrackerArtRecordCollection;
  
} //namespace gm2ringsim
#endif // TRACKERARTRECORD_RHH
