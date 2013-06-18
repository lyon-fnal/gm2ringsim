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
#include "TLorentzVector.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "artg4/pluginActions/physicalVolumeStore/PhysicalVolumeStoreData.hh"
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"
#include "artg4/util/DataFromRunOrService.hh"
#include "artg4/util/util.hh"

// Useful Stuff
#include "gm2ringsim/common/UsefulVariables.hh"

// Hit includes
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include "gm2ringsim/common/virtualringstation/VirtualRingStationArtRecord.hh"
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

#define Rhatbin_Inflector 401
#define Rhatmin_Inflector -200.5
#define Rhatmax_Inflector 200.5

#define Xbin_Inflector 101
#define Xmin_Inflector -50.5
#define Xmax_Inflector 50.5

#define Ybin_Inflector 101
#define Ymin_Inflector -50.5
#define Ymax_Inflector 50.5

#define Vbin_Inflector 101
#define Vmin_Inflector -50.5
#define Vmax_Inflector 50.5

#define Zbin_Inflector 401
#define Zmin_Inflector -200.5
#define Zmax_Inflector 200.5

#define XPrimebin_Inflector 101
#define XPrimemin_Inflector -25.5
#define XPrimemax_Inflector 25.5

#define YPrimebin_Inflector 101
#define YPrimemin_Inflector -25.5
#define YPrimemax_Inflector 25.5

#define Rhatbin_Ring 701
#define Rhatmin_Ring -300.5
#define Rhatmax_Ring 400.5

#define Xbin_Ring 261
#define Xmin_Ring -60.5
#define Xmax_Ring 200.5
#define Ybin_Ring 121
#define Ymin_Ring -60.5
#define Ymax_Ring 60.5
#define Rbin_Ring 1801
#define Rmin_Ring -9000.5
#define Rmax_Ring 9000.5

#define YPrimebin_Ring 201
#define YPrimemin_Ring -25.5
#define YPrimemax_Ring 25.5
#define XPrimebin_Ring 201
#define XPrimemin_Ring -50.5
#define XPrimemax_Ring 50.5

#define YPrimebin_Decay_Ring 251
#define YPrimemin_Decay_Ring -250.5
#define YPrimemax_Decay_Ring 250.5
#define XPrimebin_Decay_Ring 251
#define XPrimemin_Decay_Ring -500.5
#define XPrimemax_Decay_Ring 500.5

#define Polbin 101
#define Polmin -1
#define Polmax 1

#define PolYbin 101
#define PolYmin -1
#define PolYmax 1

#define Mombin 2001
#define Mommin 0.0
#define Mommax 2.0

#define Rhatbin 121
#define Rhatmin -60.5
#define Rhatmax 60.5

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

  
  virtual void beginJob();
  virtual void endJob();
  virtual void beginRun(art::Run const &r);
  virtual void endRun(art::Run const &r);
   
  
private:
  
  
  std::string ringtrackerhitModuleLabel_;
  std::string ringtrackerinstanceName_;
  std::string truthhitModuleLabel_;
  std::string truthinstanceName_;
  std::string truthfinishedhitModuleLabel_;
  std::string truthfinishedinstanceName_;
  std::string inflectorhitModuleLabel_;
  std::string inflectorinstanceName_;
  std::string ringhitModuleLabel_;
  std::string ringinstanceName_;
  std::string pvsProducerLabel_;
  std::string pvsInstanceLabel_;


  
  // Names of the sub-directories in the root file
  std::string hist_dir_, tree_dir_;

  std::string beamstart_;
  std::string basename_;
  int maxturnsreal_;
  int maxturns_;
  double LaunchAngle_;
  double InflectorAngle_;
  double StorageOffset_;

  bool SaveInfHits_;
  bool SaveRingHits_;
  bool SaveTruthHits_;
  bool SaveVRingHits_;
  bool SaveVRing1PlaneHits_;
  
  bool debug_;
  
  int IsSomething(string name);
  bool IsSomething(string name, int comp);
  bool IsKicker(string name);
  bool IsQuad(string name);
  bool IsCryostat(string name);
  bool IsCollimator(string name);
  bool IsInflector(string name);
  bool IsVacuumChamber(string name);
  bool IsVirtualRingStation(string name);
  bool IsVirtualRingStation(string name, int *num);

  double Pmagic();

  double ComputeQuadIntAngle(double r1, double r2, double d1, double d2);
  double ComputeRmagicIntAngle(double r1, double r2, double d1, double d2);
  double ComputeKickAtRmagicInt(double pr1, double pr2, double d1, double d2, double dr0);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z);

  void FillRingTracker(int hist, int type, double xe, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly);
  void FillTimeRingTracker(int hist, double xe, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly);
  void FillStoredRingTracker(int hist, double xe, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly);

  void FillTruthTracker(int hist, int status, int pdgid, double xe, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double t0, double polz, double polx, double poly, double Py, double Polarization);
  void FillTruthTracker(int hist, int particle, double xe_truth, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double t0, double polz, double polx, double poly, double Py, double Polarization);
  void FillTruthTurnTime(double dT, int counter, int status, int pdgid, double time, double xe, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e);
  void FillTruthTurnTime(double xval, int hist, int particle, double xe, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e);

  void FillTurnTime(double dT, int counter, int trackID, double time, double xe, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e);
  void FillKickDegree(int histtype, double degree_at_r0, double kick_at_r0, double mom);
  void FillInflector(int hist, double x_inf, double y_inf, double z_inf, double r_inf, double prhat_ring, double pvhat_ring, double xprime_inf, double yprime_inf);

  void InitializeBeamCoordinates();
  void RotateRingToBeamCoordinates(double theta);

  void NewFixTurnCounter(int *global_turn, int mytrack_vol, int ptrack_vol);
  void FixTurnCounter(int *myturn, int *mytrack, double time, double t1);
  
  void SetInitial(double *t0, int *mytrack, double *t1, double *theta1, double *prhat_init, double *pvhat_init, double *xe_init, double *rhat_init, double *vhat_init, double *pmom_init, double *pol_init, double time, double theta, double prhat, double pvhat, double xe, double rhat, double vhat, double mom, double pol);

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
  int Npass_final = 0;
  int Npass_finalavg = 0;
  int Ninside_midkicker = 0;
  int Ninside_allkicker = 0;
  int Noutside_kickers = 0;
  int Npass_Nturns[5][2];
  int Ntimevals;
  string timevals[2];
  int Ntimestamps;
  string timestamps[2];

  string rtnames[9];
 

  
  //-------------------------------
  // Kicker/Quad/Launch Information
  //-------------------------------
  TH2F *h_RingTracker_PrhatPvhatEntrance;
  TH2F *h_RingTracker_FirstTurnX[2];
  TH2F *h_RingTracker_FirstTurnY[2];
  TH1F *h_RingTracker_DegreeAtRhat0[2];
  TH1F *h_RingTracker_DegreeAtRhat0_Mom[2][6];
  TH2F *h_RingTracker_DegreeAtRhat0RhatInit[2];
  TH1F *h_RingTracker_DegreeAtQuad[2];
  TH1F *h_RingTracker_KickAtRhat0[2];
  TH1F *h_RingTracker_KickAtRhat0_Mom[2][6];
  TH2F *h_RingTracker_KickAtRhat0RhatInit[2];
  TH2F *h_RingTracker_DegreeAtRhat0KickAtRhat0[2];
  TH2F *h_RingTracker_DegreeAtRhat0KickAtRhat0_Mom[2][6];


  //-------------------------------
  // 30 degree virtual trackers
  //-------------------------------
  int Nringtrackers;
  string ringtrackernames[9];
  TH2F *h_RingTracker_RhatY[9];
  TH2F *h_RingTracker_XprimeX[9];
  TH2F *h_RingTracker_YprimeY[9];  
  TH1F *h_RingTracker_Mom[9];
  TH1F *h_RingTracker_Rhat[9];
  TH1F *h_RingTracker_Xe[9];
  TH1F *h_RingTracker_Vhat[9];
  TH1F *h_RingTracker_Pol[9];
  TH1F *h_RingTracker_PolX[9];
  TH1F *h_RingTracker_PolY[9];


  //--------------------------
  // Virtual Ring Trackers
  //--------------------------
  int Nringtrackertimes;
  string times[3];
  TH1F *h_RingTracker_Time_Xprime[3][2];
  TH1F *h_RingTracker_Time_Yprime[3][2];
  TH2F *h_RingTracker_Time_XprimeX[3][2];
  TH2F *h_RingTracker_Time_YprimeY[3][2];
  TH2F *h_RingTracker_Time_XprimeYprime[3][2];
  TH2F *h_RingTracker_Time_RhatY[3][2];
  TH1F *h_RingTracker_Time_Rhat[3][2];
  TH1F *h_RingTracker_Time_Xe[3][2];
  TH1F *h_RingTracker_Time_Vhat[3][2];
  TH1F *h_RingTracker_Time_Mom[3][2];
  TH1F *h_RingTracker_Time_Pol[3][2];
  TH1F *h_RingTracker_Time_PolX[3][2];
  TH1F *h_RingTracker_Time_PolY[3][2];
  TH1F *h_RingTracker_Time_dNdXe;
  TH1F *h_RingTracker_Time_dNdX;
  TH1F *h_RingTracker_Time_dNdXprime;
  TH1F *h_RingTracker_Time_dNdY;
  TH1F *h_RingTracker_Time_dNdYprime;
  TH2F *h_RingTracker_Time_d2NdXY;
  TH2F *h_RingTracker_Time_d2NdXprimeYprime;
  TH2F *h_RingTracker_Time_d2NdXprimeX;
  TH2F *h_RingTracker_Time_d2NdYprimeY;

  
  //int Nstoredtrackers;
  //string rtsnames[11];


  TH2F *h_RingTracker_Rhat_vs_Time[2];
  TH2F *h_RingTracker_Xe_vs_Time[2];
  TH2F *h_RingTracker_Prhat_vs_Time[2];
  TH2F *h_RingTracker_Pvhat_vs_Time[2];
  TH2F *h_RingTracker_Mom_vs_Time[2];
  TH2F *h_RingTracker_Y_vs_Time[2];
  TH2F *h_RingTracker_Pol_vs_Time[2];
  TH2F *h_RingTracker_PolX_vs_Time[2];
  TH2F *h_RingTracker_PolY_vs_Time[2];
  TH1F *h_RingTracker_NgtEth_vs_Time[2];
  TH1F *h_RingTracker_NwghtE_vs_Time[2];
  TH1F *h_RingTracker_N_vs_Time[2];


  //------------------
  // Truth Information
  //------------------
  int Ntruthtimes;
  int Nparticles;
  string truth_particle_names[7];
  TH1F *h_G4Tracker_Time_Xe[3][7];
  TH1F *h_G4Tracker_Time_Rhat[3][7];
  TH1F *h_G4Tracker_Time_Vhat[3][7];
  TH1F *h_G4Tracker_Time_Pol[3][7];
  TH1F *h_G4Tracker_Time_PolX[3][7];
  TH1F *h_G4Tracker_Time_PolY[3][7];
  TH1F *h_G4Tracker_Time_Xprime[3][7];
  TH1F *h_G4Tracker_Time_Yprime[3][7];
  TH2F *h_G4Tracker_Time_XprimeX[3][7];
  TH2F *h_G4Tracker_Time_YprimeY[3][7];
  TH2F *h_G4Tracker_Time_RhatY[3][7];
  TH2F *h_G4Tracker_Time_XZ[3][7];
  TH1F *h_G4Tracker_Time_Mom[3][7];
  TH1F *h_G4Tracker_Time_t0[3][7];
  TH1F *h_G4Tracker_Time_dCosTheta[3][7];
  TH1F *h_G4Tracker_Time_Polarization[3][7];
  TH2F *h_G4Tracker_Time_dXdCosTheta[3][7];
  TH1F *h_G4Tracker_Time_dX[3][7];
  
  TH2F *h_G4Tracker_Rhat_vs_Time[2][7];
  TH2F *h_G4Tracker_Xe_vs_Time[2][7];
  TH2F *h_G4Tracker_Prhat_vs_Time[2][7];
  TH2F *h_G4Tracker_Pvhat_vs_Time[2][7];
  TH2F *h_G4Tracker_Mom_vs_Time[2][7];
  TH2F *h_G4Tracker_Y_vs_Time[2][7];
  TH2F *h_G4Tracker_Pol_vs_Time[2][7];
  TH2F *h_G4Tracker_PolX_vs_Time[2][7];
  TH2F *h_G4Tracker_PolY_vs_Time[2][7];
  TH2F *h_G4Tracker_E_vs_Time[2][7];
  TH1F *h_G4Tracker_NgtEth_vs_Time[2][7];
  TH1F *h_G4Tracker_NwghtE_vs_Time[2][7];
  TH1F *h_G4Tracker_N_vs_Time[2][7];
  TH1F *h_G4Tracker_Ndown_vs_Time[2][7];
  TH1F *h_G4Tracker_Nup_vs_Time[2][7];
  TH1F *h_G4Tracker_Nud_vs_Time[2][7];
  TH1F *h_G4Tracker_Yprime_vs_Time[2][7];
  TH1F *h_G4Tracker_Vhat_vs_Time[2][7];
  int G4Tracker_Nu[7];
  int G4Tracker_Nd[7];


  //--------------------------
  // Physical Energy Deposits
  //--------------------------
  int Nsystemtrackers;
  string rhitnames[11];
  int kInflectorHit;
  int kCryostatHit;
  int kQuadHit;
  int kKickerHit;
  int kCollimatorHit;
  int kVacuumHit;
  int kStrawSystemHit;
  int kCaloSystemHit;
  int kXtalSystemHit;  
  int kVirtualRingStationHit;
  int kAllSystemsHit;
  TH1F *h_SystemHitTracker_Nhits[11][2];
  TH2F *h_SystemHitTracker_XZ[11][2];
  TH2F *h_SystemHitTracker_RhatY[11][2];
  TH2F *h_SystemHitTracker_RhatTime[11][2];
  TH2F *h_SystemHitTracker_YTime[11][2];
  TH1F *h_SystemHitTracker_DeltaPx[11][2];
  TH1F *h_SystemHitTracker_DeltaPy[11][2];


  //--------------------------
  // (Individual) Physical Energy Deposits
  //--------------------------
  int Nringhits;
  string inames[22];
  TH1F *h_RingHitTracker_Nhits[22][2];
  TH2F *h_RingHitTracker_XZ[22][2];
  TH2F *h_RingHitTracker_RhatY[22][2];
  TH2F *h_RingHitTracker_RhatTime[22][2];
  TH2F *h_RingHitTracker_YTime[22][2];
  TH1F *h_RingHitTracker_DeltaPx[22][2];
  TH1F *h_RingHitTracker_DeltaPy[22][2];


  
  //--------------------------
  // Inflector Information
  //--------------------------
  int Ninflectortrackers;
  TH2F *h_InflectorTracker_XprimeX[12];
  TH2F *h_InflectorTracker_YprimeY[12];
  TH2F *h_InflectorTracker_RhatY[12];
  TH2F *h_InflectorTracker_XZ[12];
  TH2F *h_InflectorTracker_RZ;
  TH2F *h_InflectorTracker_X;
  TH2F *h_InflectorTracker_Y;
  TH2F *h_InflectorTracker_Z;
  TH2F *h_InflectorTracker_PrhatPvhatEntrance;
  TH2F *h_InflectorTracker_PrhatPvhatExit;


  TVector3 xAxis, yAxis, zAxis;
  TVector3 xAxis_beam_, yAxis_beam_, zAxis_beam_;
  TVector3 beamStart_;
  double rhat_offset, yhat_offset;
};

void gm2ringsim::ringTrackerAnalyzer::beginJob()
{
  cout << "gm2ringsim::ringTrackerAnalyzer::beginJob()" << endl;
}

void gm2ringsim::ringTrackerAnalyzer::endJob()
{
  cout << "gm2ringsim::ringTrackerAnalyzer::endJob()" << endl;
}

void gm2ringsim::ringTrackerAnalyzer::beginRun(art::Run const &r)
{
  cout << "gm2ringsim::ringTrackerAnalyzer::beginRun(" << r.run() << ")" << endl;
}

void gm2ringsim::ringTrackerAnalyzer::endRun(art::Run const &r)
{
  cout << "gm2ringsim::ringTrackerAnalyzer::endRun(" << r.run() << ")" << endl;

  cout << "Generator \t" << Ngen << endl;
  cout << "Inflector \t" << Nstart_inflector << "\t" << Nexit_inflector << endl;
  cout << "RingStart \t" << Nstart_ring << endl;
  cout << "RingStored\t" << Nstored1_ring << "\t" << Nstored10_ring << "\t" << Nstored100_ring << endl;
  cout << "<Theta @ x=0>   = " << Ndegat0/(Ninside_midkicker+Ninside_allkicker) << endl;
  cout << "<x' @ Theta>    = " << 1000*Nxpat0/(Ninside_midkicker+Ninside_allkicker) << endl;
  cout << "[K2, K1-K3, !K] = " << Ninside_midkicker << " , " << Ninside_allkicker << " , " << Noutside_kickers << endl;
  
  ofstream out;
  string outputname = "Eff/" + basename_ + ".dat";
  out.open(outputname.c_str());
  out << Nstart_inflector << "\t" << Nexit_inflector << endl;
  out << Nstart_ring << endl;
  out << Nstored1_ring << "\t" << Nstored10_ring << "\t" << Nstored100_ring << endl;
  out << Ngen << endl;
  out << Ndegat0/(Ninside_midkicker+Ninside_allkicker) << "  0.0" << endl;
  out << 1000*Nxpat0/(Ninside_midkicker+Ninside_allkicker) << "  0.0" << endl;
  out.close();


  if ( SaveVRingHits_ ) {
    for ( int i = 0; i < Nringtrackertimes; i++ ) {
      if ( times[i] != "FinalAvg" ) { continue; }
      if ( Npass_final > 0 && Npass_finalavg > 0 ) {
	double finalavg_sf = (double)Npass_final / Npass_finalavg;
	cout << "Scaling FinalAvg histograms by " << finalavg_sf << endl;
	h_RingTracker_Time_Xprime[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_Yprime[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_XprimeX[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_YprimeY[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_XprimeYprime[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_RhatY[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_Rhat[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_Xe[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_Vhat[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_Mom[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_Pol[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_PolX[i][0]->Scale(finalavg_sf);
	h_RingTracker_Time_PolY[i][0]->Scale(finalavg_sf);
      }
    }

    if ( Npass_final > 0 && Npass_finalavg > 0 ) {
      double finalavg_sf = (double)Npass_final / Npass_finalavg;
      cout << "Scaling FinalAvg histograms by " << finalavg_sf << endl;
      h_RingTracker_Time_dNdXe->Scale(finalavg_sf);
      h_RingTracker_Time_dNdX->Scale(finalavg_sf);
      h_RingTracker_Time_dNdXprime->Scale(finalavg_sf);
      h_RingTracker_Time_d2NdXprimeX->Scale(finalavg_sf);
      h_RingTracker_Time_dNdY->Scale(finalavg_sf);
      h_RingTracker_Time_dNdYprime->Scale(finalavg_sf);
      h_RingTracker_Time_d2NdYprimeY->Scale(finalavg_sf);
      h_RingTracker_Time_d2NdXY->Scale(finalavg_sf);
      h_RingTracker_Time_d2NdXprimeYprime->Scale(finalavg_sf);
    }
  }
 

  cout << "======================" << endl;
  if ( maxturnsreal_ >= 5 ) {
    cout << "Pass 5 turns:   " << Npass_Nturns[0][1] << endl;
    if ( maxturnsreal_ >= 10 ) {
      cout << "Pass 10 turns:  " << Npass_Nturns[1][1] << endl;
      if ( maxturnsreal_ >= 50 ) {
	cout << "Pass 50 turns:  " << Npass_Nturns[2][1] << endl;
	if ( maxturnsreal_ >= 100 ) {
	  cout << "Pass 100 turns: " << Npass_Nturns[3][1] << endl;
	}
      }
    }
  }
  
  if ( maxturnsreal_ < 100 ) {
    cout << "Pass " << maxturnsreal_ << " turns:  " << Npass_final << endl;
  }
  else {
    cout << "Pass " << maxturnsreal_ << " turns: " << Npass_final << endl;
  }
  cout << "======================" << endl;
  cout << endl;
  


}


gm2ringsim::ringTrackerAnalyzer::ringTrackerAnalyzer(fhicl::ParameterSet const &p) :
  ringtrackerhitModuleLabel_ ( p.get<std::string>("ringtrackerhitModuleLabel",  "artg4"   ) ),
  ringtrackerinstanceName_   ( p.get<std::string>("ringtrackerinstanceName",    "VirtualRingStation"     ) ),
  truthhitModuleLabel_       ( p.get<std::string>("truthhitModuleLabel",  "artg4"   ) ),
  truthinstanceName_         ( p.get<std::string>("truthinstanceName",    ""     ) ),
  truthfinishedhitModuleLabel_       ( p.get<std::string>("truthfinishedhitModuleLabel",  "artg4"   ) ),
  truthfinishedinstanceName_         ( p.get<std::string>("truthfinishedinstanceName",    "FinishedTracks"     ) ),
  inflectorhitModuleLabel_   ( p.get<std::string>("inflectorhitModuleLabel",  "artg4"   ) ),
  inflectorinstanceName_     ( p.get<std::string>("inflectorinstanceName",    "inflector"     ) ),
  ringhitModuleLabel_        ( p.get<std::string>("ringhitModuleLabel",  "artg4"   ) ),
  ringinstanceName_          ( p.get<std::string>("ringinstanceName",    "Ring"     ) ),
  pvsProducerLabel_          ( p.get<std::string>("pvsProducerLabel", "artg4")),
  pvsInstanceLabel_          ( p.get<std::string>("pvsInstanceLabel", "")),
  hist_dir_           ( p.get<std::string>("hist_dir"         ) ),
  tree_dir_           ( p.get<std::string>("tree_dir"         ) ),
  beamstart_          ( p.get<std::string>("beamstart") ),
  basename_           ( p.get<std::string>("basename") ),
  maxturnsreal_       ( p.get<int>("maxturns", 200) ),
  LaunchAngle_        ( p.get<double>("LaunchAngle", -9999.9)),
  InflectorAngle_     ( p.get<double>("InflectorAngle", 0.0)),
  StorageOffset_      ( p.get<double>("StorageOffset", 0.0)),
  SaveInfHits_      ( p.get<bool>("SaveInfHits", false)),
  SaveRingHits_      ( p.get<bool>("SaveRingHits", false)),
  SaveTruthHits_      ( p.get<bool>("SaveTruthHits", false)),
  SaveVRingHits_      ( p.get<bool>("SaveVRingHits", false)),
  SaveVRing1PlaneHits_      ( p.get<bool>("SaveVRing1PlaneHits", false)),
  debug_                    ( p.get<bool>("debug", false))
	
{
  //truthinstanceName_   ( p.get<std::string>("truthinstanceName",    "TrackingAction"     ) ),
  // You could require that hist_dir and tree_dir have some real strings, but what if the
  // user just leaves them blank. We then want to use the top level directory.
  // Note that the TFileService handle dereferences to an object that inherits from
  // TFileDirectory. This will be a little tricky, so pay close attention

  if ( StorageOffset_ <= 0.0 ) { StorageOffset_ = 0.0; }

  cout << "==========================================" << endl;
  cout << "ringTrackerAnalyzer::ringTrackerAnalyzer()" << endl;
  cout << "  -> Beamstart: " << beamstart_ << endl;
  cout << "  -> Max Turns: " << maxturnsreal_ << endl;
  cout << "  -> Launch:    " << LaunchAngle_ << " mrad." << endl;
  cout << "  -> Inflector: " << InflectorAngle_ << " mrad." << endl;
  cout << "  -> Offset:    " << StorageOffset_ << " mm." << endl;
  if ( SaveInfHits_ ) { cout << "  -> Saving inflector hits." << endl; }
  if ( SaveRingHits_ ) { cout << "  -> Saving ring hits." << endl; }
  if ( SaveTruthHits_ ) { cout << "  -> Saving truth hits." << endl; }
  if ( SaveVRingHits_ ) { cout << "  -> Saving virtual ring hits." << endl; }
  if ( SaveVRing1PlaneHits_ ) { cout << "  -> Saving virtual ring (1 plane) hits." << endl; }  
  cout << "==========================================" << endl;
  
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

  //debug_ = false;

  maxturns_ = maxturnsreal_;
  Ndegat0 = 0.0;
  Nxpat0 = 0.0;
  Ngen = 0;
  Nstart_inflector = 0;
  Nexit_inflector = 0;
  Nstart_ring = 0;
  Nstored1_ring = 0;
  Nstored10_ring = 0;
  Nstored100_ring = 0;

  for ( int i = 0; i < 5; i++ ) {
    Npass_Nturns[i][0] = 0;
    Npass_Nturns[i][1] = 0;
  }


  Nsystemtrackers = 11;
  rhitnames[0] = "Inflector";
  rhitnames[1] = "Cryostat" ;
  rhitnames[2] = "Quad" ;
  rhitnames[3] = "Kicker" ;
  rhitnames[4] = "Collimator" ;
  rhitnames[5] = "Vacuum" ;
  rhitnames[6] = "StrawTracker" ;
  rhitnames[7] = "Calo" ;
  rhitnames[8] = "Xtal";
  rhitnames[9] = "VirtualRingStation";
  rhitnames[10] = "AllSystems";
  kInflectorHit = 0;
  kCryostatHit = 1;
  kQuadHit = 2;
  kKickerHit = 3;
  kCollimatorHit = 4;
  kVacuumHit = 5;
  kStrawSystemHit = 6;
  kCaloSystemHit = 7;
  kXtalSystemHit = 8;
  kVirtualRingStationHit = 9;
  kAllSystemsHit = 10;

  Nringtrackertimes = 3;
  Ntruthtimes = 3;
  times[0] = "Init";
  times[1] = "Final"; 
  times[2] = "FinalAvg"; 

  Ntimevals = 2;
  timevals[0] = "Init";
  timevals[1] = "Store";

  Nringhits = 0; 
  inames[0] = "UpstreamEndFlange";
  inames[1] = "UpstreamEquivalentNbTi";  
  inames[2] = "UpstreamEquivalentAl";  
  inames[3] = "UpstreamEquivalentCu";  
  inames[4] = "UpstreamWindow";  
  inames[5] = "DownstreamWindow";  
  inames[6] = "DownstreamEquivalentNbTi";  
  inames[7] = "DownstreamEquivalentAl";  
  inames[8] = "DownstreamEquivalentCu";  
  inames[9] = "DownstreamEndFlange";
  inames[10] = "Mandrel";  
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


  Ninflectortrackers = 9;


  Nringtrackers = 9;
  ringtrackernames[0] = "00";  
  ringtrackernames[1] = "01";  
  ringtrackernames[2] = "02";  
  ringtrackernames[3] = "03";  
  ringtrackernames[4] = "04";  
  ringtrackernames[5] = "05";  
  ringtrackernames[6] = "06";  
  ringtrackernames[7] = "07";  
  ringtrackernames[8] = "08";

  
  //   Nstoredtrackers = 11;
  //   rtsnames[0] = "0Turn";  
  //   rtsnames[1] = "1Turn";  
  //   rtsnames[2] = "2Turn";  
  //   rtsnames[3] = "5Turn";  
  //   rtsnames[4] = "10Turn";  
  //   rtsnames[5] = "50Turn";  
  //   rtsnames[6] = "100Turn";  
  //   rtsnames[7] = "200Turn";  
  //   rtsnames[8] = "500Turn";  
  //   rtsnames[9] = "1000Turn";  
  //   rtsnames[10] = "2000Turn";

  Ntimestamps = 2;
  timestamps[0] = "Turn";
  timestamps[1] = "OncePerTurn";


  Nparticles = 7;
  truth_particle_names[0] = "BirthMuon";
  truth_particle_names[1] = "DecayMuon";
  truth_particle_names[2] = "LostMuon";
  truth_particle_names[3] = "DecayElectron";
  truth_particle_names[4] = "BirthElectron";
  truth_particle_names[5] = "DecayElectronEgtEth";
  truth_particle_names[6] = "BirthElectronEgtEth";
  fill = true;


 
  stringstream hname;

  if ( SaveVRingHits_ ) {

    for ( int t = 0; t < Ntimevals; t++ ) {
      string typname = timevals[t];
    
      for ( int tt = 0; tt < Nringtrackertimes; tt++ ) {
	string ringtracker_time_name = times[tt];
      
	if ( fill ) {
	
	  hname << "RingTracker_Time_Xprime_" << ringtracker_time_name << "_" << typname;
	  //cout << hname.str() << endl;
	  h_RingTracker_Time_Xprime[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	  h_RingTracker_Time_Xprime[tt][t]->SetXTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_Time_Yprime_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_Yprime[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	  h_RingTracker_Time_Yprime[tt][t]->SetXTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_Time_XprimeX_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_XprimeX[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	  h_RingTracker_Time_XprimeX[tt][t]->SetXTitle("x_{ring} [mm]");
	  h_RingTracker_Time_XprimeX[tt][t]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_Time_YprimeY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_YprimeY[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Ybin_Ring, Ymin_Ring, Ymax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	  h_RingTracker_Time_YprimeY[tt][t]->SetXTitle("y_{ring} [mm]");
	  h_RingTracker_Time_YprimeY[tt][t]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
      
	  hname << "RingTracker_Time_XprimeYprime_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_XprimeYprime[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	  h_RingTracker_Time_XprimeYprime[tt][t]->SetXTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  h_RingTracker_Time_XprimeYprime[tt][t]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_Time_RhatY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_RhatY[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_Time_RhatY[tt][t]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_RingTracker_Time_RhatY[tt][t]->SetYTitle("y_{ring} [mm]");
	  hname.str("");
	
	  hname << "RingTracker_Time_Rhat_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_Rhat[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_Time_Rhat[tt][t]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  hname.str("");
	
	  hname << "RingTracker_Time_Xe_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_Xe[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_Time_Xe[tt][t]->SetXTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	  hname.str("");
	
	  hname << "RingTracker_Time_Vhat_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_Vhat[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_Time_Vhat[tt][t]->SetXTitle("y_{ring} [mm]");
	  hname.str("");
      
	  hname << "RingTracker_Time_Mom_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_Mom[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	  h_RingTracker_Time_Mom[tt][t]->SetXTitle("P_{#mu} / P_{m}");
	  hname.str("");

	  hname << "RingTracker_Time_Pol_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_Pol[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_RingTracker_Time_Pol[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	  hname.str("");

	  hname << "RingTracker_Time_PolX_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_PolX[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_RingTracker_Time_PolX[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	  hname.str("");

	  hname << "RingTracker_Time_PolY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Time_PolY[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	  h_RingTracker_Time_PolY[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
	  hname.str("");
	}
      }
    }

    if ( fill ) {
      hname.str("");  
      hname << "RingTracker_PrhatPvhatEntrance";
      h_RingTracker_PrhatPvhatEntrance = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring/10, XPrimemin_Ring/100, XPrimemax_Ring/100, YPrimebin_Ring/10, YPrimemin_Ring/100, YPrimemax_Ring/100);
      h_RingTracker_PrhatPvhatEntrance->SetYTitle("#hat{P}_{v}");
      h_RingTracker_PrhatPvhatEntrance->SetXTitle("#hat{P}_{r}");
      hname.str("");  

      hname << "RingTracker_Time_dNdXe";
      h_RingTracker_Time_dNdXe = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_Time_dNdXe->SetXTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
      hname.str("");

      hname << "RingTracker_Time_dNdX";
      h_RingTracker_Time_dNdX = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_Time_dNdX->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
      hname.str("");
    
      hname << "RingTracker_Time_dNdY";
      h_RingTracker_Time_dNdY = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_Time_dNdY->SetXTitle("y_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_Time_d2NdXY";
      h_RingTracker_Time_d2NdXY = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_Time_d2NdXY->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
      h_RingTracker_Time_d2NdXY->SetYTitle("y_{ring} [mm]");
      hname.str("");


      hname << "RingTracker_Time_dNdXprime";
      h_RingTracker_Time_dNdXprime = histDir.make<TH1F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
      h_RingTracker_Time_dNdXprime->SetXTitle("x'_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_Time_dNdYprime";
      h_RingTracker_Time_dNdYprime = histDir.make<TH1F>(hname.str().c_str(), "", YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
      h_RingTracker_Time_dNdYprime->SetXTitle("y'_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_Time_d2NdXprimeYprime";
      h_RingTracker_Time_d2NdXprimeYprime = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
      h_RingTracker_Time_d2NdXprimeYprime->SetYTitle("x'_{ring} #equiv R - R_{m} [mm]");
      h_RingTracker_Time_d2NdXprimeYprime->SetXTitle("y'_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_Time_dNdXprimeX";
      h_RingTracker_Time_d2NdXprimeX = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
      h_RingTracker_Time_d2NdXprimeX->SetYTitle("x'_{ring} [mm]");
      h_RingTracker_Time_d2NdXprimeX->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
      hname.str("");
    
      hname << "RingTracker_Time_dNdYprimeY";
      h_RingTracker_Time_d2NdYprimeY = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
      h_RingTracker_Time_d2NdYprimeY->SetYTitle("y'_{ring} [mm]");
      h_RingTracker_Time_d2NdYprimeY->SetXTitle("y_{ring} [mm]");
      hname.str("");
    }

    if ( fill ) {

      for ( int st = 0; st < 2; st++ ) {
	string stname = "";
	if ( st == 1 ) { stname = "_Stored"; }
      
	hname << "RingTracker" << stname << "_DegreeAtRhat0";
	h_RingTracker_DegreeAtRhat0[st] = histDir.make<TH1F>(hname.str().c_str(), "", 180, 0, 180.0);
	h_RingTracker_DegreeAtRhat0[st]->SetXTitle("#theta_{ring}(R=R_{magic}) [deg]");
	hname.str(""); 
      
	for ( int mom = 0; mom < 6; mom++ ) {
	  hname << "RingTracker" << stname << "_DegreeAtRhat0_Mom" << mom;
	  h_RingTracker_DegreeAtRhat0_Mom[st][mom] = histDir.make<TH1F>(hname.str().c_str(), "", 180, 0, 180.0);
	  h_RingTracker_DegreeAtRhat0_Mom[st][mom]->SetXTitle("#theta_{ring}(R=R_{magic}) [deg]");
	  hname.str(""); 
	}

	hname << "RingTracker" << stname << "_DegreeAtRhat0RhatInit";
	h_RingTracker_DegreeAtRhat0RhatInit[st] = histDir.make<TH2F>(hname.str().c_str(), "", 180, 0, 180.0, Rhatbin_Ring, Rhatmin_Ring, Rhatmax_Ring);
	h_RingTracker_DegreeAtRhat0RhatInit[st]->SetXTitle("#theta_{ring}(x=0) [deg]");
	h_RingTracker_DegreeAtRhat0RhatInit[st]->SetYTitle("x_{ring} #equiv (R - R_{magic})(#theta=0) [mm]");
	hname.str(""); 
      
	hname << "RingTracker" << stname << "_DegreeAtQuad";
	h_RingTracker_DegreeAtQuad[st] = histDir.make<TH1F>(hname.str().c_str(), "", 180, 0, 180.0);
	h_RingTracker_DegreeAtQuad[st]->SetXTitle("#theta_{ring}(R=R_{magic}+50 mm) [deg]");
	hname.str(""); 
      
	hname << "RingTracker" << stname << "_KickAtRhat0";
	h_RingTracker_KickAtRhat0[st] = histDir.make<TH1F>(hname.str().c_str(), "", 10*XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	h_RingTracker_KickAtRhat0[st]->SetXTitle("x'_{ring}(R=R_{magic})(x=0) [mrad]");
	hname.str(""); 
      
	for ( int mom = 0; mom < 6; mom++ ) {
	  hname << "RingTracker" << stname << "_KickAtRhat0_Mom" << mom;
	  h_RingTracker_KickAtRhat0_Mom[st][mom] = histDir.make<TH1F>(hname.str().c_str(), "", 10*XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	  h_RingTracker_KickAtRhat0_Mom[st][mom]->SetXTitle("x'_{ring}(R=R_{magic})(x=0) [mrad]");
	  hname.str(""); 
	}

	hname << "RingTracker" << stname << "_KickAtRhat0RhatInit";
	h_RingTracker_KickAtRhat0RhatInit[st] = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_KickAtRhat0RhatInit[st]->SetXTitle("x'_{ring}(x=0) [mrad]");
	h_RingTracker_KickAtRhat0RhatInit[st]->SetYTitle("x_{ring} #equiv (R - R_{magic})(#theta=0) [mm]");
	hname.str(""); 
    
	hname << "RingTracker" << stname << "_DegreeAtRhat0KickAtRhat0";
	h_RingTracker_DegreeAtRhat0KickAtRhat0[st] = histDir.make<TH2F>(hname.str().c_str(), "", 180, 0, 180.0, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	h_RingTracker_DegreeAtRhat0KickAtRhat0[st]->SetXTitle("#theta_{ring}(x=0) [deg]");
	h_RingTracker_DegreeAtRhat0KickAtRhat0[st]->SetYTitle("x'_{ring}(x=0) [mrad]");
	hname.str(""); 
      
	for ( int mom = 0; mom < 6; mom++ ) {
	  hname << "RingTracker" << stname << "_DegreeAtRhat0KickAtRhat0_Mom" << mom;
	  h_RingTracker_DegreeAtRhat0KickAtRhat0_Mom[st][mom] = histDir.make<TH2F>(hname.str().c_str(), "", 180, 0, 180.0, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	  h_RingTracker_DegreeAtRhat0KickAtRhat0_Mom[st][mom]->SetXTitle("#theta_{ring}(x=0) [deg]");
	  h_RingTracker_DegreeAtRhat0KickAtRhat0_Mom[st][mom]->SetYTitle("x'_{ring}(x=0) [mrad]");
	  hname.str(""); 
	}
    
	hname << "RingTracker" << stname << "_FirstTurnX";
	h_RingTracker_FirstTurnX[st] = histDir.make<TH2F>(hname.str().c_str(), "", 360, 0, 360.0, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_FirstTurnX[st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_FirstTurnX[st]->SetXTitle("#theta_{ring} [deg]");
	hname.str(""); 
    
	hname << "RingTracker" << stname << "_FirstTurnY";
	h_RingTracker_FirstTurnY[st] = histDir.make<TH2F>(hname.str().c_str(), "", 360, 0, 360.0, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_FirstTurnY[st]->SetYTitle("y_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_FirstTurnY[st]->SetXTitle("#theta_{ring} [deg]");
	hname.str(""); 
      }
    } 

    for ( int i = 0; i < Nringtrackers; i++ ) {

      string ring_tracker_name = ringtrackernames[i];

      if ( fill ) {
	hname << "RingTracker_" << ring_tracker_name << "_RhatY";
	h_RingTracker_RhatY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_RhatY[i]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_RhatY[i]->SetYTitle("y_{ring} [mm]");
	hname.str(""); 
    
	hname << "RingTracker_" << ring_tracker_name << "_YprimeY";
	h_RingTracker_YprimeY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	h_RingTracker_YprimeY[i]->SetXTitle("y_{ring} [mm]");
	h_RingTracker_YprimeY[i]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_XprimeX";
	h_RingTracker_XprimeX[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	h_RingTracker_XprimeX[i]->SetXTitle("x_{ring} [mm]");
	h_RingTracker_XprimeX[i]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_Mom";
	h_RingTracker_Mom[i] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	h_RingTracker_Mom[i]->SetXTitle("P_{#mu} / P_{m}");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_Rhat";
	h_RingTracker_Rhat[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Rhat[i]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_Xe";
	h_RingTracker_Xe[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Xe[i]->SetXTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_Vhat";
	h_RingTracker_Vhat[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Vhat[i]->SetXTitle("y_{ring} [mm]");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_Pol";
	h_RingTracker_Pol[i] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	h_RingTracker_Pol[i]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_PolX";
	h_RingTracker_PolX[i] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	h_RingTracker_PolX[i]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	hname.str("");

	hname << "RingTracker_" << ring_tracker_name << "_PolY";
	h_RingTracker_PolY[i] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	h_RingTracker_PolY[i]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
	hname.str("");
      }
    } 
  } // if ( SaveVRingHits_ )


  if ( SaveVRing1PlaneHits_ || SaveVRingHits_ ) {
    if ( fill ) {
      for ( int t = 0; t < Ntimestamps; t++ ) {
	string timename, xtitle;
	
	if ( SaveVRing1PlaneHits_ == true && SaveVRingHits_ == false ) {
	  if ( t == 0 || t == 1 ) { continue; }
	}

	xtitle = "Turn";
	timename = timestamps[t];
      
	int xpbin = XPrimebin_Ring;
	int xpmin = XPrimemin_Ring;
	int xpmax = XPrimemax_Ring;
	int ypbin = YPrimebin_Ring;
	int ypmin = YPrimemin_Ring;
	int ypmax = YPrimemax_Ring;

	int hitsperturn = 12;
	if ( t == 1 ) { hitsperturn = 1; }

	hname << "TrackerXe" << "_" << timename;
	h_RingTracker_Xe_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Xe_vs_Time[t]->SetYTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	h_RingTracker_Xe_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "TrackerRhat" << "_" << timename;
	h_RingTracker_Rhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Rhat_vs_Time[t]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_Rhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "TrackerMom" << "_" << timename;
	h_RingTracker_Mom_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Mombin, Mommin, Mommax);
	h_RingTracker_Mom_vs_Time[t]->SetYTitle("P_{#mu} / P_{m}");
	h_RingTracker_Mom_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "TrackerPrhat" << "_" << timename;
	h_RingTracker_Prhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, xpbin, xpmin, xpmax);
	h_RingTracker_Prhat_vs_Time[t]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	h_RingTracker_Prhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "TrackerPvhat" << "_" << timename;
	h_RingTracker_Pvhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, ypbin, ypmin, ypmax);
	h_RingTracker_Pvhat_vs_Time[t]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	h_RingTracker_Pvhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "TrackerY" << "_" << timename;
	h_RingTracker_Y_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Y_vs_Time[t]->SetYTitle("y_{ring} [mm]");
	h_RingTracker_Y_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "TrackerPol" << "_" << timename;
	h_RingTracker_Pol_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Polbin, Polmin, Polmax);
	h_RingTracker_Pol_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{z}");
	h_RingTracker_Pol_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "TrackerPolX" << "_" << timename;
	h_RingTracker_PolX_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Polbin, Polmin, Polmax);
	h_RingTracker_PolX_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{r}");
	h_RingTracker_PolX_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "TrackerPolY" << "_" << timename;
	h_RingTracker_PolY_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, PolYbin, PolYmin, PolYmax);
	h_RingTracker_PolY_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	h_RingTracker_PolY_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "TrackerNum" << "_" << timename;
	h_RingTracker_N_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	h_RingTracker_N_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "TrackerNgtEth" << "_" << timename;
	h_RingTracker_NgtEth_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	h_RingTracker_NgtEth_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "TrackerNwghtE" << "_" << timename;
	h_RingTracker_NwghtE_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	h_RingTracker_NwghtE_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      }
    }
  
  } // if ( SaveVRing1PlaneHits_ )  

  
  
  if ( SaveTruthHits_ ) {
    for ( int part = 0; part < Nparticles; part++ ) {
      string truth_part_name = truth_particle_names[part];

      for ( int t = 0; t < Ntruthtimes; t++ ) {
	string truth_time_name = times[t];
	
	int xpbin = XPrimebin_Ring;
	int xpmin = XPrimemin_Ring;
	int xpmax = XPrimemax_Ring;
	int ypbin = YPrimebin_Ring;
	int ypmin = YPrimemin_Ring;
	int ypmax = YPrimemax_Ring;
	if ( part >= 2 ) {
	  xpbin = XPrimebin_Decay_Ring;
	  xpmin = XPrimemin_Decay_Ring;
	  xpmax = XPrimemax_Decay_Ring;
	  ypbin = YPrimebin_Decay_Ring;
	  ypmin = YPrimemin_Decay_Ring;
	  ypmax = YPrimemax_Decay_Ring;
	}

	if ( fill ) {
	  hname << "G4Track_Time_dCosTheta_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_dCosTheta[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_G4Tracker_Time_dCosTheta[t][part]->SetXTitle("cos(#theta*)");
	  hname.str("");
	
	  hname << "G4Track_Time_dX_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_dX[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", 100, 0.0, 1.0);
	  h_G4Tracker_Time_dX[t][part]->SetXTitle("x #equiv E/E_{max}");
	  hname.str("");
	
	  hname << "G4Track_Time_Polarization_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Polarization[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_G4Tracker_Time_Polarization[t][part]->SetXTitle("#hat{s}_{#mu}#bullet#hat{p}_{el}");
	  hname.str("");
	
	  hname << "G4Track_Time_dXdCosTheta_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_dXdCosTheta[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", 20, 0.0, 1.0, 20, 0.0, 1.0);
	  h_G4Tracker_Time_dXdCosTheta[t][part]->SetXTitle("x #equiv E/E_{max}");
	  h_G4Tracker_Time_dXdCosTheta[t][part]->SetYTitle("cos(#theta)");
	  hname.str("");

      
      
	  hname << "G4Track_Time_XprimeX_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_XprimeX[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, xpbin, xpmin, xpmax);
	  h_G4Tracker_Time_XprimeX[t][part]->SetXTitle("x_{gen} [mm]");
	  h_G4Tracker_Time_XprimeX[t][part]->SetYTitle("x'_{gen} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
      
	  hname << "G4Track_Time_YprimeY_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_YprimeY[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, ypbin, ypmin, ypmax);
	  h_G4Tracker_Time_YprimeY[t][part]->SetXTitle("y_{gen} [mm]");
	  h_G4Tracker_Time_YprimeY[t][part]->SetYTitle("y'_{gen} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");

	  hname << "G4Track_Time_Rhat_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Rhat[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_G4Tracker_Time_Rhat[t][part]->SetXTitle("x_{gen} #equiv R - R_{m} [mm]");
	  hname.str("");

	  hname << "G4Track_Time_Xe_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Xe[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_G4Tracker_Time_Xe[t][part]->SetXTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	  hname.str("");

	  hname << "G4Track_Time_Vhat_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Vhat[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_G4Tracker_Time_Vhat[t][part]->SetXTitle("y_{gen} [mm]");
	  hname.str("");
      
	  hname << "G4Track_Time_Pol_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Pol[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_G4Tracker_Time_Pol[t][part]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	  hname.str("");
      
	  hname << "G4Track_Time_PolX_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_PolX[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_G4Tracker_Time_PolX[t][part]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	  hname.str("");
      
	  hname << "G4Track_Time_PolY_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_PolY[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	  h_G4Tracker_Time_PolY[t][part]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
	  hname.str("");

	  hname << "G4Track_Time_Xprime_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Xprime[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", xpbin, xpmin, xpmax);      
	  h_G4Tracker_Time_Xprime[t][part]->SetXTitle("x'_{gen} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
    
	  hname << "G4Track_Time_Yprime_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Yprime[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", ypbin, ypmin, ypmax);
	  h_G4Tracker_Time_Yprime[t][part]->SetXTitle("y'_{gen} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
    
	  hname << "G4Track_Time_Mom_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_Mom[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	  h_G4Tracker_Time_Mom[t][part]->SetXTitle("P_{#mu} / P_{m}");
	  hname.str("");
    
	  hname << "G4Track_Time_t0_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_t0[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", 300, -100, 200);
	  h_G4Tracker_Time_t0[t][part]->SetXTitle("t_{0} [ns]");
	  hname.str("");
    
	  hname << "G4Track_Time_RhatY_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_RhatY[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Rhatbin, Rhatmin, Rhatmax);
	  h_G4Tracker_Time_RhatY[t][part]->SetXTitle("x - x_{gen} [mm]");
	  h_G4Tracker_Time_RhatY[t][part]->SetYTitle("y - y_{gen} [mm]");
	  hname.str("");
    
	  hname << "G4Track_Time_XZ_" << truth_time_name << "_" << truth_part_name;
	  h_G4Tracker_Time_XZ[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring, Rmin_Ring, Rmax_Ring, Rbin_Ring, Rmin_Ring, Rmax_Ring);
	  h_G4Tracker_Time_XZ[t][part]->SetXTitle("z_{ring} [mm]");
	  h_G4Tracker_Time_XZ[t][part]->SetYTitle("x_{ring} [mm]");    
	  hname.str("");
	} // fill
      } // loop over times
    } // loop over 7 particles




    if ( fill ) {
      for ( int part = 0; part < Nparticles; part++ ) {
	string truth_part_name = truth_particle_names[part];

	for ( int t = 0; t < Ntimestamps; t++ ) {
	  string timename, xtitle;
	  xtitle = "Turn";
	  timename = timestamps[t];
	  
	  int rbin  = Rhatbin;
	  int rmin  = Rhatmin;
	  int rmax  = Rhatmax;
	  int xpbin = XPrimebin_Ring;
	  int xpmin = XPrimemin_Ring;
	  int xpmax = XPrimemax_Ring;
	  int ypbin = YPrimebin_Ring;
	  int ypmin = YPrimemin_Ring;
	  int ypmax = YPrimemax_Ring;
	  if ( part >= 2 ) {
	    rbin  = Rhatbin;
	    rmin  = 10*Rhatmin;
	    rmax  = 10*Rhatmax;
	    xpbin = XPrimebin_Decay_Ring;
	    xpmin = XPrimemin_Decay_Ring;
	    xpmax = XPrimemax_Decay_Ring;
	    ypbin = YPrimebin_Decay_Ring;
	    ypmin = YPrimemin_Decay_Ring;
	    ypmax = YPrimemax_Decay_Ring;
	  }
	  
	  int hitsperturn = 12;
	  if ( t == 1 ) { hitsperturn = 1; }
	  
	  hname << "G4TrackXe" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Xe_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Rhatbin, Rhatmin, Rhatmax);
	  h_G4Tracker_Xe_vs_Time[t][part]->SetYTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	  h_G4Tracker_Xe_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4TrackRhat" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Rhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, rbin, rmin, rmax);
	  h_G4Tracker_Rhat_vs_Time[t][part]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_G4Tracker_Rhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4TrackMom" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Mom_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Mombin, Mommin, Mommax);
	  h_G4Tracker_Mom_vs_Time[t][part]->SetYTitle("P_{#mu} / P_{m}");
	  h_G4Tracker_Mom_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackE" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_E_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Mombin/100, 0.0, Pmagic());
	  h_G4Tracker_E_vs_Time[t][part]->SetYTitle("Energy [GeV]");
	  h_G4Tracker_E_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackNum" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_N_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_N_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackNgtEth" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_NgtEth_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_NgtEth_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackNwghtE" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_NwghtE_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_NwghtE_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackPrhat" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Prhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, xpbin, xpmin, xpmax);
	  h_G4Tracker_Prhat_vs_Time[t][part]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  h_G4Tracker_Prhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackPvhat" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Pvhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, ypbin, ypmin, ypmax);
	  h_G4Tracker_Pvhat_vs_Time[t][part]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  h_G4Tracker_Pvhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackY" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Y_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, rbin, rmin, rmax);
	  h_G4Tracker_Y_vs_Time[t][part]->SetYTitle("y_{ring} [mm]");
	  h_G4Tracker_Y_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackPol" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Pol_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Polbin, Polmin, Polmax);
	  h_G4Tracker_Pol_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{z}");
	  h_G4Tracker_Pol_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4TrackPolX" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_PolX_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, Polbin, Polmin, Polmax);
	  h_G4Tracker_PolX_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{r}");
	  h_G4Tracker_PolX_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4TrackPolY" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_PolY_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_, PolYbin, PolYmin, PolYmax);
	  h_G4Tracker_PolY_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	  h_G4Tracker_PolY_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackNup" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Nup_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_Nup_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackNdown" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Ndown_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_Ndown_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackYprime" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Yprime_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_Yprime_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackVhat" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Vhat_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_Vhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4TrackNud" << "_" << truth_part_name << "_" << timename;
	  h_G4Tracker_Nud_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*hitsperturn, 0.0, maxturns_);
	  h_G4Tracker_Nud_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	} // Loop over hits per turn
      } // Loop over particles
    } // fill
  } // if ( SaveTruthHits_ )



  
  if ( SaveInfHits_ ) {
    for ( int i = 0; i < Ninflectortrackers; i++ ) {
      stringstream trackername;
      if ( i < Ninflectortrackers ) { trackername << "" << i << ""; }
      else { trackername << "" << i-Ninflectortrackers << ""; }

      if ( fill ) {
	hname << "InflectorTracker_" << trackername.str() << "_XprimeX";
	h_InflectorTracker_XprimeX[i] = histDir.make<TH2F>(hname.str().c_str(), "", Xbin_Inflector, Xmin_Inflector, Xmax_Inflector, XPrimebin_Inflector, XPrimemin_Inflector, XPrimemax_Inflector);
	h_InflectorTracker_XprimeX[i]->SetXTitle("x_{inf} [mm]");
	h_InflectorTracker_XprimeX[i]->SetYTitle("x'_{inf} #equiv P_{x}/P_{z} [mrad]");
	hname.str("");

	hname << "InflectorTracker_" << trackername.str() << "_YprimeY";
	h_InflectorTracker_YprimeY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Ybin_Inflector, Ymin_Inflector, Ymax_Inflector, YPrimebin_Inflector, YPrimemin_Inflector, YPrimemax_Inflector);
	h_InflectorTracker_YprimeY[i]->SetXTitle("y_{inf} [mm]");
	h_InflectorTracker_YprimeY[i]->SetYTitle("y'_{inf} #equiv P_{y}/P_{z} [mrad]");
	hname.str("");

	hname << "InflectorTracker_" << trackername.str() << "_RhatY";
	h_InflectorTracker_RhatY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin_Inflector, Rhatmin_Inflector, Rhatmax_Inflector, Vbin_Inflector, Vmin_Inflector, Vmax_Inflector);
	h_InflectorTracker_RhatY[i]->SetXTitle("x_{inf} [mm]");
	h_InflectorTracker_RhatY[i]->SetYTitle("y_{inf} [mm]");
	hname.str("");
        
	hname << "InflectorTracker_" << trackername.str() << "_XZ";
	h_InflectorTracker_XZ[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring, Rmin_Ring, Rmax_Ring, Rbin_Ring, Rmin_Ring, Rmax_Ring);
	h_InflectorTracker_XZ[i]->SetXTitle("z_{ring} [mm]");
	h_InflectorTracker_XZ[i]->SetYTitle("x_{ring} [mm]");
	hname.str("");
      }
    }

    if ( fill ) {
        
      hname.str("");  
      hname << "InflectorTracker_RZ";
      h_InflectorTracker_RZ = histDir.make<TH2F>(hname.str().c_str(), "", 90, -4701, 1, Rhatbin_Inflector, Rhatmin_Inflector, Rhatmax_Inflector);
      h_InflectorTracker_RZ->SetXTitle("z_{inf} [mm]");
      h_InflectorTracker_RZ->SetYTitle("xy_{inf} [mm]");
      hname.str("");
    
      hname.str("");  
      hname << "InflectorTracker_PrhatPvhatEntrance";
      h_InflectorTracker_PrhatPvhatEntrance = histDir.make<TH2F>(hname.str().c_str(), "", YPrimebin_Ring/10, YPrimemin_Ring/100, YPrimemax_Ring/100, YPrimebin_Ring/10, YPrimemin_Ring/100, YPrimemax_Ring/100);
      h_InflectorTracker_PrhatPvhatEntrance->SetYTitle("#hat{P}_{v}");
      h_InflectorTracker_PrhatPvhatEntrance->SetXTitle("#hat{P}_{r}");

      hname.str("");  
      hname << "InflectorTracker_PrhatPvhatExit";
      h_InflectorTracker_PrhatPvhatExit = histDir.make<TH2F>(hname.str().c_str(), "", YPrimebin_Ring/10, YPrimemin_Ring/100, YPrimemax_Ring/100, YPrimebin_Ring/10, YPrimemin_Ring/100, YPrimemax_Ring/100);
      h_InflectorTracker_PrhatPvhatExit->SetYTitle("#hat{P}_{v}");
      h_InflectorTracker_PrhatPvhatExit->SetXTitle("#hat{P}_{r}");

      hname.str("");
      hname << "InflectorTracker_X";
      h_InflectorTracker_X = histDir.make<TH2F>(hname.str().c_str(), "", Ninflectortrackers+1, -0.5, Ninflectortrackers+0.5, Xbin_Inflector, Xmin_Inflector, Xmax_Inflector);
      hname.str("");
      hname << "InflectorTracker_Y";
      h_InflectorTracker_Y = histDir.make<TH2F>(hname.str().c_str(), "", Ninflectortrackers+1, -0.5, Ninflectortrackers+0.5, Ybin_Inflector, Ymin_Inflector, Ymax_Inflector);
      hname.str("");
      hname << "InflectorTracker_Z";
      h_InflectorTracker_Z = histDir.make<TH2F>(hname.str().c_str(), "", Ninflectortrackers+1, -0.5, Ninflectortrackers+0.5, Zbin_Inflector, Zmin_Inflector, Zmax_Inflector);
      hname.str("");
    }
  }



  if ( SaveRingHits_ ) {
    for ( int st = 0; st < 2; st++ ) {
      string stname = "";
      if ( st == 1 ) { stname = "_Stored"; }

      for ( int i = 0; i < Nsystemtrackers; i++ ) {
	if ( fill ) {
	  hname << rhitnames[i] << "Hits" << stname << "_XZ";
	  //cout << hname.str() << endl;
	  h_SystemHitTracker_XZ[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring/2, Rmin_Ring, Rmax_Ring, Rbin_Ring/2, Rmin_Ring, Rmax_Ring);
	  h_SystemHitTracker_XZ[i][st]->SetYTitle("x_{ring} [mm]");
	  h_SystemHitTracker_XZ[i][st]->SetXTitle("z_{ring} [mm]");
	  hname.str("");
	
	  hname << rhitnames[i] << "Hits" << stname << "_RhatY";
	  h_SystemHitTracker_RhatY[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin_Ring/2, Rhatmin_Ring, Rhatmax_Ring, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	  h_SystemHitTracker_RhatY[i][st]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_SystemHitTracker_RhatY[i][st]->SetYTitle("y_{ring} [mm]");
	  hname.str("");
	
	  hname << rhitnames[i] << "Hits" << stname << "_RhatTime";
	  h_SystemHitTracker_RhatTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Rhatbin_Ring/2, Rhatmin_Ring, Rhatmax_Ring);
	  h_SystemHitTracker_RhatTime[i][st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_SystemHitTracker_RhatTime[i][st]->SetXTitle("t_{ring} [#mus]");
	  hname.str("");
	
	  hname << rhitnames[i] << "Hits" << stname << "_YTime";
	  h_SystemHitTracker_YTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	  h_SystemHitTracker_YTime[i][st]->SetYTitle("y_{ring} [mm]");
	  h_SystemHitTracker_YTime[i][st]->SetXTitle("t_{ring} [#mus]");
	  hname.str("");
	
	  hname << rhitnames[i] << "Hits" << stname << "_DeltaPy";
	  h_SystemHitTracker_DeltaPy[i][st] = histDir.make<TH1F>(hname.str().c_str(), "", 3*YPrimebin_Ring, 3*YPrimemin_Ring, 3*YPrimemax_Ring);
	  h_SystemHitTracker_DeltaPy[i][st]->SetXTitle("#Delta#hat{p}_{y} [MeV]");
	  hname.str("");
	
	  hname << rhitnames[i] << "Hits" << stname << "_DeltaPx";
	  h_SystemHitTracker_DeltaPx[i][st] = histDir.make<TH1F>(hname.str().c_str(), "", 3*XPrimebin_Ring, 3*XPrimemin_Ring, 3*XPrimemax_Ring);
	  h_SystemHitTracker_DeltaPx[i][st]->SetXTitle("#Delta#hat{p}_{x} [MeV]");
	  hname.str("");
      
	  hname << rhitnames[i] << stname << "_Nhits";
	  h_SystemHitTracker_Nhits[i][st] = histDir.make<TH1F>(hname.str().c_str(), "", 10, -0.5, 9.5);
	  h_SystemHitTracker_Nhits[i][st]->SetXTitle("Number of energy deposits");
	  hname.str("");
	}
      }
    }
  

    for ( int st = 0; st < 2; st++ ) {
      string stname = "";
      if ( st == 1 ) { stname = "_Stored"; }
    
      for ( int i = 0; i < Nringhits; i++ ) {
      
	if ( fill ) {
	  hname << inames[i] << "Hits" << stname << "_XZ";
	  cout << hname.str() << endl;
	  h_RingHitTracker_XZ[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring/2, Rmin_Ring, Rmax_Ring, Rbin_Ring/2, Rmin_Ring, Rmax_Ring);
	  h_RingHitTracker_XZ[i][st]->SetYTitle("x_{ring} [mm]");
	  h_RingHitTracker_XZ[i][st]->SetXTitle("z_{ring} [mm]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_RhatY";
	  h_RingHitTracker_RhatY[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin_Ring/2, Rhatmin_Ring, Rhatmax_Ring, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	  h_RingHitTracker_RhatY[i][st]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_RingHitTracker_RhatY[i][st]->SetYTitle("y_{ring} [mm]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_RhatTime";
	  h_RingHitTracker_RhatTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Rhatbin_Ring/2, Rhatmin_Ring, Rhatmax_Ring);
	  h_RingHitTracker_RhatTime[i][st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_RingHitTracker_RhatTime[i][st]->SetXTitle("t_{ring} [#mus]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_YTime";
	  h_RingHitTracker_YTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Ybin_Ring/2, Ymin_Ring, Ymax_Ring);
	  h_RingHitTracker_YTime[i][st]->SetYTitle("y_{ring} [mm]");
	  h_RingHitTracker_YTime[i][st]->SetXTitle("t_{ring} [#mus]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_DeltaPy";
	  h_RingHitTracker_DeltaPy[i][st] = histDir.make<TH1F>(hname.str().c_str(), "", 3*YPrimebin_Ring, 3*YPrimemin_Ring, 3*YPrimemax_Ring);
	  h_RingHitTracker_DeltaPy[i][st]->SetXTitle("#Delta#hat{p}_{y} [MeV]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_DeltaPx";
	  h_RingHitTracker_DeltaPx[i][st] = histDir.make<TH1F>(hname.str().c_str(), "", 3*XPrimebin_Ring, 3*XPrimemin_Ring, 3*XPrimemax_Ring);
	  h_RingHitTracker_DeltaPx[i][st]->SetXTitle("#Delta#hat{p}_{x} [MeV]");
	  hname.str("");
      
	  hname << inames[i] << stname << "_Nhits";
	  h_RingHitTracker_Nhits[i][st] = histDir.make<TH1F>(hname.str().c_str(), "", 10, -0.5, 9.5);
	  h_RingHitTracker_Nhits[i][st]->SetXTitle("Number of energy deposits");
	  hname.str("");
	}
      }
    }
  } // if we set SaveRingHits = true;



  if ( beamstart_ == "UpstreamCryo" || beamstart_ == "upstream_cryo" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart_.SetXYZ(6989 , 0 , -2394);
    rhat_offset = 275.7;
    yhat_offset = 0;
  }
  if ( beamstart_ == "UpstreamMandrel" || beamstart_ == "upstream" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart_.SetXYZ(6990 , 0 , -2388);
    rhat_offset = 247;
    yhat_offset = 0;
  }
  if ( beamstart_ == "DownstreamMandrel" || beamstart_ == "downstream" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart_.SetXYZ(7155 , 0 , -699.9);
    rhat_offset = 77;
    yhat_offset = 0;
  }
  if ( beamstart_ == "Perfect" || beamstart_ == "CentralOrbit" ) {
    xAxis.SetXYZ(1 , 0 , 0);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0 , 0 , 1);
    beamStart_.SetXYZ(7112 + StorageOffset_ , 0 , 0);
    rhat_offset = StorageOffset_;
    yhat_offset = 0;
  }
  if ( beamstart_ == "CentralOrbit_Offset77" ) {
    xAxis.SetXYZ(1 , 0 , 0);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0 , 0 , 1);
    beamStart_.SetXYZ(7112 + StorageOffset_ , 0 , 0);
    rhat_offset = StorageOffset_;
    yhat_offset = 0;
  }

  if ( beamstart_ == "UpstreamCryo" || beamstart_ == "upstream_cryo" ) {;
    //     if ( delta == -5 ) {
    //       xAxis.SetXYZ(0.9957 , 0 , -0.09238);
    //       zAxis.SetXYZ(0.09238 , 0 , 0.9957);
    //       beamStart.SetXYZ(6998 , 0 , -2394);
    //       rhat_offset = 283.8;
    //     }
    //     else if ( delta == -6 ) {
    //       xAxis.SetXYZ(0.9958 , 0 , -0.09138);
    //       zAxis.SetXYZ(0.09138 , 0 , 0.9958);
    //       beamStart.SetXYZ(7000 , 0 , -2394);
    //       rhat_offset = 285.4;
    //     }
    //     else if ( delta == -7 ) {
    //       xAxis.SetXYZ(0.9959 , 0 , -0.09039);
    //       zAxis.SetXYZ(0.09039 , 0 , 0.9959);
    //       beamStart.SetXYZ(7001 , 0 , -2394);
    //       rhat_offset = 287.1;
    //     }
    //     else { cout << "Delta not recognized!!!" << endl; }
  }

  InitializeBeamCoordinates();
  
}

void gm2ringsim::ringTrackerAnalyzer::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz)
{  
  *pol_vertical = poly;
  *pol_radial   = polx;
  *pol_azimuth  = polz; 
}
void gm2ringsim::ringTrackerAnalyzer::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta)
{
  // There is defined going counter clockwise around the ring starting at x = +1 and z = 0;
  RotateRingToBeamCoordinates(theta);
  TVector3 pol_ring(polx, poly, polz);
  *pol_vertical = pol_ring.Dot(yAxis_beam_);
  *pol_radial   = pol_ring.Dot(xAxis_beam_);
  *pol_azimuth  = pol_ring.Dot(zAxis_beam_);
}

void gm2ringsim::ringTrackerAnalyzer::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z)
{
  double theta_nom = TMath::ATan2(x, z);
  double theta = TMath::Pi()/2.0 - theta_nom;

  ComputePolarization(pol_azimuth, pol_radial, pol_vertical, polx, poly, polz, theta);
}

void gm2ringsim::ringTrackerAnalyzer::InitializeBeamCoordinates()
{
  xAxis_beam_.SetXYZ(1 , 0 , 0);
  yAxis_beam_.SetXYZ(0 , 1 , 0);
  zAxis_beam_.SetXYZ(0 , 0 , 1);
}


void gm2ringsim::ringTrackerAnalyzer::RotateRingToBeamCoordinates(double theta)
{
  // Ring Coordinates          Beam Coordinates 
  // (y out of page)           (z out of page)
  //    +x                         +y
  //-z      +z          ---->  -x      +x
  //    -x                         -y

  InitializeBeamCoordinates();
  xAxis_beam_.RotateY(-theta);
  zAxis_beam_.RotateY(-theta);
}


void gm2ringsim::ringTrackerAnalyzer::FillInflector(int hist, double x_inf, double y_inf, double z_inf, double r_inf, double prhat_ring, double pvhat_ring, double xprime_inf, double yprime_inf)
{
  if ( SaveInfHits_ == false ) { return; }
  h_InflectorTracker_RZ->Fill(z_inf, r_inf);		

  if ( hist == 0 ) { h_InflectorTracker_PrhatPvhatEntrance->Fill(prhat_ring, pvhat_ring); }
  if ( hist == 8 ) { h_InflectorTracker_PrhatPvhatExit->Fill(prhat_ring, pvhat_ring); }
	  
  if ( hist == 0 ) { Nstart_inflector++; }
  if ( hist == 8 ) { Nexit_inflector++; }
  
  h_InflectorTracker_X->Fill(hist, x_inf);
  h_InflectorTracker_Y->Fill(hist, y_inf);
  h_InflectorTracker_Z->Fill(hist, z_inf);
  
  h_InflectorTracker_RhatY[hist]->Fill(x_inf, y_inf);
  h_InflectorTracker_XprimeX[hist]->Fill(x_inf, 1000*xprime_inf);
  h_InflectorTracker_YprimeY[hist]->Fill(y_inf, 1000*yprime_inf);	
}


void gm2ringsim::ringTrackerAnalyzer::FillTimeRingTracker(int hist, double xe, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly)
{

  if ( debug_ ) { cout << "FillTimeRingTracker(" << hist << ")" << endl; }
  if ( SaveVRingHits_ == false ) { return; }

  if ( hist < Nringtrackers ) {
    h_RingTracker_Xe[hist]->Fill(xe);
    h_RingTracker_Rhat[hist]->Fill(rhat);
    h_RingTracker_Vhat[hist]->Fill(vhat);
    h_RingTracker_XprimeX[hist]->Fill(rhat, 1000*prhat);
    h_RingTracker_YprimeY[hist]->Fill(vhat, 1000*pvhat);
    h_RingTracker_Mom[hist]->Fill(mom/Pmagic());
    h_RingTracker_RhatY[hist]->Fill(rhat, vhat);
    h_RingTracker_Pol[hist]->Fill(polz);
    h_RingTracker_PolX[hist]->Fill(polx);
    h_RingTracker_PolY[hist]->Fill(poly);
  }  
  if ( debug_ ) { cout << "FillTimeRingTracker(" << hist << ") - Done" << endl; }
}
  
void gm2ringsim::ringTrackerAnalyzer::FillTruthTracker(int hist, int status, int pdgid, double xe_truth, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double t0, double polz, double polx, double poly, double Py, double Polarization)
{
  if ( debug_ ) { cout << "FillTruthTracker(" << hist << ")" << endl; }
  if ( SaveTruthHits_ == false ) { return; }
  
  int particle = -1;
  if ( pdgid == 13 || pdgid == -13 ) {
    if ( status == gm2ringsim::kBirth ) { particle = 0; }
    if ( status == gm2ringsim::kDecay ) { particle = 1; }
    if ( status == gm2ringsim::kLost )  { particle = 2; }
  }
  if ( pdgid == 11 || pdgid == -11 ) {
    if ( status == gm2ringsim::kBirth ) { particle = 4; }
    if ( status == gm2ringsim::kDecay ) { particle = 3; }
  }

  if ( particle == -1 ) { return; }

  FillTruthTracker(hist, particle, xe_truth, r_truth, y_truth, rprime_truth, yprime_truth, p, x, z, t0, polz, polx, poly, Py, Polarization);

  if ( p > 1.8*GeV && ( particle == 3 || particle == 4 ) ) {
    FillTruthTracker(hist, particle+2, xe_truth, r_truth, y_truth, rprime_truth, yprime_truth, p, x, z, t0, polz, polx, poly, Py, Polarization);
  }
}

void gm2ringsim::ringTrackerAnalyzer::FillTruthTracker(int hist, int particle, double xe_truth, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double t0, double polz, double polx, double Py, double poly, double Polarization)
{
  double yp = 1000*yprime_truth;
  if ( particle >= 3 ) {
    yp = Py;
  }
  yp = 1000*yprime_truth;

  if ( hist < Ntruthtimes ) {
    h_G4Tracker_Time_Xe[hist][particle]->Fill(xe_truth);
    h_G4Tracker_Time_Rhat[hist][particle]->Fill(r_truth);
    h_G4Tracker_Time_Vhat[hist][particle]->Fill(y_truth);
    h_G4Tracker_Time_Xprime[hist][particle]->Fill(1000*rprime_truth);
    h_G4Tracker_Time_Yprime[hist][particle]->Fill(yp);
    h_G4Tracker_Time_XprimeX[hist][particle]->Fill(r_truth, 1000*rprime_truth);
    h_G4Tracker_Time_YprimeY[hist][particle]->Fill(y_truth, yp);
    h_G4Tracker_Time_Mom[hist][particle]->Fill(p/Pmagic());
    h_G4Tracker_Time_RhatY[hist][particle]->Fill(r_truth, y_truth);
    h_G4Tracker_Time_XZ[hist][particle]->Fill(x, z);
    h_G4Tracker_Time_t0[hist][particle]->Fill(t0);
    h_G4Tracker_Time_Pol[hist][particle]->Fill(polz);
    h_G4Tracker_Time_PolX[hist][particle]->Fill(polx);
    h_G4Tracker_Time_PolY[hist][particle]->Fill(poly);
    h_G4Tracker_Time_Polarization[hist][particle]->Fill(Polarization);
  }
}

void gm2ringsim::ringTrackerAnalyzer::FillRingTracker(int hist, int type, double xe, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly)
{
  if ( debug_ ) { cout << "FillRingTracker(" << hist << ", " << type << ")" << endl; }
  if ( SaveVRingHits_ == false ) { return; }
  if ( hist < Nringtrackertimes ) {
    h_RingTracker_Time_Xe[hist][type]->Fill(xe);
    h_RingTracker_Time_Rhat[hist][type]->Fill(rhat);
    h_RingTracker_Time_Vhat[hist][type]->Fill(vhat);
    h_RingTracker_Time_XprimeX[hist][type]->Fill(rhat, 1000*prhat);
    h_RingTracker_Time_YprimeY[hist][type]->Fill(vhat, 1000*pvhat);
    h_RingTracker_Time_Xprime[hist][type]->Fill(1000*prhat);
    h_RingTracker_Time_Yprime[hist][type]->Fill(1000*pvhat);
    h_RingTracker_Time_Mom[hist][type]->Fill(mom/Pmagic());
    h_RingTracker_Time_RhatY[hist][type]->Fill(rhat, vhat);
    h_RingTracker_Time_XprimeYprime[hist][type]->Fill(1000*prhat, 1000*pvhat);
    h_RingTracker_Time_Pol[hist][type]->Fill(polz);
    h_RingTracker_Time_PolX[hist][type]->Fill(polx);
    h_RingTracker_Time_PolY[hist][type]->Fill(poly);
  }  
  if ( debug_ ) { cout << "FillRingTracker(" << hist << ", " << type << ") - Done" << endl; }
}
 
void gm2ringsim::ringTrackerAnalyzer::FillTruthTurnTime(double dT, int counter, int status, int pdgid, double time, double xe, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e)
{  
  bool debug = debug_;
  if ( debug ) { cout << "FillTruthTurnTime(" << pdgid << " , " << status << ")" << endl; }

  int particle = -1;
  if ( pdgid == 13 || pdgid == -13 ) {
    if ( status == gm2ringsim::kBirth ) { particle = 0; }
    if ( status == gm2ringsim::kDecay ) { particle = 1; }
    if ( status == gm2ringsim::kLost )  { particle = 2; }
  }
  if ( pdgid == 11 || pdgid == -11 ) {
    if ( status == gm2ringsim::kBirth ) { particle = 4; }
    if ( status == gm2ringsim::kDecay ) { particle = 3; }
  }

  if ( particle == -1 ) { return; }

  double xval;
  for ( int hist = 0; hist < 2; hist++ ) {
    xval = dT;
    if ( hist == 2 ) { xval = time;}
    if ( hist == 1 && counter != 0 ) { continue; }


    FillTruthTurnTime(xval, hist, particle, xe, rhat, y, prhat, pvhat, mom, polz, polx, poly, e);
    if ( e > 1.8*GeV && ( particle == 3 || particle == 4 ) ) {
      FillTruthTurnTime(xval, hist, particle+2, xe, rhat, y, prhat, pvhat, mom, polz, polx, poly, e);
    }
  }
}

void gm2ringsim::ringTrackerAnalyzer::FillTruthTurnTime(double xval, int hist, int particle, double xe, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e)  
{
  bool debug = debug_;
  if ( debug ) { cout << "FillTruthTurnTime(" << hist << " , " << particle << ")" << endl; }

  h_G4Tracker_Xe_vs_Time[hist][particle]->Fill(xval, xe);
  h_G4Tracker_Rhat_vs_Time[hist][particle]->Fill(xval, rhat);
  h_G4Tracker_Y_vs_Time[hist][particle]->Fill(xval, y);
  h_G4Tracker_Pol_vs_Time[hist][particle]->Fill(xval, polz);
  h_G4Tracker_PolX_vs_Time[hist][particle]->Fill(xval, polx);
  h_G4Tracker_PolY_vs_Time[hist][particle]->Fill(xval, poly);
  h_G4Tracker_Prhat_vs_Time[hist][particle]->Fill(xval, 1000*prhat);
  h_G4Tracker_Pvhat_vs_Time[hist][particle]->Fill(xval, 1000*pvhat);
  h_G4Tracker_Mom_vs_Time[hist][particle]->Fill(xval, mom/Pmagic());
  h_G4Tracker_E_vs_Time[hist][particle]->Fill(xval, mom);

  h_G4Tracker_Yprime_vs_Time[hist][particle]->Fill(xval, 1000*pvhat);
  h_G4Tracker_Vhat_vs_Time[hist][particle]->Fill(xval, y);

  
  if ( y > 0 ) { 
    h_G4Tracker_Nup_vs_Time[hist][particle]->Fill(xval, 1);
    h_G4Tracker_Nud_vs_Time[hist][particle]->Fill(xval, 1);
  }
  else if ( y < 0 ) { 
    h_G4Tracker_Ndown_vs_Time[hist][particle]->Fill(xval, -1);
    h_G4Tracker_Nud_vs_Time[hist][particle]->Fill(xval, -1);
  }
  
  if ( e > 1.8*GeV ) {
    h_G4Tracker_NgtEth_vs_Time[hist][particle]->Fill(xval);
  }
  h_G4Tracker_NwghtE_vs_Time[hist][particle]->Fill(xval, e/Pmagic());
  h_G4Tracker_N_vs_Time[hist][particle]->Fill(xval);
}

void gm2ringsim::ringTrackerAnalyzer::FillTurnTime(double dT, int counter, int trackID, double time, double xe, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e)
{
  bool debug = debug_;

  if ( debug ) { cout << "FillTurnTime(" << trackID << " , " << counter << ")" << endl; }

  double xval;

  if ( trackID != 1 ) { return; }
  
  for ( int hist = 0; hist < Ntimestamps; hist++ ) {
    if ( SaveVRing1PlaneHits_ == true && SaveVRingHits_ == false ) {
      if ( hist == 0 ) { continue; }
    }
    if ( SaveVRing1PlaneHits_ == false ) {
      if ( hist == 1 ) { continue; }
    }
    
    xval = dT;
    if ( hist == 2 ) { xval = time; }

    if ( hist == 1 && counter != 0 ) { continue; }

    
    h_RingTracker_Xe_vs_Time[hist]->Fill(xval, xe);
    h_RingTracker_Rhat_vs_Time[hist]->Fill(xval, rhat);
    h_RingTracker_Y_vs_Time[hist]->Fill(xval, y);
    h_RingTracker_Pol_vs_Time[hist]->Fill(xval, polz);
    h_RingTracker_PolX_vs_Time[hist]->Fill(xval, polx);
    h_RingTracker_PolY_vs_Time[hist]->Fill(xval, poly);
    h_RingTracker_Prhat_vs_Time[hist]->Fill(xval, 1000*prhat);
    h_RingTracker_Pvhat_vs_Time[hist]->Fill(xval, 1000*pvhat);
    h_RingTracker_Mom_vs_Time[hist]->Fill(xval, mom/Pmagic());

    if ( e > 1.8*GeV ) {
      h_RingTracker_NgtEth_vs_Time[hist]->Fill(xval);
    }
    h_RingTracker_NwghtE_vs_Time[hist]->Fill(xval, e/Pmagic());
    h_RingTracker_N_vs_Time[hist]->Fill(xval);
  }
  if ( debug ) { cout << "FillTurnTime(" << trackID << " , " << counter << ") - Done" << endl; }
}

void gm2ringsim::ringTrackerAnalyzer::FillKickDegree(int histtype, double degree_at_r0, double kick_at_r0, double mom)
{
  if ( SaveVRingHits_ == false ) { return; }

  int hist1 = -1;
  int hist2 = -1;
  if ( mom/Pmagic() <= 1 - 0.005 ) { hist1 = 0; }
  else if ( mom/Pmagic() >= 1 + 0.005 ) { hist1 = 2; }
  else { hist1 = 1; }

  if ( mom/Pmagic() <= 1 - 0.0015 ) { hist1 = 3; }
  else if ( mom/Pmagic() >= 1 + 0.0015 ) { hist1 = 5; }
  else { hist1 = 4; }  
  
  h_RingTracker_KickAtRhat0[histtype]->Fill(1000*kick_at_r0);
  h_RingTracker_KickAtRhat0_Mom[histtype][hist1]->Fill(1000*kick_at_r0);
  h_RingTracker_KickAtRhat0_Mom[histtype][hist2]->Fill(1000*kick_at_r0);
  
  h_RingTracker_DegreeAtRhat0[histtype]->Fill(degree_at_r0);
  h_RingTracker_DegreeAtRhat0_Mom[histtype][hist1]->Fill(degree_at_r0);
  h_RingTracker_DegreeAtRhat0_Mom[histtype][hist2]->Fill(degree_at_r0);
  
  h_RingTracker_DegreeAtRhat0KickAtRhat0[histtype]->Fill(degree_at_r0, 1000*kick_at_r0);
  h_RingTracker_DegreeAtRhat0KickAtRhat0_Mom[histtype][hist1]->Fill(degree_at_r0, 1000*kick_at_r0);
  h_RingTracker_DegreeAtRhat0KickAtRhat0_Mom[histtype][hist2]->Fill(degree_at_r0, 1000*kick_at_r0);
}
	  


double gm2ringsim::ringTrackerAnalyzer::ComputeQuadIntAngle(double r1, double r2, double d1, double d2)
{
  double quad_int_angle = -1.0;

  double slope_r_over_deg = (r1 - r2)/(d1 - d2);

  quad_int_angle = (50 - r1)/slope_r_over_deg + d1;

  return( quad_int_angle );
}

double gm2ringsim::ringTrackerAnalyzer::ComputeRmagicIntAngle(double r1, double r2, double d1, double d2)
{
  double rmagic_int_angle = -1.0;
  
  // Fit x @ theta0 to [ a - b*theta^{2} ]
  double a_r = ( r1 * d2*d2 ) - ( r2 * d1*d1 );
  double b_r = r2 - r1;
  a_r /= ( d2*d2 - d1*d1 );
  b_r /= ( d2*d2 - d1*d1 );
  rmagic_int_angle = TMath::Sqrt(a_r/b_r);

  return( rmagic_int_angle );
}

double gm2ringsim::ringTrackerAnalyzer::ComputeKickAtRmagicInt(double pr1, double pr2, double d1, double d2, double dr0)
{
  double kick_at_rmagic_int = -1.0;

  // Fit x' @ theta0 to [ a - b*theta^{2} ]
  double a = ( pr1 * d2 * d2) - (pr2 * d1 * d1);
  double b = pr1 - pr2;
  a /= ( d2*d2 - d1*d1 );
  b /= ( d2*d2 - d1*d1 );

  kick_at_rmagic_int = a - b*dr0*dr0;

  return( kick_at_rmagic_int );
}

void gm2ringsim::ringTrackerAnalyzer::NewFixTurnCounter(int *global_turn, int mytrack_vol, int ptrack_vol)
{
  if ( mytrack_vol <= ptrack_vol ) { (*global_turn)++; }  
}

void gm2ringsim::ringTrackerAnalyzer::FixTurnCounter(int *myturn, int *mytrack, double time, double t1)
{
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
    (*mytrack) += nskips;
    //int mytrk_tmp = mytrack + nskips;
    
    if ( (*mytrack) > 12 ) {
      ;//cout << " =======> Mytrack is now greater than 12!!!" << endl;
      (*myturn)++; (*mytrack) -= 12;
    }
    // 	      myturn++; mytrack = mytrk_tmp - 12;
    // 	    }
    ///cout << "\t\tMytrack is now : " << mytrack << endl;
  }
  else {
    (*mytrack)++;
  }
}

void gm2ringsim::ringTrackerAnalyzer::SetInitial(double *t0, int *mytrack, double *t1, double *theta1, double *prhat_init, double *pvhat_init, double *xe_init, double *rhat_init, double *vhat_init, double *pmom_init, double *pol_init, double time, double theta, double prhat, double pvhat, double xe, double rhat, double vhat, double mom, double pol)
{
  *t0 = time; 
  (*mytrack)++; 
  *t1 = time;
  *theta1 = theta;
  *prhat_init = prhat;
  *pvhat_init = pvhat;
  *xe_init  = xe;
  *rhat_init  = rhat;
  *vhat_init  = vhat;
  *pmom_init  = mom;
  *pol_init = pol;
}

double gm2ringsim::ringTrackerAnalyzer::Pmagic()
{
  //  NSF: The easiest way to calculate....
  static double const pMagic_ = 105.6583715 / TMath::Sqrt( 11659208.9e-10 );
  //  cout << "pMagic = " << pMagic_/GeV << " GeV/c" << endl;
  return pMagic_;
}

int gm2ringsim::ringTrackerAnalyzer::IsSomething(string name)
{
  if ( gm2ringsim::ringTrackerAnalyzer::IsKicker(name) ) { return( kKickerHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsQuad(name) ) { return( kQuadHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsCollimator(name) ) { return( kCollimatorHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsCryostat(name) ) { return( kCryostatHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsInflector(name) ) { return( kInflectorHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsVacuumChamber(name) ) { return( kVacuumHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsVirtualRingStation(name) ) { return( kVirtualRingStationHit ); }

  return( -1 );
}



bool gm2ringsim::ringTrackerAnalyzer::IsSomething(string name, int comp)
{
  if ( gm2ringsim::ringTrackerAnalyzer::IsKicker(name) && comp == kKickerHit ) { return( true ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsCollimator(name) && comp == kCollimatorHit ) { return( true ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsQuad(name) && comp == kQuadHit ) { return( true ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsCryostat(name) && comp == kCryostatHit ) { return( true ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsInflector(name) && comp == kInflectorHit ) { return( true ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsVacuumChamber(name) && comp == kVacuumHit ) { return( true ); }
  return( false );
}


bool gm2ringsim::ringTrackerAnalyzer::IsVirtualRingStation(string name)
{
  if ( name.find("VirtualRingStationPV") != string::npos ) { return( true ); }
  return( false );
}

bool gm2ringsim::ringTrackerAnalyzer::IsVacuumChamber(string name)
{
  if ( name.find("VacuumChamber") != string::npos ) { return( true ); }
  return( false );
}

bool gm2ringsim::ringTrackerAnalyzer::IsKicker(string name)
{
  if ( name.find("Kicker") != string::npos ) { return( true ); }
  return( false );
  //VID = 6         Name = KickerOuterPlate[01]
  //VID = 7         Name = KickerInnerPlate[01]
  //VID = 10        Name = KickerInnerPlate[02]
  //VID = 27        Name = KickerOuterPlate[00]
  //VID = 43        Name = KickerInnerPlate[00]
  //VID = 53        Name = KickerOuterPlate[02]
  //VID = 63        Name = KickerInnerPlate[01]

  
  //   if ( num == 6 ) { return( true ); }
  //   if ( num == 7 ) { return( true ); }
  //   if ( num == 10 ) { return( true ); }
  //   if ( num == 27 ) { return( true ); }
  //   if ( num == 43 ) { return( true ); }
  //   if ( num == 53 ) { return( true ); }
  //   if ( num == 63 ) { return( true ); }
  
  //   return( false );
}

bool gm2ringsim::ringTrackerAnalyzer::IsQuad(string name)
{
  if ( name.find("Quad") != string::npos ) { return( true ); }
  return( false );
  //VID = 1         Name = QuadInnerPlate[1][0]
  //VID = 2         Name = QuadInnerSupport[1][0][2]
  //VID = 3         Name = QuadInnerSupport[1][1][0]
  //VID = 4         Name = QuadInnerSupport[1][1][1]
  //VID = 11        Name = QuadInnerSupport[1][0][0]
  //VID = 12        Name = QuadInnerSupport[1][0][1]
  //VID = 16        Name = QuadOuterPlate[2][1]
  //VID = 17        Name = QuadBottomPlate[2][1]
  //VID = 18        Name = QuadTopPlate[2][1]
  //VID = 19        Name = QuadOuterSupport[1][0][0]
  //VID = 20        Name = QuadOuterSupport[1][0][1]
  //VID = 21        Name = QuadOuterSupport[1][0][2]
  //VID = 22        Name = QuadOuterSupport[1][1][0]
  //VID = 23        Name = QuadOuterSupport[1][1][1]
  //VID = 25        Name = QuadInnerPlate[2][0]
  //VID = 26        Name = QuadInnerSupport[2][0][0]
  //VID = 28        Name = QuadOuterPlate[1][0]
  //VID = 29        Name = QuadBottomPlate[1][0]
  //VID = 30        Name = QuadTopPlate[1][0]
  //VID = 32        Name = QuadOuterPlate[1][1]
  //VID = 33        Name = QuadInnerSupport[2][0][1]
  //VID = 34        Name = QuadInnerSupport[2][0][2]
  //VID = 39        Name = QuadTopPlate[2][0]
  //VID = 40        Name = QuadBottomPlate[2][0]
  //VID = 41        Name = QuadOuterPlate[2][0]
  //VID = 42        Name = QuadInnerPlate[2][1]
  //VID = 44        Name = QuadBottomPlate[1][1]
  //VID = 45        Name = QuadTopPlate[1][1]
  //VID = 46        Name = QuadInnerPlate[1][1]
  //VID = 47        Name = QuadInnerSupport[1][1][4]
  //VID = 48        Name = QuadInnerSupport[1][1][5]
  //VID = 49        Name = QuadInnerPlate[3][1]
  //VID = 56        Name = QuadOuterPlate[3][0]
  //VID = 57        Name = QuadInnerSupport[2][1][1]
  //VID = 58        Name = QuadOuterSupport[1][1][2]
  //VID = 59        Name = QuadOuterSupport[1][1][3]
  //VID = 60        Name = QuadOuterSupport[1][1][4]
  //VID = 61        Name = QuadOuterSupport[1][1][5]
  //VID = 64        Name = QuadTopPlate[0][0]
  //VID = 67        Name = QuadInnerPlate[3][0]
  //VID = 68        Name = QuadInnerSupport[3][0][0]
  //VID = 69        Name = QuadInnerSupport[3][0][1]
  //VID = 70        Name = QuadInnerSupport[1][1][3]
  //VID = 72        Name = QuadBottomPlate[0][0]
  //VID = 74        Name = QuadInnerPlate[0][0]

  //   if ( num == 1  ) { return( true ); }
  //   if ( num == 2  ) { return( true ); }
  //   if ( num == 3  ) { return( true ); }
  //   if ( num == 4  ) { return( true ); }
  //   if ( num == 11 ) { return( true ); }
  //   if ( num == 12 ) { return( true ); }
  //   if ( num == 16 ) { return( true ); }
  //   if ( num == 17 ) { return( true ); }
  //   if ( num == 18 ) { return( true ); }
  //   if ( num == 19 ) { return( true ); }
  //   if ( num == 20 ) { return( true ); }
  //   if ( num == 21 ) { return( true ); }
  //   if ( num == 22 ) { return( true ); }
  //   if ( num == 23 ) { return( true ); }
  //   if ( num == 25 ) { return( true ); }
  //   if ( num == 26 ) { return( true ); }
  //   if ( num == 28 ) { return( true ); }
  //   if ( num == 29 ) { return( true ); }
  //   if ( num == 30 ) { return( true ); }
  //   if ( num == 32 ) { return( true ); }
  //   if ( num == 33 ) { return( true ); }
  //   if ( num == 34 ) { return( true ); }
  //   if ( num == 39 ) { return( true ); }
  //   if ( num == 40 ) { return( true ); }
  //   if ( num == 41 ) { return( true ); }
  //   if ( num == 42 ) { return( true ); }
  //   if ( num == 44 ) { return( true ); }
  //   if ( num == 45 ) { return( true ); }
  //   if ( num == 46 ) { return( true ); }
  //   if ( num == 47 ) { return( true ); }
  //   if ( num == 48 ) { return( true ); }
  //   if ( num == 49 ) { return( true ); }
  //   if ( num == 56 ) { return( true ); }
  //   if ( num == 57 ) { return( true ); }
  //   if ( num == 58 ) { return( true ); }
  //   if ( num == 59 ) { return( true ); }
  //   if ( num == 60 ) { return( true ); }
  //   if ( num == 61 ) { return( true ); }
  //   if ( num == 64 ) { return( true ); }
  //   if ( num == 67 ) { return( true ); }
  //   if ( num == 68 ) { return( true ); }
  //   if ( num == 69 ) { return( true ); }
  //   if ( num == 70 ) { return( true ); }
  //   if ( num == 72 ) { return( true ); }
  //   if ( num == 74 ) { return( true ); }
  
  //   return( false );
}

bool gm2ringsim::ringTrackerAnalyzer::IsCollimator(string name)
{
  if ( name.find("Collimator") != string::npos ) { return( true ); }

  return( false );
}


bool gm2ringsim::ringTrackerAnalyzer::IsCryostat(string name)
{
  if ( name.find("Cryostat") != string::npos ) { return( true ); }
  if ( name.find("DownstreamWindow") != string::npos ) { return( true ); }
  if ( name.find("UpstreamWindow") != string::npos ) { return( true ); }
  return( false );
}


bool gm2ringsim::ringTrackerAnalyzer::IsVirtualRingStation(string name, int *num)
{
  *num = -1;
  for(int arc=0; arc!=12; ++arc){
    std::string ttLVName = artg4::addNumberToName("VirtualRingStationPV", arc);
    //G4cout << ttLVName << G4endl;
    if ( name == ttLVName ) { *num = arc; return( true ); }
  }

  return ( false );
}

bool gm2ringsim::ringTrackerAnalyzer::IsInflector(string name)
{
  if ( name.find("Inflector") != string::npos ) { return( true ); }
  if ( name.find("DownstreamEquivalent") != string::npos ) { return( true ); }
  if ( name.find("UpstreamEquivalent") != string::npos ) { return( true ); }
  if ( name.find("DownstreamEndFlange") != string::npos ) { return( true ); }
  if ( name.find("UpstreamEndFlange") != string::npos ) { return( true ); }
  if ( name.find("BeamChannel") != string::npos ) { return( true ); }
  return( false );
}

gm2ringsim::ringTrackerAnalyzer::~ringTrackerAnalyzer() {
  // Clean up dynamic memory and other resources here.

  
}

void gm2ringsim::ringTrackerAnalyzer::analyze(art::Event const &e)
{

  // Let's get the physical volume store. It's either in the run or the service
  auto const & pvs = artg4::dataFromRunOrService<artg4::PhysicalVolumeStoreData, artg4::PhysicalVolumeStoreService>(e.getRun(), pvsProducerLabel_, pvsInstanceLabel_);
  
  // Print it out
  //   for ( unsigned int i = 0; i < pvs.size(); ++i ) {
  //     mf::LogInfo("ParticleTrackAnalyzer") << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i);
  //     cout << "\t" << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i) << endl;
  //   }
  
  //return;
  

  Ngen++;
  
  //if ( Ngen > 2000 ) { return; }
  
  // Extract the hits
  
  // Make the handle
  //art::Handle< StrawArtRecordCollection > hitDataHandle;
  art::Handle< VirtualRingStationArtRecordCollection > ringtrackerhitDataHandle;
  art::Handle< TrackingActionArtRecordCollection > truthhitDataHandle;
  art::Handle< TrackingActionArtRecordCollection > truthfinishedhitDataHandle;
  art::Handle< InflectorArtRecordCollection > inflectorhitDataHandle;
  art::Handle< RingArtRecordCollection > ringhitDataHandle;
  
  // Fill the handle (note the use of the member data)
  //std::cout<<"Event ID: "<<e.id()<<std::endl;
  
  //e.getByLabel(hitModuleLabel_, instanceName_, hitDataHandle);
  e.getByLabel(ringtrackerhitModuleLabel_, ringtrackerinstanceName_, ringtrackerhitDataHandle);
  e.getByLabel(truthhitModuleLabel_, truthinstanceName_, truthhitDataHandle);
  e.getByLabel(truthfinishedhitModuleLabel_, truthfinishedinstanceName_, truthfinishedhitDataHandle);
  e.getByLabel(inflectorhitModuleLabel_, inflectorinstanceName_, inflectorhitDataHandle);
  e.getByLabel(ringhitModuleLabel_, ringinstanceName_, ringhitDataHandle);
  
  // Resolve the handle
  //StrawArtRecordCollection const & hits = *hitDataHandle;
  VirtualRingStationArtRecordCollection const & ringtrackerhits = *ringtrackerhitDataHandle;
  TrackingActionArtRecordCollection const & truthhits = *truthhitDataHandle;
  TrackingActionArtRecordCollection const & truthfinishedhits = *truthfinishedhitDataHandle;
  InflectorArtRecordCollection const & inflectorhits = *inflectorhitDataHandle;
  RingArtRecordCollection const & ringhits = *ringhitDataHandle;
  // Let's use the nice C++11 vector iteration
  //int i = 0;


  if ( debug_ ) {
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << pvs.size() << " entries in the PVS";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << ringhits.size() << " ring hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << inflectorhits.size() << " inflector hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << truthhits.size() << " truth hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << truthfinishedhits.size() << " truth hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << ringtrackerhits.size() << " virtual ring station hits in this event.";
  }

  //return;

     
  //-----------------
  // Pass Information
  //-----------------
//   bool pass_5us = false;
//   bool pass_20us = false;
//   bool pass_50us = false;
  bool pass_final = false;
//   bool pass_5turns = false;
//   bool pass_10turns = false;
//   bool pass_50turns = false;
//   bool pass_100turns = false;
  



      

  //----------------------
  // Inflector Information
  //----------------------
  if ( SaveInfHits_ ) {
    for ( auto hinfdata : inflectorhits ) {    
      double x_ring_inf = hinfdata.x_ring;
      double y_ring_inf = hinfdata.y_ring;
      double z_ring_inf = hinfdata.z_ring;
    
      double px_ring_inf = hinfdata.px_ring;
      double py_ring_inf = hinfdata.py_ring;
      double pz_ring_inf = hinfdata.pz_ring;

      double prhat_ring = hinfdata.prhat_ring;
      double pvhat_ring = hinfdata.pvhat_ring;
    
      int inflector_track_ID = hinfdata.trackID;
    
      TVector3 mom_ring_inf(px_ring_inf, py_ring_inf, pz_ring_inf);
      TVector3 pos_ring_inf(x_ring_inf, y_ring_inf, z_ring_inf);
      //double x_acc = x_ring_inf - beamStart.X();
      //double y_acc = y_ring_inf - beamStart.Y();
      //double z_acc = z_ring_inf - beamStart.Z();
    
      pos_ring_inf -= beamStart_;
    
      double x_inf = pos_ring_inf.Dot(xAxis);
      double y_inf = pos_ring_inf.Dot(yAxis);
      double r_inf = TMath::Sqrt(x_inf*x_inf + y_inf*y_inf);
      double z_inf = pos_ring_inf.Dot(zAxis);
    
      //cout << inum << "\t" << x_acc << "\t" << y_acc << "\t" << z_acc << endl;
    
      double xprime_inf = mom_ring_inf.Dot(xAxis)/mom_ring_inf.Mag();
      double yprime_inf = mom_ring_inf.Dot(yAxis)/mom_ring_inf.Mag();	  
      //double zprime_acc = mom.Dot(zAxis)/mom.Mag();
    
      //int inflector_volume_UID = hinfdata.volumeUID;
      string inflector_volume_name = ""; //uom->lookup(inflector_volume_UID);
    
      if ( inflector_track_ID == 1 ) { 
	for ( int vol = 0; vol < 9; vol++ ) {
	  stringstream volname;
	  if ( vol < 9 ) { volname << "InflectorTracker[0" << vol << "]"; }
	  else { volname << "InflectorTracker[0" << vol-9 << "]"; }
	
	  if ( inflector_volume_name == volname.str() ) {
	    if ( fill ) {
	      FillInflector(vol, x_inf, y_inf, z_inf, r_inf, prhat_ring, pvhat_ring, xprime_inf, yprime_inf);
	    }
	  }
	}
      } // loop over trackings
    } // loop over inflector hits
  } // if ( SaveInfHits_ )


  if ( debug_ ) { cout << "Made it past the inflector." << endl; }


  
  
  //-------------------------
  // Ring Tracker Information
  //-------------------------
  double t0 = 0.0;
  double t1 = 0.0;
  double theta1 = -1.0;
  int myturn = 0;
  int mytrack = 0;
  int mytrack_vol = 0;
  int pmytrack_vol = 0;
  int global_turn = 0;
  double degree_at_r0 = -1.0;
  
  int station = 0;
  double mom_init = -1.0;
  
  double prhat_init = -9999.9;
  double pvhat_init = -9999.9;
  double rhat_init = -9999.9;
  double vhat_init = -9999.9;
  double pmom_init = -9999.9;
  double pol_init = -9999.9;
  double xe_init = -9999.9;
  
  double previous_rhat = 9999.9;
  double previous_prhat = 999.9;
  double previous_degree = 999.9;
  
  double degree_at_quad = -9999.9;
  double degree_r_at_r0 = -9999.9;
  double kick_at_r0 = -9999.9;
  if ( SaveVRingHits_ || SaveVRing1PlaneHits_ ) {
    for ( auto hdata : ringtrackerhits) {
      int track_ID   = hdata.trackID;
      int track_volumeUID = hdata.volumeUID; 
      string track_volname = pvs.stringGivenID(track_volumeUID);
      IsVirtualRingStation(track_volname, &mytrack_vol);

      NewFixTurnCounter(&global_turn, mytrack_vol, pmytrack_vol);
      pmytrack_vol = mytrack_vol;
      if ( SaveVRing1PlaneHits_ == true && SaveVRingHits_ == false ) {
	if ( mytrack_vol != 0 ) { continue; }
      }

      double time = hdata.time;
      double mom = hdata.p;
      double momentum = mom;
      double rhat = hdata.rhat;
      double theta = hdata.theta;
      double vhat = hdata.vhat;
      double prhat = hdata.prhat;
      double pvhat = hdata.pvhat;
      double y = vhat;
      //double e = hdata.e;
      double e = mom;
      double polx = hdata.polx;
      double poly = hdata.poly;
      double polz = hdata.polz;
      //int pdgid = hdata.pdgid;
      double xe = ComputeXe(mom, 0.137);


      double pol_azimuth, pol_radial, pol_vertical;
      ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, polx, poly, polz, theta);

      if ( track_ID == 1 ) {      
	if ( t0 <= 0.0 ) { 
	  SetInitial(&t0, &mytrack, &t1, &theta1, &prhat_init, &pvhat_init, &xe_init, &rhat_init, &vhat_init, &pmom_init, &pol_init, 
		     time, theta, prhat, pvhat, xe, rhat, vhat, mom, pol_azimuth);
	}
      }
    
      if ( mytrack >= 12 ) { myturn++; mytrack = 0; }      

      t1 = time;
      theta1 = theta;

    
      if ( debug_ ) { cout << "Made it past the track/turn definitions for turn/track: " << global_turn << " , " << pmytrack_vol << "." << endl; }
    

    
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
      if ( SaveVRingHits_ ) {
	double degree = 360 * theta / TMath::TwoPi();
	if ( track_ID == 1 ) {
	  if ( global_turn == 0 && degree_at_quad < 0 ) {	
	    //cout << degree << "\t" << rhat << endl;
	    if ( rhat < 50 && previous_rhat >= 50 ) {
	      degree_at_quad = ComputeQuadIntAngle(rhat, previous_rhat, degree, previous_degree);
	      if ( fill ) { h_RingTracker_DegreeAtQuad[0]->Fill(degree_at_quad); }	  
	    }
	  }
    
	  if ( global_turn == 0 && degree_at_r0 < 0 ) {
	    if ( fill ) {
	      h_RingTracker_FirstTurnX[0]->Fill(degree, rhat);
	      h_RingTracker_FirstTurnY[0]->Fill(degree, vhat);
	    }
	    if ( rhat < 0 && previous_rhat >= 0 ) {
	      degree_r_at_r0 = ComputeRmagicIntAngle(rhat, previous_rhat, degree, previous_degree);
	      Ndegat0 += degree_r_at_r0;
	
	      if ( degree_r_at_r0 > 75.2 && degree_r_at_r0 < 89.3 ) { Ninside_midkicker++; }
	      else if ( degree_r_at_r0 > 60.3 && degree_r_at_r0 < 104.3 ) { Ninside_allkicker++; }
	      else { Noutside_kickers++; }
	
	      kick_at_r0 = ComputeKickAtRmagicInt(prhat, previous_prhat, degree, previous_degree, degree_r_at_r0);
	      Nxpat0 += kick_at_r0;
	
	      if ( fill ) { FillKickDegree(0, degree_at_r0, kick_at_r0, mom); }
	    }
	  }
	}

	      
	previous_rhat = rhat;
	previous_prhat = prhat;
	previous_degree = degree;
      }
	      
    
      if ( debug_ ) { cout << "Made it past the kick/angle definitions." << endl; }


      if ( track_ID == 1 ) {
	if ( fill ) {
	  //cout << "Polarization [" << mytrack << "/" << global_turn << "]: " << endl;
	  //cout << "\t" << polx << "\t" << poly << "\t" << polz << endl;
	  //cout << "\t" << pol_azimuth << "\t" << pol_radial << "\t" << pol_vertical << endl;

	  if ( global_turn == 0 && mytrack_vol == 0 ) {	  
	    FillRingTracker(0, 0, xe, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical);
	  }
      
	  if ( global_turn >= maxturnsreal_ - 30 - 1 ) {
	    Npass_finalavg++;
	    FillRingTracker(2, 0, xe, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical);
	    if ( SaveVRingHits_ == true ) {	
	      h_RingTracker_Time_dNdXe->Fill(xe);
	      h_RingTracker_Time_dNdX->Fill(rhat);
	      h_RingTracker_Time_dNdXprime->Fill(1000*prhat);
	      h_RingTracker_Time_d2NdXprimeX->Fill(rhat, 1000*prhat);
	      h_RingTracker_Time_dNdY->Fill(vhat);
	      h_RingTracker_Time_dNdYprime->Fill(1000*pvhat);
	      h_RingTracker_Time_d2NdYprimeY->Fill(vhat, 1000*pvhat);
	      h_RingTracker_Time_d2NdXY->Fill(rhat, vhat);
	      h_RingTracker_Time_d2NdXprimeYprime->Fill(1000*prhat, 1000*pvhat);
	    }
	  }
      
	  //cout << global_turn << "\t" << maxturnsreal_ << "\t" << pass_final << endl;
	  if ( global_turn >= maxturnsreal_ - 1 ) {
	    if ( pass_final == false ) {
	      pass_final = true;
	      Npass_final++;
	      FillRingTracker(1, 0, xe, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical);	  
	    }
	  }
	}
    
	if ( pass_final ) {
	  if ( fill ) { FillKickDegree(1, degree_at_r0, kick_at_r0, mom); }
	}
      }

    
      if ( debug_ ) { cout << "Made it past the stored fills." << endl; }
    


      if ( track_ID == 1 && mytrack_vol == 0 && global_turn == 0 ) { Nstart_ring++; }
      if ( track_ID == 1 && mytrack_vol == 0 && global_turn == 1 ) { Nstored1_ring++; }
      if ( track_ID == 1 && mytrack_vol == 0 && global_turn == 10 ) { Nstored10_ring++; }
      if ( track_ID == 1 && mytrack_vol == 0 && global_turn == 100-1 ) { Nstored100_ring++; }
	
      if ( track_ID == 1 && mytrack_vol == 0 ) {
	int stored_val = -1;
	if ( global_turn == 0 )   { stored_val = 0; }
	if ( global_turn == 1 )   { stored_val = 1; }
	if ( global_turn == 2 )   { stored_val = 2; }
	if ( global_turn == 5 )   { stored_val = 3; }
	if ( global_turn == 10-1 )  { stored_val = 4; }
	if ( global_turn == 50-1 ) { stored_val = 5; }
	if ( global_turn == 100-1 ) { stored_val = 6; }
	if ( global_turn == 200-1 ) { stored_val = 7; }
	if ( global_turn == 500-1 ) { stored_val = 8; }
	if ( global_turn == 1000-1 ) { stored_val = 9; }
	if ( global_turn == 2000-1 ) { stored_val = 10; }
	//cout << stored_val << endl;
	  
	if ( stored_val > -1 ) {
	  if ( fill ) { 
	    FillRingTracker(stored_val, 1, xe_init, rhat_init, vhat_init, prhat_init, pvhat_init, pmom_init, pol_azimuth, pol_radial, pol_vertical);	  
	  }	    
	}
      }

    
      if ( debug_ ) { cout << "Made it past all of the stored fills." << endl; }
    


      if ( track_ID == 1 && global_turn == 0 ) {
	if ( mom_init < 0 ) { 
	  mom_init = momentum;
	}
	station = -1;

	if ( global_turn == 0 ) {
	  if ( mytrack >= 1 && mytrack <= 7 ) { station = mytrack - 1; }
	  if ( mytrack_vol == 10 ) { station = 7; }
	}
	else if ( global_turn == 1 ) {
	  if ( mytrack_vol == 0 ) { station = 8; }
	}
	
	if ( station >= 0 ) {
	  if ( fill ) { 
	    if ( station < Nringtrackers ) {
	      FillTimeRingTracker(station, xe, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical);	  
	    }
	  }
	}
      }


      if ( debug_ ) { cout << "Made it past the 1st turn fills." << endl; }
   


      double dT = global_turn + theta/TMath::TwoPi();
      //cout << global_turn << "\t" << dT << "\t" << theta << endl;
      if ( fill ) {
	//if ( track_ID == 1 ) { G4cout << "Trk: " << pdgid << "\t" << track_ID << "\t" << dT << "\t" << mytrack << "\t" << track_volname << G4endl; }
	//if ( pdgid > -13 && pdgid < 13 ) { G4cout << "Trk-e: " << pdgid << "\t" << track_ID << "\t" << dT << "\t" << mytrack_vol << "\t" << track_volname << G4endl; }
	FillTurnTime(dT, mytrack_vol, track_ID, time, xe, rhat, y, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical, e);
      }


      if ( debug_ ) { cout << "Made it past the time/turn fills." << endl; }
    

    } // loop over ring tracker hits
  } // if ( SaveVRingHits_ )


  if ( debug_ ) { cout << "Made it past the ring trackers." << endl; }


      
  //-----------------
  // Ring Information
  //-----------------  
  if ( debug_ ) { cout << " -- Ring -- " << endl; }
  int systemhits[10];
  int numringhits[30];
  
  for ( int sh = 0; sh < 10; sh++ ) { systemhits[sh] = 0; }
  for ( int rh = 0; rh < 30; rh++ ) { numringhits[rh] = 0; }

  if ( SaveRingHits_ ) {
    //for ( auto ringhit : ringhits) {
    for ( unsigned int i = 0; i < ringhits.size(); ++i ) {
      RingArtRecord ringhit = ringhits[i];
      //for ( auto ringhit : ringhits) {
      int ringtrackID   = ringhit.trackID;
      int ringvolumeUID = ringhit.volumeUID;    
      double rhat = ringhit.rhat;
      double vhat = ringhit.vhat;
      double ringtheta = ringhit.theta;
      double x_ring = (rhat+7112) * cos(ringtheta);
      double z_ring = (rhat+7112) * sin(ringtheta);    
      double ringtime = ringhit.time;
      double deltaE = 1.0;
      ///double deltaE = -1*ringhit.deltaE;
      if ( deltaE <= 0.0 ) { deltaE = 1e-10; }
      //double deltaPrhat = ringhit.deltaPrhat;
      //double deltaPvhat = ringhit.deltaPvhat;
 
      string ringname = pvs.stringGivenID(ringvolumeUID);
      //cout << "Time = " << ringtime << "\tVID = " << ringvolumeUID << " \tName = " << pvs.stringGivenID(ringvolumeUID) << endl;
      //continue;
//       if ( !IsSomething(ringname, kKickerHit) && 
// 	   !IsSomething(ringname, kQuadHit) && 
// 	   !IsSomething(ringname, kCryostatHit) && 
// 	   !IsSomething(ringname, kInflectorHit) && 
// 	   !IsSomething(ringname, kVacuumChamberHit) ) {
// 	cout << "Time = " << ringtime << "\tVID = " << ringvolumeUID << " \tName = " << pvs.stringGivenID(ringvolumeUID) << endl;
//       }
      //continue;

      if ( ringtrackID != 1 ) { continue; }
	
      int system = IsSomething(ringname);
      //cout << system << "\t" << ringname << "\t" << endl;
      
      if ( system == -1 || system >= Nsystemtrackers) {
	cout << "Time = " << ringtime << "\tVID = " << ringvolumeUID << " \tName = " << pvs.stringGivenID(ringvolumeUID) << endl;
	continue;
      }

      systemhits[system]++;
      if ( fill ) { 
	for ( int st = 0; st < 2; st++ ) {
	  //cout << "\t" << system << "\t" << st << "\t" << pass_final << endl;
	  if ( st == 1 && pass_final == false ) { continue; }
	  h_SystemHitTracker_XZ[system][st]->Fill(z_ring, x_ring, deltaE);
	  h_SystemHitTracker_RhatY[system][st]->Fill(rhat, vhat, deltaE);
	  double dT = ringtime/1000;
	  h_SystemHitTracker_YTime[system][st]->Fill(dT, vhat, deltaE);
	  h_SystemHitTracker_RhatTime[system][st]->Fill(dT, rhat, deltaE);
	}

	system = kAllSystemsHit;
	for ( int st = 0; st < 2; st++ ) {
	  if ( st == 1 && pass_final == false ) { continue; }
	  h_SystemHitTracker_XZ[system][st]->Fill(z_ring, x_ring, deltaE);
	  h_SystemHitTracker_RhatY[system][st]->Fill(rhat, vhat, deltaE);
	  double dT = ringtime/1000;
	  h_SystemHitTracker_YTime[system][st]->Fill(dT, vhat, deltaE);
	  h_SystemHitTracker_RhatTime[system][st]->Fill(dT, rhat, deltaE);
	}

      }

      continue;
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
  }

  if ( debug_ ) { cout << "Made it past the ring hits." << endl; }





  //-------------------
  // G4 Generated Stuff
  //-------------------
  //double x_gen, y_gen, xprime_gen, yprime_gen;

  TLorentzVector mu, el, nu_mu, nu_el, mu_decay, el_detector, mu_lost, mu_store;
  TVector3 mu_pol, el_pol, mu_boost;
  //double CosTheta = -99.9;
  if ( SaveTruthHits_ ) {

    //TrackingActionArtRecordCollection const & truthhits = *truthhitDataHandle;
    //TrackingActionArtRecordCollection const & truthfinishedhits = *truthfinishedhitDataHandle;
    TrackingActionArtRecordCollection thits;
    double t0 = 0.0;

    int Npart[5] = {0, 0, 0, 0, 0};
    //cout << endl;
    for ( int i = 0; i < 2; i++ ) {
      if ( i == 0 ) {
	thits = *truthhitDataHandle;
      }
      else {
	thits = *truthfinishedhitDataHandle;
      }
      //cout << i << "\t" << thits.size() << endl;

      //for ( auto htruthdata : truthhits) { 
      for ( auto htruthdata : thits) {  
	//cout << "i = " << i << endl;
	std::string trackType = htruthdata.trackType;
	//int trackID = htruthdata.trackID;
	//int parentTrackID = htruthdata.parentTrackID;
	int turn = htruthdata.turn;
	//int volumeUID = htruthdata.volumeUID;
	int status = htruthdata.status;
	double rhat = htruthdata.rhat;
	double vhat = htruthdata.vhat;
	double theta = htruthdata.theta;
	double time = htruthdata.time;
	double p = htruthdata.p;
	double e = htruthdata.e;
	double prhat = htruthdata.prhat;
	double pvhat = htruthdata.pvhat;
	double polx_pre = htruthdata.polx;
	double poly_pre = htruthdata.poly;
	double polz_pre = htruthdata.polz;
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
	double r = rhat + 7112;
	int pdgid = 0;
	double xe_truth = ComputeXe(p, 0.137);
	double Polarization = -99.9;


	TVector3 pol_pre(polx_pre, poly_pre, polz_pre);
	double polx = pol_pre.Dot(xAxis);      
	double poly = pol_pre.Dot(yAxis);
	double polz = pol_pre.Dot(zAxis);    
	double pol_azimuth_truth, pol_radial_truth, pol_vertical_truth;
	ComputePolarization(&pol_azimuth_truth, &pol_radial_truth, &pol_vertical_truth, polx, poly, polz);


	if ( trackType == "mu+" ) { pdgid = -13; }
	if ( trackType == "e+" ) { pdgid = -11; }
	if ( trackType == "mu-" ) { pdgid = 13; }
	if ( trackType == "e-" ) { pdgid = 11; }


	//cout << "i = " << i << "\t" << trackType << "\t" << time << "\t" << pdgid << "\t" << status << endl;

	int part = -1;
    
	if ( trackType == "pi+" || trackType == "pi-" ) { part = 0; }
	else if ( trackType == "mu+" || trackType == "mu-" ) { part = 0; }
	else if ( trackType == "proton" ) { part = 0; }
	else if ( trackType == "e+" || trackType == "e-" ) { part = 1; }
	else { part = 2; }
    
	if ( status == gm2ringsim::kBirth ) {
	  if ( trackType == "mu+" || trackType == "mu-" ) { 
	    mu.SetPxPyPzE(px, py, pz, e);
	    t0 = time;
	  }
	  if ( trackType == "e+" || trackType == "e-" ) { el.SetPxPyPzE(px, py, pz, e); }
	  if ( trackType == "anti_nu_mu" || trackType == "nu_mu" ) { nu_mu.SetPxPyPzE(px, py, pz, e); }
	  if ( trackType == "anti_nu_e" || trackType == "nu_e" ) { nu_el.SetPxPyPzE(px, py, pz, e); }
	}

	if ( status == gm2ringsim::kStore ) {
	  if ( pdgid == 13 || pdgid == -13 ) {
	    mu_store.SetPxPyPzE(px, py, pz, e);
	  }
	}
	if ( status == gm2ringsim::kLost ) {
	  if ( pdgid == 13 || pdgid == -13 ) {
	    mu_lost.SetPxPyPzE(px, py, pz, e);
	  }
	}

	if ( status == gm2ringsim::kDecay ) {
	  if ( pdgid == 13 || pdgid == -13 ) {
	    mu_pol.SetXYZ(pol_radial_truth, pol_vertical_truth, pol_azimuth_truth);
	    mu_decay.SetPxPyPzE(px, py, pz, e);
	  }
	  if ( pdgid == 11 || pdgid == -11 ) {
	    el_detector.SetPxPyPzE(px, py, pz, e);
	    el_pol.SetXYZ(px, py, pz);
	    
// 	    cout << "Muon Polarization: " << mu_pol.X() << "\t" << mu_pol.Y() << "\t" << mu_pol.Z() << endl;
// 	    cout << "Electron Momentum: " << el_pol.X() << "\t" << el_pol.Y() << "\t" << el_pol.Z() << endl;
// 	    cout << "S-dot-P:           " << el_pol.Dot(mu_pol)/el_pol.Mag() << endl;
	    Polarization = el_pol.Dot(mu_pol)/el_pol.Mag();
	  }
	}

	if ( status == gm2ringsim::kBirth ) {
	  if ( pdgid == 11 || pdgid == -11 ) {
	    el_pol.SetXYZ(px, py, pz);
	    
// 	    cout << "Muon Polarization: " << mu_pol.X() << "\t" << mu_pol.Y() << "\t" << mu_pol.Z() << endl;
// 	    cout << "Electron Momentum: " << el_pol.X() << "\t" << el_pol.Y() << "\t" << el_pol.Z() << endl;
// 	    cout << "S-dot-P:           " << el_pol.Dot(mu_pol)/el_pol.Mag() << endl;
	    Polarization = el_pol.Dot(mu_pol)/el_pol.Mag();
	  }
	}
	  
	

	if ( part == 1 && 0 ) {
	  cout << "Polarization: " << endl;
	  cout << "\t" << polx_pre << "\t" << poly_pre << "\t" << polz_pre << endl;
	  cout << "\t" << polx << "\t" << poly << "\t" << polz << endl;
	  cout << "\t" << pol_azimuth_truth << "\t" << pol_radial_truth << "\t" << pol_vertical_truth << endl;
	}
	//continue;
      
    
	TVector3 truthmom(px, py, pz);
	double x_truth = x - beamStart_.X();
	double y_truth = y - beamStart_.Y();
	double z_truth = z - beamStart_.Z();
	double r_truth = TMath::Sqrt(x_truth*x_truth + z_truth*z_truth);
	double rhat_truth = r_truth;
	double xprime_truth = truthmom.Dot(xAxis)/truthmom.Mag();
	double yprime_truth = truthmom.Dot(yAxis)/truthmom.Mag();
	double zprime_truth = truthmom.Dot(zAxis)/truthmom.Mag();
	double rprime_truth = TMath::Sqrt(xprime_truth*xprime_truth + zprime_truth*zprime_truth);

	
    
	rhat_truth = rhat;
	rprime_truth = prhat;
	y_truth = y;
	x_truth = x;
	z_truth = z;
	
	
	
	if ( part == 1 && 0 ) {
	  G4cout << "\tRhat = " << rhat << "\tRt = " << rhat_truth << "\tR = " << r << "\t" << y << "\t" << y_truth << "\t" << rprime_truth << "\t" << yprime_truth << G4endl;
	}


	//     x_gen = x_truth;
	//     y_gen = y_gen;
	//     xprime_gen = xprime_truth;
	//     yprime_gen = yprime_truth;
    
	//     double rhat_offset = 274.3;
	//     rhat -= rhat_offset;
    
	//     double vhat_offset = 0;
	//     vhat -= vhat_offset;
    
	if ( fill ) { 
	  for ( int mytime = 0; mytime < Ntruthtimes; mytime++ ) {
	    //if ( mytime == 5 ) { cout << "pass_final = " << pass_final << endl; }

	    if ( mytime == 1 && pass_final == false ) { continue; }
	    if ( mytime == 2 && pass_final == false ) { continue; }

	    FillTruthTracker(mytime, status, pdgid, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, t0, pol_azimuth_truth, pol_radial_truth, pol_radial_truth, py, Polarization);
	  }
	}
    
	int counter = -1;
	if ( theta > 0 && theta <= TMath::TwoPi()/12.0 ) {
	  counter = 0;
	}


	int particle = -1;
	if ( pdgid == 13 || pdgid == -13 ) {
	  if ( status == gm2ringsim::kBirth ) { particle = 0; }
	  if ( status == gm2ringsim::kDecay ) { particle = 1; }
	  if ( status == gm2ringsim::kLost )  { particle = 2; }
	}
	if ( pdgid == 11 || pdgid == -11 ) {
	  if ( status == gm2ringsim::kBirth ) { particle = 4; }
	  if ( status == gm2ringsim::kDecay ) { particle = 3; }
	}
	if ( particle > -1 ) { 
	  Npart[particle]++;
	}
  

	turn = (int)(time-t0)/(149*ns);
	double dT = turn + theta/TMath::TwoPi();
	//if ( status == gm2ringsim::kDecay ) {
	//cout << i << "\t" << status << "\tTurn = " << turn << "\t" << time << "\t" << t0 << "\t" << dT << endl;
	//}
	if ( fill ) {
	  //if ( track_ID == 1 ) { G4cout << "Trk: " << pdgid << "\t" << track_ID << "\t" << dT << "\t" << mytrack << "\t" << track_volname << G4endl; }
	  //if ( pdgid > -13 && pdgid < 13 ) { G4cout << "Trk-e: " << pdgid << "\t" << track_ID << "\t" << dT << "\t" << mytrack_vol << "\t" << track_volname << G4endl; }
	  FillTruthTurnTime(dT, counter, status, pdgid, time, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, e);
	}
      } // loop over G4 track
    }
  
    if ( mu.Pt() > 0 && el.Pt() > 0 && nu_el.Pt() > 0 && nu_mu.Pt() > 0 && mu_decay.Pt() > 0 && 0 ) {
    
      double Emax = (105.7*105.7 + 0.511*0.511)/(2*105.7);

//       int Nud = 0;
//       if ( el.Py() < 0 ) { Nud = -1; }
//       if ( el.Py() > 0 ) { Nud = 1; }

      //cout << "Found all 4 decay products." << G4endl;
      TLorentzVector mu_comp = el + nu_el + nu_mu;
      //TVector3 mrf = mu_comp.BoostVector();
      TVector3 mrf = mu_decay.BoostVector();
      //cout << mu.M() << "\t" << el.M() << "\t" << nu_el.M() << "\t" << nu_mu.M() << endl;
      mu_decay.Boost(-mrf);
      el.Boost(-mrf);
      //TVector3 el_vec = el.Vect();
      nu_el.Boost(-mrf);
      nu_mu.Boost(-mrf);
      //cout << mu_comp.P() << "\t" << el.P() << "\t" << el.CosTheta() << "\t" << el_vec.Dot(el_pol) << "\t" << endl;

      

      double x = el.E() / Emax;
      double pol = 1.0;
      double cosT = 1.0;

      for ( int mytime = 0; mytime < Ntruthtimes; mytime++ ) {
	//if ( mytime == 5 ) { cout << "pass_final = " << pass_final << endl; }

	if ( mytime == 1 && pass_final == false ) { continue; }
	if ( mytime == 2 && pass_final == false ) { continue; }
      
	for ( int part = 0; part < Nparticles; part++ ) {
	  string truth_part_name = truth_particle_names[part];
	  if ( truth_part_name.find("Electron") ) {
	    h_G4Tracker_Time_dCosTheta[mytime][part]->Fill(cosT);
	    h_G4Tracker_Time_dX[mytime][part]->Fill(x);
	    h_G4Tracker_Time_Polarization[mytime][part]->Fill(pol);
	    h_G4Tracker_Time_dXdCosTheta[mytime][part]->Fill(x, cosT);
	  }
	}
      }
    }


  }

  if ( debug_ ) { cout << "Made it past the truth info." << endl; }


} // analyze

using gm2ringsim::ringTrackerAnalyzer;
DEFINE_ART_MODULE(ringTrackerAnalyzer)
