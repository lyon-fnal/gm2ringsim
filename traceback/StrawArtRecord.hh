#ifndef STRAWARTRECORD_HH
#define STRAWARTRECORD_HH

/** @file StrawArtRecord.h
 
 Provides the declaration of the Root storable straw tracking
 record class strawRecord.
 
 
 @author Kevin Lynch
 @date 2009
 @ARTized from strawRecord.rhh
 @author Leah Welty-Rieger
 @data Dec 2012
 */
#include <vector>

namespace gm2ringsim {
  
  struct StrawArtRecord {
   /* The value increases outward from the center of the ring. */
    float x_inf;
    /** This is the out-of-plane direction; the value increases
     vertically when in the ring symmetry plane. */
    float y_inf;
    /** Increases in the downstream direction. */
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
    
    
    StrawArtRecord() :
    x_inf(0.0), y_inf(0.0), z_inf(0.0),
    px_inf(0.0),py_inf(0.0),pz_inf(0.0),
    x_loc(0.0),y_loc(0.0),z_loc(0.0),
    px_loc(0.0),py_loc(0.0),pz_loc(0.0),
    time(0.0), trackID(0),volumeUID(0) {}
    virtual ~StrawArtRecord() {}
    
    
    //Root Shouldn't know about this
#ifndef __GCCXML__
    StrawArtRecord(float xInf, float yInf, float zInf, float pxInf, float pyInf,
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
    
  }; // end of StrawArtRecord struct
  typedef std::vector<StrawArtRecord> StrawArtRecordCollection;
} //namespace gm2ringsim

#endif
