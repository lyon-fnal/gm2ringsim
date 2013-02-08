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
  
  const artg4::PhysicalVolumeStoreData & getPVS(art::Run const & r);
  
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


const artg4::PhysicalVolumeStoreData & gm2ringsim_ex::particleTrackAnalyzer::getPVS(art::Run const & r) {
  // Is it in the Run record? The following will get ALL
  // instances of PhysicalVolumeStoreData. You can then loop
  // thtrough through them, examine the provenance, and choose the
  // one you want. 
  std::vector< art::Handle<artg4::PhysicalVolumeStoreData> > pvsHV;
  r.getManyByType(pvsHV);
  
  if ( ! pvsHV.empty() ) {
    // We found something in the run record. Let's see what it is.
    for ( auto aHandle : pvsHV ) {
      // Check the provenance for the handle
      art::Provenance const * prov = aHandle.provenance();
      
      // Check the module label
      if ( producerLabel_.empty() || producerLabel_ == prov->moduleLabel() ) {
        // Check the instance label
        if ( producerInstance_.empty() || producerInstance_ == prov->productInstanceName() ) {
          // This is it!
          mf::LogDebug("ParticleTrackAnalyzer") << "Getting PVS from Run record";
          return *aHandle;
        }
      }
    }
  }
  
  // If we've made it this far, then we could not find the physical volume store
  // in the Run record. So let's look a the service

  mf::LogDebug("ParticleTrackAnalyzer") << "Cannot find PVS in Run record. Trying PhysicalVolumeStoreService.";

  // This should throw an exception if the service can't be found
  art::ServiceHandle<artg4::PhysicalVolumeStoreService> pvsS;
  
  mf::LogDebug("ParticleTrackAnalyzer") << "Getting PVS from service";
  return pvsS->getData();
}

void gm2ringsim_ex::particleTrackAnalyzer::analyze(art::Event const & e)
{
  // Let's get the physical volume store
  const artg4::PhysicalVolumeStoreData & pvs = getPVS( e.getRun() );
  
  mf::LogInfo("ParticleTrackAnalyzer") << "There are " << pvs.size() << " entries in the PVS";
  
  // Print it out
  for ( unsigned int i = 0; i < pvs.size(); ++i ) {
    mf::LogInfo("ParticleTrackAnalyzer") << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i);
  }
  
  
}


DEFINE_ART_MODULE(gm2ringsim_ex::particleTrackAnalyzer)
