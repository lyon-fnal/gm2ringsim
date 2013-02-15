// Particle track analyzer

// Analyze particle tracks using the output of the
// gm2ringsim::TrackingAction_service and the
// artg4::physicalVolumeStore

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"


#include "artg4/pluginActions/physicalVolumeStore/PhysicalVolumeStoreData.hh"
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"

#include <vector>

namespace gm2ringsim_ex {
  class particleTrackAnalyzer;
}

class gm2ringsim_ex::particleTrackAnalyzer : public art::EDAnalyzer {
public:
  explicit particleTrackAnalyzer(fhicl::ParameterSet const & p);
  virtual ~particleTrackAnalyzer();

  void analyze(art::Event const & e) override;
    
private:

  // Declare member data here.
  
  // Producer
  std::string producerLabel_;
  std::string producerInstance_;
  
};


gm2ringsim_ex::particleTrackAnalyzer::particleTrackAnalyzer(fhicl::ParameterSet const & p) :
  producerLabel_( p.get<std::string>("producerLabel", "artg4")),
  producerInstance_( p.get<std::string>("producerInstance", ""))
{ }

gm2ringsim_ex::particleTrackAnalyzer::~particleTrackAnalyzer()
{
  // Clean up dynamic memory and other resources here.
}

void gm2ringsim_ex::particleTrackAnalyzer::analyze(art::Event const &)
{
  // Let's get the physical volume store
  const artg4::PhysicalVolumeStoreData & pvs = art::ServiceHandle< artg4::PhysicalVolumeStoreService >() -> getData();
  
  mf::LogInfo("ParticleTrackAnalyzer") << "There are " << pvs.size() << " entries in the PVS";
  
  // Print it out
  for ( unsigned int i = 0; i < pvs.size(); ++i ) {
    mf::LogInfo("ParticleTrackAnalyzer") << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i);
  }
  
  
}


DEFINE_ART_MODULE(gm2ringsim_ex::particleTrackAnalyzer)
