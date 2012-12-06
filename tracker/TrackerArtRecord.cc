/** @file trackerRecord.rcc

    Provides very minimal implementation for the recordable tracking
    data class.  This file needed by the build system.

    - Kevin Lynch 2009
*/

#include "trackerRecord.rhh"

trackerRecord::trackerRecord() :
rhat(0), vhat(0), theta(0), time(0), p(0), prhat(0), pvhat(0),
  turn(-1), volumeUID(0), trackID(-1) {}

