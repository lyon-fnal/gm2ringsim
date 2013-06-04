////////////////////////////////////////////////////////////////////////
// Class:       geantFreeReadCalo
// Module Type: analyzer
// File:        geantFreeReadCalo_module.cc
//
// Generated at Wed May 29 16:29:15 2013 by Natasha Arvanitis using artmod
// from art v1_02_04.
////////////////////////////////////////////////////////////////////////
// STL includes
#include <vector>
#include <iostream>

// Art includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Root includes
#include "TTree.h"
#include "TVector3.h"
// art service for root files.
#include "art/Framework/Services/Optional/TFileService.h"

#include "gm2ringsim/calo/arthits/CaloArtRecord.hh"

namespace gm2ringsim {
  class geantFreeReadCalo;
}

class gm2ringsim::geantFreeReadCalo : public art::EDAnalyzer {
public:
  explicit geantFreeReadCalo(fhicl::ParameterSet const & p);
  virtual ~geantFreeReadCalo();

  void analyze(art::Event const & e) override;


private:

  // To find the hit data, we need the name of the module that produced it and an
  // instance name.
  std::string hitModuleLabel_;
  std::string instanceName_;

  // The tree
  TTree * t_myTree_;

  // Variables for the tree
  float tf_x_global;
  float tf_y_global;
  float tf_z_global;
  float tf_px_global;
  float tf_py_global;
  float tf_pz_global;
  int tf_track_ID;
  int tf_calo_number;
  int tf_turn;
  float tf_time;
};


gm2ringsim::geantFreeReadCalo::geantFreeReadCalo(fhicl::ParameterSet const & p)
 :  hitModuleLabel_ ( p.get<std::string>("hitModuleLabel",  ""   ) ),
	instanceName_   ( p.get<std::string>("instanceName",    ""     ) )
{
	std::cout << "Geant4-free analyzer created!" << std::endl;
	// Get the service handle - dereferencing this will be an object that
	// inherits from TFileDirectory - so it can do the same things as 
	// TFileDirectory
	art::ServiceHandle<art::TFileService> tfs;
	art::TFileDirectory treeDir = *tfs;
	t_myTree_ = treeDir.make<TTree>("hitTree", "Tree of hits");
	t_myTree_->Branch("caloNumber", &tf_calo_number, "caloNumber/I");
    t_myTree_->Branch("globalX", &tf_x_global, "globalX/F");
    t_myTree_->Branch("globalY", &tf_y_global, "globalY/F");
    t_myTree_->Branch("globalZ", &tf_z_global, "globalZ/F");
    t_myTree_->Branch("globalpX", &tf_px_global, "globalpX/F");
    t_myTree_->Branch("globalpY", &tf_py_global, "globalpY/F");
    t_myTree_->Branch("globalpZ", &tf_pz_global, "globalpZ/F");
    t_myTree_->Branch("trackID", &tf_track_ID, "trackID/I");
    t_myTree_->Branch("time", &tf_time, "time/F");
    t_myTree_->Branch("turn", &tf_turn, "turn/I");
}

gm2ringsim::geantFreeReadCalo::~geantFreeReadCalo()
{
  // Clean up dynamic memory and other resources here.
}

void gm2ringsim::geantFreeReadCalo::analyze(art::Event const & e)
{
  // Implementation of required member function here.
  art::Handle< CaloArtRecordCollection > hitDataHandle;
  
  // Fill the handle (note the use of the member data)
  e.getByLabel(hitModuleLabel_, instanceName_, hitDataHandle);
  
  // Resolve the handle
  CaloArtRecordCollection const & hits = *hitDataHandle;
  // Let's use the nice C++11 vector iteration
  for ( auto hdata : hits) {
	  // Add to the tree	
	  tf_calo_number = hdata.caloNum;
	  tf_x_global = hdata.x;
	  tf_y_global = hdata.y;
	  tf_z_global = hdata.z;
	  tf_px_global = hdata.px;
	  tf_py_global = hdata.py;
	  tf_pz_global = hdata.pz;
	  tf_track_ID = hdata.trackID;
	  tf_time = hdata.time;
	  tf_turn = hdata.turn;
	  
  	  t_myTree_->Fill();

	  std::cout << "Calorimeter " << hdata.caloNum 
		  << " hit recorded." << std::endl;
  }

  

}

DEFINE_ART_MODULE(gm2ringsim::geantFreeReadCalo)
