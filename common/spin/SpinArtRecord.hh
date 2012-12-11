#ifndef SPINARTRECORD_HH
#define SPINARTRECORD_HH

/** @file SpinArtRecord.rhh

    Provides a Root storable class for spin tracking.

    - Kevin Lynch 2009
*/

//#include "Rtypes.h"

/** A Root storable record class for spin tracking studies.

    Data is provided by the trackerSD instance.  IF the spin tracking
    data is stored, THEN there is a one-to-one correspondance between
    trackerRecord objects in their stored vector<> and SpinArtRecord
    objects in their stored vector<>.
*/

namespace gm2ringsim {
  struct SpinArtRecord {
    
    /** Spin component tangent to the ring. */
    float sthat;
    /** Spin component radial to the ring. */
    float srhat;
    /** Spin component vertical to the ring. */
    float svhat;
    
    SpinArtRecord() :
      sthat(0), srhat(0), svhat(0)
    {}
    
    ~SpinArtRecord() {}
    
#ifndef __GCCXML__
    SpinArtRecord(float that, float rhat, float vhat) :
      sthat(that), srhat(rhat), svhat(vhat)
    { }
    
#endif // GCCXML
}; // struct SpinArtRecord
  
  typedef std::vector<SpinArtRecord> SpinArtRecordCollection;

} //namespace gm2ringsim  


#endif // SPINARTRECORD_HH
  
