#ifndef INFLECTORRECORD_HH
#define INFLECTORRECORD_HH

/** @file InflectorRecord.h

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

  struct InflectorRecord {
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


    InflectorRecord(); // no intialization list
    ~InflectorRecord();

    
#ifndef __GCCXML__
    InflectorRecord(int);


#endif

  };
  typedef std::vector<InflectorRecord> InflectorRecordCollection;
} //namespace gm2ringsim

#endif
