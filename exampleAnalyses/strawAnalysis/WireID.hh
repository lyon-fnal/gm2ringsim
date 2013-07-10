// This file defines the WireID struct that is used by the tracking analysis
// framework to identify an individual wire.  Specification for the wire
// includes tracker number (which scallop it's in) station, view, layer, and
// wire number.
//
// For now, it is not a 'smart struct' as suggested in docDB entry 1153 (Data
// structure study for tracking software), but a naive struct. Expansion to a
// smart struct is anticipated to be fairly straightforward.

// @author Tasha Arvanitis
// @date July 2013

// Include guard
#ifndef WIREID_HH
#define WIREID_HH

// All in the namespace for straw tracking
namespace gm2strawtracking {

    // Define the StrawView enum to keep track of the views.
    enum StrawView {
        u_view,
        v_view
    };

    struct WireID {
        private:
        // Tracker number should range from 0-1 and describes which of the
        // tracking stations contains this wire: 0 for the one directly opposite
        // the inflector, and 1 for the 270 degree one.
        short trackerNumber; 

        // Station number (describes which of the straw station/boxes this wire
        // is in) should range from 0 to around 20 at most.
        short station;

        // View is a member of the StrawView enum and describes whether the
        // wire in question is a u-view or a v-view wire. We still need to
        // figure out or define which direction is u-view.
        StrawView view;

        // Layer describes which of the two layers of that view this wire is in
        // (see the 3d model). It should range from 0-1.
        short layer;

        // Wire gives the wire number within the previously-specified layer. It
        // will range from 0-n, depending on the size of the station in
        // question, and wire 0 is (I think?) the one closest to the scallop
        // edge.
        short wire;

        public:
        // For the sake of future expansion to a smart struct, access to the
        // fields is through methods. Names are self-explanatory, and the
        // methods are simple enough to put in here. Should they be inlined?
        // Getters
        short getTrackerNumber() {
            return trackerNumber;
        }
        short getStation() {
            return station;
        }
        StrawView getView() {
            return view;
        }
        short getLayer() {
            return layer;
        }
        short getWire() {
            return wire;
        }

        // Setters
        void setTrackerNumber(short newValue) {
            trackerNumber = newValue;
        }
        void setStation(short newValue) {
            station = newValue;
        }
        void setView(StrawView newValue) {
            view = newValue;
        }
        void setLayer(short newValue) {
            layer = newValue;
        }
        void setWire(short newValue) {
            wire = newValue;
        }

    };

}

#endif

