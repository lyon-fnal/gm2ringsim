/** @file PhotodetectorSD.cc
 
    Implements the "stop and kill" photodetector sensitive detector.
 
    Ported to Art from g2migtrace file pmtSD.cc
        @author Kevin Lynch
        @date 2009
 
    @author Robin Bjorkquist
    @date 2013
 */

#include "PhotodetectorSD.hh"
#include "PhotonHitCorrelator.hh"
#include "Calorimeter_service.hh" // need this for addPhotonToName

#include "Geant4/G4ios.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4TrackStatus.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4VSolid.hh"

#include "CLHEP/Random/RandFlat.h"

gm2ringsim::PhotodetectorSD::PhotodetectorSD(G4String name) :
G4VSensitiveDetector( name ),
printLevel(0), drawLevel(0)
{
    collectionName.insert( name );
    G4String photonName = Calorimeter::addPhotonToName(name);
    collectionName.insert( photonName ) ;
        
    // Register with SDManager
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    SDman->AddNewDetector(this);
}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
gm2ringsim::PhotodetectorSD::~PhotodetectorSD(){}

void gm2ringsim::PhotodetectorSD::Initialize(G4HCofThisEvent* HCoTE){
    
    thisHC = new PhotodetectorHitsCollection
    ( SensitiveDetectorName, collectionName[0]);
    thisPhotonHC = new PhotodetectorPhotonHitsCollection
    ( SensitiveDetectorName, collectionName[1]);
    
    G4int const thisHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(thisHC);
    G4int const thisPhotonHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(thisPhotonHC);
    
    // Add to HCoTE
    HCoTE->AddHitsCollection( thisHCID, thisHC );
    HCoTE->AddHitsCollection( thisPhotonHCID, thisPhotonHC );
    
    if (photodetectorID_.size() < nPhotodetectors_) photodetectorID_.resize(nPhotodetectors_);
    for( unsigned int i = 0 ; i < photodetectorID_.size() ; ++i )
    {
        photodetectorID_[i] = -1 ;
    }
}

G4bool gm2ringsim::PhotodetectorSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){
    G4Track* track = thisStep->GetTrack() ;
    
    
    // get local position & momentum (to check that this photon should make a hit --
    //     don't want to include photons that reflect off photodetector face or enter sides of photodetector)
    G4ThreeVector global_pos = thisStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector global_mom = thisStep->GetPreStepPoint()->GetMomentum();
    G4TouchableHandle const touchy = thisStep->GetPreStepPoint()->GetTouchableHandle();
    G4ThreeVector local_pos = touchy->GetHistory()->GetTopTransform().TransformPoint(global_pos);
    G4VTouchable const *feely = thisStep->GetPreStepPoint()->GetTouchable();
    G4RotationMatrix const *rot = feely->GetRotation();
    G4ThreeVector local_mom = global_mom;
    local_mom.transform(*rot);
    
    
    //   if( track->GetParticleDefinition()->GetPDGEncoding() == 0 ) // opticalphoton
    if( track->GetDefinition()->GetPDGEncoding() == 0 ) // opticalphoton
    {
        
        if (local_mom.z() < 0) return true; // photon reflected off face of photodetector: do not process the hit
        // and do not kill the track
        
        // make sure photon enters front face of photodetector:
        G4ThreeVector origin(0,0,0);
        G4ThreeVector minuszhat(0,0,-1); //Due to rotation, use -z direction of photodetector (local coords.)
        G4double photodetectorFront = -feely->GetSolid()->DistanceToOut(origin,minuszhat); // local z-coord of photodetector front
        if (fabs(local_pos.z() - photodetectorFront) < 0.0001) // checks that photon entered front face of photodetector
        {                // if not, skip processing the hit (but still kill the track at the end)
            
            // Store ALL photons
            PhotodetectorPhotonHit* ph = new PhotodetectorPhotonHit(thisStep) ;
            thisPhotonHC->insert(ph);
            
            // Apply photon detection efficiency
            double eff = 0. ;
            double wavelength = 0.001240 * MeV * nm /
            thisStep->GetPreStepPoint()->GetTotalEnergy() ;
            double x1, x2, y1, y2;
            
            
            // Apply SiPM photon detection efficiency (Garutti arXiv:1108.3166)
            if( wavelength < 332.*nm)
            {
                x1 = 303.*nm;
                y1 = 0.000;
                x2 = 332.*nm;
                y2 = 0.1424;
            }
            else if(332.*nm <= wavelength && wavelength < 370.*nm)
            {
                x1 = 332.*nm;
                y1 = 0.1424;
                x2 = 370.*nm;
                y2 = 0.2155;
            }
            else if(370.*nm <= wavelength && wavelength < 411.*nm)
            {
                x1 = 370.*nm;
                y1 = 0.2155;
                x2 = 411.*nm;
                y2 = 0.2663;
            }
            else if(411.*nm <= wavelength && wavelength < 451.*nm)
            {
                x1 = 411.*nm;
                y1 = 0.2663;
                x2 = 451.*nm;
                y2 = 0.3000;
            }
            else if(451.*nm <= wavelength && wavelength < 467.*nm)
            {
                x1 = 451.*nm;
                y1 = 0.3000;
                x2 = 467.*nm;
                y2 = 0.31099;
            }
            else if(467.*nm <= wavelength && wavelength < 480.*nm)
            {
                x1 = 467.*nm;
                y1 = 0.31099;
                x2 = 480.*nm;
                y2 = 0.31363;
            }
            else if(480.*nm <= wavelength && wavelength < 491.*nm)
            {
                x1 = 480.*nm;
                y1 = 0.31363;
                x2 = 491.*nm;
                y2 = 0.31212;
            }
            else if(491.*nm <= wavelength && wavelength < 510.*nm)
            {
                x1 = 491.*nm;
                y1 = 0.31212;
                x2 = 510.*nm;
                y2 = 0.30189;
            }
            else if(510.*nm <= wavelength && wavelength < 530.*nm)
            {
                x1 = 510.*nm;
                y1 = 0.30189;
                x2 = 530.*nm;
                y2 = 0.28977;
            }
            else if(530.*nm <= wavelength && wavelength < 560.*nm)
            {
                x1 = 530.*nm;
                y1 = 0.28977;
                x2 = 560.*nm;
                y2 = 0.2621;
            }
            else if(560.*nm <= wavelength && wavelength < 580.*nm)
            {
                x1 = 560.*nm;
                y1 = 0.2621;
                x2 = 580.*nm;
                y2 = 0.2375;
            }
            else if(580.*nm <= wavelength && wavelength < 614.*nm)
            {
                x1 = 580.*nm;
                y1 = 0.2375;
                x2 = 614.*nm;
                y2 = 0.2091;
            }
            else if(614.*nm <= wavelength && wavelength < 650.*nm)
            {
                x1 = 614.*nm;
                y1 = 0.2091;
                x2 = 650.*nm;
                y2 = 0.1788;
            }
            else if(650.*nm <= wavelength && wavelength < 691.*nm)
            {
                x1 = 650.*nm;
                y1 = 0.1788;
                x2 = 691.*nm;
                y2 = 0.1496;
            }
            else if(691.*nm <= wavelength && wavelength < 751.*nm)
            {
                x1 = 691.*nm;
                y1 = 0.1496;
                x2 = 751.*nm;
                y2 = 0.1098;
            }
            else // (751.*nm <= wavelength )
            {
                x1 = 751.*nm;
                y1 = 0.1098;
                x2 = 930.*nm;
                y2 = 0.000;
            }
            
            eff = (y2-y1)/(x2-x1) * (wavelength - x1) + y1;
            
            
            //       // Apply Hamamatsu R9800 detection efficiency (from Hamamatsu data sheet)
            //       if( wavelength < 300.*nm )
            // 	{
            // 	  x1 = 297.*nm;
            // 	  y1 = 0.00997;
            // 	  x2 = 300.*nm;
            // 	  y2 = 0.027;
            // 	}
            //       else if(300.*nm <= wavelength && wavelength < 307.*nm)
            // 	{
            // 	  x1 = 300.*nm;
            // 	  y1 = 0.027;
            // 	  x2 = 307.*nm;
            // 	  y2 = 0.06;
            // 	}
            //       else if(307.*nm <= wavelength && wavelength < 333.*nm)
            // 	{
            // 	  x1 = 307.*nm;
            // 	  y1 = 0.06;
            // 	  x2 = 333.*nm;
            // 	  y2 = 0.159;
            // 	}
            //       else if(333.*nm <= wavelength && wavelength < 363.*nm)
            // 	{
            // 	  x1 = 333.*nm;
            // 	  y1 = 0.159;
            // 	  x2 = 363.*nm;
            // 	  y2 = 0.224;
            // 	}
            //       else if(363.*nm <= wavelength && wavelength < 393.*nm)
            // 	{
            // 	  x1 = 363.*nm;
            // 	  y1 = 0.224;
            // 	  x2 = 393.*nm;
            // 	  y2 = 0.243;
            // 	}
            //       else if(393.*nm <= wavelength && wavelength < 424.*nm)
            // 	{
            // 	  x1 = 393.*nm;
            // 	  y1 = 0.243;
            // 	  x2 = 424.*nm;
            // 	  y2 = 0.234;
            // 	}
            //       else if(424.*nm <= wavelength && wavelength < 483.*nm)
            // 	{
            // 	  x1 = 424.*nm;
            // 	  y1 = 0.234;
            // 	  x2 = 393.*nm;
            // 	  y2 = 0.173;
            // 	}
            //       else if(483.*nm <= wavelength && wavelength < 534.*nm)
            // 	{
            // 	  x1 = 483.*nm;
            // 	  y1 = 0.173;
            // 	  x2 = 534.*nm;
            // 	  y2 = 0.099;
            // 	}
            //       else if(534.*nm <= wavelength && wavelength < 615.*nm)
            // 	{
            // 	  x1 = 534.*nm;
            // 	  y1 = 0.099;
            // 	  x2 = 615.*nm;
            // 	  y2 = 0.019;
            // 	}
            //       else if(615.*nm <= wavelength && wavelength < 650.*nm)
            // 	{
            // 	  x1 = 615.*nm;
            // 	  y1 = 0.019;
            // 	  x2 = 650.*nm;
            // 	  y2 = 0.006;
            // 	}
            //       else if(650.*nm <= wavelength && wavelength < 687.*nm)
            // 	{
            // 	  x1 = 650.*nm;
            // 	  y1 = 0.006;
            // 	  x2 = 687.*nm;
            // 	  y2 = 0.001;
            // 	}
            //       else // (687.*nm <= wavelength )
            // 	{
            // 	  x1 = 687.*nm;
            // 	  y1 = 0.001;
            // 	  x2 = 700.*nm;
            // 	  y2 = 0.000;
            // 	}
            
            //       eff = (y2-y1)/(x2-x1) * (wavelength - x1) + y1;
            
            
            
            //       // Apply photon detection efficiency (from Hamamatsu paper)
            //       double eff = 0. ;
            //       double wavelength = 0.001240 * MeV * nm /
            // 	 thisStep->GetPreStepPoint()->GetTotalEnergy() ;
            
            //       if( wavelength > 450.*nm )
            //       {
            // 	 eff = -0.60 / ( 600.*nm ) * ( wavelength - 300.*nm ) + 0.65 ;
            //       }
            //       else if( wavelength > 330.*nm )
            //       {
            // 	 eff = 0.13 / ( 120.*nm ) * ( wavelength - 330.*nm ) + 0.37 ;
            //       }
            //       else
            //       {
            // 	 eff = 0.24 / ( 10.*nm ) * ( wavelength - 320.*nm ) + 0.13 ;
            //       }
            
            
            if( eff < 0. ) eff = 0. ;
            
            double rand = CLHEP::RandFlat::shoot() ;
            if( rand > eff ) // photon not accepted by detector
            {
                PhotonHitCorrelator::getInstance().registerPhotodetectorTrack( track->GetTrackID(), false );
                thisStep->GetTrack()->SetTrackStatus(fStopAndKill);
                return true ; // a photon hit has been generated, even if not accepted
            }
            
            // Passed efficiency simulation; now make Photodetector hits.
            
            // Mark stored photon as accepted
            ph->accepted = true ;
            PhotonHitCorrelator::getInstance().registerPhotodetectorTrack( track->GetTrackID(), true );
            
            //       std::cout << "Photodetector Track " << track->GetTrackID()
            // 		<< " name " << track->GetParticleDefinition()->GetParticleName()
            // 		<< " mat " << track->GetMaterial()->GetName()
            // 		<< std::endl
            // 		<< " pos " << track->GetPosition()
            // 		<< " mom " << track->GetMomentum()
            // 		<< std::endl ;
            
            G4TouchableHistory* hist =
            (G4TouchableHistory*)(thisStep->GetPreStepPoint()->GetTouchable());
            //const G4VPhysicalVolume* physVol = hist->GetVolume();
            G4int copyID = hist->GetReplicaNumber();
            
            //       std::cout << "Photodetector copy " << copyID
            // 		<< " track " << track->GetTrackID()
            // 		<< " pdg " << track->GetParticleDefinition()->GetPDGEncoding()
            // 		<< std::endl ;
            
            if(photodetectorID_[copyID]==-1)
            {
                PhotodetectorHit* pHit = new PhotodetectorHit( thisStep );
                pHit->energy = thisStep->GetPreStepPoint()->GetTotalEnergy() ;
                ++( pHit->nphoton ) ;
                
                G4int icell = thisHC->insert( pHit );
                photodetectorID_[copyID] = icell - 1;
                
                //  	 std::cout << " New PhotodetectorHit on photodetectorID " << copyID
                //  		   << " track " << track->GetTrackID()
                // 		   << " parent " << track->GetParentID()
                //  		   << " pdg " << track->GetParticleDefinition()->GetPDGEncoding()
                //  		   << " energy " << pHit->energy
                //  		   << " nphoton " << pHit->nphoton << std::endl ;
            }
            else
            { 
                ( *thisHC )[ photodetectorID_[ copyID ] ]->energy +=
                thisStep->GetPreStepPoint()->GetTotalEnergy() ;
                ++( ( *thisHC )[ photodetectorID_[ copyID ] ]->nphoton ) ;
                
                // 	 std::cout << " Incremented PhotodetectorHit on photodetectorID " << copyID
                // 		   << " track " << track->GetTrackID()
                // 		   << " pdg " << track->GetParticleDefinition()->GetPDGEncoding()
                // 		   << " energy " << ( *thisHC )[ photodetectorID_[ copyID ] ]->energy
                // 		   << " nphoton " << ( *thisHC )[ photodetectorID_[ copyID ] ]->nphoton
                // 		   << std::endl ;
            }
        }
    }
    
    thisStep->GetTrack()->SetTrackStatus(fStopAndKill);
    
    return true;
}

void gm2ringsim::PhotodetectorSD::EndOfEvent(G4HCofThisEvent*) {
    
    G4int n = thisHC->entries();
    G4int np = thisPhotonHC->entries() ;
    
    if(! (printLevel > 0 || drawLevel > 0) )
        return;
    
    if(printLevel > 0 && (n>0||np>0)){
        G4cout << "Sensitive Detector: " << SensitiveDetectorName << '\n';
        for(G4int i=0; i<n; ++i){
            G4cout << '\t';
            (*thisHC)[i]->Print();
        }
        for(G4int i=0; i<np; ++i){
            G4cout << '\t';
            (*thisPhotonHC)[i]->Print();
        }
    }
    
    if(drawLevel > 0 && (n>0||np>0))
    {
        for(G4int i=0; i<n; ++i)
            (*thisHC)[i]->Draw();
        for(G4int i=0; i<np; ++i)
            (*thisPhotonHC)[i]->Draw();
    }
}


G4int gm2ringsim::PhotodetectorSD::PrintLevel(G4int newLevel){
    G4int temp = printLevel;
    printLevel = newLevel;
    return temp;
}

G4int gm2ringsim::PhotodetectorSD::DrawLevel(G4int newLevel){
    G4int temp = drawLevel;
    drawLevel = newLevel;
    return temp;
}