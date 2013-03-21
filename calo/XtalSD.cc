/** @file XtalSD.cc
 
    Implements the xtal sensitive detector.
 
    Ported to Art from g2migtrace file xtalSD.cc
        @author Kevin Lynch
        @date 2009
 
    @author Robin Bjorkquist
    @date 2013
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

const int kInitialListSize = 300000;
const int kSizeIncrement = 100000;
const int kNoTrackNumber = -1;

gm2ringsim::XtalSD::XtalSD(G4String name) :
G4VSensitiveDetector( name ),
printLevel(0), drawLevel(0)
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
    for( G4int i = 0 ; i < nXtals_ ; ++i )
    {
        xtalID_.push_back(-1) ;
    }
    
    photonTracks.clear();
    photonTracks.resize( kInitialListSize, false );
    
    ShowerListManager::instance().resetList();
    nShowerElectrons_ = 0;
}

G4bool gm2ringsim::XtalSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){
    
    // get basic information regarding the particle involved in this step
    G4Track* track = thisStep->GetTrack() ;
    int parentID = track->GetParentID() ;
    int thisID = track->GetTrackID() ;
    bool chargedParticle = (track->GetDefinition()->GetPDGCharge() != 0);
    int signedPdg = track->GetDefinition()->GetPDGEncoding();
    int pdg = abs( signedPdg ) ;
    
    bool alreadyPartOfShower = false;
    bool initiatedShower = false;
    
    if( pdg != 0 )
    {
        
        // To determine which xtals belong to a single shower, keep track of all
        // non-optical-photon daughters of an e+ or e-.  If a new xtal is started
        // by a particle whose parent is in this list, then it is in the same
        // cluster, and the parentID is set to the ID in the list (which is the
        // initiating e+ or e-).  Otherwise, we are starting a new cluster, and the
        // list is cleared.    The bookkeeping must be coordinated with calorimeterSD,
        // hence the singleton ShowerListManager
        
        // make sure our list is long enough to accomodate this particle
        ShowerListManager &listMan = ShowerListManager::instance();
        ShowerListManager::particleStatus status = listMan.addToList( thisID, parentID );
        if ( status == ShowerListManager::kAlreadyPartOfShower ) {
            alreadyPartOfShower = true;
        } else {
            if ( status == ShowerListManager::kInitiatedShower ) {
                initiatedShower = true;
            }
        }
        
    }
    
    // really not sure that we want to do this!
    if( pdg != 11 && // electron
       pdg != 13 && // muon
       pdg != 0 ) // optical photon
    {
        // std::cout << "leaving processhits for wrong particle" << std::endl;
        return false ;
    }
    
    if ( pdg == 11 && !alreadyPartOfShower ) {
        ++nShowerElectrons_;
    }
    //    std::cout << "XTAL Track " << thisID
    // 	     << " name " << track->GetParticleDefinition()->GetParticleName()
    // 	     << " pdg " << track->GetParticleDefinition()->GetPDGEncoding()
    // 	     << " mat " << track->GetMaterial()->GetName()
    // 	     << std::endl
    // 	     << " pos " << track->GetPosition()
    // 	     << " mom " << track->GetMomentum()
    // 	     << std::endl ;
    
    // From example N05
    G4double edep = thisStep->GetTotalEnergyDeposit();
    if(edep<=0. && ( pdg == 11 || pdg == 13 ) ) return false;
    
    G4TouchableHistory* hist =
    (G4TouchableHistory*)(thisStep->GetPreStepPoint()->GetTouchable());
    //const G4VPhysicalVolume* physVol = hist->GetVolume();
    G4int copyID = hist->GetReplicaNumber();
    
	// std::cout << " copyID, xtalID: " << copyID << " " << xtalID[copyID] << std::endl;
    
    if(xtalID_[copyID]==-1)
    {
        // Only count optical photons if a track has already been seen, so don't make
        // new hits only for photons.
        
        // ****!!!*** Note: we can change the crystal hit structure to be a map of <trackID,xtalHit>
        // ****!!!*** to allow more than one particle that initiates a shower per crystal
        
        // ****!!!*** this needs to be generalized to allow any particle except for optical
        // ****!!!*** photons to be the particle that initiates a shower
        if( pdg == 11 || pdg == 13 )
        {
            int initiateId = ShowerListManager::instance().showerParentList()[thisID];
            XtalHit* xHit = new XtalHit( thisStep, initiateId );
            
            xHit->energyDep = edep ;
            if ( chargedParticle ) {
                xHit->trackLength = thisStep->GetStepLength() ;
            }
            
            G4int hcsize = thisHC->insert( xHit );
            xtalID_[copyID] = hcsize - 1;
            
            //            xHit->parentID = particleToInitiatingParticle[thisID];
            xHit->parentID = ShowerListManager::instance().showerParentList()[thisID];
            
            // std::cout << "New  copyID seen: " << copyID << " for parent " << xHit->parentID << std::endl;
            // 	 std::cout << " New XtalHit on xtalID " << copyID << " " << hcsize-1
            // 		   << " track " << thisID << " pos " << track->GetPosition() << " mom " << track->GetMomentum()
            // 	      << " parent " << parentID
            // 	      << " parentID " << xHit->parentID
            // 	      << " pdg " << track->GetDefinition()->GetPDGEncoding()
            // 	      << " edep " << xHit->energyDep
            // 	      << " length " << xHit->trackLength << std::endl ;
            
        } else if ( pdg == 0 ) {
            std::cout << "Track id " << thisID << " for optical photon entering new volume" << std::endl;
        }
        if ( initiatedShower ) {
            int hitIndex = xtalID_[ copyID ];
            m_parentPosition.setY(0);
            m_parentPosition.setX(  (*thisHC)[hitIndex]->local_pos.x() );
            m_parentPosition.setZ( -(*thisHC)[hitIndex]->local_pos.z() );
        }
        
    }
    else
    {
        int hitIndex = xtalID_[ copyID ] ;
        
        // ***!!!*** @bug error condition: we have a second particle that has initiated a shower
        // ***!!!*** this should be true in general
        
        if (initiatedShower) {
            std::cout << "*!*!*!*!*!*!*! Error: 2nd shower initiated in a previously hit crystal" << std::endl;
        }
        
        if( chargedParticle )
        {
            ( *thisHC )[ hitIndex ]->energyDep += edep ;
            ( *thisHC )[ hitIndex ]->trackLength += thisStep->GetStepLength() ;
            
        }
        else if ( pdg == 0 ) // optical photon
        {
            int opListSize = photonTracks.size();
            if ( thisID >= opListSize ) {
                photonTracks.resize( thisID+kSizeIncrement, false ); // increase the size of the list
            }
            // Check if this photon has already been counted
            if ( !photonTracks[thisID] ) {
                
                photonTracks[thisID] = true; // we've seen this track before
                
                // increment the energy deposited
                ++( ( *thisHC )[ hitIndex ]->nphoton ) ; // increment the photon counter
                ( *thisHC )[ hitIndex ]->ephoton += thisStep->GetPreStepPoint()->GetTotalEnergy() ; // add
                
                // create a photonHit entry for this photon
                XtalPhotonHit *xph = new XtalPhotonHit(thisStep);
                thisPhotonHC->insert(xph);
                PhotonHitCorrelator::getInstance().addTrack(thisID,xph);
            }
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