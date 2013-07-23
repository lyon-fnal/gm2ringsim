////////////////////////////////////////////////////////////////////////
// Class:       GoodStrawHits
// Module Type: filter
// File:        GoodStrawHits_module.cc
//
// Generated at Tue Apr  9 15:55:46 2013 by Leah Welty-Rieger using artmod
// from art v1_02_04.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#include "gm2ringsim/strawtracker/StrawArtRecord.hh"

#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "TVector3.h"

namespace gm2ringsim {
  struct Track;
  //struct Hit;
  class GoodStrawHits;
}


/*struct gm2ringsim::Hit{
  int strawNumber;
  int layerNumber;
  int viewNumber;
  int strawInLayer;
  int stationNumber;
  TVector3 position;
};*/

struct gm2ringsim::Track{
  std::vector<int> strawsHit;
  std::vector<std::string> particle_name;
  std::vector<int> parentID;
};



//class GoodStrawHits;

class gm2ringsim::GoodStrawHits : public art::EDFilter {
public:
  explicit GoodStrawHits(fhicl::ParameterSet const & p);
  virtual ~GoodStrawHits();

  bool filter(art::Event & e) override;


private:

  // Declare member data here.
  std::string strawHitModuleLabel_;
  std::string strawInstanceName_;
};


gm2ringsim::GoodStrawHits::GoodStrawHits(fhicl::ParameterSet const & p) :
  strawHitModuleLabel_( p.get<std::string>("strawHitModuleLabel","") ),
  strawInstanceName_( p.get<std::string>("strawInstanceName","") )
 // Initialize member data here.
{
  // Call appropriate Produces<>() functions here.
}

gm2ringsim::GoodStrawHits::~GoodStrawHits()
{
  // Clean up dynamic memory and other resources here.
}

bool gm2ringsim::GoodStrawHits::filter(art::Event & e)
{
  // Implementation of required member function here.
  art::Handle< StrawArtRecordCollection > hitDataHandle;
  
  // Fill the handle (note the use of the member data)
  std::cout<<"Event ID: "<<e.id()<<std::endl;
  
  e.getByLabel(strawHitModuleLabel_, strawInstanceName_, hitDataHandle);
  
  // Resolve the handle
  StrawArtRecordCollection const & hits = *hitDataHandle;
  // Let's use the nice C++11 vector iteration
  //std::cout<<"The number of hits is: "<<hits.size()<<std::endl;
  Track track_info;;

  for ( auto hdata : hits) {
    track_info.strawsHit.push_back(hdata.strawNumber);
    track_info.particle_name.push_back(hdata.particle_name);
    track_info.parentID.push_back(hdata.parent_ID);
    
  }
  
  std::vector<std::string>::iterator it_partname;
  it_partname = std::unique (track_info.particle_name.begin(), track_info.particle_name.end());
  track_info.particle_name.resize(std::distance(track_info.particle_name.begin(),it_partname));
  
  std::vector<int>::iterator it_parentID;
  it_parentID = std::unique (track_info.parentID.begin(), track_info.parentID.end());
  track_info.parentID.resize(std::distance(track_info.parentID.begin(),it_parentID));
  
  bool is_all_electrons=false;
  bool is_muon_parent=false;
  bool is_in_order = false;
  //bool is_nHits_gt4_lt11 = false;
  
  std::vector<int> strawsHitCompare = track_info.strawsHit;
  
  std::sort(strawsHitCompare.begin(),strawsHitCompare.end());
  
  if(strawsHitCompare == track_info.strawsHit) is_in_order = true;
  //if(track_info.strawsHit.size() > 4 && track_info.strawsHit.size() < 11) is_nHits_gt4_lt11 = true;
  
  if (track_info.particle_name.size() == 1 && track_info.particle_name[0] == "e-") is_all_electrons = true;
  if (track_info.parentID.size() == 1 && track_info.parentID[0] == 1) is_muon_parent = true;
  
  if(is_all_electrons && is_muon_parent && is_in_order){
    std::cout<<"The event was good"<<std::endl;
    return true;
  }
  else return false;

  
}
DEFINE_ART_MODULE(gm2ringsim::GoodStrawHits)
