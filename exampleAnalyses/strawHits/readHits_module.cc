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
#include "TH2F.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

// Hit includes
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include <map>
#include <vector>

// Namespace
namespace gm2ringsim {
  class readHits;
  struct myTrack;
}

struct gm2ringsim::myTrack{
  std::vector<int> strawsHit;
  std::vector<int> trackID;
  std::vector<TVector3> position;
  std::vector<std::string> particle_name;
  std::vector<int> parentID;
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
 
  TTree * t_hitTree_;
  
  // Variables for the tree
  float tf_x_global;
  float tf_y_global;
  float tf_z_global;
  float tf_r_global;
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
    
  //Tree to hold track location information
  TTree *t_trackTree_;
  std::vector<int> strawNumber;
  std::vector<float> globalR;
  int nstraw;
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
  
  //t_trackTree_->Branch("strawNumber",strawNumber,"strawNumber[9]/I");
  //t_trackTree_->Branch("globalR",globalR,"globalR[9]/F");
  t_trackTree_->Branch("nstraw",&nstraw,"nstraw/I");

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
  //std::cout<<"The number of hits is: "<<hits.size()<<std::endl;
  myTrack track_info;;
  for ( auto hdata : hits) {
    //hdata is a strawartrecord
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
    
    tf_straw_number = hdata.strawNumber;
    tf_track_ID = hdata.trackID;
    
    TVector3 the_position(hdata.x_global, hdata.z_global, hdata.y_global);
    track_info.strawsHit.push_back(hdata.strawNumber);
    track_info.position.push_back(the_position);
    track_info.trackID.push_back(hdata.trackID);
    track_info.particle_name.push_back(hdata.particle_name);
    track_info.parentID.push_back(hdata.parent_ID);
    t_hitTree_->Fill();
    
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
  
  std::vector<int> strawsHitCompare = track_info.strawsHit;
  
  std::sort(strawsHitCompare.begin(),strawsHitCompare.end());

  if(strawsHitCompare == track_info.strawsHit && track_info.strawsHit.size()!=0) is_in_order = true;
  
    
  if (track_info.particle_name.size() == 1 && track_info.particle_name[0] == "e-") is_all_electrons = true;
  if (track_info.parentID.size() == 1 && track_info.parentID[0] == 1) is_muon_parent = true;
  
  if (is_in_order){
     std::cout<<"track_info.particle_name[0]: "<<track_info.particle_name[0]<<std::endl;
     std::cout<<"track_info.parentID[0]: "<<track_info.parentID[0]<<std::endl;
  }
  if(is_all_electrons && is_muon_parent && is_in_order){
    std::cout<<"I am all electrons, my parent is a muon and the straw numbers are in order"<<std::endl;
    nstraw = track_info.strawsHit.size();
    std::cout<<"number of straws hit: "<<nstraw<<std::endl;
  
    t_trackTree_->Fill();
	}
}

using gm2ringsim::readHits;
DEFINE_ART_MODULE(readHits)
