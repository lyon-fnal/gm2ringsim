#ifndef FiberHarpRecord_hh__
#define FiberHarpRecord_hh__

#include <vector>

#ifndef __GCCXML__ // Don't let ROOT see things it shouldn't see
#include "CLHEP/Vector/ThreeVector.h"
#endif

namespace gm2ringsim {
  struct FiberHarpRecord {
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


    FiberHarpRecord() : 
      turn(0), harp(0), fiber(0), trackID(0),
      time(0.), deposit(0.), Gr(0.), Gt(0.), Gv(0.)  
    {}

    virtual ~FiberHarpRecord(){};

    // ROOT doesn't need to know the rest
#ifndef __GCCXML__
    FiberHarpRecord(int n, int h, int f, int id,
		    double time, double d, double Gr, double Gt, double Gv) :
      turn(n), harp(h), fiber(f), trackID(id),
      time(time), deposit(d), Gr(Gr), Gt(Gt), Gv(Gv)
    {}

#endif
  }; //end of FiberHarpRecord struct

  typedef std::vector<FiberHarpRecord> FiberHarpRecordCollection;
} // end namespace gm2ringsim

#endif
