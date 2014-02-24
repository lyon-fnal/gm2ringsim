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

// Root + Art includes
#include "TH1F.h"
#include "TTree.h"
#include "TVector3.h"
#include "TH2F.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

// Hit includes
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include <map>
#include <vector>
#include <iostream>

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
  std::string tree_dir_;
  
  TTree * t_trackTree_;

  // Variables for the tree
  float x_local_, y_local_, z_local_;
  float x_station_, y_station_, z_station_;
  float x_scallop_, y_scallop_, z_scallop_;
  

  int station_number_, view_number_, layer_number_, straw_in_layer_, straw_;
  
  
};


gm2ringsim::TrackReconstruct::TrackReconstruct(fhicl::ParameterSet const & p) :
  art::EDAnalyzer(p),
  trackModuleLabel_ ( p.get<std::string>("trackModuleLabel",  ""   ) ),
  trackInstanceName_   ( p.get<std::string>("trackInstanceName",    ""     ) ),
  tree_dir_       ( p.get<std::string>("tree_dir", ""         ) )
// Initialize member data here.
{
  art::ServiceHandle<art::TFileService> tfs;
  
  art::TFileDirectory treeDir = *tfs;

  if ( ! tree_dir_.empty() ) {
    treeDir = tfs->mkdir( tree_dir_ );
  }
  
  // Create the tree
  t_trackTree_ = treeDir.make<TTree>("trackTree", "Tree of hits");
  t_trackTree_->Branch("x_local", &x_local_, "x_local/F");
  t_trackTree_->Branch("y_local", &y_local_, "y_local/F");
  t_trackTree_->Branch("z_local", &z_local_, "z_local/F");
  t_trackTree_->Branch("x_station", &x_station_, "x_station/F");
  t_trackTree_->Branch("y_station", &y_station_, "y_station/F");
  t_trackTree_->Branch("z_station", &z_station_, "z_station/F");
  t_trackTree_->Branch("x_scallop", &x_scallop_, "x_scallop/F");
  t_trackTree_->Branch("y_scallop", &y_scallop_, "y_scallop/F");
  t_trackTree_->Branch("z_scallop", &z_scallop_, "z_scallop/F");

  t_trackTree_->Branch("station_number", &station_number_,"station_number/I");
  t_trackTree_->Branch("view_number", &view_number_,"view_number/I");
  t_trackTree_->Branch("layer_number", &layer_number_,"layer_number/I");
  t_trackTree_->Branch("straw_in_layer", &straw_in_layer_,"straw_in_layer/I");
  t_trackTree_->Branch("straw", &straw_,"straw/I");


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
    
    x_local_ = hdata.x_local;
    y_local_ = hdata.y_local;
    z_local_ = hdata.z_local;
    x_station_ = hdata.x_station;
    y_station_ = hdata.y_station;
    z_station_ = hdata.z_station;
    x_scallop_ = hdata.x_scallop;
    y_scallop_ = hdata.y_scallop;
    z_scallop_ = hdata.z_scallop;
    
    
    station_number_ = hdata.stationNumber;
    view_number_ = hdata.viewNumber;
    layer_number_ = hdata.layerNumber;
    straw_in_layer_ = hdata.strawInRow;
    straw_ = hdata.strawNumber;
    t_trackTree_->Fill();
    
    std::cout <<"StrawNumber: "<< hdata.strawNumber
              <<" StrawInRow: "<< hdata.strawInRow
              <<" layerNumber: "<<hdata.layerNumber
              <<" viewNumber: "<<hdata.viewNumber
              <<" stationNumber: "<<hdata.stationNumber
              <<" \nglobalPosition: ("<<hdata.x_global<< ","<<hdata.y_global<<","<< hdata.z_global<<")"
              <<" \nlocalPosition: ("<<hdata.x_local<< ","<<hdata.y_local<<","<< hdata.z_local<<")"
              <<" \nstationPosition: ("<<hdata.x_station<< ","<<hdata.y_station<<","<< hdata.z_station<<")"
    <<std::endl;
    
  }
}

DEFINE_ART_MODULE(gm2ringsim::TrackReconstruct)
