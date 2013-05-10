#ifndef INFLECTORARTRECORD_HH
#define INFLECTORARTRECORD_HH

/** @file InflectorArtRecord.h

    Provides the declaration of the Root storable inflector tracking
    record class inflectorRecord.

    
    @author Kevin Lynch
    @date 2009
    @ARTized from inflectorRecord.rhh
    @author Brendan Kiburg
    @data Dec 2012
*/
#include <vector>

namespace gm2ringsim {

  struct InflectorArtRecord {
    /** Inflector offset coordinate from the aperture central axis in
	the "most radial" direction.
	
	This is the in-plane direction normal to the inflector long axis
	when then inflector is in the ring symmetry plane.  The value
	increases outward from the center of the ring. */
    float x_inf;
    /** Inflector offset coordinate from the aperture central axis in
      the "most vertical" direction.
      
      This is the out-of-plane direction; the value increases
      vertically when the inflector is in the ring symmetry plane. */ 
    float y_inf;
    /** Inflector offset along the inflector beam aperture.
	Increases in the downstream direction. */
    float z_inf;
    /** Momentum component parallel to the x_inf axis. */
    float px_inf;
    /** Momentum component parallel to the y_inf axis. */
    float py_inf;
    /** Momentum component parallel to the z_inf axis. */
    float pz_inf;
    // Local transformation coordinates
    float x_ring,  y_ring,  z_ring;
    float px_ring, py_ring, pz_ring;

    float rhat_ring, vhat_ring, prhat_ring, pvhat_ring;

    /** Global time for this tracking record. */
    float time;
    /** Track ID for this hit. */
    int trackID; 
    /** Global volume ID for the current tracking volume. */
    int volumeUID;

    InflectorArtRecord() :
      x_inf(0.0), y_inf(0.0), z_inf(0.0),
      px_inf(0.0),py_inf(0.0),pz_inf(0.0),
      x_ring(0.0),y_ring(0.0),z_ring(0.0),
      px_ring(0.0),py_ring(0.0),pz_ring(0.0),
      rhat_ring(0.0), vhat_ring(0.0), 
      prhat_ring(0.0), pvhat_ring(0.0), 
      time(0.0), trackID(0),volumeUID(0) {}
    virtual ~InflectorArtRecord() {}


    //Root Shouldn't know about this
#ifndef __GCCXML__
    InflectorArtRecord(float xInf, float yInf, float zInf, float pxInf, float pyInf,
		       float pzInf, float xRing, float yRing, float zRing, float pxRing,
		       float pyRing, float pzRing, float time_input, int trackID_input,
		       float rhatRing, float vhatRing, float prhatRing, float pvhatRing,
		       int volumeUID_input ) :
      x_inf(xInf), y_inf(yInf), z_inf(zInf),
      px_inf(pxInf),py_inf(pyInf),pz_inf(pzInf),
      x_ring(xRing),y_ring(yRing),z_ring(zRing),
      px_ring(pxRing),py_ring(pyRing),pz_ring(pzRing),
      rhat_ring(rhatRing), vhat_ring(vhatRing), 
      prhat_ring(prhatRing), pvhat_ring(pvhatRing), 
      time(time_input),
      trackID(trackID_input),
      volumeUID(volumeUID_input) 
    { }
#endif //__GCCXML__

  }; // end of InflectorArtRecord struct
  typedef std::vector<InflectorArtRecord> InflectorArtRecordCollection;
} //namespace gm2ringsim

#endif
