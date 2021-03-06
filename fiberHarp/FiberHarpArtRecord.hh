#ifndef FiberHarpArtRecord_hh__
#define FiberHarpArtRecord_hh__

/** @file FiberHarpArtRecord.hh

    Implements the fiber harp hit stored in the Art output file

    @author Peter Winter
    @date 2012
*/

#include <vector>

#ifndef __GCCXML__ // Don't let ROOT see things it shouldn't see
#include "CLHEP/Vector/ThreeVector.h"
#endif

namespace gm2ringsim {
  struct FiberHarpArtRecord {
    /** Current value of turn counter for the @b primary that caused
	this hit. */
    int turn;
    
    /** Which harp was hit. */
    int harp;
    
    /** Which fiber on the harp was hit */
    int fiber;
    
    /** The ID of the impinging particle; obtain particle information
	from the trackRecord. */
    int trackID;
    
    /** Global time of the fiber harp hit. */
    double time;
    
    /** Energy deposition in the fiber harp. */
    float deposit;
    
    /** these are I believe the global coordinates */	
    float Gr;
    float Gt;
    float Gv;


    FiberHarpArtRecord() : 
      turn(0), harp(0), fiber(0), trackID(0),
      time(0.), deposit(0.), Gr(0.), Gt(0.), Gv(0.)  
    {}

    virtual ~FiberHarpArtRecord(){};

    // ROOT doesn't need to know the rest
#ifndef __GCCXML__
    FiberHarpArtRecord(int n, int h, int f, int id,
		    double time, double d, double Gr, double Gt, double Gv) :
      turn(n), harp(h), fiber(f), trackID(id),
      time(time), deposit(d), Gr(Gr), Gt(Gt), Gv(Gv)
    {}

#endif
  }; //end of FiberHarpArtRecord struct

  typedef std::vector<FiberHarpArtRecord> FiberHarpArtRecordCollection;
} // end namespace gm2ringsim

#endif
