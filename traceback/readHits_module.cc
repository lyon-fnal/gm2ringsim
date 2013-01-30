// This is the readHits Art module. 

//  This is an analyzer that will read HitData from the event and make some plots and a root tree

// Art includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

// Root + Art includes
#include "TH1F.h"
#include "TTree.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

// Hit includes
#include "gm2ringsim/traceback/StrawArtRecord.hh"

// Namespace
namespace gm2ringsim {
  class readHits;
}

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
    TH1F * h_x_;
    
    // The root-tuple
    TTree * t_hitTree_;
    
    // Variables for the tree
    float tf_x_;
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
    h_x_= histDir.make<TH1F>("hitX","x of hits",50, -6000.0, -5000.0);
    // Do the tree next
    art::TFileDirectory treeDir = *tfs;
    if ( ! tree_dir_.empty() ) {
        treeDir = tfs->mkdir( tree_dir_ );
    }
    
    // Create the tree
    t_hitTree_ = treeDir.make<TTree>("hitTree", "Tree of hits");
    t_hitTree_->Branch("x", &tf_x_, "x/F");
   
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
  
    std::cout<<"THIS SIZE OF THE HITS IS: "<<hits.size()<<std::endl;
    // Let's use the nice C++11 vector iteration
    for ( auto hdata : hits) {
        //hdata is a strawartrecord
      std::cout<<"The hitdata.x_inf: "<<hdata.x_inf<<std::endl;
      h_x_ -> Fill(hdata.x_inf);
      tf_x_=hdata.x_inf;
    }
}

using gm2ringsim::readHits;
DEFINE_ART_MODULE(readHits)