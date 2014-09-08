/** @file XtalSD.cc
    
    Implements the xtal sensitive detector.
 
    Ported to Art from g2migtrace file xtalSD.cc
    @author Kevin Lynch
    @date 2009
    
    @author Robin Bjorkquist
    @date 2013, 2014

*/

#include "XtalSD.hh"
#include "XtalHit.hh"
#include "XtalPhotonHit.hh"
#include "PhotonHitCorrelator.hh"
#include "ShowerListManager.hh"
#include "Calorimeter_service.hh" // need this for addPhotonToName

#include "Geant4/G4ios.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4TrackStatus.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4ThreeVector.hh"


gm2ringsim::XtalSD::XtalSD(G4String name) :
  G4VSensitiveDetector( name ),
  printLevel(0), drawLevel(0), killOpticalPhotons_(true)
{
  collectionName.insert( name );
  G4String photonName = Calorimeter::addPhotonToName(name);
  collectionName.insert( photonName );
  
  // Register with SDManager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(this);
}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
gm2ringsim::XtalSD::~XtalSD(){}

void gm2ringsim::XtalSD::Initialize(G4HCofThisEvent* HCoTE){
  
  thisHC = new XtalHitsCollection
    ( SensitiveDetectorName, collectionName[0]);
  thisPhotonHC = new XtalPhotonHitsCollection
    ( SensitiveDetectorName, collectionName[1]);
  
  G4int const thisHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(thisHC);
  G4int const thisPhotonHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(thisPhotonHC);
  
  // Add to HCoTE
  HCoTE->AddHitsCollection( thisHCID,       thisHC );
  HCoTE->AddHitsCollection( thisPhotonHCID, thisPhotonHC );
  
  photonTracks_.clear();
  
  xtalHitList_.clear();
  
  ShowerListManager::instance().resetList();
  nShowerElectrons_ = 0;
}

G4bool gm2ringsim::XtalSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){
  
  // get basic information regarding the particle involved in this step
  G4Track* track = thisStep->GetTrack() ;
  int parentID = track->GetParentID() ;
  int trackID = track->GetTrackID() ;
  int signedPdg = track->GetDefinition()->GetPDGEncoding();
  int pdg = abs( signedPdg ) ;
  
  // find out which calorimeter station & which xtal contain this step
  int xtalCopyID = thisStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo(); // each xtal in the entire ring has a unique copy number
  int caloNum = xtalCopyID / nXtalsPerCalo_ ; // integer division
  int xtalNum = xtalCopyID % nXtalsPerCalo_ ; // xtal num local to that calo
  
  ShowerListManager &listMan = ShowerListManager::instance();
  
  if( pdg != 0 ) // anything other than an optical photon
    {
      
      // Use the ShowerListManager to look up the initiating particle of this
      // shower (i.e., trace back through particle parents to find the trackID
      // of the "ancestor" particle that made a calo hit)
      
      ShowerListManager::particleStatus showerStatus = listMan.addToList( caloNum, trackID, parentID );
      if (showerStatus==ShowerListManager::kInitiatedShower)
        {
	  std::cout << "Warning: internal particle marked as shower initiator" << std::endl;
        }
      int initiateID = listMan.getShowerParentID(caloNum, trackID);
      
      // From example N05
      G4double edep = thisStep->GetTotalEnergyDeposit();
      if(edep<=0.) return false;
      
      // check to see if we already have an xtalHitList for this sub-shower
      std::vector<int>& xtalHitSubList = xtalHitList_[initiateID];
      if (xtalHitSubList.size() < nXtalsTotal_) xtalHitSubList.resize(nXtalsTotal_, -1);
      
      if(xtalHitSubList[xtalCopyID]==-1) // this crystal does not have an xtal hit yet
	{
	  XtalHit* xHit = new XtalHit( thisStep );
	  xHit->caloHitID = initiateID;
	  xHit->caloNum = caloNum ;
	  xHit->xtalNum = xtalNum ;
	  xHit->energyDep = edep ;
	  xHit->trackLength = thisStep->GetStepLength() ;
          
	  G4int hcsize = thisHC->insert( xHit );
	  xtalHitSubList[xtalCopyID] = hcsize - 1;
          
	}
      else // this crystal already has a hit for this initiating particle
	{
	  int hitIndex = xtalHitSubList[ xtalCopyID ] ;
          
	  ( *thisHC )[ hitIndex ]->energyDep += edep ;
	  ( *thisHC )[ hitIndex ]->trackLength += thisStep->GetStepLength() ;
	}
      
    }
  else if ( pdg == 0 ) // optical photon
    {
      if ( photonTracks_.count(trackID)==0) // we haven't seen this photon before
	{
	  photonTracks_.insert(trackID); // add this photon to the list
          
	  // Use the ShowerListManager to look up the initiating particle of this
	  // shower (i.e., trace back through particle parents to find the trackID
	  // of the "ancestor" particle that made a calo hit). Optical photons
	  // aren't on the list, so look up this particle's parent.
	  int initiateID = listMan.getShowerParentID(caloNum, parentID);
	  if (initiateID==-1) // parent was not on the shower list
	    {
	      std::cout << "Warning: parent of optical photon not found in ShowerListManager" << std::endl;
	    }
	  
	  // make sure we already have an xtalHit for this xtal & initiating particle
	  std::vector<int>& xtalHitSubList = xtalHitList_[initiateID];
	  if (xtalHitSubList.size() < nXtalsTotal_) return false;
	  if (xtalHitSubList[xtalCopyID]==-1) return false;
	  int hitIndex = xtalHitSubList[xtalCopyID];
          
	  // increment the photon counts in the xtal hit
	  ( *thisHC )[ hitIndex ]->nphoton ++  ;
	  ( *thisHC )[ hitIndex ]->ephoton += thisStep->GetPreStepPoint()->GetTotalEnergy() ;
          
	  // create an xtalPhotonHit for this photon
	  XtalPhotonHit *xph = new XtalPhotonHit(thisStep);
	  xph->caloNum = caloNum;
	  xph->xtalNum = xtalNum;
	  thisPhotonHC->insert(xph);
	  PhotonHitCorrelator::getInstance().addTrack(trackID,xph);
        }
      
      // If the killOpticalPhotons_ parameter is set to true, kill the track
      if (killOpticalPhotons_)
        {
	  thisStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
    }
  
  return true;
}

void gm2ringsim::XtalSD::EndOfEvent(G4HCofThisEvent*) {
  
  G4int n = thisHC->entries();
  
  if(! (printLevel > 0 || drawLevel > 0) )
    return;
  
  if(printLevel > 0 && n>0){
    G4cout << "Sensitive Detector: " << SensitiveDetectorName << '\n';
    for(G4int i=0; i<n; ++i){
      G4cout << '\t';
      (*thisHC)[i]->Print();
    }
  }
  
  if(drawLevel > 0 && n>0)
    for(G4int i=0; i<n; ++i)
      (*thisHC)[i]->Draw();
  
}


G4int gm2ringsim::XtalSD::PrintLevel(G4int newLevel){
  G4int temp = printLevel;
  printLevel = newLevel;
  return temp;
}

G4int gm2ringsim::XtalSD::DrawLevel(G4int newLevel){
  G4int temp = drawLevel;
  drawLevel = newLevel;
  return temp;
}

bool gm2ringsim::XtalSD::killOpticalPhotons(bool newLevel){
  bool temp = killOpticalPhotons_;
  killOpticalPhotons_ = newLevel;
  return temp;
}
