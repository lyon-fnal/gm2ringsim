#ifndef STRAWARTRECORD_HH
#define STRAWARTRECORD_HH

/** @file StrawArtRecord.h
 
 Provides the declaration of the Root storable Straw tracking
 record class StrawRecord.
 
 
 @author Kevin Lynch
 @date 2009
 @ARTized from strawRecord.rhh
 @author Brendan Kiburg
 @data Dec 2012
 */
#include <vector>

namespace gm2ringsim {

  struct StrawArtRecord {
    /** Straw offset coordinate from the aperture central axis in
     the "most radial" direction.
     
     The value
     increases outward from the center of the ring. */
    float x_global;
    /** Straw offset coordinate from the aperture central axis in
     the "most vertical" direction.
     
     This is the out-of-plane direction; the value increases
     vertically when the Straw is in the ring symmetry plane. */
    float y_global;
    /** Straw offset along the Straw beam aperture.
     Increases in the downstream direction. */
    float z_global;
    float r_global;
    /** Momentum component parallel to the x_global axis. */
    float px_global;
    /** Momentum component parallel to the y_global axis. */
    float py_global;
    /** Momentum component parallel to the z_global axis. */
    float pz_global;
    // Local transformation coordinates
    float x_local,  y_local,  z_local;
    float px_local, py_local, pz_local;
    
    /** Global time for this tracking record. */
    float time;
    /** Track ID for this hit. */
    int trackID;
    /** Global volume ID for the current tracking volume. */
    int volumeUID;
    
    int strawInRow;
    int rowNumber;
    int stationNumber;
    int strawNumber;

    std::string particle_name;
    int parent_ID;
    
    StrawArtRecord() :
    x_global(0.0), y_global(0.0), z_global(0.0), r_global(0.0),
    px_global(0.0),py_global(0.0),pz_global(0.0),
    x_local(0.0),y_local(0.0),z_local(0.0),
    px_local(0.0),py_local(0.0),pz_local(0.0),
    time(0.0), trackID(0),volumeUID(0), strawNumber(0), particle_name(), parent_ID(0) {}
    virtual ~StrawArtRecord() {}
    
    
    //Root Shouldn't know about this
#ifndef __GCCXML__
    StrawArtRecord(float xglobal, float yglobal, float zglobal, float rglobal, float pxglobal, float pyglobal,
                   float pzglobal, float xLoc, float yLoc, float zLoc, float pxLoc,
                   float pyLoc, float pzLoc, float time_input, int trackID_input,
                   int volumeUID_input, int straw_in_row, int row_number, int station_number, int straw_number,
                   std::string particleName, int parentID ) :
    x_global(xglobal), y_global(yglobal), z_global(zglobal), r_global(rglobal),
    px_global(pxglobal),py_global(pyglobal),pz_global(pzglobal),
    x_local(xLoc),y_local(yLoc),z_local(zLoc),
    px_local(pxLoc),py_local(pyLoc),pz_local(pzLoc),
    time(time_input),
    trackID(trackID_input),
    volumeUID(volumeUID_input),
    strawInRow(straw_in_row),
    rowNumber(row_number),
    stationNumber(station_number),
    strawNumber(straw_number),
    particle_name(particleName),
    parent_ID(parentID)
    { }
#endif //__GCCXML__
    
  }; // end of StrawArtRecord struct
  typedef std::vector<StrawArtRecord> StrawArtRecordCollection;
} //namespace gm2ringsim

#endif
