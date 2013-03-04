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
    float x_loc,  y_loc,  z_loc;
    float px_loc, py_loc, pz_loc;

    /** Global time for this tracking record. */
    float time;
    /** Track ID for this hit. */
    int trackID; 
    /** Global volume ID for the current tracking volume. */
    int volumeUID;


    InflectorArtRecord() :
      x_inf(0.0), y_inf(0.0), z_inf(0.0),
      px_inf(0.0),py_inf(0.0),pz_inf(0.0),
      x_loc(0.0),y_loc(0.0),z_loc(0.0),
      px_loc(0.0),py_loc(0.0),pz_loc(0.0),
      time(0.0), trackID(0),volumeUID(0) {}
    virtual ~InflectorArtRecord() {}


    //Root Shouldn't know about this
#ifndef __GCCXML__
    InflectorArtRecord(float xInf, float yInf, float zInf, float pxInf, float pyInf,
		    float pzInf, float xLoc, float yLoc, float zLoc, float pxLoc,
		    float pyLoc, float pzLoc, float time_input, int trackID_input,
		    int volumeUID_input ) :
      x_inf(xInf), y_inf(yInf), z_inf(zInf),
      px_inf(pxInf),py_inf(pyInf),pz_inf(pzInf),
      x_loc(xLoc),y_loc(yLoc),z_loc(zLoc),
      px_loc(pxLoc),py_loc(pyLoc),pz_loc(pzLoc),
      time(time_input),
      trackID(trackID_input),
      volumeUID(volumeUID_input) 
    { }
#endif //__GCCXML__

  }; // end of InflectorArtRecord struct
  typedef std::vector<InflectorArtRecord> InflectorArtRecordCollection;
} //namespace gm2ringsim

#endif
