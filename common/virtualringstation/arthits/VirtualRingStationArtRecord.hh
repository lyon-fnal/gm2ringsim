#ifndef VIRTUALRINGSTATIONARTRECORD_RHH
#define VIRTUALRINGSTATIONARTRECORD_RHH

/** @file VirtualRingStationArtRecord.hh

    Provides a Root storable class to record in-ring beam tracking
    data. 
    @author  Kevin Lynch
    @date 2009
    @ARTized from virtualringstationRecord.rhh,rcc
    @author Brendan Kiburg
    @data Dec 2012 ARTified
    
*/

//#include "Rtypes.h"

namespace gm2ringsim{
  
  /** Provides a Root storable class to record in-ring beam tracking
      data.  Uses the data recorded by the virtualringstationSD class. */
  
  struct VirtualRingStationArtRecord {
    /** Horizontal offset from the central orbit, mm. */
    float rhat;
    /** Vertical offset from the central orbit, mm. */
    float vhat; 
    /** Global angular offset downstream of the ar0/arc11 boundary (NOT
	the inflection point!). */
    float theta;
    /** Global event time, ns. */
    float time;

    int pdgid;
    
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

    /** Spin component tangent to the ring. */
    float polx;
    /** Spin component radial to the ring. */
    float poly;
    /** Spin component vertical to the ring. */
    float polz;
    
    
    VirtualRingStationArtRecord() :
      rhat(0), vhat(0), theta(0), time(0), pdgid(0), p(0), prhat(0), pvhat(0),
      turn(-1), volumeUID(0), trackID(-1), polx(0), poly(0), polz(0) {}

    ~VirtualRingStationArtRecord(){};
    
#ifndef __GCCXML__
    VirtualRingStationArtRecord(float rin, float vin, float thetain,
				float tin, int pdgidin, float pin, float prin, float pvin,
				float turnin,float volin,int trackin,
				float polxin, float polyin, float polzin) :
      rhat(rin), vhat(vin), theta(thetain),
      time(tin), pdgid(pdgidin), p(pin), prhat(prin), pvhat(pvin),
      turn(turnin), volumeUID(volin),trackID(trackin),
      polx(polxin), poly(polyin), polz(polzin)
    {}
    
#endif //__GCCXML__
    
  }; //struct VirtualRingStationArtRecord
  
  typedef std::vector<VirtualRingStationArtRecord> VirtualRingStationArtRecordCollection;
  
} //namespace gm2ringsim
#endif // VIRTUALRINGSTATIONARTRECORD_RHH
