// This is the ringTrackerAnalyzer Art module.

//  This is an analyzer that will read HitData from the event and make some plots and a root tree

// Art includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

// Root + Art includes
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector3.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "artg4/pluginActions/physicalVolumeStore/PhysicalVolumeStoreData.hh"
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"
#include "artg4/util/DataFromRunOrService.hh"

// Hit includes
#include "gm2ringsim/traceback/StrawArtRecord.hh"
#include "gm2ringsim/common/tracker/TrackerArtRecord.hh"
#include "gm2ringsim/common/ring/RingArtRecord.hh"
#include "gm2ringsim/actions/track/TrackingActionArtRecord.hh"
#include "gm2ringsim/inflector/InflectorArtRecord.hh"

#include <map>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

#define Rhobin_Inflector 400
#define Rhomin_Inflector -200
#define Rhomax_Inflector 200

#define Xbin_Inflector 100
#define Xmin_Inflector -50
#define Xmax_Inflector 50

#define Ybin_Inflector 100
#define Ymin_Inflector -50
#define Ymax_Inflector 50

#define Vbin_Inflector 100
#define Vmin_Inflector -50
#define Vmax_Inflector 50

#define Zbin_Inflector 400
#define Zmin_Inflector -200
#define Zmax_Inflector 200

#define XPrimebin_Inflector 100
#define XPrimemin_Inflector -25
#define XPrimemax_Inflector 25

#define YPrimebin_Inflector 100
#define YPrimemin_Inflector -25
#define YPrimemax_Inflector 25

#define Rhobin_Ring 700
#define Rhomin_Ring -300
#define Rhomax_Ring 400

#define Xbin_Ring 240
#define Xmin_Ring -60.0
#define Xmax_Ring 200.0
#define Ybin_Ring 120
#define Ymin_Ring -60.0
#define Ymax_Ring 60.0
#define Rbin_Ring 1800
#define Rmin_Ring -9000.0
#define Rmax_Ring 9000.0

#define YPrimebin_Ring 200
#define YPrimemin_Ring -25
#define YPrimemax_Ring 25
#define XPrimebin_Ring 200
#define XPrimemin_Ring -50
#define XPrimemax_Ring 50

using namespace std;

// Namespace
namespace gm2ringsim {
  class ringTrackerAnalyzer;
}

// The class
class gm2ringsim::ringTrackerAnalyzer : public art::EDAnalyzer {
public:
  explicit ringTrackerAnalyzer(fhicl::ParameterSet const &p);
  virtual ~ringTrackerAnalyzer();
  
  virtual void analyze(art::Event const &e) override;
  
  
private:
  
  // Declare member data here.
  
  std::string ringtrackerhitModuleLabel_;
  std::string truthhitModuleLabel_;
  std::string inflectorhitModuleLabel_;
  std::string ringhitModuleLabel_;
  std::string ringtrackerinstanceName_;
  std::string truthinstanceName_;
  std::string inflectorinstanceName_;
  std::string ringinstanceName_;

  // Physical Volume Store data
  std::string pvsProducerLabel_;
  std::string pvsInstanceLabel_;


  
  // Names of the sub-directories in the root file
  std::string hist_dir_, tree_dir_;

  std::string beamstart_;
  int maxturnsreal_;

  bool fill;
  
  double Ndegat0;
  double Nxpat0;
  int Ngen;
  int Nstart_inflector;
  int Nexit_inflector;
  int Nstart_ring;
  int Nstored1_ring;
  int Nstored10_ring;
  int Nstored100_ring;


  int Nringtrackers;
  TH2F *h_RingTracker_FirstTurnX[2];
  TH2F *h_RingTracker_FirstTurnY[2];
  TH1F *h_RingTracker_DegreeAtRhat0[2];
  TH2F *h_RingTracker_DegreeAtRhat0RhatInit[2];
  TH1F *h_RingTracker_DegreeAtQuad[2];
  TH1F *h_RingTracker_KickAtRhat0[2];
  TH2F *h_RingTracker_KickAtRhat0RhatInit[2];

  TH2F *h_RingTracker_RhoY[17];
  TH2F *h_RingTracker_XZ[17];
  TH2F *h_RingTracker_XprimeX[17];
  TH2F *h_RingTracker_YprimeY[17];
  TH1F *h_RingTracker_DeltaPy[17];
  TH2F *h_RingTracker_XprimeXTime[5];
  TH2F *h_RingTracker_YprimeYTime[5];
  TH2F *h_RingTracker_XprimeYprimeTime[5];
  TH2F *h_RingTracker_RhoYTime[5];
  TH1F *h_RingTracker_dNdX;
  TH1F *h_RingTracker_dNdXprime;
  TH1F *h_RingTracker_dNdY;
  TH1F *h_RingTracker_dNdYprime;
  TH2F *h_RingTracker_d2NdXY;
  TH2F *h_RingTracker_d2NdXprimeYprime;
  TH2F *h_RingTracker_d2NdXprimeX;
  TH2F *h_RingTracker_d2NdYprimeY;

  


  int Nstoredtrackers;
  TH2F *h_RingTracker_Stored_RhoY[11];
  TH1F *h_RingTracker_Stored_Momentum[11];
  TH2F *h_RingTracker_Stored_XprimeX[11];
  TH2F *h_RingTracker_Stored_YprimeY[11];
  TH2F *h_RingTracker_Stored_XprimeYprime[11];
  TH1F *h_RingTracker_Stored_thetaX[11];
  TH1F *h_RingTracker_Stored_thetaY[11];
  TH2F *h_RingTracker_Stored_thetaXY[11];
  TH2F *h_RingTracker_Rho_Time;
  TH2F *h_RingTracker_Prhat_Time;
  TH2F *h_RingTracker_Pvhat_Time;
  TH2F *h_RingTracker_Mom_Time;
  TH2F *h_RingTracker_Y_Time;

  TH2F *h_RingTracker_Rho_Turn;
  TH2F *h_RingTracker_Prhat_Turn;
  TH2F *h_RingTracker_Pvhat_Turn;
  TH2F *h_RingTracker_Mom_Turn;
  TH2F *h_RingTracker_Y_Turn;  




  TH2F *h_G4Tracker_XprimeXTime[5];
  TH2F *h_G4Tracker_YprimeYTime[5];
  TH2F *h_G4Tracker_RhoYTime[5];
  TH2F *h_G4Tracker_XZTime[5];
  TH1F *h_G4Tracker_MomTime[5];
  TH1F *h_G4Tracker_t0Time[5];



  int Nsystemtrackers;
  string rhitnames[9];
  int kInflectorHit;
  int kStrawSystemHit;
  int kCaloSystemHit;
  int kXtalSystemHit;
  TH1F *h_SystemHitTracker_Nhits[10][2];
  TH2F *h_SystemHitTracker_XZ[10][2];
  TH2F *h_SystemHitTracker_RhoY[10][2];
  TH2F *h_SystemHitTracker_RhoTime[10][2];
  TH2F *h_SystemHitTracker_YTime[10][2];
  TH1F *h_SystemHitTracker_DeltaPx[10][2];
  TH1F *h_SystemHitTracker_DeltaPy[10][2];

  int Nringhits;
  string inames[22];
  TH1F *h_RingHitTracker_Nhits[25][2];
  TH2F *h_RingHitTracker_XZ[25][2];
  TH2F *h_RingHitTracker_RhoY[25][2];
  TH2F *h_RingHitTracker_RhoTime[25][2];
  TH2F *h_RingHitTracker_YTime[25][2];
  TH1F *h_RingHitTracker_DeltaPx[25][2];
  TH1F *h_RingHitTracker_DeltaPy[25][2];




  // Loop over the 8 tracking detectors "virtual"
  int Ninflectortrackers;
  TH2F *h_InflectorTracker_XprimeX_VD[12];
  TH2F *h_InflectorTracker_YprimeY_VD[12];
  TH2F *h_InflectorTracker_RhoY_VD[12];
  TH2F *h_InflectorTracker_XZ_VD[12];


  TVector3 xAxis, yAxis, zAxis;
  TVector3 beamStart;
  double rhat_offset, yhat_offset;
};

gm2ringsim::ringTrackerAnalyzer::ringTrackerAnalyzer(fhicl::ParameterSet const &p) :
  ringtrackerhitModuleLabel_ ( p.get<std::string>("ringtrackerhitModuleLabel",  "artg4"   ) ),
  truthhitModuleLabel_ ( p.get<std::string>("truthhitModuleLabel",  "artg4"   ) ),
  inflectorhitModuleLabel_ ( p.get<std::string>("inflectorhitModuleLabel",  "artg4"   ) ),
  ringhitModuleLabel_ ( p.get<std::string>("ringhitModuleLabel",  "artg4"   ) ),
  ringtrackerinstanceName_   ( p.get<std::string>("ringtrackerinstanceName",    "Tracker"     ) ),
  truthinstanceName_   ( p.get<std::string>("truthinstanceName",    "TrackingAction"     ) ),
  inflectorinstanceName_   ( p.get<std::string>("inflectorinstanceName",    "inflector"     ) ),
  ringinstanceName_   ( p.get<std::string>("ringinstanceName",    "Ring"     ) ),
  pvsProducerLabel_( p.get<std::string>("pvsProducerLabel", "artg4")),
  pvsInstanceLabel_( p.get<std::string>("pvsInstanceLabel", "")),
  hist_dir_       ( p.get<std::string>("hist_dir"         ) ),
  tree_dir_       ( p.get<std::string>("tree_dir"         ) ),
  beamstart_      ( p.get<std::string>("beamstart") ),
  maxturnsreal_   ( p.get<int>("maxturns", 200) )
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


  Ndegat0 = 0.0;
  Nxpat0 = 0.0;
  Ngen = 0;
  Nstart_inflector = 0;
  Nexit_inflector = 0;
  Nstart_ring = 0;
  Nstored1_ring = 0;
  Nstored10_ring = 0;
  Nstored100_ring = 0;

  Nstoredtrackers = 11;
  Ninflectortrackers = 9;
  Nringtrackers = 17;

  Nsystemtrackers = 9;
  rhitnames[0] = "Inflector";
  rhitnames[1] = "Cryostat" ;
  rhitnames[2] = "Quad" ;
  rhitnames[3] = "Kicker" ;
  rhitnames[4] = "Collimator" ;
  rhitnames[5] = "Vacuum" ;
  rhitnames[6] = "StrawTracker" ;
  rhitnames[7] = "Calo" ;
  rhitnames[8] = "Xtal";
  kInflectorHit = 0;
  kStrawSystemHit = 6;
  kCaloSystemHit = 7;
  kXtalSystemHit = 8;

  Nringhits = 22;  
  inames[0] = "UpstreamEndFlange"; 
  inames[1] =  "UpstreamEquivalentNbTi"; 
  inames[2] = "UpstreamEquivalentAl"; 
  inames[3] = "UpstreamEquivalentCu"; 
  inames[4] = "UpstreamWindow"; 
  inames[5] = "DownstreamWindow"; 
  inames[6] = "DownstreamEquivalentNbTi"; 
  inames[7] = "DownstreamEquivalentAl"; 
  inames[8] = "DownstreamEquivalentCu"; 
  inames[9] = "DownstreamEndFlange"; 
  inames[10] =  "Mandrel"; 
  inames[11] = "Quad10"; 
  inames[12] = "Quad20"; 
  inames[13] = "Quad30"; 
  inames[14] = "Quad40"; 
  inames[15] = "Quad11"; 
  inames[16] = "Quad21"; 
  inames[17] = "Quad31"; 
  inames[18] = "Quad41"; 
  inames[19] = "Kicker1"; 
  inames[20] = "Kicker2"; 
  inames[21] = "Kicker3";

  fill = true;

  stringstream hname;
  string times[5] = {"-", "5us", "20us", "50us", "Final"};
  for ( int t = 0; t < 5; t++ ) {
    string time = times[t];
    if ( times[t] == "-" ) { time = ""; }

    hname << "RingTracker_XprimeXTime" << time;
    h_RingTracker_XprimeXTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring/4, Rhomin_Ring, Rhomax_Ring, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
    h_RingTracker_XprimeXTime[t]->SetXTitle("x_{ring} [mm]");
    h_RingTracker_XprimeXTime[t]->SetYTitle("x'_{ring} (Px/Pz) [mrad]");
    hname.str("");
      
    hname << "RingTracker_YprimeYTime" << time;
    h_RingTracker_YprimeYTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", Ybin_Ring, Ymin_Ring, Ymax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
    h_RingTracker_YprimeYTime[t]->SetXTitle("y_{ring} [mm]");
    h_RingTracker_YprimeYTime[t]->SetYTitle("y'_{ring} (Py/Pz) [mrad]");
    hname.str("");
      
    hname << "RingTracker_RhoYTime" << time;
    h_RingTracker_RhoYTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring/4, Rhomin_Ring, Rhomax_Ring, Ybin_Ring, Ymin_Ring, Ymax_Ring);
    h_RingTracker_RhoYTime[t]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
    h_RingTracker_RhoYTime[t]->SetYTitle("y_{ring} [mm]");
    hname.str("");
      
    hname << "RingTracker_XprimeYprimeTime" << time;
    h_RingTracker_XprimeYprimeTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
    h_RingTracker_XprimeYprimeTime[t]->SetXTitle("x'_{ring} (Px/Pz) [mrad]");
    h_RingTracker_XprimeYprimeTime[t]->SetYTitle("y'_{ring} (Py/Pz) [mrad]");
    hname.str("");
  }
  
  
  hname << "RingTracker_dNdX";
  h_RingTracker_dNdX = histDir.make<TH1F>(hname.str().c_str(), "", Rhobin_Ring/4, Rhomin_Ring, Rhomax_Ring);
  h_RingTracker_dNdX->SetXTitle("x_{e} [mm]");
  hname.str("");
    
  hname << "RingTracker_dNdY";
  h_RingTracker_dNdY = histDir.make<TH1F>(hname.str().c_str(), "", Ybin_Ring, Ymin_Ring, Ymax_Ring);
  h_RingTracker_dNdY->SetXTitle("y_{e} [mm]");
  hname.str("");
    
  hname << "RingTracker_d2NdXY";
  h_RingTracker_d2NdXY = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring/4, Rhomin_Ring, Rhomax_Ring, Ybin_Ring, Ymin_Ring, Ymax_Ring);
  h_RingTracker_d2NdXY->SetXTitle("x_{e} [mm]");
  h_RingTracker_d2NdXY->SetYTitle("y_{e} [mm]");
  hname.str("");


  hname << "RingTracker_dNdXprime";
  h_RingTracker_dNdXprime = histDir.make<TH1F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
  h_RingTracker_dNdXprime->SetXTitle("x'_{e} [mm]");
  hname.str("");
    
  hname << "RingTracker_dNdYprime";
  h_RingTracker_dNdYprime = histDir.make<TH1F>(hname.str().c_str(), "", YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
  h_RingTracker_dNdYprime->SetXTitle("y'_{e} [mm]");
  hname.str("");
    
  hname << "RingTracker_d2NdXprimeYprime";
  h_RingTracker_d2NdXprimeYprime = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
  h_RingTracker_d2NdXprimeYprime->SetYTitle("x'_{e} [mm]");
  h_RingTracker_d2NdXprimeYprime->SetXTitle("y'_{e} [mm]");
  hname.str("");
    
  hname << "RingTracker_dNdXprimeX";
  h_RingTracker_d2NdXprimeX = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring/4, Rhomin_Ring, Rhomax_Ring, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
  h_RingTracker_d2NdXprimeX->SetYTitle("x' [mm]");
  h_RingTracker_d2NdXprimeX->SetXTitle("x_{e} [mm]");
  hname.str("");
    
  hname << "RingTracker_dNdYprimeY";
  h_RingTracker_d2NdYprimeY = histDir.make<TH2F>(hname.str().c_str(), "", Ybin_Ring, Ymin_Ring, Ymax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
  h_RingTracker_d2NdYprimeY->SetYTitle("y' [mm]");
  h_RingTracker_d2NdYprimeY->SetXTitle("y_{e} [mm]");
  hname.str("");



  for ( int st = 0; st < 2; st++ ) {
    string stname = "";
    if ( st == 1 ) { stname = "_Stored"; }
      
    hname << "RingTracker" << stname << "_DegreeAtRhat0";
    h_RingTracker_DegreeAtRhat0[st] = histDir.make<TH1F>(hname.str().c_str(), "", 180, 0, 180.0);
    h_RingTracker_DegreeAtRhat0[st]->SetXTitle("#theta_{ring}(R=R_{magic}) [deg]");
    hname.str(""); 
    
    hname << "RingTracker" << stname << "_DegreeAtRhat0RhatInit";
    h_RingTracker_DegreeAtRhat0RhatInit[st] = histDir.make<TH2F>(hname.str().c_str(), "", 180, 0, 180.0, Rhobin_Ring, Rhomin_Ring, Rhomax_Ring);
    h_RingTracker_DegreeAtRhat0RhatInit[st]->SetXTitle("#theta_{ring}(x=0) [deg]");
    h_RingTracker_DegreeAtRhat0RhatInit[st]->SetYTitle("x_{ring} #equiv (R - R_{magic})(#theta=0) [mm]");
    hname.str(""); 
      
    hname << "RingTracker" << stname << "_DegreeAtQuad";
    h_RingTracker_DegreeAtQuad[st] = histDir.make<TH1F>(hname.str().c_str(), "", 180, 0, 180.0);
    h_RingTracker_DegreeAtQuad[st]->SetXTitle("#theta_{ring}(R=R_{magic}+50 mm) [deg]");
    hname.str(""); 
      
    hname << "RingTracker" << stname << "_KickAtRhat0";
    h_RingTracker_KickAtRhat0[st] = histDir.make<TH1F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
    h_RingTracker_KickAtRhat0[st]->SetXTitle("x'_{ring}(R=R_{magic})(x=0) [mrad]");
    hname.str(""); 
    
    hname << "RingTracker" << stname << "_KickAtRhat0RhatInit";
    h_RingTracker_KickAtRhat0RhatInit[st] = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, Rhobin_Ring, Rhomin_Ring, Rhomax_Ring);
    h_RingTracker_KickAtRhat0RhatInit[st]->SetXTitle("x'_{ring}(x=0) [mrad]");
    h_RingTracker_KickAtRhat0RhatInit[st]->SetYTitle("x_{ring} #equiv (R - R_{magic})(#theta=0) [mm]");
    hname.str(""); 
    
    hname << "RingTracker" << stname << "_FirstTurnX";
    h_RingTracker_FirstTurnX[st] = histDir.make<TH2F>(hname.str().c_str(), "", 360, 0, 360.0, Rhobin_Ring, Rhomin_Ring, Rhomax_Ring);
    h_RingTracker_FirstTurnX[st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
    h_RingTracker_FirstTurnX[st]->SetXTitle("#theta_{ring} [deg]");
    hname.str(""); 
    
    hname << "RingTracker" << stname << "_FirstTurnY";
    h_RingTracker_FirstTurnY[st] = histDir.make<TH2F>(hname.str().c_str(), "", 360, 0, 360.0, Ybin_Ring, Ymin_Ring, Ymax_Ring);
    h_RingTracker_FirstTurnY[st]->SetYTitle("y_{ring} #equiv R - R_{m} [mm]");
    h_RingTracker_FirstTurnY[st]->SetXTitle("#theta_{ring} [deg]");
    hname.str(""); 
  }
  

  string rtnames[17] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16"};
  Nringtrackers = 17;
  for ( int i = 0; i < Nringtrackers; i++ ) {
    hname << "RingTracker_" << rtnames[i] << "_RhoY";
    h_RingTracker_RhoY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring, Rhomin_Ring, Rhomax_Ring, Ybin_Ring, Ymin_Ring, Ymax_Ring);
    h_RingTracker_RhoY[i]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
    h_RingTracker_RhoY[i]->SetYTitle("y_{ring} [mm]");
    hname.str(""); 
    
    hname << "RingTracker_" << rtnames[i] << "_XZ";
    h_RingTracker_XZ[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring, Rmin_Ring, Rmax_Ring, Rbin_Ring, Rmin_Ring, Rmax_Ring);
    h_RingTracker_XZ[i]->SetXTitle("x_{ring} [mm]");
    h_RingTracker_XZ[i]->SetYTitle("z_{ring} [mm]");
    hname.str(""); 

    hname << "RingTracker_" << rtnames[i] << "_YprimeY";
    h_RingTracker_YprimeY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Ybin_Ring, Ymin_Ring, Ymax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
    h_RingTracker_YprimeY[i]->SetXTitle("y_{ring} [mm]");
    h_RingTracker_YprimeY[i]->SetYTitle("y'_{ring} (Py/Pz) [mrad]");
    hname.str("");

    hname << "RingTracker_" << rtnames[i] << "_XprimeX";
    h_RingTracker_XprimeX[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring, Rhomin_Ring, Rhomax_Ring, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
    h_RingTracker_XprimeX[i]->SetXTitle("x_{ring} [mm]");
    h_RingTracker_XprimeX[i]->SetYTitle("x'_{ring} (Px/Pz) [mrad]");
    hname.str("");

    hname << "RingTracker_" << rtnames[i] << "_DeltaPy";
    h_RingTracker_DeltaPy[i] = histDir.make<TH1F>(hname.str().c_str(), "", 200, -1e-2, 1e-2);
    h_RingTracker_DeltaPy[i]->SetXTitle("#Delta #hat{P}_{v} [mrad]");
    hname.str("");
  } 

  string rtsnames[11] = {"0Turn", "1Turn", "2Turn", "5Turn", "10Turn", "50Turn", "100Turn", "200Turn", "500Turn", "1000Turn", "2000Turn"};
  Nstoredtrackers = 11;
  for ( int i = 0; i < Nstoredtrackers; i++ ) {
    hname << "RingTracker_" << rtsnames[i] << "_RhoY";
    h_RingTracker_Stored_RhoY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring, Rhomin_Ring, Rhomax_Ring, Ybin_Ring, Ymin_Ring, Ymax_Ring);
    h_RingTracker_Stored_RhoY[i]->SetXTitle("#rho_{ring} #equiv R - R_{m} [mm]");
    h_RingTracker_Stored_RhoY[i]->SetYTitle("y_{ring} [mm]");
    hname.str(""); 
    
    hname << "RingTracker_" << rtsnames[i] << "_YprimeY";
    h_RingTracker_Stored_YprimeY[i] = histDir.make<TH2F>(hname.str().c_str(), "", 10*Ybin_Ring, Ymin_Ring, Ymax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
    h_RingTracker_Stored_YprimeY[i]->SetXTitle("y_{ring} [mm]");
    h_RingTracker_Stored_YprimeY[i]->SetYTitle("y'_{ring} (Py/Pz) [mrad]");
    hname.str("");
    
    hname << "RingTracker_" << rtsnames[i] << "_XprimeX";
    h_RingTracker_Stored_XprimeX[i] = histDir.make<TH2F>(hname.str().c_str(), "", 10*Rhobin_Ring, Rhomin_Ring, Rhomax_Ring, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
    h_RingTracker_Stored_XprimeX[i]->SetXTitle("x_{ring} [mm]");
    h_RingTracker_Stored_XprimeX[i]->SetYTitle("x'_{ring} (Px/Pz) [mrad]");
    hname.str("");
    
    hname << "RingTracker_" << rtsnames[i] << "_XprimeYprime";
    h_RingTracker_Stored_XprimeYprime[i] = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
    h_RingTracker_Stored_XprimeYprime[i]->SetXTitle("x'_{ring} (Px/Pz) [mrad]");
    h_RingTracker_Stored_XprimeYprime[i]->SetYTitle("y'_{ring} (Py/Pz) [mrad]");
    hname.str("");
    
    hname << "RingTracker_" << rtsnames[i] << "_thetaX";
    h_RingTracker_Stored_thetaX[i] = histDir.make<TH1F>(hname.str().c_str(), "", 10*XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
    h_RingTracker_Stored_thetaX[i]->SetXTitle("#theta_{ring} (sin^{-1}(Px/P)) [mrad]");
    hname.str("");
    
    hname << "RingTracker_" << rtsnames[i] << "_thetaY";
    h_RingTracker_Stored_thetaY[i] = histDir.make<TH1F>(hname.str().c_str(), "", 10*YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
    h_RingTracker_Stored_thetaY[i]->SetXTitle("#theta_{ring} (sin^{-1}(Py/P)) [mrad]");
    hname.str("");
    
    hname << "RingTracker_" << rtsnames[i] << "_thetaXY";
    h_RingTracker_Stored_thetaXY[i] = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
    h_RingTracker_Stored_thetaXY[i]->SetXTitle("#theta_{ring} (sin^{-1}(Px/P)) [mrad]");
    h_RingTracker_Stored_thetaXY[i]->SetYTitle("#theta_{ring} (sin^{-1}(Py/P)) [mrad]");
    hname.str("");
    
    hname << "RingTracker_" << rtsnames[i] << "_Momentum";
    h_RingTracker_Stored_Momentum[i] = histDir.make<TH1F>(hname.str().c_str(), "", 2600, 3090-1300, 3090+1300);
    h_RingTracker_Stored_Momentum[i]->SetXTitle("|P_{#mu}| [MeV]");
    hname.str("");
  }

  int maxturns = 200;
  
  hname << "TrackerRhoTime";
  h_RingTracker_Rho_Time = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, maxturns, 120, -60, 60);
  h_RingTracker_Rho_Time->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
  //h_RingTracker_Rho_Time->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Rho_Time->SetXTitle("time [#mus]");
  hname.str("");
  
  hname << "TrackerMomTime";
  h_RingTracker_Mom_Time = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, maxturns, 100, 3000, 3200);
  h_RingTracker_Mom_Time->SetYTitle("|P| [MeV]");
  //h_RingTracker_Mom_Time->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Mom_Time->SetXTitle("time [#mus]");
  hname.str("");
  
  hname << "TrackerPrhatTime";
  h_RingTracker_Prhat_Time = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, maxturns, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
  h_RingTracker_Prhat_Time->SetYTitle("P_{r} [MeV]");
  //h_RingTracker_Prhat_Time->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Prhat_Time->SetXTitle("time [#mus]");
  hname.str("");
  
  hname << "TrackerPvhatTime";
  h_RingTracker_Pvhat_Time = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, maxturns, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
  h_RingTracker_Pvhat_Time->SetYTitle("P_{v} [MeV]");
  //h_RingTracker_Pvhat_Time->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Pvhat_Time->SetXTitle("time [#mus]");
  hname.str("");
  
  hname << "TrackerYTime";
  h_RingTracker_Y_Time = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, maxturns, 120, -60, 60);
  h_RingTracker_Y_Time->SetYTitle("y_{ring} [mm]");
  //h_RingTracker_Y_Time->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Y_Time->SetXTitle("time [#mus]");
  hname.str("");
  
  
  
  hname << "TrackerRhoTurn";
  h_RingTracker_Rho_Turn = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, (double)maxturns, 120, -60, 60);
  h_RingTracker_Rho_Turn->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
  //h_RingTracker_Rho_Turn->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Rho_Turn->SetXTitle("Turn [#approx1/7#mus]");
  hname.str("");
  
  hname << "TrackerMomTurn";
  h_RingTracker_Mom_Turn = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, (double)maxturns, 100, 2900, 3200);
  h_RingTracker_Mom_Turn->SetYTitle("|P| [MeV]");
  //h_RingTracker_Mom_Turn->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Mom_Turn->SetXTitle("Turn [#approx1/7#mus]");
  hname.str("");
  
  hname << "TrackerPrhatTurn";
  h_RingTracker_Prhat_Turn = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, (double)maxturns, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
  h_RingTracker_Prhat_Turn->SetYTitle("P_{r} [MeV]");
  //h_RingTracker_Prhat_Turn->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Prhat_Turn->SetXTitle("Turn [#approx1/7#mus]");
  hname.str("");
  
  hname << "TrackerPvhatTurn";
  h_RingTracker_Pvhat_Turn = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, (double)maxturns, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
  h_RingTracker_Pvhat_Turn->SetYTitle("P_{v} [MeV]");
  //h_RingTracker_Pvhat_Turn->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Pvhat_Turn->SetXTitle("Turn [#approx1/7#mus]");
  hname.str("");
  
  hname << "TrackerYTurn";
  h_RingTracker_Y_Turn = histDir.make<TH2F>(hname.str().c_str(), "", maxturns*12, 0.0, (double)maxturns, 120, -60, 60);
  h_RingTracker_Y_Turn->SetYTitle("y_{ring} [mm]");
  //h_RingTracker_Y_Turn->SetXTitle("#frac{t - t_{0}}{#gamma_{#mu}#tau_{#mu}}");
  h_RingTracker_Y_Turn->SetXTitle("Turn [#approx1/7#mus]");
  hname.str("");
  




  for ( int t = 0; t < 5; t++ ) {
    string time = times[t];
    if ( times[t] == "-" ) { time = ""; }

    if ( fill ) {
      hname << "G4Track_XprimeXTime" << time;
      h_G4Tracker_XprimeXTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", Xbin_Ring, Xmin_Ring, Xmax_Ring, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
      h_G4Tracker_XprimeXTime[t]->SetXTitle("x_{gen} [mm]");
      h_G4Tracker_XprimeXTime[t]->SetYTitle("x'_{gen} (Px/Pz) [mrad]");
      hname.str("");
    
      hname << "G4Track_YprimeYTime" << time;
      h_G4Tracker_YprimeYTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", Ybin_Ring, Ymin_Ring, Ymax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
      h_G4Tracker_YprimeYTime[t]->SetXTitle("y_{gen} [mm]");
      h_G4Tracker_YprimeYTime[t]->SetYTitle("y'_{gen} (Py/Pz) [mrad]");
      hname.str("");
    
      hname << "G4Track_MomTime" << time;
      h_G4Tracker_MomTime[t] = histDir.make<TH1F>(hname.str().c_str(), "", 2600, 3090-1300, 3090+1300);
      h_G4Tracker_MomTime[t]->SetXTitle("|P_{#mu}| [MeV]");
      hname.str("");
    
      hname << "G4Track_t0Time" << time;
      h_G4Tracker_t0Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", 300, -100, 200);
      h_G4Tracker_t0Time[t]->SetXTitle("t_{0} [ns]");
      hname.str("");
    
      hname << "G4Track_RhoYTime" << time;
      h_G4Tracker_RhoYTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhobin_Ring, Rhomin_Ring, Rhomax_Ring, Ybin_Ring, Ymin_Ring, Ymax_Ring);
      h_G4Tracker_RhoYTime[t]->SetXTitle("x - x_{gen} [mm]");
      h_G4Tracker_RhoYTime[t]->SetYTitle("y - y_{gen} [mm]");
      hname.str("");
    
      hname << "G4Track_XZTime" << time;
      h_G4Tracker_XZTime[t] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring, Rmin_Ring, Rmax_Ring, Rbin_Ring, Rmin_Ring, Rmax_Ring);
      h_G4Tracker_XZTime[t]->SetXTitle("z_{ring} [mm]");
      h_G4Tracker_XZTime[t]->SetYTitle("x_{ring} [mm]");    
      hname.str("");
    }
  }
  




  
  for ( int i = 0; i < Ninflectortrackers; i++ ) {
    stringstream trackername;
    if ( i < Ninflectortrackers ) { trackername << "" << i << ""; }
    else { trackername << "" << i-Ninflectortrackers << ""; }

    if ( fill ) {
      hname << "InflectorTracker_" << trackername.str() << "_XprimeX";
      h_InflectorTracker_XprimeX_VD[i] = new TH2F(hname.str().c_str(), "", Xbin_Inflector, Xmin_Inflector, Xmax_Inflector, XPrimebin_Inflector, XPrimemin_Inflector, XPrimemax_Inflector);
      h_InflectorTracker_XprimeX_VD[i]->SetXTitle("x_{inf} [mm]");
      h_InflectorTracker_XprimeX_VD[i]->SetYTitle("x'_{inf} (Px/Pz) [mrad]");
      hname.str("");

      hname << "InflectorTracker_" << trackername.str() << "_YprimeY";
      h_InflectorTracker_YprimeY_VD[i] = new TH2F(hname.str().c_str(), "", Ybin_Inflector, Ymin_Inflector, Ymax_Inflector, YPrimebin_Inflector, YPrimemin_Inflector, YPrimemax_Inflector);
      h_InflectorTracker_YprimeY_VD[i]->SetXTitle("y_{inf} [mm]");
      h_InflectorTracker_YprimeY_VD[i]->SetYTitle("y'_{inf} (Py/Pz) [mrad]");
      hname.str("");

      hname << "InflectorTracker_" << trackername.str() << "_RhoY";
      h_InflectorTracker_RhoY_VD[i] = new TH2F(hname.str().c_str(), "", Rhobin_Inflector, Rhomin_Inflector, Rhomax_Inflector, Vbin_Inflector, Vmin_Inflector, Vmax_Inflector);
      h_InflectorTracker_RhoY_VD[i]->SetXTitle("x_{inf} [mm]");
      h_InflectorTracker_RhoY_VD[i]->SetYTitle("y_{inf} [mm]");
      hname.str("");
        
      hname << "InflectorTracker_" << trackername.str() << "_XZ";
      h_InflectorTracker_XZ_VD[i] = new TH2F(hname.str().c_str(), "", Rbin_Ring, Rmin_Ring, Rmax_Ring, Rbin_Ring, Rmin_Ring, Rmax_Ring);
      h_InflectorTracker_XZ_VD[i]->SetXTitle("z_{ring} [mm]");
      h_InflectorTracker_XZ_VD[i]->SetYTitle("x_{ring} [mm]");
      hname.str("");
    }
  }







  for ( int st = 0; st < 2; st++ ) {
    string stname = "";
    if ( st == 1 ) { stname = "_Stored"; }

    for ( int i = 0; i < Nsystemtrackers; i++ ) {
      if ( fill ) {
	hname << rhitnames[i] << "Hits" << stname << "_XZ";
	h_SystemHitTracker_XZ[i][st] = new TH2F(hname.str().c_str(), "", Rbin_Ring/2, Rmin_Ring, Rmax_Ring, Rbin_Ring/2, Rmin_Ring, Rmax_Ring);
	h_SystemHitTracker_XZ[i][st]->SetYTitle("x_{ring} [mm]");
	h_SystemHitTracker_XZ[i][st]->SetXTitle("z_{ring} [mm]");
	hname.str("");
	
	hname << rhitnames[i] << "Hits" << stname << "_RhoY";
	h_SystemHitTracker_RhoY[i][st] = new TH2F(hname.str().c_str(), "", Rhobin_Ring/2, Rhomin_Ring, Rhomax_Ring, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	h_SystemHitTracker_RhoY[i][st]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_SystemHitTracker_RhoY[i][st]->SetYTitle("y_{ring} [mm]");
	hname.str("");
	
	hname << rhitnames[i] << "Hits" << stname << "_RhoTime";
	h_SystemHitTracker_RhoTime[i][st] = new TH2F(hname.str().c_str(), "", 20000, 0.0, 200, Rhobin_Ring/2, Rhomin_Ring, Rhomax_Ring);
	h_SystemHitTracker_RhoTime[i][st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_SystemHitTracker_RhoTime[i][st]->SetXTitle("t_{ring} [#mus]");
	hname.str("");
	
	hname << rhitnames[i] << "Hits" << stname << "_YTime";
	h_SystemHitTracker_YTime[i][st] = new TH2F(hname.str().c_str(), "", 20000, 0.0, 200, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	h_SystemHitTracker_YTime[i][st]->SetYTitle("y_{ring} [mm]");
	h_SystemHitTracker_YTime[i][st]->SetXTitle("t_{ring} [#mus]");
	hname.str("");
	
	hname << rhitnames[i] << "Hits" << stname << "_DeltaPy";
	h_SystemHitTracker_DeltaPy[i][st] = new TH1F(hname.str().c_str(), "", 3*YPrimebin_Ring, 3*YPrimemin_Ring, 3*YPrimemax_Ring);
	h_SystemHitTracker_DeltaPy[i][st]->SetXTitle("#Delta#hat{p}_{y} [MeV]");
	hname.str("");
	
	hname << rhitnames[i] << "Hits" << stname << "_DeltaPx";
	h_SystemHitTracker_DeltaPx[i][st] = new TH1F(hname.str().c_str(), "", 3*XPrimebin_Ring, 3*XPrimemin_Ring, 3*XPrimemax_Ring);
	h_SystemHitTracker_DeltaPx[i][st]->SetXTitle("#Delta#hat{p}_{x} [MeV]");
	hname.str("");
      
	hname << rhitnames[i] << stname << "_Nhits";
	h_SystemHitTracker_Nhits[i][st] = new TH1F(hname.str().c_str(), "", 10, -0.5, 9.5);
	h_SystemHitTracker_Nhits[i][st]->SetXTitle("Number of energy deposits");
	hname.str("");
      }
    }
  }
  
  string inames[22] = {"UpstreamEndFlange",  "UpstreamEquivalentNbTi", "UpstreamEquivalentAl", "UpstreamEquivalentCu", "UpstreamWindow", "DownstreamWindow", "DownstreamEquivalentNbTi", "DownstreamEquivalentAl", "DownstreamEquivalentCu", "DownstreamEndFlange",  "Mandrel", "Quad10", "Quad20", "Quad30", "Quad40", "Quad11", "Quad21", "Quad31", "Quad41", "Kicker1", "Kicker2", "Kicker3"};
  Nringhits = 22;
  for ( int st = 0; st < 2; st++ ) {
    string stname = "";
    if ( st == 1 ) { stname = "_Stored"; }
    
    for ( int i = 0; i < Nringhits; i++ ) {
      
      if ( fill ) {
	hname << inames[i] << "Hits" << stname << "_XZ";
	h_RingHitTracker_XZ[i][st] = new TH2F(hname.str().c_str(), "", Rbin_Ring/2, Rmin_Ring, Rmax_Ring, Rbin_Ring/2, Rmin_Ring, Rmax_Ring);
	h_RingHitTracker_XZ[i][st]->SetYTitle("x_{ring} [mm]");
	h_RingHitTracker_XZ[i][st]->SetXTitle("z_{ring} [mm]");
	hname.str("");
	
	hname << inames[i] << "Hits" << stname << "_RhoY";
	h_RingHitTracker_RhoY[i][st] = new TH2F(hname.str().c_str(), "", Rhobin_Ring/2, Rhomin_Ring, Rhomax_Ring, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	h_RingHitTracker_RhoY[i][st]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingHitTracker_RhoY[i][st]->SetYTitle("y_{ring} [mm]");
	hname.str("");
	
	hname << inames[i] << "Hits" << stname << "_RhoTime";
	h_RingHitTracker_RhoTime[i][st] = new TH2F(hname.str().c_str(), "", 20000, 0.0, 200, Rhobin_Ring/2, Rhomin_Ring, Rhomax_Ring);
	h_RingHitTracker_RhoTime[i][st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingHitTracker_RhoTime[i][st]->SetXTitle("t_{ring} [#mus]");
	hname.str("");
	
	hname << inames[i] << "Hits" << stname << "_YTime";
	h_RingHitTracker_YTime[i][st] = new TH2F(hname.str().c_str(), "", 20000, 0.0, 200, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	h_RingHitTracker_YTime[i][st]->SetYTitle("y_{ring} [mm]");
	h_RingHitTracker_YTime[i][st]->SetXTitle("t_{ring} [#mus]");
	hname.str("");
	
	hname << inames[i] << "Hits" << stname << "_DeltaPy";
	h_RingHitTracker_DeltaPy[i][st] = new TH1F(hname.str().c_str(), "", 3*YPrimebin_Ring, 3*YPrimemin_Ring, 3*YPrimemax_Ring);
	h_RingHitTracker_DeltaPy[i][st]->SetXTitle("#Delta#hat{p}_{y} [MeV]");
	hname.str("");
	
	hname << inames[i] << "Hits" << stname << "_DeltaPx";
	h_RingHitTracker_DeltaPx[i][st] = new TH1F(hname.str().c_str(), "", 3*XPrimebin_Ring, 3*XPrimemin_Ring, 3*XPrimemax_Ring);
	h_RingHitTracker_DeltaPx[i][st]->SetXTitle("#Delta#hat{p}_{x} [MeV]");
	hname.str("");
      
	hname << inames[i] << stname << "_Nhits";
	h_RingHitTracker_Nhits[i][st] = new TH1F(hname.str().c_str(), "", 10, -0.5, 9.5);
	h_RingHitTracker_Nhits[i][st]->SetXTitle("Number of energy deposits");
	hname.str("");
      }
    }
  }




  if ( beamstart_ == "UpstreamCryo" || beamstart_ == "upstream_cryo" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart.SetXYZ(6989 , 0 , -2394);
    rhat_offset = 275.7;
    yhat_offset = 0;
  }
  if ( beamstart_ == "UpstreamMandrel" || beamstart_ == "upstream" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart.SetXYZ(6990 , 0 , -2388);
    rhat_offset = 247;
    yhat_offset = 0;
  }
  if ( beamstart_ == "DownstreamMandrel" || beamstart_ == "downstream" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart.SetXYZ(7155 , 0 , -699.9);
    rhat_offset = 77;
    yhat_offset = 0;
  }
  if ( beamstart_ == "Perfect" || beamstart_ == "CentralOrbit" ) {
    int start_perfect_offset = 0.0;
    xAxis.SetXYZ(1 , 0 , 0);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0 , 0 , 1);
    beamStart.SetXYZ(7112 + start_perfect_offset , 0 , 0);
    rhat_offset = start_perfect_offset;
    yhat_offset = 0;
  }
  if ( beamstart_ == "CentralOrbit_Offset77" ) {
    int start_perfect_offset = 77;
    xAxis.SetXYZ(1 , 0 , 0);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0 , 0 , 1);
    beamStart.SetXYZ(7112 + start_perfect_offset , 0 , 0);
    rhat_offset = start_perfect_offset;
    yhat_offset = 0;
  }

}

gm2ringsim::ringTrackerAnalyzer::~ringTrackerAnalyzer() {
  // Clean up dynamic memory and other resources here.
  cout << "Generator \t" << Ngen << endl;
  cout << "Inflector \t" << Nstart_inflector << "\t" << Nexit_inflector << endl;
  cout << "RingStart \t" << Nstart_ring << endl;
  cout << "RingStored\t" << Nstored1_ring << "\t" << Nstored10_ring << "\t" << Nstored100_ring << endl;
  cout << "<Theta @ x=0> = " << Ndegat0/Nstart_ring << endl;
  cout << "<x' @ Theta>  = " << Nxpat0/Nstart_ring << endl;
  
  ofstream out;
  out.open("Eff/myout.dat");
  out << Nstart_inflector << "\t" << Nexit_inflector << endl;
  out << Nstart_ring << endl;
  out << Nstored1_ring << "\t" << Nstored10_ring << "\t" << Nstored100_ring << endl;
  out << Ngen << endl;
  out << Ndegat0/Nstart_ring << endl;
  out << Nxpat0/Nstart_ring << endl;
  out.close();

}

void gm2ringsim::ringTrackerAnalyzer::analyze(art::Event const &e)
{

  // Let's get the physical volume store. It's either in the run or the service
  auto const & pvs = artg4::dataFromRunOrService<artg4::PhysicalVolumeStoreData, artg4::PhysicalVolumeStoreService>(e.getRun(), pvsProducerLabel_, pvsInstanceLabel_);

  cout << "There are " << pvs.size() << " entries in the PVS" << endl;
  mf::LogInfo("ParticleTrackAnalyzer") << "There are " << pvs.size() << " entries in the PVS";
  
  // Print it out
//   for ( unsigned int i = 0; i < pvs.size(); ++i ) {
//     mf::LogInfo("ParticleTrackAnalyzer") << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i);
//     cout << "\t" << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i) << endl;
//   }
  
  //return;
  
 
  
  // Extract the hits
  
  // Make the handle
  //art::Handle< StrawArtRecordCollection > hitDataHandle;
  art::Handle< TrackerArtRecordCollection > ringtrackerhitDataHandle;
  art::Handle< TrackingActionArtRecordCollection > truthhitDataHandle;
  art::Handle< InflectorArtRecordCollection > inflectorhitDataHandle;
  art::Handle< RingArtRecordCollection > ringhitDataHandle;
  
  // Fill the handle (note the use of the member data)
  std::cout<<"Event ID: "<<e.id()<<std::endl;
  
  //e.getByLabel(hitModuleLabel_, instanceName_, hitDataHandle);
  e.getByLabel(ringtrackerhitModuleLabel_, ringtrackerinstanceName_, ringtrackerhitDataHandle);
  e.getByLabel(truthhitModuleLabel_, truthinstanceName_, truthhitDataHandle);
  e.getByLabel(inflectorhitModuleLabel_, inflectorinstanceName_, inflectorhitDataHandle);
  e.getByLabel(ringhitModuleLabel_, ringinstanceName_, ringhitDataHandle);
  
  // Resolve the handle
  //StrawArtRecordCollection const & hits = *hitDataHandle;
  TrackerArtRecordCollection const & ringtrackerhits = *ringtrackerhitDataHandle;
  TrackingActionArtRecordCollection const & truthhits = *truthhitDataHandle;
  InflectorArtRecordCollection const & inflectorhits = *inflectorhitDataHandle;
  RingArtRecordCollection const & ringhits = *ringhitDataHandle;
  // Let's use the nice C++11 vector iteration
  //int i = 0;

  cout << "Got it" << endl;

  //-----------------
  // Pass Information
  //-----------------
  bool pass_5us = false;
  bool pass_20us = false;
  bool pass_50us = false;
  bool pass_final = false;



      

  //----------------------
  // Inflector Information
  //----------------------
  for ( auto hinfdata : inflectorhits ) {    
    double x_ring_inf = hinfdata.x_loc;
    double y_ring_inf = hinfdata.y_loc;
    double z_ring_inf = hinfdata.z_loc;
    
    double px_ring_inf = hinfdata.px_loc;
    double py_ring_inf = hinfdata.py_loc;
    double pz_ring_inf = hinfdata.pz_loc;
    
    int inflector_track_ID = hinfdata.trackID;
    
    TVector3 mom_ring_inf(px_ring_inf, py_ring_inf, pz_ring_inf);
    TVector3 pos_ring_inf(x_ring_inf, y_ring_inf, z_ring_inf);
    //double x_acc = x_ring_inf - beamStart.X();
    //double y_acc = y_ring_inf - beamStart.Y();
    //double z_acc = z_ring_inf - beamStart.Z();
    
    pos_ring_inf -= beamStart;
    
    double x_inf = pos_ring_inf.Dot(xAxis);
    double y_inf = pos_ring_inf.Dot(yAxis);
    //double z_acc = pos_inf.Dot(zAxis);
    
    //cout << inum << "\t" << x_acc << "\t" << y_acc << "\t" << z_acc << endl;
    
    double xprime_inf = mom_ring_inf.Dot(xAxis)/mom_ring_inf.Mag();
    double yprime_inf = mom_ring_inf.Dot(yAxis)/mom_ring_inf.Mag();	  
    //double zprime_acc = mom.Dot(zAxis)/mom.Mag();
    
    //int inflector_volume_UID = hinfdata.volumeUID;
    string inflector_volume_name = ""; //uom->lookup(inflector_volume_UID);
    
      for ( int vol = 0; vol < 9; vol++ ) {
      stringstream volname;
      if ( vol < 9 ) { volname << "InflectorTracker[0" << vol << "]"; }
      else { volname << "InflectorTracker[0" << vol-9 << "]"; }
      
      if ( inflector_volume_name == volname.str() ) {
	if ( inflector_track_ID == 1 ) { 
	  if ( vol == 0 ) { Nstart_inflector++; }
	  if ( vol == 8 ) { Nexit_inflector++; }
	}
	if ( vol == -1 ) {
	  cout << volname.str() << "\t" << x_ring_inf << "\t" << y_ring_inf << "\t" << z_ring_inf << endl;
	}
	//if ( vol == 0 ) { cout << inflector_volume_UID << endl; }
	if ( fill ) { 
	  h_InflectorTracker_RhoY_VD[vol]->Fill(x_inf, y_inf);
	  h_InflectorTracker_XprimeX_VD[vol]->Fill(x_inf, 1000*xprime_inf);
	  h_InflectorTracker_YprimeY_VD[vol]->Fill(y_inf, 1000*yprime_inf);
	}
      }
    } // loop over trackings
  } // loop over inflector hits



  
  
  //-------------------------
  // Ring Tracker Information
  //-------------------------
  //int ttnum = 0;
  //double pathlength = 0.0;
  double t0 = 0.0;
  double t1 = 0.0;
  double theta1 = -1.0;
  //bool found_muon_in_tracker = false;
  //cout << ttR->size() << endl;
  int myturn = 0;
  int mytrack = 0;
  double degree_at_r0 = -1.0;
  
  int station = 0;
  double mom_init = -1.0;
  
  double dmom = 0.0;
  double maxPr =  0.15;
  double minPr = -0.25;
  double maxPv =  0.125;
  double minPv = -0.125;
  double prhat_init = -9999.9;
  double pvhat_init = -9999.9;
  double rhat_init = -9999.9;
  double vhat_init = -9999.9;
  double pmom_init = -9999.9;
  
  double previous_rhat = 9999.9;
  double previous_prhat = 999.9;
  double previous_degree = 999.9;
  
  //double final_x = -9999.9;
  //double final_xprime = -9999.9;
  //double final_y = -9999.9;
  //double final_yprime = -9999.9;
  
  double degree_at_quad = -9999.9;
  double degree_r_at_r0 = -9999.9;
  double degree_r_at_quad = -9999.9;
  double kick_at_r0 = -9999.9;
  
  
  for ( auto hdata : ringtrackerhits) {
    //std::cout << "  tracker = " << i++ << std::endl;
    
    int track_ID   = hdata.trackID;
    //int track_turn = hdata.turn;
    //int track_UID  = hdata.volumeUID;
    double time = hdata.time;
    double mom = hdata.p;
    //double p = hdata.p;
    double momentum = mom;
    //double deltaE = 0.0;
    //double turn = track_turn;
    double rhat = hdata.rhat;
    double theta = hdata.theta;
    //double x = (rhat) * TMath::Cos(theta);
    //double z = (rhat) * TMath::Sin(theta);
    double x_ring = (rhat+7112) * TMath::Cos(theta);
    double z_ring = (rhat+7112) * TMath::Sin(theta);
    double vhat = hdata.vhat;
    double prhat = hdata.prhat;
    double pvhat = hdata.pvhat;
    //double pzhat = TMath::Sqrt(mom*mom - prhat*prhat - pvhat*pvhat);
    double y = vhat;

    if ( track_ID != 1 ) { continue; }

    if ( 0 ) {
      std::cout << "  track_ID   = " << hdata.trackID << std::endl;
    }
    if ( 0 ) {
    std::cout << "  track_turn = " << hdata.turn << "\t";
    std::cout << "  track_UID  = " << hdata.volumeUID << "\t";
    std::cout << "  time = " << hdata.time << "\t";
    std::cout << "  mom = " << hdata.p << "\t";
    std::cout << "  p = " << hdata.p << "\t";
    std::cout << "  deltaE = " << "0.0" << "\t";
    std::cout << "  rhat = " << hdata.rhat << "\t";
    std::cout << "  theta = " << hdata.theta << "\t";
    std::cout << "  x = " << (rhat) * TMath::Cos(theta) << "\t";
    std::cout << "  z = " << (rhat) * TMath::Sin(theta) << "\t";
    std::cout << "  x_ring = " << (rhat+7112) * TMath::Cos(theta) << "\t";
    std::cout << "  z_ring = " << (rhat+7112) * TMath::Sin(theta) << "\t";
    std::cout << "  vhat = " << hdata.vhat << "\t";
    std::cout << "  prhat = " << hdata.prhat << "\t";
    std::cout << "  pvhat = " << hdata.pvhat << "\t";
    std::cout << "  pzhat = " << TMath::Sqrt(mom*mom - prhat*prhat - pvhat*pvhat) << "\t";
    std::cout << "  y = " << vhat << "\t";
    std::cout << std::endl;
    }

    if ( t0 <= 0.0 ) {
      t0 = time; mytrack++; t1 = time; theta1 = theta;
      prhat_init = prhat;
      pvhat_init = pvhat;
      rhat_init  = rhat;
      vhat_init  = vhat;
      pmom_init  = mom;
      
      //cout << rhat_init << "\t" << prhat_init << endl;
    }
    
    //GetTurn(&myturn, &mytrack, time, t0, t1, theta, theta1);
    
    if ( mytrack >= 12 ) { myturn++; mytrack = 0; }
    
    
    if ( time > t0 ) { 
      int nskips = 0;
      if ( time - t1 > 16 ) {
	nskips = 1;
	for ( int tbegin = 10; tbegin > 1; tbegin-- ) {
	  if ( time - t1 > tbegin*12 ) { nskips = tbegin; break; }
	}
      }
      
      if ( nskips > 0 ) { 
	//cout << "\t\tWe skipped " << nskips << " counters because t-t' = " << (int)(time - t1) << " ..." << endl;
	//cout << "\t\tMytrack was :    " << mytrack << endl;
	mytrack += nskips;
	//int mytrk_tmp = mytrack + nskips;
	
	if ( mytrack > 12 ) {
	  ;//cout << " =======> Mytrack is now greater than 12!!!" << endl;
	  myturn++; mytrack -= 12;
	}
	// 	      myturn++; mytrack = mytrk_tmp - 12;
	// 	    }
	///cout << "\t\tMytrack is now : " << mytrack << endl;
      }
      else {
	mytrack++;
      }
    }
    //cout << "MyTurn = " << myturn << "\tMyTrack =" << mytrack << endl;
    
    
    //PrintTrack(ncounts, uom, tracker_b, t0, t1, myturn, mytrack);
    
    if ( 0 ) {
      if ( time - t1 > 18 ) {
	;//cout << "Something is really wrong with this event (" << ncounts << ")!!!" << endl;
	if ( theta - theta1 < 0 ) { 
	  //cout << "Something is really wrong with this event (" << ncounts << ")!!!" << endl; 
	  myturn++; mytrack = 0;
	}
	else {
	  ;//cout << "This is the same turn I believe." << endl;
	}
      }
    }
    
    
    t1 = time;
    theta1 = theta;
    
    //if ( track_ID == 1 ) { found_muon_in_tracker = true; }
    //if ( track_ID == 1 ) { PrintTrack(ncounts, uom, tracker_b); }
    
    if ( myturn == 0 ) {	  
      if ( fill ) {
	;//h_TimeHitTracker[mytrack-1]->Fill(time);
      }
    }
    
    //--------------------
    // Check Initial Slope
    //
    // --
    //  --
    //    --    r - r0 = m(deg - deg0)
    //     --
    //       --
    //
    // deg(@r=0) = - r0/m + deg0
    //--------------------
    double degree = mytrack * 30;
    if ( track_ID == 1 ) {
      if ( myturn == 0 && degree_at_quad < 0 ) {
	//cout << degree << "\t" << rhat << endl;
	if ( rhat < 50 && previous_rhat >= 50 ) {
	  double slope_r_over_deg = (rhat - previous_rhat)/(degree - previous_degree);
	  degree_at_quad = previous_degree - previous_rhat/slope_r_over_deg;

	  degree_at_quad = (50 - rhat)/slope_r_over_deg + degree;

	  if ( fill ) {
	    h_RingTracker_DegreeAtQuad[0]->Fill(degree_at_quad);
	  }
	  
	  //double degree_at_quad1 = (50 - rhat)/slope_r_over_deg + degree;
	  //double degree_at_quad2 = (50 - previous_rhat)/slope_r_over_deg + previous_degree;
		
	  // Fit x @ theta0 to [ a - b*theta^{2} ]
	  double a_r = ( rhat * previous_degree * previous_degree) - (previous_rhat * degree * degree);
	  double b_r = rhat - previous_rhat;
	  a_r /= ( previous_degree*previous_degree - degree*degree );
	  b_r /= ( previous_degree*previous_degree - degree*degree );
	  degree_r_at_quad = TMath::Sqrt(a_r/b_r);
	  //cout << "  Degree @ R=50 = " << degree_at_quad << "\t" << degree_r_at_quad << endl;
	  //cout << "  Degree @ R=50 = " << degree_at_quad1 << "\t" << degree_at_quad2 << endl;
	  if ( degree_at_quad - degree_r_at_quad > 5 || degree_at_quad - degree_r_at_quad < -5 ) {
	    if ( 0 ) {
	      cout << "Degree @ Quad = " << degree_at_quad << "\t" << degree_r_at_quad << endl;
	      cout << "[r,th]1 = " << previous_rhat << " , " << previous_degree << endl;
	      cout << "[r,th]2 = " << rhat << " , " << degree << endl;
	    }
	  }
	}
      }
      if ( myturn == 0 && degree_at_r0 < 0 ) {
	if ( fill ) {
	  h_RingTracker_FirstTurnX[0]->Fill(degree, rhat);
	  h_RingTracker_FirstTurnY[0]->Fill(degree, vhat);
	}
	if ( rhat < 0 && previous_rhat >= 0 ) {
	  double slope_r_over_deg = (rhat - previous_rhat)/(degree - previous_degree);
	  degree_at_r0 = previous_degree - previous_rhat/slope_r_over_deg;
	  //Ndegat0 += degree_at_r0;

	  // Fit x @ theta0 to [ a - b*theta^{2} ]
	  double a_r = ( rhat * previous_degree * previous_degree) - (previous_rhat * degree * degree);
	  double b_r = rhat - previous_rhat;
	  a_r /= ( previous_degree*previous_degree - degree*degree );
	  b_r /= ( previous_degree*previous_degree - degree*degree );
	  degree_r_at_r0 = TMath::Sqrt(a_r/b_r);
	  Ndegat0 += degree_r_at_r0;
	  //cout << "Degree @ r0 = " << degree_at_r0 << "\t" << degree_r_at_r0 << endl;
	  if ( degree_at_r0 - degree_r_at_r0 > 5 || degree_at_r0 - degree_r_at_r0 < -5 ) {
	    if ( 0 ) {
	      cout << "Degree @ r0 = " << degree_at_r0 << "\t" << degree_r_at_r0 << endl;
	      cout << "[r,th]1 = " << previous_rhat << " , " << previous_degree << endl;
	      cout << "[r,th]2 = " << rhat << " , " << degree << endl;
	    }
	  }

	  // Fit x' @ theta0 to [ a - b*theta^{2} ]
	  double a = ( prhat * previous_degree * previous_degree) - (previous_prhat * degree * degree);
	  double b = prhat - previous_prhat;
	  a /= ( previous_degree*previous_degree - degree*degree );
	  b /= ( previous_degree*previous_degree - degree*degree );
		
	  kick_at_r0 = a - b*degree_at_r0*degree_at_r0;
	  Nxpat0 += kick_at_r0;
	  //cout << degree_at_r0 << "\t" << previous_prhat << "\t" << prhat << "\t" << kick_at_r0 << endl;

	  //cout.precision(3);
	  //cout << myturn << "\t" << mytrack << "\tth: " << theta << " \td': " << previous_degree << " \td'': " << degree << " \tr': " << previous_rhat << " \tr'': " << rhat << " \t" << "\tr0: " << rhat_init << " \t" << "d(r=0) = " << degree_at_r0 << endl;
	  if ( fill ) {
	    h_RingTracker_DegreeAtRhat0[0]->Fill(degree_at_r0);
	    h_RingTracker_DegreeAtRhat0RhatInit[0]->Fill(degree_at_r0, rhat_init);
	    h_RingTracker_KickAtRhat0[0]->Fill(1000*kick_at_r0);
	    h_RingTracker_KickAtRhat0RhatInit[0]->Fill(1000*kick_at_r0, rhat_init);
	  }
	}
      }
    }
	      
    previous_rhat = rhat;
    previous_prhat = prhat;
    previous_degree = degree;
	      
	
    if ( track_ID == 1 ) {
      if ( fill ) {
	h_RingTracker_XprimeXTime[0]->Fill(rhat, 1000*prhat);
	h_RingTracker_YprimeYTime[0]->Fill(vhat, 1000*pvhat);
	h_RingTracker_RhoYTime[0]->Fill(rhat, vhat);
	h_RingTracker_XprimeYprimeTime[0]->Fill(1000*prhat, 1000*pvhat);
	      
	if ( myturn >= maxturnsreal_ ) {
	  if ( pass_final == false ) {
	    pass_final = true;
	    h_RingTracker_XprimeXTime[4]->Fill(rhat, 1000*prhat);
	    h_RingTracker_YprimeYTime[4]->Fill(vhat, 1000*pvhat);
	    h_RingTracker_RhoYTime[4]->Fill(rhat, vhat);
	    h_RingTracker_XprimeYprimeTime[4]->Fill(1000*prhat, 1000*pvhat);
		  
		  
	    h_RingTracker_dNdX->Fill(rhat);
	    h_RingTracker_dNdXprime->Fill(1000*prhat);
	    h_RingTracker_d2NdXprimeX->Fill(rhat, 1000*prhat);
	    h_RingTracker_dNdY->Fill(vhat);
	    h_RingTracker_dNdYprime->Fill(1000*pvhat);
	    h_RingTracker_d2NdYprimeY->Fill(vhat, 1000*pvhat);
	    h_RingTracker_d2NdXY->Fill(rhat, vhat);
	    h_RingTracker_d2NdXprimeYprime->Fill(1000*prhat, 1000*pvhat);
	  }
	}
	    
	if ( time > 5e3 ) {
	  //pass_5us = true;
	  h_RingTracker_XprimeXTime[1]->Fill(rhat, 1000*prhat);
	  h_RingTracker_YprimeYTime[1]->Fill(vhat, 1000*pvhat);
	  h_RingTracker_RhoYTime[1]->Fill(rhat, vhat);
	  h_RingTracker_XprimeYprimeTime[1]->Fill(1000*prhat, 1000*pvhat);
	    
	  if ( time > 2e4 ) {
	    pass_20us = true;
	    h_RingTracker_XprimeXTime[2]->Fill(rhat, 1000*prhat);
	    h_RingTracker_YprimeYTime[2]->Fill(vhat, 1000*pvhat);
	    h_RingTracker_RhoYTime[2]->Fill(rhat, vhat);
	    h_RingTracker_XprimeYprimeTime[2]->Fill(1000*prhat, 1000*pvhat);
		  
	    if ( time > 5e4 ) {
	      pass_50us = true;
	      h_RingTracker_XprimeXTime[3]->Fill(rhat, 1000*prhat);
	      h_RingTracker_YprimeYTime[3]->Fill(vhat, 1000*pvhat);
	      h_RingTracker_RhoYTime[3]->Fill(rhat, vhat);
	      h_RingTracker_XprimeYprimeTime[3]->Fill(1000*prhat, 1000*pvhat);
	    }
	  }
	}
      }
    }

    if ( pass_final ) {
      h_RingTracker_DegreeAtRhat0[1]->Fill(degree_at_r0);
      h_RingTracker_DegreeAtRhat0RhatInit[1]->Fill(degree_at_r0, rhat_init);
      h_RingTracker_KickAtRhat0[1]->Fill(1000*kick_at_r0);
      h_RingTracker_KickAtRhat0RhatInit[1]->Fill(1000*kick_at_r0, rhat_init);
      h_RingTracker_DegreeAtQuad[1]->Fill(degree_at_quad);
    }

    if ( track_ID == 1 && mytrack == 1 && myturn == 0 ) { Nstart_ring++; }
    if ( track_ID == 1 && mytrack == 1 && myturn == 1 ) { Nstored1_ring++; }
    if ( track_ID == 1 && mytrack == 1 && myturn == 10 ) { Nstored10_ring++; }
    if ( track_ID == 1 && mytrack == 1 && myturn == 100-1 ) { Nstored100_ring++; }
	
    if ( track_ID == 1 && mytrack == 1 ) {
      int stored_val = -1;
      if ( myturn == 0 )   { stored_val = 0; }
      if ( myturn == 1 )   { stored_val = 1; }
      if ( myturn == 2 )   { stored_val = 2; }
      if ( myturn == 5 )   { stored_val = 3; }
      if ( myturn == 10-1 )  { stored_val = 4; }
      if ( myturn == 50-1 ) { stored_val = 5; }
      if ( myturn == 100-1 ) { stored_val = 6; }
      if ( myturn == 200-1 ) { stored_val = 7; }
      if ( myturn == 500-1 ) { stored_val = 8; }
      if ( myturn == 1000-1 ) { stored_val = 9; }
      if ( myturn == 2000-1 ) { stored_val = 10; }
      //cout << stored_val << endl;
	  
      if ( stored_val > -1 ) {
	double thetaX = TMath::ASin(prhat_init);
	double thetaY = TMath::ASin(pvhat_init);
	if ( fill ) { 
	  h_RingTracker_Stored_RhoY[stored_val]->Fill(rhat_init, vhat_init);
	  h_RingTracker_Stored_XprimeX[stored_val]->Fill(rhat_init, 1000*prhat_init);
	  h_RingTracker_Stored_YprimeY[stored_val]->Fill(vhat_init, 1000*pvhat_init);
	  h_RingTracker_Stored_XprimeYprime[stored_val]->Fill(1000*prhat_init, 1000*pvhat_init);
	  h_RingTracker_Stored_Momentum[stored_val]->Fill(pmom_init);
	  h_RingTracker_Stored_thetaX[stored_val]->Fill(thetaX);
	  h_RingTracker_Stored_thetaY[stored_val]->Fill(thetaY);
	  h_RingTracker_Stored_thetaXY[stored_val]->Fill(thetaX, thetaY);
	}	    
      }
    }


    if ( track_ID == 1 && myturn == 0 ) {
      if ( mom_init < 0 ) { 
	mom_init = momentum;
      }
      station = -1;
      dmom = momentum-mom_init;

      if ( mytrack == 1 ) { 
	station = 0;
      }
      if ( mytrack == 2 ) {
	station = 1;
      }
      if ( mytrack == 3 ) {
	station = 2;
      }
      if ( mytrack == 4 ) { 
	station = 3;
      }
      if ( mytrack == 5 ) { 
	station = 4;
      }
      if ( mytrack == 6 ) { 
	station = 5;
      }
      if ( mytrack == 7 ) { 
	station = 6;
      }
      if ( mytrack == 10 ) { 
	station = 7;
      }
      if ( station >= 0 ) {
	double xprime = prhat;
	double yprime = pvhat;
	xprime = prhat;
	yprime = pvhat;
	if ( fill ) { 
	  h_RingTracker_DeltaPy[station]->Fill(pvhat);
	  h_RingTracker_RhoY[station]->Fill(rhat, vhat);
	}
	if ( station == 0 ) { 
	  ;//PrintTrack(ncounts, uom, tracker_b, t0, t1);
	}
	//cout << "  Event: " << ncounts << "\tTurn: " << myturn << "\tFilling h_RingTracker_RhoY[" << station << "]->Fill(" << rhat << ", " << vhat << ");" << endl; 
	if ( fill ) {
	  h_RingTracker_XZ[station]->Fill(z_ring, x_ring);
	  h_RingTracker_XprimeX[station]->Fill(rhat, 1000*xprime);
	  h_RingTracker_YprimeY[station]->Fill(vhat, 1000*yprime);
	}

	if ( dmom <= -100.0 ) { dmom = -99.9; }
	if ( pvhat >= maxPv )  { pvhat = maxPv-0.01; }
	if ( pvhat <= minPv ) { pvhat = minPv+0.01; }
	if ( prhat >= maxPr )  { prhat = maxPr-0.01; }
	if ( prhat <= minPr ) { prhat = minPr+0.01; }	    
      }
    }

    if ( track_ID == 1 ) {
      station = -1;

      if ( myturn == 1 && mytrack == 1 ) {
	station = 8;
      }
      if ( myturn == 5 && mytrack == 1 ) { 
	station = 9;
      }
      if ( myturn == 9 && mytrack == 1 ) {
	station = 10;
      }
      if ( myturn == 49 && mytrack == 1 ) {
	station = 11;
      }
      if ( myturn == 99 && mytrack == 1 ) { 
	station = 12;
      }
      if ( myturn == 199 && mytrack == 1 ) { 
	station = 13;
      }
      if ( myturn == 499 && mytrack == 1 ) {
	station = 14;
      }
      if ( myturn == 999 && mytrack == 1 ) {
	station = 15;
      }
      if ( myturn == 1999 && mytrack == 1 ) {
	station = 16;
      }

	  
      dmom = momentum-mom_init;
	  
      if ( station >= 0 ) {
	double xprime = prhat;
	double yprime = pvhat;
	xprime = prhat;
	yprime = pvhat;
	if ( fill ) { 
	  h_RingTracker_DeltaPy[station]->Fill(pvhat);
	  h_RingTracker_RhoY[station]->Fill(rhat, vhat);
	  h_RingTracker_XZ[station]->Fill(z_ring, x_ring);
	  h_RingTracker_XprimeX[station]->Fill(rhat, 1000*xprime);
	  h_RingTracker_YprimeY[station]->Fill(vhat, 1000*yprime);
	}


	if ( dmom <= -100.0 ) { dmom = -99.9; }
	if ( pvhat >= maxPv )  { pvhat = maxPv-0.01; }
	if ( pvhat <= minPv ) { pvhat = minPv+0.01; }
	if ( prhat >= maxPr )  { prhat = maxPr-0.01; }
	if ( prhat <= minPr ) { prhat = minPr+0.01; }
      }
    }


    //if ( vol < 10 ) { volname << "TrackerTubs[0" << vol << "]"; }
    //else { volname << "TrackerTubs[" << vol << "]"; }

    if ( track_ID == 1 ) {
      //pathlength = TMath::TwoPi()*myturn + theta;
      //cout << "Event: " << i << "\t" << " w/ {rho,theta,y} = {" << rhat << " , " << theta << " , " << vhat << "} and Vol[" << track_volume_name << "]" << endl;
      //double dT = (time - t0)/(29.3*2200.0);

      double dT = myturn + theta/TMath::TwoPi();
      //cout << theta << "\t" << dT << endl;

      if ( fill ) {
	h_RingTracker_Rho_Turn->Fill(dT, rhat);
	h_RingTracker_Y_Turn->Fill(dT, y);
	h_RingTracker_Rho_Time->Fill(time/1000, rhat);
	h_RingTracker_Y_Time->Fill(time/1000, y);
      }

      //if ( prhat < -0.02 ) { prhat = -0.02+0.0001; }
      //if ( prhat >  0.02 ) { prhat =  0.02-0.0001; }
      //if ( pvhat < -0.02 ) { pvhat = -0.02+0.0001; }
      //if ( pvhat >  0.02 ) { pvhat =  0.02-0.0001; }
      //if ( mom > 3200 ) { mom = 3200-1; }
      //if ( mom < 3000 ) { mom = 3000+1; }
      if ( fill ) { 
	h_RingTracker_Prhat_Turn->Fill(dT, 1000*prhat);
	h_RingTracker_Pvhat_Turn->Fill(dT, 1000*pvhat);
	h_RingTracker_Mom_Turn->Fill(dT, mom);
	h_RingTracker_Prhat_Time->Fill(time/1000, 1000*prhat);
	h_RingTracker_Pvhat_Time->Fill(time/1000, 1000*pvhat);
	h_RingTracker_Mom_Time->Fill(time/1000, mom);
      }
    }
  } // loop over ring tracker hits



      
  //-----------------
  // Ring Information
  //-----------------  
  cout << " -- Ring -- " << endl;
  int systemhits[10];
  int numringhits[30];
  
  for ( int sh = 0; sh < 10; sh++ ) { systemhits[sh] = 0; }
  for ( int rh = 0; rh < 30; rh++ ) { numringhits[rh] = 0; }

  
      
  for ( auto ringhit : ringhits) {
    int ringtrackID   = ringhit.trackID;
    int ringvolumeUID = ringhit.volumeUID;    
    double rhat = ringhit.rhat;
    double vhat = ringhit.vhat;
    double ringtheta = ringhit.theta;
    double x_ring = (rhat+7112) * cos(ringtheta);
    double z_ring = (rhat+7112) * sin(ringtheta);    
    double ringtime = ringhit.time;
    double deltaE = -1*ringhit.deltaE;
    if ( deltaE <= 0.0 ) { deltaE = 1e-10; }
    double deltaPrhat = ringhit.deltaPrhat;
    double deltaPvhat = ringhit.deltaPvhat;
 

    cout << "VID = " << ringvolumeUID << " \tName = " << pvs.stringGivenID(ringvolumeUID) << endl;

    if ( ringtrackID == 1 ) {
	    
	    
      //bool foundhit = false;
      // 	    if ( IsSomething(uom, volumeUID, "Equivalent") ||
      // 		 IsSomething(uom, volumeUID, "Window")  ||
      // 		 IsSomething(uom, volumeUID, "Flange")  ||
      // 		 IsSomething(uom, volumeUID, "Mandrel") ) {
      if ( 1 ) {
	int system = kInflectorHit;
	systemhits[system]++;
	if ( fill ) {
	  for ( int st = 0; st < 2; st++ ) {
	    if ( st == 1 && pass_final == false ) { continue; }
	    h_SystemHitTracker_XZ[system][st]->Fill(z_ring, x_ring, deltaE);
	    h_SystemHitTracker_RhoY[system][st]->Fill(rhat, vhat, deltaE);
	    h_SystemHitTracker_DeltaPy[system][st]->Fill(deltaPvhat);
	    h_SystemHitTracker_DeltaPx[system][st]->Fill(deltaPrhat);		  	    
	    double dT = ringtime/1000;
	    h_SystemHitTracker_YTime[system][st]->Fill(dT, vhat, deltaE);
	    h_SystemHitTracker_RhoTime[system][st]->Fill(dT, rhat, deltaE);
	  }
	}
      }

      for ( int system = 0; system < Nsystemtrackers; system++ ) {
	if ( system == kInflectorHit || 
	     system == kCaloSystemHit ||
	     system == kStrawSystemHit ||
	     system == kXtalSystemHit ) { continue; }
	if ( 1 ) {
	  systemhits[system]++;
	  if ( fill ) { 
	    for ( int st = 0; st < 2; st++ ) {
	      //cout << "\t" << system << "\t" << st << "\t" << pass_final << endl;
	      if ( st == 1 && pass_final == false ) { continue; }
	      h_SystemHitTracker_XZ[system][st]->Fill(z_ring, x_ring, deltaE);
	      h_SystemHitTracker_RhoY[system][st]->Fill(rhat, vhat, deltaE);
	      h_SystemHitTracker_DeltaPy[system][st]->Fill(deltaPvhat);
	      h_SystemHitTracker_DeltaPx[system][st]->Fill(deltaPrhat);
	      //cout << system << "\t" << deltaPvhat << endl;
	      double dT = ringtime/1000;
	      h_SystemHitTracker_YTime[system][st]->Fill(dT, vhat, deltaE);
	      h_SystemHitTracker_RhoTime[system][st]->Fill(dT, rhat, deltaE);
	    }
	  }
	}
      }



      for ( int ringhit = 0; ringhit < Nringhits; ringhit++ ) {
	if ( 1 ) {
	  numringhits[ringhit]++;
	  if ( fill ) { 
	    for ( int st = 0; st < 2; st++ ) {
	      //cout << "\t" << ringhit << "\t" << st << "\t" << pass_final << endl;
	      if ( st == 1 && pass_final == false ) { continue; }		    
	      h_RingHitTracker_XZ[ringhit][st]->Fill(z_ring, x_ring, deltaE);
	      h_RingHitTracker_RhoY[ringhit][st]->Fill(rhat, vhat, deltaE);
	      h_RingHitTracker_DeltaPy[ringhit][st]->Fill(deltaPvhat);
	      h_RingHitTracker_DeltaPx[ringhit][st]->Fill(deltaPrhat);
	      //if ( st == 0 ) {cout << inames[ringhit] << "\t" << deltaPvhat << "\t" << deltaPrhat << endl;}
	      double dT = ringtime/1000;
	      h_RingHitTracker_YTime[ringhit][st]->Fill(dT, vhat, deltaE);
	      h_RingHitTracker_RhoTime[ringhit][st]->Fill(dT, rhat, deltaE);
	    }
	  }
	}
      }
    }
  }

  for ( int system = 0; system < Nsystemtrackers; system++ ) {
    for ( int st = 0; st < 2; st++ ) {
      if ( st == 1 && pass_final == false ) { continue; }
      h_SystemHitTracker_Nhits[system][st]->Fill(systemhits[system]);
    }
  }

  for ( int rhit = 0; rhit < Nringhits; rhit++ ) {
    for ( int st = 0; st < 2; st++ ) {
      if ( st == 1 && pass_final == false ) { continue; }
      h_RingHitTracker_Nhits[rhit][st]->Fill(numringhits[rhit]);
    }
  }






  //-------------------
  // G4 Generated Stuff
  //-------------------
  //double x_gen, y_gen, xprime_gen, yprime_gen;

  for ( auto htruthdata : truthhits) {  

    std::string trackType = htruthdata.trackType;
    int trackID = htruthdata.trackID;
    //int parentTrackID = htruthdata.parentTrackID;
    //int turn = htruthdata.turn;
    //int volumeUID = htruthdata.volumeUID;
    double rhat = htruthdata.rhat;
    double vhat = htruthdata.vhat;
    double theta = htruthdata.theta;
    double time = htruthdata.time;
    double p = htruthdata.p;
    double prhat = htruthdata.prhat;
    double pvhat = htruthdata.pvhat;
    double t0 = time;
    //double xprime = prhat;
    //double yprime = pvhat;
    //double x_ring;
    //double y_ring;
    //double z_ring;
    double py = pvhat * p;
    double px = prhat * p;
    double pz = TMath::Sqrt(p*p - px*px - py*py);
    double y = vhat;
    double x = (rhat + 7112) * TMath::Cos(theta);
    double z = (rhat + 7112) * TMath::Sin(theta);

    if ( trackID != 1 ) { continue; }
    
    //cout << px << "\t" << py << "\t" << pz << endl;
    //cout << xprime << "\t" << yprime << endl;
    
    TVector3 truthmom(px, py, pz);
    double x_truth = x - beamStart.X();
    double y_truth = y - beamStart.Y();
    //double z_truth = z - beamStart.Z();
    double xprime_truth = truthmom.Dot(xAxis)/truthmom.Mag();
    double yprime_truth = truthmom.Dot(yAxis)/truthmom.Mag();	  
    //double zprime_truth = truthmom.Dot(zAxis)/truthmom.Mag();
    
    //     x_gen = x_truth;
    //     y_gen = y_gen;
    //     xprime_gen = xprime_truth;
    //     yprime_gen = yprime_truth;
    
    double rhat_offset = 274.3;
    rhat -= rhat_offset;
    
    double vhat_offset = 0;
    vhat -= vhat_offset;
    
    if ( fill ) { 
      for ( int mytime = 0; mytime < 5; mytime++ ) {
	if ( mytime == 1 && pass_5us == false ) { continue; }
	if ( mytime == 2 && pass_20us == false ) { continue; }
	if ( mytime == 3 && pass_50us == false ) { continue; }
	if ( mytime == 4 && pass_final == false ) { continue; }
	h_G4Tracker_XprimeXTime[mytime]->Fill(x_truth, 1000*xprime_truth);
	h_G4Tracker_YprimeYTime[mytime]->Fill(y_truth, 1000*yprime_truth);
	h_G4Tracker_MomTime[mytime]->Fill(p);
	h_G4Tracker_RhoYTime[mytime]->Fill(x_truth, y_truth);
	h_G4Tracker_XZTime[mytime]->Fill(x, z);
	h_G4Tracker_t0Time[mytime]->Fill(t0);
      }
    }
  } // loop over G4 track




} // analyze

using gm2ringsim::ringTrackerAnalyzer;
DEFINE_ART_MODULE(ringTrackerAnalyzer)
