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

// Data product includes
#include "TrackerHitArtRecord.hh"
#include "WireID.hh"
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"

// Do all this in the *tracking analysis* namespace
namespace gm2strawtracking {
  class makeAnalysisHits;
}

class gm2strawtracking::makeAnalysisHits : public art::EDProducer {
public:
  explicit makeAnalysisHits(fhicl::ParameterSet const & p);
  virtual ~makeAnalysisHits();

  void produce(art::Event & e) override;


private:

  // To find the hit data, we need the name of the module that produced it
  // and an instance name.
  std::string hitModuleLabel_;
  std::string instanceName_;

};

// Constructor gets parameters from the FHiCL configuration file
gm2strawtracking::makeAnalysisHits::makeAnalysisHits(fhicl::ParameterSet const & p)
 :  hitModuleLabel_ ( p.get<std::string>("hitModuleLabel",  "artg4"   ) ),
    instanceName_   ( p.get<std::string>("instanceName", "Straws"))
{
    // Tell art what we'll be putting in the event.
    produces<TrackerHitArtRecordCollection>();
}

// Destructor; nothing much to do here
gm2strawtracking::makeAnalysisHits::~makeAnalysisHits()
{
    // Clean up dynamic memory and other resources here.
}

// produce(...) is called once for each event, and it adds a hit collection to
// the art event.
void gm2strawtracking::makeAnalysisHits::produce(art::Event & e)
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
    TrackerHitArtRecord rec;

    // Loop over all the hits in the c++11 way.
    for ( auto hdata : hits) {
        // Set up WireID
        WireID currID;
        currID.setTrackerNumber(-1);
        currID.setStation(hdata.stationNumber);
        currID.setWire(hdata.strawInRow);
        // rowNumber ranges from 0 to 4 for a station. We want layer number,
        // which ranges from 0 to 1 for each view.
        int rowNum = hdata.rowNumber;
        // For now, define u-view to be the view for the first plane in each
        // station (planes 0 and 2)
        if (rowNum == 0 || rowNum == 2) {
            currID.setView(u_view);
        }
        else if (rowNum == 1 || rowNum == 3) {
            currID.setView(v_view);
        }
        // And the layer is just row/2 rounded down (truncated)
        currID.setLayer(int(rowNum/2));

        // Set the record's WireID to the one we just concocted.
        rec.id = currID;


        rec.time = hdata.time;
        // For now, don't bother with width. 
        rec.width = 0;
        // For now, don't bother with the index
        rec.index = 0;
        // No error as far as we can tell...
        rec.errorFlag = false;

        // Add this record to the collection.
        toAddToEvent->push_back(rec);
    }
    // Add the collection to the event
    e.put(std::move(toAddToEvent));
}

DEFINE_ART_MODULE(gm2strawtracking::makeAnalysisHits)
