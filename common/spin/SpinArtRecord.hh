#ifndef SPINRECORD_RHH
#define SPINRECORD_RHH

/** @file spinRecord.rhh

    Provides a Root storable class for spin tracking.

    - Kevin Lynch 2009
*/

#include "Rtypes.h"

/** A Root storable record class for spin tracking studies.

    Data is provided by the trackerSD instance.  IF the spin tracking
    data is stored, THEN there is a one-to-one correspondance between
    trackerRecord objects in their stored vector<> and spinRecord
    objects in their stored vector<>.
*/
struct spinRecord {

  /** Spin component tangent to the ring. */
  Float_t sthat;
  /** Spin component radial to the ring. */
  Float_t srhat;
  /** Spin component vertical to the ring. */
  Float_t svhat;

  spinRecord();

};

#endif // SPINRECORD_RHH
