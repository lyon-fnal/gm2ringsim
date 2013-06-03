////////////////////////////////////////////////////////////////////////
// Class:       TrackReconstruct
// Module Type: analyzer
// File:        TrackReconstruct_module.cc
//
// Generated at Wed Apr 10 10:36:23 2013 by Leah Welty-Rieger using artmod
// from art v1_02_04.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "gm2ringsim/strawtracker/arthits/StrawArtRecord.hh"


namespace gm2ringsim {
  class TrackReconstruct;
}

class gm2ringsim::TrackReconstruct : public art::EDAnalyzer {
public:
  explicit TrackReconstruct(fhicl::ParameterSet const & p);
  virtual ~TrackReconstruct();

  void analyze(art::Event const & e) override;


private:
  std::string trackModuleLabel_;
  std::string trackInstanceName_;

  // Declare member data here.
  
};


gm2ringsim::TrackReconstruct::TrackReconstruct(fhicl::ParameterSet const & p) :
trackModuleLabel_ ( p.get<std::string>("trackModuleLabel",  ""   ) ),
trackInstanceName_   ( p.get<std::string>("trackInstanceName",    ""     ) )
// Initialize member data here.
{
}

gm2ringsim::TrackReconstruct::~TrackReconstruct()
{
  // Clean up dynamic memory and other resources here.
}

void gm2ringsim::TrackReconstruct::analyze(art::Event const & e)
{
  // Implementation of required member function here.
  
  art::Handle< gm2ringsim::StrawArtRecordCollection > hitDataHandle;
  
  // Fill the handle (note the use of the member data)
  mf::LogInfo("TrackReconstruct") << e.id();
  e.getByLabel(trackModuleLabel_, trackInstanceName_, hitDataHandle);

  StrawArtRecordCollection const & hits = *hitDataHandle;

  // Resolve the handle
  // Let's use the nice C++11 vector iteration
  for (auto hdata : hits){
    std::cout <<"StrawNumber: "<< hdata.strawNumber
              <<"; StrawInRow: "<< hdata.strawInRow
              <<"; rowNumber: "<<hdata.rowNumber
              <<"; stationNumber: "<<hdata.stationNumber
    <<std::endl;
    
  }
}

DEFINE_ART_MODULE(gm2ringsim::TrackReconstruct)
