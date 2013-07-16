// This producer module reads straw hits from the simulation and creates the
// hits required for the tracker analysis. This module does not add in any
// random noise or smearing.
//
// @author Tasha Arvanitis
// @date July 2013

// art includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include <map>
#include <iostream>

// Data product includes
#include "gm2dataproducts/strawtracker/TrackerHitArtRecord.hh"
#include "gm2dataproducts/strawtracker/WireID.hh"
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"

using std::cout;
using std::endl;

// Do all this in the *tracking analysis* namespace
namespace gm2strawtracker {
  class makeAnalysisHits;
}

class gm2strawtracker::makeAnalysisHits : public art::EDProducer {
public:
  explicit makeAnalysisHits(fhicl::ParameterSet const & p);
  virtual ~makeAnalysisHits();

  void produce(art::Event & e) override;



private:

  // To find the hit data, we need the name of the module that produced it
  // and an instance name.
  std::string hitModuleLabel_;
  std::string instanceName_;

  // To keep track of how many times a single straw has been hit in an event, we
  // use a map from WireID to a counter
  std::map<WireID, int> indexMap_;

  // A helper function to construct the WireID for a straw hit.
  WireID makeWireID(gm2ringsim::StrawArtRecord const hdata);

};

// Constructor gets parameters from the FHiCL configuration file
gm2strawtracker::makeAnalysisHits::makeAnalysisHits(fhicl::ParameterSet const & p)
 :  hitModuleLabel_ ( p.get<std::string>("hitModuleLabel",  "artg4"   ) ),
    instanceName_   ( p.get<std::string>("instanceName", "Straws")),
    indexMap_ ()
{
    // Tell art what we'll be putting in the event.
    produces<TrackerHitArtRecordCollection>();
}

// Destructor; nothing much to do here
gm2strawtracker::makeAnalysisHits::~makeAnalysisHits()
{
    // Clean up dynamic memory and other resources here.
}

// produce(...) is called once for each event, and it adds a hit collection to
// the art event.
void gm2strawtracker::makeAnalysisHits::produce(art::Event & e)
{
    // Get hits and such, from the virtual ring station.
    art::Handle<gm2ringsim::StrawArtRecordCollection> hitDataHandle;

    // Fill the handle (note the use of the member data)
    e.getByLabel(hitModuleLabel_, instanceName_, hitDataHandle);

    // Resolve the handle
    gm2ringsim::StrawArtRecordCollection const & hits = *hitDataHandle;
    
    // Create the collection we'll add to the event
    std::unique_ptr<TrackerHitArtRecordCollection> 
        toAddToEvent(new TrackerHitArtRecordCollection);

    // Variables to use inside the loop; avoid creating and destroying them for
    // each iteration.
    TrackerHitArtRecord rec;
    WireID currID;
    bool wasAnError;

    // Loop over all the hits in the c++11 way.
    for ( auto hdata : hits) {
        // Reset the error flag for each iteration.
        wasAnError = false;
        
        // Get the WireID specification for this hit
        currID = makeWireID(hdata);

        // Use the WireID we just made as a key into that map of ours, so that
        // we can count the number of times it's been hit.
        indexMap_[currID]++;
        cout << "straw " << currID << " was hit " 
            << indexMap_[currID] << " time(s)." << endl;

        // Set the record's WireID to the one we just concocted.
        rec.id = currID;

        // For now, ignore drift time and just pass the actual time at which the
        // particle hit the straw.
        rec.time = hdata.time;

        // For now, don't bother with width. 
        rec.width = 0;

        // Since we increment the value corresponding to currID in the map every
        // iteration, the index of this hit (with index 0 being the first hit)
        // is just the value in the map minus one.
        rec.index = indexMap_[currID] - 1;

        // Set the error flag based on our little boolean wasAnError.
        rec.errorFlag = wasAnError;

        // Add this record to the collection.
        toAddToEvent->push_back(rec);
    }
    // Add the collection to the event
    e.put(std::move(toAddToEvent));
}

gm2strawtracker::WireID gm2strawtracker::makeAnalysisHits::makeWireID(gm2ringsim::StrawArtRecord const hdata) {
    WireID currID;

    currID.setTrackerNumber(0);
    currID.setStation(hdata.stationNumber);
    currID.setWire(hdata.strawInRow);
    // rowNumber ranges from 0 to 4 for a station. We want layer number,
    // which ranges from 0 to 1 for each view.
    int rowNum = hdata.layerNumber;
    // For now, define u-view to be the view for the first plane in each
    // station (planes 0 and 1)
    if (rowNum == 0 || rowNum == 1) {
        currID.setView(u_view);
    }
    else if (rowNum == 2 || rowNum == 3) {
        currID.setView(v_view);
    }
    else {
        // Hmm... we don't seem to have any view. This might be a problem;
        // set the view to N/A.
        currID.setView(na_view);
    }
    // And the layer is just row/2 rounded down (truncated)
    currID.setLayer(int(rowNum/2));

    // Return the WireID we've created
    return currID;
}

DEFINE_ART_MODULE(gm2strawtracker::makeAnalysisHits)
