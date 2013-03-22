// This is the readHits Art module.

//  This is an analyzer that will read HitData from the event and make some plots and a root tree

// Art includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

// Root + Art includes
#include "TH1F.h"
#include "TTree.h"
#include "TVector3.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

// Hit includes
#include "gm2ringsim/traceback/StrawArtRecord.hh"
#include <map>
#include <vector>

// Namespace
namespace gm2ringsim {
  class readHits;
  struct myTrack;
}

struct gm2ringsim::myTrack{
  std::vector<int> strawPlanes;
  std::vector<int> tracebackLocations;
  
  std::vector<TVector3> position;
  
};

// The class
class gm2ringsim::readHits : public art::EDAnalyzer {
public:
  explicit readHits(fhicl::ParameterSet const &p);
  virtual ~readHits();
  
  virtual void analyze(art::Event const &e) override;
  
  
private:
  
  // Declare member data here.
  
  // To find the hit data, we need the name of the module that produced it and an
  // instance name.
  std::string hitModuleLabel_;
  std::string instanceName_;
  
  // Names of the sub-directories in the root file
  std::string hist_dir_, tree_dir_;
  
  // The histograms
  TH1F *h_x_global;
  TH1F *h_y_global;
  TH1F *h_z_global;
  TH1F *h_r_global;
  TH1F *h_myr_global;
  TH1F *h_px_global;
  TH1F *h_py_global;
  TH1F *h_pz_global;
  TH1F *h_x_local;
  TH1F *h_y_local;
  TH1F *h_z_local;
  TH1F *h_px_local;
  TH1F *h_py_local;
  TH1F *h_pz_local;

  // The root-tuple
  TTree * t_hitTree_;
  
  // Variables for the tree
  float tf_x_global;
  float tf_y_global;
  float tf_z_global;
  float tf_r_global;
  float tf_myr_global;
  float tf_px_global;
  float tf_py_global;
  float tf_pz_global;
  float tf_x_local;
  float tf_y_local;
  float tf_z_local;
  float tf_px_local;
  float tf_py_local;
  float tf_pz_local;
  int tf_traceback_number;
  int tf_straw_number;
  int tf_track_ID;
    
  std::map<int, gm2ringsim::myTrack> track_positions;
  //Tree to hold track location information
  TTree *t_trackTree_;
  
  std::vector<int> tt_tracebackNumbers;
};


gm2ringsim::readHits::readHits(fhicl::ParameterSet const &p) :
hitModuleLabel_ ( p.get<std::string>("hitModuleLabel",  ""   ) ),
instanceName_   ( p.get<std::string>("instanceName",    ""     ) ),
hist_dir_       ( p.get<std::string>("hist_dir"         ) ),
tree_dir_       ( p.get<std::string>("tree_dir"         ) )
{
  // You could require that hist_dir and tree_dir have some real strings, but what if the
  // user just leaves them blank. We then want to use the top level directory.
  // Note that the TFileService handle dereferences to an object that inherits from
  // TFileDirectory. This will be a little tricky, so pay close attention
  
  // Get the service handle - dereferencing this will be an object that inherits
  // from TFileDirectory - so it can do the same things as TFileDirectory
  art::ServiceHandle<art::TFileService> tfs;
  
  // Do the histograms first.
  // Let's assume the top directory. This is *tfs itself
  art::TFileDirectory histDir = *tfs;
  
  // Did we specify a directory? If so, reassign histDir to the new directory
  if ( ! hist_dir_.empty() ) {
    histDir = tfs->mkdir( hist_dir_ );  // Note how we re-assigned histDir
  }
  
  // Create the histogram objects
  h_x_global= histDir.make<TH1F>("global_hitX","Global X of hits",50, -6000.0, 6000.0);
  h_y_global= histDir.make<TH1F>("global_hitY","Global Y of hits",50, -70.0, 70.0);
  h_z_global= histDir.make<TH1F>("global_hitZ","Global Z of hits",50, 3000.0, 5000.0);
  h_r_global= histDir.make<TH1F>("global_hitR","Global R of the hits",50,6800,7050);
  h_myr_global = histDir.make<TH1F>("global_hitMYR","Global MyR of the hits",50,6800,7050);
  h_px_global= histDir.make<TH1F>("global_hitpX","Global pX of hits",50, -100.0, 100.0);
  h_py_global= histDir.make<TH1F>("global_hitpY","Global pY of hits",50, -100.0, 100.0);
  h_pz_global= histDir.make<TH1F>("global_hitpZ","Global pZ of hits",50, -100.0, 100.0);
  h_x_local= histDir.make<TH1F>("local_hitX","Local X of hits",50, -3000.0, 0.0);
  h_y_local= histDir.make<TH1F>("local_hitY","Local Y of hits",50, -50.0, 50.0);
  h_z_local= histDir.make<TH1F>("local_hitZ","Local Z of hits",50, -3000, 0.0);
  h_px_local= histDir.make<TH1F>("local_hitpX","Local pX of hits",50, -500.0, 500.0);
  h_py_local= histDir.make<TH1F>("local_hitpY","Local pY of hits",50, -500.0, 500.0);
  h_pz_local= histDir.make<TH1F>("lpca;_hitpZ","Local pZ of hits",50, -500.0, 500.0);
  
  // Do the tree next
  art::TFileDirectory treeDir = *tfs;
  if ( ! tree_dir_.empty() ) {
    treeDir = tfs->mkdir( tree_dir_ );
  }
  
  // Create the tree
  t_hitTree_ = treeDir.make<TTree>("hitTree", "Tree of hits");
  t_hitTree_->Branch("x_global", &tf_x_global, "x_global/F");
  t_hitTree_->Branch("y_global", &tf_y_global, "y_global/F");
  t_hitTree_->Branch("z_global", &tf_z_global, "z_global/F");
  t_hitTree_->Branch("r_global", &tf_r_global, "r_global/F");
  t_hitTree_->Branch("myr_global", &tf_myr_global, "myr_global/F");
  t_hitTree_->Branch("px_global", &tf_px_global, "px_global/F");
  t_hitTree_->Branch("py_global", &tf_py_global, "py_global/F");
  t_hitTree_->Branch("pz_global", &tf_pz_global, "pz_global/F");
  t_hitTree_->Branch("x_local", &tf_x_local, "x_local/F");
  t_hitTree_->Branch("y_local", &tf_y_local, "y_local/F");
  t_hitTree_->Branch("z_local", &tf_z_local, "z_local/F");
  t_hitTree_->Branch("px_local", &tf_px_local, "px_local/F");
  t_hitTree_->Branch("py_local", &tf_py_local, "py_local/F");
  t_hitTree_->Branch("pz_local", &tf_pz_local, "pz_local/F");
  t_hitTree_->Branch("tracebackNumber", &tf_traceback_number, "tracebackNumber/I");
  t_hitTree_->Branch("strawNumber", &tf_straw_number, "strawNumber/I");
  t_hitTree_->Branch("trackID",&tf_track_ID,"trackID/I");
  
  t_trackTree_ = treeDir.make<TTree>("trackTree", "Tree of tracks");
  t_trackTree_->Branch("track_tracebackNumber",&tt_tracebackNumbers);


}

gm2ringsim::readHits::~readHits() {
  // Clean up dynamic memory and other resources here.
}

void gm2ringsim::readHits::analyze(art::Event const &e) {
  
  // Extract the hits
  
  // Make the handle
  art::Handle< StrawArtRecordCollection > hitDataHandle;
  
  // Fill the handle (note the use of the member data)
  std::cout<<"Event ID: "<<e.id()<<std::endl;
  
  e.getByLabel(hitModuleLabel_, instanceName_, hitDataHandle);
  
  // Resolve the handle
  StrawArtRecordCollection const & hits = *hitDataHandle;
  // Let's use the nice C++11 vector iteration
  
  for ( auto hdata : hits) {
    //hdata is a strawartrecord
    h_x_global -> Fill(hdata.x_global);
    h_y_global -> Fill(hdata.y_global);
    h_z_global -> Fill(hdata.z_global);
    h_r_global -> Fill(hdata.r_global);
    h_px_global -> Fill(hdata.px_global);
    h_py_global -> Fill(hdata.py_global);
    h_pz_global -> Fill(hdata.pz_global);
    h_x_local -> Fill(hdata.x_local);
    h_y_local -> Fill(hdata.y_local);
    h_z_local -> Fill(hdata.z_local);
    h_px_local -> Fill(hdata.px_local);
    h_py_local -> Fill(hdata.py_local);
    h_pz_local -> Fill(hdata.pz_local);

    
    tf_x_global=hdata.x_global;
    tf_y_global=hdata.y_global;
    tf_z_global=hdata.z_global;
    tf_r_global=hdata.r_global;
    tf_px_global=hdata.px_global;
    tf_py_global=hdata.py_global;
    tf_pz_global=hdata.pz_global;
    tf_x_local=hdata.x_local;
    tf_y_local=hdata.y_local;
    tf_z_local=hdata.z_local;
    tf_px_local=hdata.px_local;
    tf_py_local=hdata.py_local;
    tf_pz_local=hdata.pz_local;
    
    tf_traceback_number = hdata.tracebackNumber;
    tf_straw_number = hdata.strawNumber;
    tf_track_ID = hdata.trackID;
    
    myTrack oneTrack;
    
    if (track_positions.find(hdata.trackID) != track_positions.end() ) {
      
      oneTrack = track_positions[hdata.trackID];
    }
    
    TVector3 the_position(hdata.x_global, hdata.y_global, hdata.z_global);
    oneTrack.strawPlanes.push_back(hdata.strawNumber);
    oneTrack.tracebackLocations.push_back(hdata.tracebackNumber);
    oneTrack.position.push_back(the_position);
    track_positions[hdata.trackID] = oneTrack;
   
    t_hitTree_->Fill();
    
  }
  
  for (auto& a : track_positions){
    std::cout<<"The Track number is: "<<a.first<<std::endl;
    std::cout<<"The number of Straw Planes hit is: "<<a.second.strawPlanes.size()<<std::endl;
    for (auto s : a.second.strawPlanes)
      std::cout<<"The straw number is: "<<s<<std::endl;
  }
}

using gm2ringsim::readHits;
DEFINE_ART_MODULE(readHits)