#ifndef TRACKERHITARTRECORD_HH
#define TRACKERHITARTRECORD_HH

// Provides the declaration of the ROOT-storable class used as a starting point
// for the straw tracker analysis.
//
// @author Leah Welty-Rieger
// @author Tasha Arvanitis
// @date July 2013
//
// NOTE: THIS FILE WAS COPY-PASTED FROM THE TRACKING ANALYSIS PROJECT, AS AN
// INTERIM SOLUTION UNTIL g-2 HAS A DATA PRODUCTS PACKAGE! IF YOU CHANGE ONE
// FILE, YOU BETTER CHANGE THE OTHER!!!

#include <vector>
#include "WireID.hh"

namespace gm2strawtracking {
    struct TrackerHitArtRecord {
        // Specification for the wire: includes station, view, layer,
        // wire #
        WireID id;

        // Time at which the hit was registered (includes both the time of the
        // hit and drift) (units: nanoseconds)
        double time;

        // Width of the pulse from the sensors (in ns)
        double width;

        // Index of the hit (this is the nth time that this straw was hit in the
        // fill)
        int index;

        // Was there some sort of error in reading this out?
        bool errorFlag;

        // Default constructor
        TrackerHitArtRecord() :
            id(),
            time(0.),
            width(0.),
            index(0),
            errorFlag(false)
        {}

        // A nicer constructor: ROOT doesn't see it, but other classes can.
#ifndef __GCCXML__
        TrackerHitArtRecord(WireID inid, double intime, double inwidth, 
                int inindex, bool inerrorFlag) :
            id(inid),
            time(intime),
            width(inwidth),
            index(inindex),
            errorFlag(inerrorFlag)
        {}
#endif // __GCCXML__

    }; // TrackerHitArtRecord struct definition

    typedef std::vector<TrackerHitArtRecord> TrackerHitArtRecordCollection;
    
} // namespace

#endif

