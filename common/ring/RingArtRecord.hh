#ifndef RINGARTRECORD_HH
#define RINGARTRECORD_HH

/** @file RingArtRecord.hh

    Declares a Root storable class to record energy loss data in the
    ring. 

    - @author Kevin Lynch
    - @date 2009
    - ARTized based on old ringRecord.rhh
    - @author Brendan Kiburg
    - @date Dec 2012
*/

//#include "Rtypes.h"

namespace gm2ringsim{
  /** Root storable type that holds energy loss data for each Geant
      Hit. */
  struct RingArtRecord {
    
    float x_loc, y_loc, z_loc;
    float px_loc, py_loc, pz_loc, prhat_loc, pvhat_loc;
    
    /** Horizontal distance from the central orbit, mm. */
    float rhat;
    /** Vertical distance from the central orbit, mm. */
    float vhat; 
    /** Global angle downstream from the arc0/arc11 boundary (not the
	inflection point!) */
    float theta;
    /** Global event time, ns. */
    float time;
    
    /** Total momentum at hit, MeV */
    float p;
    /** Momentum fraction along unit vectors in rhat direction. */
    float prhat;
    /** Momentum fraction along unit vectors in vhat direction. */
    float pvhat; 
    
    /** Current turn of primary. */
    int turn;
    /** Current physical volume UID; see g2UniqueObjectManager */
    int volumeUID;
    /** This particle's trackID; see trackRecord. */
    int trackID; 
    
    // total momentum change, and components along rhat and vhat directions
    /** Total momentum change during scattering. */
    float deltaP;
    /** Momentum lost/gained along the rhat direction. */
    float deltaPrhat;
    /** Momentum lost/gained along the vhat direction. */
    float deltaPvhat;
    /** Total energy loss. */
    float deltaE;
    
    ~RingArtRecord();
    RingArtRecord() : 
      x_loc(0), y_loc(0), z_loc(0),
      px_loc(0),py_loc(0),pz_loc(0),
      prhat_loc(0), pvhat_loc(0),      
      rhat(0), vhat(0), theta(0), time(0),
      p(0), prhat(0), pvhat(0), 
      turn(-1), volumeUID(0), trackID(-1),
      deltaP(0), deltaPrhat(0), deltaPvhat(0), deltaE(0) 
    {}
    
#ifndef __GCCXML__
    RingArtRecord(float xloc,float yloc,float zloc,
		  float pxloc,float pyloc, float pzloc,
		  float prhat_loc_in, float pvhat_loc_in,
		  float rin, float vin, float thetain,float timein,
		  float pin,float prhatin,float pvhatin,
		  int turnin,int volin,int trackin,
		  float dP, float dPrhat, float dPVhat, float dE
		  ) : 
      x_loc(xloc), y_loc(yloc), z_loc(zloc),
      px_loc(pxloc),py_loc(pyloc), pz_loc(pzloc),
      prhat_loc(prhat_loc_in), pvhat_loc(pvhat_loc_in),
      rhat(rin), vhat(vin), theta(thetain), time(timein),
      p(pin),prhat(prhatin),pvhat(pvhatin),
      turn(turnin), volumeUID(volin), trackID(trackin),
      deltaP(dP), deltaPrhat(dPrhat), deltaPvhat(dPVhat), deltaE(dE) 
    {}
    
#endif //__GCCXML__
    
  }; //struct RingArtRecord

  typedef std::vector<RingArtRecord> RingArtRecordCollection;

}//namespace gm2ringsim

#endif // RINGRECORD_HH
  
