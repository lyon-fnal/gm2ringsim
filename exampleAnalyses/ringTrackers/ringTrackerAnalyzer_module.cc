// This is the ringTrackerAnalyzer Art module.

//  This is an analyzer that will read HitData from the event and make some plots and a root tree

// Art includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

// Root + Art include
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
#include "gm2ringsim/common/g2PreciseValues.hh"

// Hit includes
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include "gm2ringsim/common/virtualringstation/VirtualRingStationArtRecord.hh"
#include "gm2ringsim/common/ring/RingArtRecord.hh"
#include "gm2ringsim/actions/track/TrackingActionArtRecord.hh"
#include "gm2ringsim/inflector/InflectorArtRecord.hh"
#include "gm2ringsim/calo/CaloArtRecord.hh"


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

#define Zhatbin 200
#define Zhatmin -TMath::Pi()
#define Zhatmax TMath::Pi()

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

#define vring1p 11

#define Nstations 72*4
#define Nstations_spin 72*4
#define Nstations_nospin 12

#define Nringtimestamps 3
#define Ntruthtimestamps 3

#define MaxNparticles 10

#define kElectronID 11
#define kPositronID -11
#define kMuonID 13
#define kAntiMuonID -13

using namespace std;

enum {kGeneratedDist, kRemainingDist};
enum {kBegin, kFinal};

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
  std::string calohitModuleLabel_;
  std::string caloinstanceName_;
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
  bool SaveCaloHits_;
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
  int Rbin(double rhat);

  double ComputeQuadIntAngle(double r1, double r2, double d1, double d2);
  double ComputeRmagicIntAngle(double r1, double r2, double d1, double d2);
  double ComputeKickAtRmagicInt(double pr1, double pr2, double d1, double d2, double dr0);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z);


  bool FillByTimestamp(int counter, int timestamp);
  double TimeOffset(int timestamp, int counter);

  void FillRingTracker(int hist, int type, double theta, double xe, double xe_rms, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly, double zhat);
  void FillTimeRingTracker(int hist, double xe, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly);


  void FillTruthTracker(int hist, int particle, int counter, double xe, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double theta, double t0, double time, double tDecay, double polz, double polx, double poly, double Polarization);
  void FillTruthTracker(int hist, int particle, int counter, int calonum, double xe_truth, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double theta, double t0, double time, double tDecay, double polz, double polx, double poly, double Polarization);
  void FillTruthTurnTime(double dT, int counter, int status, int pdgid, double time, double xe, double rhat, double y, double prhat, double pvhat, double mom, double theta, double polz, double polx, double poly, double e);
  void FillTruthTurnTime(double xval, int hist, int particle, double xe, double rhat, double y, double prhat, double pvhat, double mom, double theta, double polz, double polx, double poly);
  void FillNumTruthTurnTime(double xval, int hist, int particle, int counter, double y, double e);

  void FillTurnTime(double dT, double time, int counter, int trackID, double zhat, double xe, double xe_rms, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e);
  void FillKickDegree(int histtype, double degree_at_r0, double kick_at_r0, double mom);
  void FillInflector(int hist, double x_inf, double y_inf, double z_inf, double r_inf, double prhat_ring, double pvhat_ring, double xprime_inf, double yprime_inf);

  void InitializeBeamCoordinates();
  void RotateRingToBeamCoordinates(double theta);

  void NewFixTurnCounter(int *global_turn, int mytrack_vol, int ptrack_vol);
  void FixTurnCounter(int *myturn, int *mytrack, double time, double t1);

  int FindParticle(int pdgid, int status);
  string ParticleStatusName(int status, int id);
  
  void SetInitial(double *t0, double *t1, double *theta1, double *prhat_init, double *pvhat_init, double *xe_init, double *xe_rms_init, double *rhat_init, double *vhat_init, double *pmom_init, double *pol_init, double time, double theta, double prhat, double pvhat, double xe, double xe_rms, double rhat, double vhat, double mom, double pol);

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
  string timestamps[10];
  map<int, int> timestamp_map;

  string rtnames[9];
 

  bool spin_decay;
  bool keep_edm_plots;
  int Nstat;
  
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
  TH2F *h_RingTracker_Snapshot_RhatY[9];
  TH2F *h_RingTracker_Snapshot_XprimeX[9];
  TH2F *h_RingTracker_Snapshot_YprimeY[9];  
  TH1F *h_RingTracker_Snapshot_Mom[9];
  TH1F *h_RingTracker_Snapshot_Rhat[9];
  TH1F *h_RingTracker_Snapshot_Xe[9];
  TH1F *h_RingTracker_Snapshot_Vhat[9];
  TH1F *h_RingTracker_Snapshot_Pol[9];
  TH1F *h_RingTracker_Snapshot_PolX[9];
  TH1F *h_RingTracker_Snapshot_PolY[9];



  //--------------------------
  // Virtual Ring Trackers
  //--------------------------
  int Nringtrackertimes;
  string times[3];
  TH1F *h_RingTracker_Xprime[3][2];
  TH1F *h_RingTracker_Yprime[3][2];
  TH2F *h_RingTracker_XprimeX[3][2];
  TH2F *h_RingTracker_YprimeY[3][2];
  TH2F *h_RingTracker_XprimeYprime[3][2];
  TH2F *h_RingTracker_RhatY[3][2];
  TH1F *h_RingTracker_Rhat[3][2];
  TH1F *h_RingTracker_EqRad[3][2];
  TH2F *h_RingTracker_EqRadMom[3][2];
  TH1F *h_RingTracker_EqRadRMS[3][2];
  TH2F *h_RingTracker_EqRadRMSMom[3][2];
  TH1F *h_RingTracker_Vhat[3][2];
  TH1F *h_RingTracker_Mom[3][2];
  TH1F *h_RingTracker_Theta[3][2];
  TH1F *h_RingTracker_Pol[3][2];
  TH1F *h_RingTracker_PolX[3][2];
  TH1F *h_RingTracker_PolY[3][2];
  TH1F *h_RingTracker_Zhat[3][2];
  TH1F *h_RingTracker_dNdXe;
  TH1F *h_RingTracker_dNdX;
  TH1F *h_RingTracker_dNdXprime;
  TH1F *h_RingTracker_dNdY;
  TH1F *h_RingTracker_dNdYprime;
  TH2F *h_RingTracker_d2NdXY;
  TH2F *h_RingTracker_d2NdXprimeYprime;
  TH2F *h_RingTracker_d2NdXprimeX;
  TH2F *h_RingTracker_d2NdYprimeY;

  
  TH1F *h_RingTracker_Nhits;

  TH2F *h_RingTracker_OmegaC_Radbin;
  TH2F *h_RingTracker_OmegaC_Mombin;
  TH1F *h_RingTracker_OmegaC[10];
  //int Nstoredtrackers;
  //string rtsnames[11];
  

  TH2F *h_RingTracker_Zhat_vs_Time[3];
  TH2F *h_RingTracker_Rhat_vs_Time[3];
  TH2F *h_RingTracker_EqRad_vs_Time[3];
  TH2F *h_RingTracker_EqRadRMS_vs_Time[3];
  TH2F *h_RingTracker_Prhat_vs_Time[3];
  TH2F *h_RingTracker_Pvhat_vs_Time[3];
  TH2F *h_RingTracker_Mom_vs_Time[3];
  TH2F *h_RingTracker_Y_vs_Time[3];
  TH2F *h_RingTracker_Pol_vs_Time[3];
  TH2F *h_RingTracker_PolX_vs_Time[3];
  TH2F *h_RingTracker_PolY_vs_Time[3];
  TH1F *h_RingTracker_NgtEth_vs_Time[3];
  TH1F *h_RingTracker_NwghtE_vs_Time[3];
  TH1F *h_RingTracker_Num_vs_Time[3];



  //------------------
  // Truth Information
  //------------------
  int Ntruthtimes;
  int Nparticles;
  map<string, int> truth_particle_nums;
  string truth_particle_names[20];
  TH1F *h_G4Tracker_Xe[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Rhat[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_ThetaRhat[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Vhat[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Pol[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_PolX[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_PolY[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Theta[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_ThetaR[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Xprime[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Yprime[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_XprimeX[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_YprimeY[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_RhatY[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_XZ[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_R[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Mom[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_t0[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumCalo[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Time[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_DriftTime[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Polarization[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_dYprimedY[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_dXprimedX[Ntruthtimestamps][24][MaxNparticles];
  
  TH2F *h_G4Tracker_R_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Rhat_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Xe_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Prhat_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Pvhat_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Mom_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Y_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Pol_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_PolX_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_PolY_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Theta_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_E_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH1F *h_G4Tracker_Yprime_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH1F *h_G4Tracker_Vhat_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_NumCalo_vs_Time[Ntruthtimestamps][MaxNparticles];
  
  TH1F *h_G4Tracker_TimeOffset_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH1F *h_G4Tracker_NgtEth_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_NwghtE_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Num_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Ndown_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Nup_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Nud_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  int G4Tracker_Nu[MaxNparticles];
  int G4Tracker_Nd[MaxNparticles];


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
      if ( times[i] != "Final" ) { continue; }
      for ( int t = 0; t < Ntimevals; t++ ) {
	if ( Npass_final > 0 && Npass_finalavg > 0 ) {
	  double finalavg_sf = (double)Npass_final / Npass_finalavg;
	  cout << "Scaling Final histograms by " << finalavg_sf << endl;
	  h_RingTracker_Xprime[i][t]->Scale(finalavg_sf);
	  h_RingTracker_Yprime[i][t]->Scale(finalavg_sf);
	  h_RingTracker_XprimeX[i][t]->Scale(finalavg_sf);
	  h_RingTracker_YprimeY[i][t]->Scale(finalavg_sf);
	  h_RingTracker_XprimeYprime[i][t]->Scale(finalavg_sf);
	  h_RingTracker_RhatY[i][t]->Scale(finalavg_sf);
	  h_RingTracker_Rhat[i][t]->Scale(finalavg_sf);
	  h_RingTracker_EqRad[i][t]->Scale(finalavg_sf);
	  h_RingTracker_EqRadMom[i][t]->Scale(finalavg_sf);
	  h_RingTracker_EqRadRMS[i][t]->Scale(finalavg_sf);
	  h_RingTracker_EqRadRMSMom[i][t]->Scale(finalavg_sf);
	  h_RingTracker_Vhat[i][t]->Scale(finalavg_sf);
	  h_RingTracker_Mom[i][t]->Scale(finalavg_sf);
	  h_RingTracker_Theta[i][t]->Scale(finalavg_sf);
	  h_RingTracker_Pol[i][t]->Scale(finalavg_sf);
	  h_RingTracker_PolX[i][t]->Scale(finalavg_sf);
	  h_RingTracker_PolY[i][t]->Scale(finalavg_sf);
	}
      }
    }
      
    if ( Npass_final > 0 && Npass_finalavg > 0 ) {
      double finalavg_sf = (double)Npass_final / Npass_finalavg;
      cout << "Scaling Final histograms by " << finalavg_sf << endl;
      h_RingTracker_dNdXe->Scale(finalavg_sf);
      h_RingTracker_dNdX->Scale(finalavg_sf);
      h_RingTracker_dNdXprime->Scale(finalavg_sf);
      h_RingTracker_d2NdXprimeX->Scale(finalavg_sf);
      h_RingTracker_dNdY->Scale(finalavg_sf);
      h_RingTracker_dNdYprime->Scale(finalavg_sf);
      h_RingTracker_d2NdYprimeY->Scale(finalavg_sf);
      h_RingTracker_d2NdXY->Scale(finalavg_sf);
      h_RingTracker_d2NdXprimeYprime->Scale(finalavg_sf);
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
  calohitModuleLabel_        ( p.get<std::string>("calohitModuleLabel",  "artg4"   ) ),
  caloinstanceName_          ( p.get<std::string>("caloinstanceName",    "calorimeter") ),
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
  SaveCaloHits_      ( p.get<bool>("SaveCaloHits", false)),
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
  if ( SaveCaloHits_ ) { cout << "  -> Saving calo hits." << endl; }
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

  Nringtrackertimes = kFinal + 1;
  Ntruthtimes = kFinal + 1;
  times[kBegin] = "Begin";
  times[kFinal] = "Final"; 

  Ntimevals = kRemainingDist + 1;
  timevals[kGeneratedDist] = "GeneratedDist";
  timevals[kRemainingDist] = "RemainingDist";

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

  Ntimestamps = 0;
  timestamp_map[Ntimestamps] = 0;
  timestamps[Ntimestamps++] = "Turn";

  timestamp_map[Ntimestamps] = 2;
  timestamps[Ntimestamps++] = "TimeOncePerTurn";

  timestamp_map[Ntimestamps] = 1;
  timestamps[Ntimestamps++] = "Time";


  truth_particle_nums["BirthMuon"] = -1;
  truth_particle_nums["DecayMuon"] = -1;
  truth_particle_nums["LostMuon"] = -1;
  truth_particle_nums["StoredMuon"] = -1;
  truth_particle_nums["BirthElectron"] = -1;
  truth_particle_nums["BirthElectronEgtEth"] = -1;
  truth_particle_nums["DecayElectron"] = -1;
  truth_particle_nums["DecayElectronEgtEth"] = -1;
  truth_particle_nums["DecayElectronEgtHghEth"] = -1;
  for ( int i = 0; i < MaxNparticles; i++ ) { 
    truth_particle_names[i] = "";
  }

  
  if ( basename_.find("NoSpinTracking") != string::npos ) {
    spin_decay = true;
  }
  else {
    spin_decay = true;
  }
  keep_edm_plots = false;

  Nstat = Nstations;
  if ( spin_decay ) { Nstat = Nstations_spin; }
  else { Nstat = Nstations_nospin; }
  
  cout << "------- Spin Info -------" << endl;
  cout << "Spin Decay:   " << spin_decay << endl;
  cout << "EDM Plots:    " << keep_edm_plots << endl;
  cout << "Num Stations: " << Nstat << endl;

  if ( basename_.find("NoMuonDecay") != string::npos ) {
    Nparticles = 0;
    truth_particle_names[Nparticles++] = "BirthMuon";
    truth_particle_names[Nparticles++] = "LostMuon";
    truth_particle_names[Nparticles++] = "StoredMuon";
  }
  else {
    Nparticles = 0;
    truth_particle_names[Nparticles++] = "BirthMuon";
    truth_particle_names[Nparticles++] = "DecayMuon";
    truth_particle_names[Nparticles++] = "LostMuon";
    truth_particle_names[Nparticles++] = "StoredMuon";
    truth_particle_names[Nparticles++] = "BirthElectron";
    truth_particle_names[Nparticles++] = "BirthElectronEgtEth";
    truth_particle_names[Nparticles++] = "DecayElectron";
    truth_particle_names[Nparticles++] = "DecayElectronEgtEth";
    truth_particle_names[Nparticles++] = "DecayElectronEgtHghEth";
  }

  for ( int i = 0; i < Nparticles; i++ ) { 
    string name = truth_particle_names[i];
    truth_particle_nums[name] = i;
    cout << i << "\t" << name << "\t" << truth_particle_nums[name] << endl;
  }
  fill = true;


 
  stringstream hname;

  hname << "RingTracker_Nhits";
  h_RingTracker_Nhits = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_ + 2, 0 - 0.5, maxturns_ + 1.5);
  h_RingTracker_Nhits->SetXTitle("Number of VRS Hits");
  hname.str("");


    


  if ( SaveVRingHits_ ) {

    // Save OmegaC Histograms
    hname << "RingTracker_OmegaC_" << "Rbin";
    double mean = 1.0/(gm2ringsim::omegaCMagic()/TMath::TwoPi());      
    h_RingTracker_OmegaC_Radbin = histDir.make<TH2F>(hname.str().c_str(), "", 100, mean - 0.2, mean + 0.2, 900, -45.0, 45.0);
    h_RingTracker_OmegaC_Radbin->SetXTitle("#tau_{c} [ns]");
    h_RingTracker_OmegaC_Radbin->SetYTitle("#hat{r} [mm]");
    hname.str("");
    
    hname << "RingTracker_OmegaC_" << "Mombin";
    h_RingTracker_OmegaC_Mombin = histDir.make<TH2F>(hname.str().c_str(), "", 100, mean - 0.2, mean + 0.2, 900, 1-0.01, 1+0.01);
    h_RingTracker_OmegaC_Mombin->SetXTitle("#tau_{c} [ns]");
    h_RingTracker_OmegaC_Mombin->SetYTitle("|p|/p_{m}");
    hname.str("");
    
    for ( int r = 0; r < 10; r++ ) {
      hname << "RingTracker_OmegaC_" << "Rbin" << r;
      double mean = 1.0/(gm2ringsim::omegaCMagic()/TMath::TwoPi());      
      //cout << "HIST-" << r << "\t" << mean << endl;
      h_RingTracker_OmegaC[r] = histDir.make<TH1F>(hname.str().c_str(), "", 1000, mean - 0.2, mean + 0.2);
      h_RingTracker_OmegaC[r]->SetXTitle("#tau_{c} [ns]");
      hname.str("");
    }



    // Save other ring tracker histograms
    for ( int t = 0; t < Ntimevals; t++ ) {
      string typname = timevals[t];
    
      for ( int tt = 0; tt < Nringtrackertimes; tt++ ) {
	string ringtracker_time_name = times[tt];
      
	if ( fill ) {
	
	  hname << "RingTracker_Xprime_" << ringtracker_time_name << "_" << typname;
	  //cout << hname.str() << endl;
	  h_RingTracker_Xprime[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	  h_RingTracker_Xprime[tt][t]->SetXTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_Yprime_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Yprime[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	  h_RingTracker_Yprime[tt][t]->SetXTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_XprimeX_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_XprimeX[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	  h_RingTracker_XprimeX[tt][t]->SetXTitle("x_{ring} [mm]");
	  h_RingTracker_XprimeX[tt][t]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_YprimeY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_YprimeY[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Ybin_Ring, Ymin_Ring, Ymax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	  h_RingTracker_YprimeY[tt][t]->SetXTitle("y_{ring} [mm]");
	  h_RingTracker_YprimeY[tt][t]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
      
	  hname << "RingTracker_XprimeYprime_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_XprimeYprime[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	  h_RingTracker_XprimeYprime[tt][t]->SetXTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  h_RingTracker_XprimeYprime[tt][t]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
	
	  hname << "RingTracker_RhatY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_RhatY[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_RhatY[tt][t]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_RingTracker_RhatY[tt][t]->SetYTitle("y_{ring} [mm]");
	  hname.str("");
	
	  hname << "RingTracker_Rhat_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Rhat[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_Rhat[tt][t]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  hname.str("");
	
	  hname << "RingTracker_Zhat_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Zhat[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Zhatbin, Zhatmin, Zhatmax);
	  h_RingTracker_Zhat[tt][t]->SetXTitle("z_{ring} #equiv #omega_{c}[t-t_{0}] - 2#piN_{turns}");
	  hname.str("");
	  
	  hname << "RingTracker_EqRad_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_EqRad[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_EqRad[tt][t]->SetXTitle("x_{e} #equiv <R - R_{m}> [mm]");
	  hname.str("");
	  
	  hname << "RingTracker_EqRadMom_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_EqRadMom[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Mombin, Mommin, Mommax);
	  h_RingTracker_EqRadMom[tt][t]->SetXTitle("x_{e} #equiv <R - R_{m}> [mm]");
	  h_RingTracker_EqRadMom[tt][t]->SetYTitle("P_{#mu} / P_{m}");
	  hname.str("");
	  
	  hname << "RingTracker_EqRadRMS_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_EqRadRMS[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_EqRadRMS[tt][t]->SetXTitle("x_{e} #equiv <R - R_{m}>^{2} [mm]");
	  hname.str("");
	  
	  hname << "RingTracker_EqRadRMSMom_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_EqRadRMSMom[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Mombin, Mommin, Mommax);
	  h_RingTracker_EqRadRMSMom[tt][t]->SetXTitle("x_{e} #equiv <R - R_{m}>^{2} [mm]");
	  h_RingTracker_EqRadRMSMom[tt][t]->SetYTitle("P_{#mu} / P_{m}");
	  hname.str("");
	
	  hname << "RingTracker_Vhat_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Vhat[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	  h_RingTracker_Vhat[tt][t]->SetXTitle("y_{ring} [mm]");
	  hname.str("");
      
	  hname << "RingTracker_Mom_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Mom[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	  h_RingTracker_Mom[tt][t]->SetXTitle("P_{#mu} / P_{m}");
	  hname.str("");

	  hname << "RingTracker_Theta_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Theta[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", 200, 0.0, TMath::TwoPi());
	  h_RingTracker_Theta[tt][t]->SetXTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	  hname.str("");

	  hname << "RingTracker_Pol_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Pol[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_RingTracker_Pol[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	  hname.str("");

	  hname << "RingTracker_PolX_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_PolX[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_RingTracker_PolX[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	  hname.str("");

	  hname << "RingTracker_PolY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_PolY[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	  h_RingTracker_PolY[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
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

      hname << "RingTracker_dNdXe";
      h_RingTracker_dNdXe = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_dNdXe->SetXTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
      hname.str("");

      hname << "RingTracker_dNdX";
      h_RingTracker_dNdX = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_dNdX->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
      hname.str("");
    
      hname << "RingTracker_dNdY";
      h_RingTracker_dNdY = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_dNdY->SetXTitle("y_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_d2NdXY";
      h_RingTracker_d2NdXY = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Rhatbin, Rhatmin, Rhatmax);
      h_RingTracker_d2NdXY->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
      h_RingTracker_d2NdXY->SetYTitle("y_{ring} [mm]");
      hname.str("");


      hname << "RingTracker_dNdXprime";
      h_RingTracker_dNdXprime = histDir.make<TH1F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
      h_RingTracker_dNdXprime->SetXTitle("x'_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_dNdYprime";
      h_RingTracker_dNdYprime = histDir.make<TH1F>(hname.str().c_str(), "", YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
      h_RingTracker_dNdYprime->SetXTitle("y'_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_d2NdXprimeYprime";
      h_RingTracker_d2NdXprimeYprime = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
      h_RingTracker_d2NdXprimeYprime->SetYTitle("x'_{ring} #equiv R - R_{m} [mm]");
      h_RingTracker_d2NdXprimeYprime->SetXTitle("y'_{ring} [mm]");
      hname.str("");
    
      hname << "RingTracker_dNdXprimeX";
      h_RingTracker_d2NdXprimeX = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
      h_RingTracker_d2NdXprimeX->SetYTitle("x'_{ring} [mm]");
      h_RingTracker_d2NdXprimeX->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
      hname.str("");
    
      hname << "RingTracker_dNdYprimeY";
      h_RingTracker_d2NdYprimeY = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
      h_RingTracker_d2NdYprimeY->SetYTitle("y'_{ring} [mm]");
      h_RingTracker_d2NdYprimeY->SetXTitle("y_{ring} [mm]");
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
	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_RhatY";
	h_RingTracker_Snapshot_RhatY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Snapshot_RhatY[i]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_Snapshot_RhatY[i]->SetYTitle("y_{ring} [mm]");
	hname.str(""); 
    
	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_YprimeY";
	h_RingTracker_Snapshot_YprimeY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	h_RingTracker_Snapshot_YprimeY[i]->SetXTitle("y_{ring} [mm]");
	h_RingTracker_Snapshot_YprimeY[i]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_XprimeX";
	h_RingTracker_Snapshot_XprimeX[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	h_RingTracker_Snapshot_XprimeX[i]->SetXTitle("x_{ring} [mm]");
	h_RingTracker_Snapshot_XprimeX[i]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Mom";
	h_RingTracker_Snapshot_Mom[i] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	h_RingTracker_Snapshot_Mom[i]->SetXTitle("P_{#mu} / P_{m}");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Rhat";
	h_RingTracker_Snapshot_Rhat[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Snapshot_Rhat[i]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Xe";
	h_RingTracker_Snapshot_Xe[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Snapshot_Xe[i]->SetXTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Vhat";
	h_RingTracker_Snapshot_Vhat[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin, Rhatmin, Rhatmax);
	h_RingTracker_Snapshot_Vhat[i]->SetXTitle("y_{ring} [mm]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Pol";
	h_RingTracker_Snapshot_Pol[i] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	h_RingTracker_Snapshot_Pol[i]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_PolX";
	h_RingTracker_Snapshot_PolX[i] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	h_RingTracker_Snapshot_PolX[i]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_PolY";
	h_RingTracker_Snapshot_PolY[i] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	h_RingTracker_Snapshot_PolY[i]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
	hname.str("");
      }
    } 
  } // if ( SaveVRingHits_ )


  if ( SaveVRing1PlaneHits_ || SaveVRingHits_ ) {
    if ( fill ) {
      for ( int t = 0; t < Nringtimestamps; t++ ) {
	string timename, xtitle;
	
	if ( SaveVRing1PlaneHits_ == true && SaveVRingHits_ == false ) {
	  if ( t == 0 || t == 1 ) { continue; }
	}

	if ( timestamp_map[t] == 0 ) { xtitle = "Turn"; }
	if ( timestamp_map[t] == 1 ||
	     timestamp_map[t] == 2 ||
	     timestamp_map[t] == 3 ) { xtitle = "Time [#mus]"; }
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

	int hitsperturn = 12;
	double maxt = maxturns_;
	if ( timestamp_map[t] == 0 ) { hitsperturn = 1; maxt = maxturns_; }
	if ( timestamp_map[t] == 1 ) { hitsperturn = (int)(1/0.150)+1; maxt = maxturns_*0.150; }
	if ( timestamp_map[t] == 2 ) { hitsperturn = (int)(1/0.150)+1; maxt = maxturns_*0.150; }
	if ( timestamp_map[t] == 3 ) { hitsperturn = (int)(1/0.150)+1; maxt = maxturns_*0.150; }

	hname << "Tracker_EqRad" << "_vs_" << timename;
	h_RingTracker_EqRad_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, rbin, rmin, rmax);
	h_RingTracker_EqRad_vs_Time[t]->SetYTitle("x_{e} #equiv <R - R_{m}> [mm]");
	h_RingTracker_EqRad_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_EqRadRMS" << "_vs_" << timename;
	h_RingTracker_EqRadRMS_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, rbin, rmin, rmax);
	h_RingTracker_EqRadRMS_vs_Time[t]->SetYTitle("x_{e} #equiv <R - R_{m}>^{2} [mm]");
	h_RingTracker_EqRadRMS_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Rhat" << "_vs_" << timename;
	h_RingTracker_Rhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, rbin, rmin, rmax);
	h_RingTracker_Rhat_vs_Time[t]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_Rhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Zhat" << "_vs_" << timename;
	h_RingTracker_Zhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Zhatbin, Zhatmin, Zhatmax);
	h_RingTracker_Zhat_vs_Time[t]->SetYTitle("z_{ring} #equiv #omega_{c}[t-t_{0}] - 2#piN_{turns}");
	h_RingTracker_Zhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Mom" << "_vs_" << timename;
	h_RingTracker_Mom_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Mombin, Mommin, Mommax);
	h_RingTracker_Mom_vs_Time[t]->SetYTitle("P_{#mu} / P_{m}");
	h_RingTracker_Mom_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Prhat" << "_vs_" << timename;
	h_RingTracker_Prhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, xpbin, xpmin, xpmax);
	h_RingTracker_Prhat_vs_Time[t]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	h_RingTracker_Prhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Pvhat" << "_vs_" << timename;
	h_RingTracker_Pvhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, ypbin, ypmin, ypmax);
	h_RingTracker_Pvhat_vs_Time[t]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	h_RingTracker_Pvhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Y" << "_vs_" << timename;
	h_RingTracker_Y_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, rbin, rmin, rmax);
	h_RingTracker_Y_vs_Time[t]->SetYTitle("y_{ring} [mm]");
	h_RingTracker_Y_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Pol" << "_vs_" << timename;
	h_RingTracker_Pol_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Polbin, Polmin, Polmax);
	h_RingTracker_Pol_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{z}");
	h_RingTracker_Pol_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_PolX" << "_vs_" << timename;
	h_RingTracker_PolX_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Polbin, Polmin, Polmax);
	h_RingTracker_PolX_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{r}");
	h_RingTracker_PolX_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_PolY" << "_vs_" << timename;
	h_RingTracker_PolY_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, PolYbin, PolYmin, PolYmax);
	h_RingTracker_PolY_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	h_RingTracker_PolY_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Num" << "_vs_" << timename;
	h_RingTracker_Num_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn*1000, 0.0, maxt);
	h_RingTracker_Num_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_NgtEth" << "_vs_" << timename;
	h_RingTracker_NgtEth_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn*1000, 0.0, maxt);
	h_RingTracker_NgtEth_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_NwghtE" << "_vs_" << timename;
	h_RingTracker_NwghtE_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn*1000, 0.0, maxt);
	h_RingTracker_NwghtE_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      }
    }
  
  } // if ( SaveVRing1PlaneHits_ )  

  
  
  if ( SaveTruthHits_ ) {
    for ( int part = 0; part < Nparticles; part++ ) {
      string truth_part_name = truth_particle_names[part];

      for ( int t = 0; t < Ntruthtimes; t++ ) {
	string truth_time_name = timevals[t];

	int NstatHist = 1;
// 	if ( truth_part_name.find("Electron") != string::npos ) { NstatHist = 25; }
	for ( int s = 0; s < NstatHist; s++ ) {
	  string statname = "";
	  if ( s == 0 ) { statname = ""; }
	  else {
	    stringstream statname_ss;
	    statname_ss << "_CaloStation" << s-1;
	    statname = statname_ss.str();
	  }
	
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
	  rbin  = Rhatbin_Ring;
	  rmin  = Rhatmin_Ring;
	  rmax  = Rhatmax_Ring;
	  xpbin = XPrimebin_Decay_Ring;
	  xpmin = XPrimemin_Decay_Ring;
	  xpmax = XPrimemax_Decay_Ring;
	  ypbin = YPrimebin_Decay_Ring;
	  ypmin = YPrimemin_Decay_Ring;
	  ypmax = YPrimemax_Decay_Ring;
	}

	if ( fill ) {
	  hname << "G4Track_Polarization_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Polarization[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_G4Tracker_Polarization[t][s][part]->SetXTitle("#hat{s}_{#mu}#bullet#hat{p}_{el}");
	  hname.str("");
	
	  hname << "G4Track_dXprimedX_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_dXprimedX[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", rbin, rmin, rmax, xpbin, xpmin, xpmax);
	  h_G4Tracker_dXprimedX[t][s][part]->SetXTitle("x_{gen} [mm]");
	  h_G4Tracker_dXprimedX[t][s][part]->SetYTitle("x'_{gen} #equiv P_{x}/P_{z} (p_{#mu}=0) [mrad]");
	  hname.str("");
	
	  hname << "G4Track_dYprimedY_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_dYprimedY[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", rbin, rmin, rmax, ypbin, ypmin, ypmax);
	  h_G4Tracker_dYprimedY[t][s][part]->SetYTitle("y_{gen} [mm]");
	  h_G4Tracker_dYprimedY[t][s][part]->SetYTitle("y'_{gen} #equiv P_{y}/P_{z} (p_{#mu}=0) [mrad]");
	  hname.str("");      
      
	  hname << "G4Track_XprimeX_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_XprimeX[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", rbin, rmin, rmax, xpbin, xpmin, xpmax);
	  h_G4Tracker_XprimeX[t][s][part]->SetXTitle("x_{gen} [mm]");
	  h_G4Tracker_XprimeX[t][s][part]->SetYTitle("x'_{gen} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
      
	  hname << "G4Track_YprimeY_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_YprimeY[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", rbin, rmin, rmax, ypbin, ypmin, ypmax);
	  h_G4Tracker_YprimeY[t][s][part]->SetXTitle("y_{gen} [mm]");
	  h_G4Tracker_YprimeY[t][s][part]->SetYTitle("y'_{gen} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");

	  hname << "G4Track_Rhat_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Rhat[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", rbin, rmin, rmax);
	  h_G4Tracker_Rhat[t][s][part]->SetXTitle("x_{gen} #equiv R - R_{m} [mm]");
	  hname.str("");

	  hname << "G4Track_ThetaRhat_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_ThetaRhat[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 200, 0.0, TMath::TwoPi(), rbin, rmin, rmax);
	  h_G4Tracker_ThetaRhat[t][s][part]->SetXTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	  h_G4Tracker_ThetaRhat[t][s][part]->SetYTitle("x_{gen} #equiv R - R_{m} [mm]");
	  hname.str("");

	  hname << "G4Track_Xe_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Xe[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", rbin, rmin, rmax);
	  h_G4Tracker_Xe[t][s][part]->SetXTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	  hname.str("");

	  hname << "G4Track_Vhat_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Vhat[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", rbin, rmin, rmax);
	  h_G4Tracker_Vhat[t][s][part]->SetXTitle("y_{gen} [mm]");
	  hname.str("");

	  hname << "G4Track_R_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_R[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", rbin/2, 0.0, rmax);
	  h_G4Tracker_R[t][s][part]->SetXTitle("R = |#hat{r} + #hat{y}| [mm]");
	  hname.str("");
      
	  hname << "G4Track_Pol_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Pol[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_G4Tracker_Pol[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	  hname.str("");
      
	  hname << "G4Track_PolX_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_PolX[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_G4Tracker_PolX[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	  hname.str("");
      
	  hname << "G4Track_PolY_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_PolY[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	  h_G4Tracker_PolY[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
	  hname.str("");
      
	  hname << "G4Track_Theta_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Theta[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 200, 0.0, TMath::TwoPi());
	  h_G4Tracker_Theta[t][s][part]->SetXTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	  hname.str("");
      
	  hname << "G4Track_ThetaR_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_ThetaR[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 200, 0.0, TMath::TwoPi(), rbin/2, 0.0, rmax);
	  h_G4Tracker_ThetaR[t][s][part]->SetXTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	  h_G4Tracker_ThetaR[t][s][part]->SetYTitle("R = |#hat{r} + #hat{y}| [mm]");
	  hname.str("");

	  hname << "G4Track_Xprime_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Xprime[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", xpbin, xpmin, xpmax);      
	  h_G4Tracker_Xprime[t][s][part]->SetXTitle("x'_{gen} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
    
	  hname << "G4Track_Yprime_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Yprime[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", ypbin, ypmin, ypmax);
	  h_G4Tracker_Yprime[t][s][part]->SetXTitle("y'_{gen} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
    
	  hname << "G4Track_Mom_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Mom[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	  h_G4Tracker_Mom[t][s][part]->SetXTitle("P_{#mu} / P_{m}");
	  hname.str("");
    
	  hname << "G4Track_t0_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_t0[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 300, -100, 200);
	  h_G4Tracker_t0[t][s][part]->SetXTitle("t_{0} [ns]");
	  hname.str("");
    
	  hname << "G4Track_NumCalo_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_NumCalo[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 27, -1.5, 25.5);
	  h_G4Tracker_NumCalo[t][s][part]->SetXTitle("Calo Station");
	  hname.str("");
    
	  hname << "G4Track_Time_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", (int)(maxturns_*0.2)*150, 0.0, maxturns_*0.2);
	  h_G4Tracker_Time[t][s][part]->SetXTitle("t_{Birth/Lost/Decay/Store} - t_{0} [#mus]");
	  hname.str("");
    
	  hname << "G4Track_DriftTime_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_DriftTime[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 101, -0.5, 100.5);
	  h_G4Tracker_DriftTime[t][s][part]->SetXTitle("t_{Meas} - t_{Decay/Store} [#mus]");
	  hname.str("");
    
	  hname << "G4Track_RhatY_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_RhatY[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", rbin, rmin, rmax, rbin, rmin, rmax);
	  h_G4Tracker_RhatY[t][s][part]->SetXTitle("x - x_{gen} [mm]");
	  h_G4Tracker_RhatY[t][s][part]->SetYTitle("y - y_{gen} [mm]");
	  hname.str("");
    
	  hname << "G4Track_XZ_" << truth_part_name << "_" << truth_time_name << statname;
	  h_G4Tracker_XZ[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring, Rmin_Ring, Rmax_Ring, Rbin_Ring, Rmin_Ring, Rmax_Ring);
	  h_G4Tracker_XZ[t][s][part]->SetXTitle("z_{ring} [mm]");
	  h_G4Tracker_XZ[t][s][part]->SetYTitle("x_{ring} [mm]");    
	  hname.str("");
	} // fill
	} // Nstat
      } // loop over times
    } // loop over 7 particles




    if ( fill ) {
      for ( int part = 0; part < Nparticles; part++ ) {
	string truth_part_name = truth_particle_names[part];

	for ( int t = 0; t < Ntruthtimestamps; t++ ) {
	  string timename, xtitle;
	  if ( timestamp_map[t] == 0 ) { xtitle = "Turn"; }
	  if ( timestamp_map[t] == 1 ||
	       timestamp_map[t] == 2 ||
	       timestamp_map[t] == 3 ) { xtitle = "Time [#mus]"; }
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
	    rbin  = Rhatbin_Ring;
	    rmin  = Rhatmin_Ring;
	    rmax  = Rhatmax_Ring;
	    xpbin = XPrimebin_Decay_Ring;
	    xpmin = XPrimemin_Decay_Ring;
	    xpmax = XPrimemax_Decay_Ring;
	    ypbin = YPrimebin_Decay_Ring;
	    ypmin = YPrimemin_Decay_Ring;
	    ypmax = YPrimemax_Decay_Ring;
	  }

	  int hitsperturn = 12;
	  double maxt = maxturns_;
	  if ( timestamp_map[t] == 0 ) { hitsperturn = 1; maxt = maxturns_; }
	  if ( timestamp_map[t] == 1 ) { hitsperturn = (int)(1/0.150)+1; maxt = maxturns_*0.150; }
	  if ( timestamp_map[t] == 2 ) { hitsperturn = (int)(1/0.150)+1; maxt = maxturns_*0.150; }
	  if ( timestamp_map[t] == 3 ) { hitsperturn = (int)(1/0.150)+1; maxt = maxturns_*0.150; }
	  
	  hname << "G4Track_Xe" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Xe_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Rhatbin, rmin, rmax);
	  h_G4Tracker_Xe_vs_Time[t][part]->SetYTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	  h_G4Tracker_Xe_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4Track_Rhat" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Rhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, rbin, rmin, rmax);
	  h_G4Tracker_Rhat_vs_Time[t][part]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_G4Tracker_Rhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4Track_R" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_R_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, rbin/2, 0.0, rmax);
	  h_G4Tracker_R_vs_Time[t][part]->SetYTitle("R = |#hat{r} + #hat{y}| [mm]");
	  h_G4Tracker_R_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4Track_Mom" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Mom_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Mombin, Mommin, Mommax);
	  h_G4Tracker_Mom_vs_Time[t][part]->SetYTitle("P_{#mu} / P_{m}");
	  h_G4Tracker_Mom_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_E" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_E_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Mombin/100, 0.0, Pmagic());
	  h_G4Tracker_E_vs_Time[t][part]->SetYTitle("Energy [GeV]");
	  h_G4Tracker_E_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_NumCalo" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_NumCalo_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, 24, -0.5, 23.5);
	  h_G4Tracker_NumCalo_vs_Time[t][part]->SetYTitle("Calo Station");
	  h_G4Tracker_NumCalo_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_TimeOffset" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_TimeOffset_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", 200, 0.0, 0.200);
	  h_G4Tracker_TimeOffset_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  int NstatHist = 2;
	  if ( truth_part_name.find("DecayElectron") != string::npos ) { NstatHist = 26; }
	  for ( int s = 0; s < NstatHist; s++ ) {
	    string statname = "";
	    if ( s == 0 ) { statname = "AllStations"; }
	    else if ( s == 1 ) { statname = "AllStationsNoTimeOffset"; }
	    else {
	      stringstream statname_ss;
	      statname_ss << "CaloStation" << s-2;
	      statname = statname_ss.str();
	    }
	    
	    hname << "G4Track_Num" << statname << "_" << truth_part_name << "_vs_" << timename;
	    h_G4Tracker_Num_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn*1000, 0.0, maxt);
	    h_G4Tracker_Num_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	    hname.str("");
	    
	    if ( keep_edm_plots ) {
	      hname << "G4Track_NgtEth" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_NgtEth_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn*1000, 0.0, maxt);
	      h_G4Tracker_NgtEth_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      hname.str("");
	      
	      hname << "G4Track_NwghtE" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_NwghtE_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn*1000, 0.0, maxt);
	      h_G4Tracker_NwghtE_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      hname.str("");
	      
	      hname << "G4Track_Nup" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_Nup_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt);
	      h_G4Tracker_Nup_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      hname.str("");
	      
	      hname << "G4Track_Ndown" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_Ndown_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt);
	      h_G4Tracker_Ndown_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      hname.str("");
	      
	      hname << "G4Track_Nud" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_Nud_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt);
	      h_G4Tracker_Nud_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      hname.str("");
	    }
	  }
	  
	  hname << "G4Track_Prhat" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Prhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, xpbin, xpmin, xpmax);
	  h_G4Tracker_Prhat_vs_Time[t][part]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  h_G4Tracker_Prhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Pvhat" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Pvhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, ypbin, ypmin, ypmax);
	  h_G4Tracker_Pvhat_vs_Time[t][part]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  h_G4Tracker_Pvhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Y" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Y_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, rbin, rmin, rmax);
	  h_G4Tracker_Y_vs_Time[t][part]->SetYTitle("y_{ring} [mm]");
	  h_G4Tracker_Y_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Pol" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Pol_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Polbin, Polmin, Polmax);
	  h_G4Tracker_Pol_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{z}");
	  h_G4Tracker_Pol_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4Track_PolX" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_PolX_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, Polbin, Polmin, Polmax);
	  h_G4Tracker_PolX_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{r}");
	  h_G4Tracker_PolX_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4Track_PolY" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_PolY_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, PolYbin, PolYmin, PolYmax);
	  h_G4Tracker_PolY_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	  h_G4Tracker_PolY_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4Track_Theta" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Theta_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt, 200, 0.0, TMath::TwoPi());
	  h_G4Tracker_Theta_vs_Time[t][part]->SetYTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	  h_G4Tracker_Theta_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4Track_Yprime" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Yprime_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt);
	  h_G4Tracker_Yprime_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Vhat" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Vhat_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxt*hitsperturn, 0.0, maxt);
	  h_G4Tracker_Vhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
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
	  //cout << hname.str() << endl;
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
  if ( hist < 0 ) { return; }	

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

double gm2ringsim::ringTrackerAnalyzer::TimeOffset(int timestamp, int counter)
{
  double t_mod;

  bool is_time = false;
  if ( timestamp == 1 || timestamp == 2 ) { is_time = true; }
  if ( timestamp == 10 || timestamp == 20 ) { is_time = true; }

  double t_scale = 1.0;
  if ( is_time ) { t_scale = 0.14915; }

  if ( timestamp == 10 || timestamp == 20 ) { 
    t_mod = t_scale*((float)counter/24);
  }
  else {
    t_mod = t_scale*((float)counter/Nstat);
  }

  return( t_mod );
}

bool gm2ringsim::ringTrackerAnalyzer::FillByTimestamp(int counter, int timestamp)
{
  int hist = timestamp;

  if ( timestamp_map[hist] == 0 || timestamp_map[hist] == 1 ) { return( true ); }

  if ( timestamp_map[hist] == 2 ) {
    if ( counter != vring1p ) { return( false ); }
    return( true );
  }

  return( true );
}


void gm2ringsim::ringTrackerAnalyzer::FillTimeRingTracker(int hist, double xe, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly)
{

  if ( debug_ ) { cout << "FillTimeRingTracker(" << hist << ")" << endl; }
  if ( SaveVRingHits_ == false ) { return; }
  if ( hist < 0 ) { return; }

  if ( hist < Nringtrackers ) {
    h_RingTracker_Snapshot_Xe[hist]->Fill(xe);
    h_RingTracker_Snapshot_Rhat[hist]->Fill(rhat);
    h_RingTracker_Snapshot_Vhat[hist]->Fill(vhat);
    h_RingTracker_Snapshot_XprimeX[hist]->Fill(rhat, 1000*prhat);
    h_RingTracker_Snapshot_YprimeY[hist]->Fill(vhat, 1000*pvhat);
    h_RingTracker_Snapshot_Mom[hist]->Fill(mom/Pmagic());
    h_RingTracker_Snapshot_RhatY[hist]->Fill(rhat, vhat);
    h_RingTracker_Snapshot_Pol[hist]->Fill(polz);
    h_RingTracker_Snapshot_PolX[hist]->Fill(polx);
    h_RingTracker_Snapshot_PolY[hist]->Fill(poly);
  }  
  if ( debug_ ) { cout << "FillTimeRingTracker(" << hist << ") - Done" << endl; }
}
  
void gm2ringsim::ringTrackerAnalyzer::FillTruthTracker(int hist, int particle, int counter, double xe_truth, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double theta, double t0, double time, double tDecay, double polz, double polx, double poly, double Polarization)
{
  if ( debug_ ) { cout << "FillTruthTracker(" << hist << ", " << particle << ", " << counter << ")" << endl; }
  if ( SaveTruthHits_ == false ) { return; }
  
  int calonum = 0;
  if ( particle == truth_particle_nums["DecayElectron"] ) {
    calonum = counter;
    counter = 0;
  }

  //cout << "Filling particle: " << truth_particle_names[particle] << endl;
  //cout << r_truth << "\t" << pdgid << "\t" << status << "\t" << particle << "\tDecayMuon=" << truth_particle_nums["DecayMuon"] << "\tBirthMuon=" << truth_particle_nums["BirthMuon"] << "\tStoredMuon=" << truth_particle_nums["StoredMuon"] << "\tBirthElectron=" << truth_particle_nums["BirthElectron"] << endl;
  
  if ( particle == -1 ) { return; }
  
  FillTruthTracker(hist, particle, counter, calonum, xe_truth, r_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, polz, polx, poly, Polarization);
  
  if ( p > 1.8*GeV ) {
    if ( particle == truth_particle_nums["BirthElectron"] ) {
      int tmp_particle = truth_particle_nums["BirthElectronEgtEth"];
      FillTruthTracker(hist, tmp_particle, counter, calonum, xe_truth, r_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, polz, polx, poly, Polarization);
    }
    if ( particle == truth_particle_nums["DecayElectron"] ) { 
      //G4cout << time - tDecay << G4endl;
      int tmp_particle = truth_particle_nums["DecayElectronEgtEth"];
      FillTruthTracker(hist, tmp_particle, counter, calonum, xe_truth, r_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, polz, polx, poly, Polarization);
    }

    if ( p > 2.6*GeV ) {
      if ( particle == truth_particle_nums["DecayElectron"] ) { 
	int tmp_particle = truth_particle_nums["DecayElectronEgtHghEth"];
	FillTruthTracker(hist, tmp_particle, counter, calonum, xe_truth, r_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, polz, polx, poly, Polarization);
      }
    }
  }
  if ( debug_ ) { cout << "FillTruthTracker(" << hist << ") - Done" << endl; }
}

void gm2ringsim::ringTrackerAnalyzer::FillTruthTracker(int hist, int particle, int counter, int calonum, double xe_truth, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double theta, double t0, double time, double tDecay, double polz, double polx, double poly, double Polarization)
{
  if ( debug_ ) { cout << "FillTruthTracker(" << hist << " , " << particle << ", " << counter << ")" << endl; }
  if ( hist >= Ntruthtimes ) { return; }
  if ( particle >= Nparticles ) { return; }
  if ( hist < 0 || particle < 0 ) { return; }


  //if ( particle == truth_particle_nums["StoredMuon"] ) {
//     cout << "==> (HIST=" << hist << ") , Found stored muon: " << "rhat=" << r_truth << "\ty=" << y_truth << "\t" << "p=" << p << endl;
//   }

  h_G4Tracker_Xe[hist][counter][particle]->Fill(xe_truth);
  h_G4Tracker_Rhat[hist][counter][particle]->Fill(r_truth);
  h_G4Tracker_Vhat[hist][counter][particle]->Fill(y_truth);
  h_G4Tracker_R[hist][counter][particle]->Fill(TMath::Sqrt(y_truth*y_truth + r_truth*r_truth));
  h_G4Tracker_Xprime[hist][counter][particle]->Fill(1000*rprime_truth);
  h_G4Tracker_Yprime[hist][counter][particle]->Fill(1000*yprime_truth);
  h_G4Tracker_XprimeX[hist][counter][particle]->Fill(r_truth, 1000*rprime_truth);
  h_G4Tracker_YprimeY[hist][counter][particle]->Fill(y_truth, 1000*yprime_truth);
  h_G4Tracker_Mom[hist][counter][particle]->Fill(p/Pmagic());
  h_G4Tracker_RhatY[hist][counter][particle]->Fill(r_truth, y_truth);
  h_G4Tracker_XZ[hist][counter][particle]->Fill(z, x);
  h_G4Tracker_t0[hist][counter][particle]->Fill(t0);
  h_G4Tracker_NumCalo[hist][counter][particle]->Fill(calonum);
  h_G4Tracker_Time[hist][counter][particle]->Fill((time-t0)/1000);
  h_G4Tracker_DriftTime[hist][counter][particle]->Fill(time-tDecay);
  h_G4Tracker_Pol[hist][counter][particle]->Fill(polz);
  h_G4Tracker_PolX[hist][counter][particle]->Fill(polx);
  h_G4Tracker_PolY[hist][counter][particle]->Fill(poly);
  h_G4Tracker_Theta[hist][counter][particle]->Fill(theta);
  h_G4Tracker_ThetaR[hist][counter][particle]->Fill(theta, TMath::Sqrt(y_truth*y_truth + r_truth*r_truth));
  h_G4Tracker_ThetaRhat[hist][counter][particle]->Fill(theta, r_truth);
  h_G4Tracker_Polarization[hist][counter][particle]->Fill(Polarization);
}
  
void gm2ringsim::ringTrackerAnalyzer::FillRingTracker(int hist, int type, double theta, double xe, double xe_rms, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly, double zhat)
{
  if ( debug_ ) { cout << "FillRingTracker(" << hist << ", " << type << ")" << endl; }
  if ( SaveVRingHits_ == false ) { return; }
  if ( hist >= Nringtrackertimes ) { return; }
  if ( type >= Ntimevals ) { return; }
  if ( hist < 0 || type < 0 ) { return; }
    // hist [] ; 0 <=> Begin (t0) , 1 <=> End (t_{inf})
    // type [] ; 0 <=> GeneratedDist , 1 <=> RemainingDist

  h_RingTracker_EqRad[hist][type]->Fill(xe);
  h_RingTracker_EqRadMom[hist][type]->Fill(xe, mom/Pmagic());
  h_RingTracker_EqRadRMS[hist][type]->Fill(xe_rms);
  h_RingTracker_EqRadRMSMom[hist][type]->Fill(xe_rms, mom/Pmagic());
  h_RingTracker_Rhat[hist][type]->Fill(rhat);
  h_RingTracker_Vhat[hist][type]->Fill(vhat);
  h_RingTracker_XprimeX[hist][type]->Fill(rhat, 1000*prhat);
  h_RingTracker_YprimeY[hist][type]->Fill(vhat, 1000*pvhat);
  h_RingTracker_Xprime[hist][type]->Fill(1000*prhat);
  h_RingTracker_Yprime[hist][type]->Fill(1000*pvhat);
  h_RingTracker_Mom[hist][type]->Fill(mom/Pmagic());
  h_RingTracker_Theta[hist][type]->Fill(theta);
  h_RingTracker_RhatY[hist][type]->Fill(rhat, vhat);
  h_RingTracker_XprimeYprime[hist][type]->Fill(1000*prhat, 1000*pvhat);
  h_RingTracker_Pol[hist][type]->Fill(polz);
  h_RingTracker_PolX[hist][type]->Fill(polx);
  h_RingTracker_PolY[hist][type]->Fill(poly);
  h_RingTracker_Zhat[hist][type]->Fill(zhat);

  if ( debug_ ) { cout << "FillRingTracker(" << hist << ", " << type << ") - Done" << endl; }
}
 
void gm2ringsim::ringTrackerAnalyzer::FillTruthTurnTime(double dT, int counter, int status, int pdgid, double time, double xe, double rhat, double y, double prhat, double pvhat, double mom, double theta, double polz, double polx, double poly, double e)
{  
  bool debug = debug_;
  if ( debug ) { cout << "FillTruthTurnTime(" << pdgid << " , " << status << ")" << endl; }
  
  int particle = FindParticle(pdgid, status);
  if ( particle == -1 ) { return; }

  double xval;
  for ( int hist = 0; hist < Ntruthtimestamps; hist++ ) {
    xval = dT;
    if ( timestamp_map[hist] == 1 ||
	 timestamp_map[hist] == 2 ||
	 timestamp_map[hist] == 3 ) { xval = time/1000;}

    if ( FillByTimestamp(hist, counter) ) {
      FillTruthTurnTime(xval, hist, particle, xe, rhat, y, prhat, pvhat, mom, theta, polz, polx, poly);
      if ( e > 1.8*GeV ) {
	if ( particle == truth_particle_nums["BirthElectron"] ) {
	  int tmp_particle = truth_particle_nums["BirthElectronEgtEth"];
	  FillTruthTurnTime(xval, hist, tmp_particle, xe, rhat, y, prhat, pvhat, mom, theta, polz, polx, poly);
	}
	if ( particle == truth_particle_nums["DecayElectron"] ) { 
	  int tmp_particle = truth_particle_nums["DecayElectronEgtEth"];
	  FillTruthTurnTime(xval, hist, tmp_particle, xe, rhat, y, prhat, pvhat, mom, theta, polz, polx, poly);
	}
	if ( e > 2.6*GeV ) {
	  if ( particle == truth_particle_nums["DecayElectron"] ) { 
	    int tmp_particle = truth_particle_nums["DecayElectronEgtHghEth"];
	    FillTruthTurnTime(xval, hist, tmp_particle, xe, rhat, y, prhat, pvhat, mom, theta, polz, polx, poly);
	  }
	}
      }
    }

    FillNumTruthTurnTime(xval, hist, particle, counter, y, e);
    if ( e > 1.8*GeV ) {
      if ( particle == truth_particle_nums["BirthElectron"] ) {
	int tmp_particle = truth_particle_nums["BirthElectronEgtEth"];
	FillNumTruthTurnTime(xval, hist, tmp_particle, counter, y, e);
      }
      if ( particle == truth_particle_nums["DecayElectron"] ) { 
	int tmp_particle = truth_particle_nums["DecayElectronEgtEth"];
	FillNumTruthTurnTime(xval, hist, tmp_particle, counter, y, e);
      }
      if ( e > 2.6*GeV ) {
	if ( particle == truth_particle_nums["DecayElectron"] ) { 
	  int tmp_particle = truth_particle_nums["DecayElectronEgtHghEth"];
	  FillNumTruthTurnTime(xval, hist, tmp_particle, counter, y, e);
	}
      }
    }
  }
}

void gm2ringsim::ringTrackerAnalyzer::FillTruthTurnTime(double xval, int hist, int particle, double xe, double rhat, double y, double prhat, double pvhat, double mom, double theta, double polz, double polx, double poly)
{
  bool debug = debug_;
  if ( debug ) { cout << "FillTruthTurnTime(" << hist << " , " << particle << ")" << endl; }
  if ( hist < 0 ) { return; }
  if ( particle < 0 ) { return; }

  h_G4Tracker_Xe_vs_Time[hist][particle]->Fill(xval, xe);
  h_G4Tracker_R_vs_Time[hist][particle]->Fill(xval, TMath::Sqrt(rhat*rhat + y*y));
  h_G4Tracker_Rhat_vs_Time[hist][particle]->Fill(xval, rhat);
  h_G4Tracker_Y_vs_Time[hist][particle]->Fill(xval, y);
  h_G4Tracker_Pol_vs_Time[hist][particle]->Fill(xval, polz);
  h_G4Tracker_PolX_vs_Time[hist][particle]->Fill(xval, polx);
  h_G4Tracker_PolY_vs_Time[hist][particle]->Fill(xval, poly);
  h_G4Tracker_Theta_vs_Time[hist][particle]->Fill(xval, theta);
  h_G4Tracker_Prhat_vs_Time[hist][particle]->Fill(xval, 1000*prhat);
  h_G4Tracker_Pvhat_vs_Time[hist][particle]->Fill(xval, 1000*pvhat);
  h_G4Tracker_Mom_vs_Time[hist][particle]->Fill(xval, mom/Pmagic());
  h_G4Tracker_E_vs_Time[hist][particle]->Fill(xval, mom);

  h_G4Tracker_Yprime_vs_Time[hist][particle]->Fill(xval, 1000*pvhat);
  h_G4Tracker_Vhat_vs_Time[hist][particle]->Fill(xval, y);
}

void gm2ringsim::ringTrackerAnalyzer::FillNumTruthTurnTime(double xval, int hist, int particle, int counter, double y, double e)
{
  bool debug = debug_;
  if ( debug ) { cout << "FillNumTruthTurnTime(" << hist << " , " << particle << ")" << endl; }
  if ( hist < 0 ) { return; }
  if ( particle < 0 ) { return; }

  double tmod = 0.0;
  if ( particle == truth_particle_nums["DecayElectron"] || particle == truth_particle_nums["DecayElectronEgtEth"] || particle == truth_particle_nums["DecayElectronEgtHghEth"] ) {
    tmod = TimeOffset(timestamp_map[hist]*10, counter);
  }
  else {
    tmod = TimeOffset(timestamp_map[hist], counter);
  }
  double xvals[2];
  xvals[0] = xval - tmod;
  xvals[1] = xval;
  
  h_G4Tracker_TimeOffset_vs_Time[hist][particle]->Fill(xvals[1] - xvals[0]);
  if ( h_G4Tracker_NumCalo_vs_Time[hist][particle] ) {
    h_G4Tracker_NumCalo_vs_Time[hist][particle]->Fill(xval, counter+2);
  }
  for ( int s = 0; s < 2; s++ ) {
    if ( h_G4Tracker_Num_vs_Time[hist][s][particle] ) {
      h_G4Tracker_Num_vs_Time[hist][s][particle]->Fill(xvals[s]);
    }

    if ( keep_edm_plots ) {
      if ( y > 0 ) { 
	h_G4Tracker_Nup_vs_Time[hist][s][particle]->Fill(xvals[s], 1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(xvals[s], 1);
      }
      else if ( y < 0 ) { 
	h_G4Tracker_Ndown_vs_Time[hist][s][particle]->Fill(xvals[s], -1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(xvals[s], -1);
      }
      
      if ( e > 1.8*GeV ) {
	h_G4Tracker_NgtEth_vs_Time[hist][s][particle]->Fill(xvals[s]);
      }
      h_G4Tracker_NwghtE_vs_Time[hist][s][particle]->Fill(xvals[s], e/Pmagic());
      h_G4Tracker_Num_vs_Time[hist][s][particle]->Fill(xvals[s]);
    }
  }

  if ( particle == truth_particle_nums["DecayElectron"] || particle == truth_particle_nums["DecayElectronEgtEth"] || particle == truth_particle_nums["DecayElectronEgtHghEth"] ) {
    int s = counter + 2;
    //cout << "Counter = " << s << endl;
    //double tmod = TimeOffset(3, counter);
    double tmpxval = xval;// - tmod;
    if ( h_G4Tracker_Num_vs_Time[hist][s][particle] ) {
      h_G4Tracker_Num_vs_Time[hist][s][particle]->Fill(tmpxval);
    }
    
    if ( keep_edm_plots ) {
      if ( y > 0 ) { 
	h_G4Tracker_Nup_vs_Time[hist][s][particle]->Fill(tmpxval, 1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(tmpxval, 1);
      }
      else if ( y < 0 ) { 
	h_G4Tracker_Ndown_vs_Time[hist][s][particle]->Fill(tmpxval, -1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(tmpxval, -1);
      }
      
      if ( e > 1.8*GeV ) {
	h_G4Tracker_NgtEth_vs_Time[hist][s][particle]->Fill(tmpxval);
      }
      h_G4Tracker_NwghtE_vs_Time[hist][s][particle]->Fill(tmpxval, e/Pmagic());
      h_G4Tracker_Num_vs_Time[hist][s][particle]->Fill(tmpxval);
    }
  }
}

void gm2ringsim::ringTrackerAnalyzer::FillTurnTime(double dT, double time, int counter, int trackID, double zhat, double xe, double xe_rms, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e)
{
  bool debug = debug_;

  if ( debug ) { cout << "FillTurnTime(" << trackID << " , " << counter << ")" << endl; }

  double xval;

  if ( trackID != 1 ) { return; }
  
  for ( int hist = 0; hist < Nringtimestamps; hist++ ) {
//     if ( SaveVRing1PlaneHits_ == true && SaveVRingHits_ == false ) {
//       if ( hist == 0 ) { continue; }
//     }
//     if ( SaveVRing1PlaneHits_ == false ) {
//       if ( hist == 1 ) { continue; }
//     }
    
    xval = dT;
    if ( timestamp_map[hist] == 1 ||
	 timestamp_map[hist] == 2 ) { xval = time/1000;}

    if ( !FillByTimestamp(hist, counter) ) { continue; }
    
    h_RingTracker_EqRad_vs_Time[hist]->Fill(xval, xe);
    h_RingTracker_EqRadRMS_vs_Time[hist]->Fill(xval, xe_rms);
    h_RingTracker_Rhat_vs_Time[hist]->Fill(xval, rhat);
    h_RingTracker_Y_vs_Time[hist]->Fill(xval, y);
    h_RingTracker_Pol_vs_Time[hist]->Fill(xval, polz);
    h_RingTracker_PolX_vs_Time[hist]->Fill(xval, polx);
    h_RingTracker_PolY_vs_Time[hist]->Fill(xval, poly);
    h_RingTracker_Prhat_vs_Time[hist]->Fill(xval, 1000*prhat);
    h_RingTracker_Pvhat_vs_Time[hist]->Fill(xval, 1000*pvhat);
    h_RingTracker_Mom_vs_Time[hist]->Fill(xval, mom/Pmagic());
    h_RingTracker_Zhat_vs_Time[hist]->Fill(xval, zhat);

    if ( e > 1.8*GeV ) {
      h_RingTracker_NgtEth_vs_Time[hist]->Fill(xval);
    }
    h_RingTracker_NwghtE_vs_Time[hist]->Fill(xval, e/Pmagic());
    h_RingTracker_Num_vs_Time[hist]->Fill(xval);
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

void gm2ringsim::ringTrackerAnalyzer::SetInitial(double *t0, double *t1, double *theta1, double *prhat_init, double *pvhat_init, double *xe_init, double *xe_rms_init, double *rhat_init, double *vhat_init, double *pmom_init, double *pol_init, double time, double theta, double prhat, double pvhat, double xe, double xe_rms, double rhat, double vhat, double mom, double pol)
{
  *t0 = time; 
  *t1 = time;
  *theta1 = theta;
  *prhat_init = prhat;
  *pvhat_init = pvhat;
  *xe_init  = xe;
  *xe_rms_init  = xe_rms;
  *rhat_init  = rhat;
  *vhat_init  = vhat;
  *pmom_init  = mom;
  *pol_init = pol;
}

int gm2ringsim::ringTrackerAnalyzer::FindParticle(int pdgid, int status)
{
  int particle = -1;
  if ( pdgid == kMuonID || pdgid == kAntiMuonID ) {
    if ( status == gm2ringsim::kBirth ) { particle = truth_particle_nums["BirthMuon"]; }
    if ( status == gm2ringsim::kDecay ) { particle = truth_particle_nums["DecayMuon"]; }
    if ( status == gm2ringsim::kLost )  { particle = truth_particle_nums["LostMuon"]; }
    if ( status == gm2ringsim::kStore ) { particle = truth_particle_nums["StoredMuon"]; }
  }
  if ( pdgid == kElectronID || pdgid == kPositronID ) {
    if ( status == gm2ringsim::kBirth ) { particle = truth_particle_nums["BirthElectron"]; }
    if ( status == gm2ringsim::kDecay ) { particle = truth_particle_nums["DecayElectron"]; }
  }
  return( particle );
}

string gm2ringsim::ringTrackerAnalyzer::ParticleStatusName(int status, int pdgid)
{
  string status_name = "UNKNOWN";
  string id_name = "No Name";
  if ( pdgid == kMuonID ) { id_name = "mu-"; }
  if ( pdgid == kAntiMuonID ) { id_name = "mu+"; }
  if ( pdgid == kElectronID ) { id_name = "e-"; }
  if ( pdgid == kPositronID ) { id_name = "e+"; }

  if ( status == gm2ringsim::kBirth ) { id_name = "Birth"; }
  if ( status == gm2ringsim::kLost ) { id_name = "Lost"; }
  if ( status == gm2ringsim::kDecay ) { id_name = "Decay"; }
  if ( status == gm2ringsim::kStore ) { id_name = "Stored"; }

  string retval = id_name + "__" + status_name;

  return( retval );
}

double gm2ringsim::ringTrackerAnalyzer::Pmagic()
{
  //  NSF: The easiest way to calculate....
  static double const pMagic_ = 105.6583715 / TMath::Sqrt( 11659208.9e-10 );
  //  cout << "pMagic = " << pMagic_/GeV << " GeV/c" << endl;
  return pMagic_;
}

int gm2ringsim::ringTrackerAnalyzer::Rbin(double rhat)
{
  int bin = -1;
  double Nbins = 10;
  double Width = 90.0;
  double rhatstep = Width/(Nbins-1);
  int c = 0;
  for ( double rhatmin = -Width/2; rhatmin <= Width/2; rhatmin += rhatstep ) {    
    if ( rhat >= rhatmin && rhat < rhatmin + rhatstep ) {
      bin = c;
      break;
    }
    c++;
  }

  return( bin );
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
  if ( name.find("turnCounterPV") != string::npos ) { return( true ); }
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

  std::string ttLVName = artg4::addNumberToName("VirtualRingStationPV", vring1p);
  if ( name == ttLVName ) { *num = 11; return( true ); }

  for(int arc=0; arc!=12; ++arc){
    if ( arc == 11 ) { continue; }
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
  art::Handle< CaloArtRecordCollection > calohitDataHandle;
  
  // Fill the handle (note the use of the member data)
  if ( debug_ ) { std::cout<<"Ngen: " << Ngen << " \t" << "Event ID: "<<e.id()<<std::endl; }
  
  //e.getByLabel(hitModuleLabel_, instanceName_, hitDataHandle);
  e.getByLabel(ringtrackerhitModuleLabel_, ringtrackerinstanceName_, ringtrackerhitDataHandle);
  e.getByLabel(truthhitModuleLabel_, truthinstanceName_, truthhitDataHandle);
  e.getByLabel(truthfinishedhitModuleLabel_, truthfinishedinstanceName_, truthfinishedhitDataHandle);
  e.getByLabel(inflectorhitModuleLabel_, inflectorinstanceName_, inflectorhitDataHandle);
  e.getByLabel(ringhitModuleLabel_, ringinstanceName_, ringhitDataHandle);
  e.getByLabel(calohitModuleLabel_, caloinstanceName_, calohitDataHandle);
  
  // Resolve the handle
  //StrawArtRecordCollection const & hits = *hitDataHandle;
  VirtualRingStationArtRecordCollection const & ringtrackerhits = *ringtrackerhitDataHandle;
  TrackingActionArtRecordCollection const & truthhits = *truthhitDataHandle;
  TrackingActionArtRecordCollection const & truthfinishedhits = *truthfinishedhitDataHandle;
  InflectorArtRecordCollection const & inflectorhits = *inflectorhitDataHandle;
  RingArtRecordCollection const & ringhits = *ringhitDataHandle;
  CaloArtRecordCollection const & calohits = *calohitDataHandle;
  // Let's use the nice C++11 vector iteration
  //int i = 0;


  unsigned int vringhits =  ringtrackerhits.size();
  if ( debug_ ) {
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << pvs.size() << " entries in the PVS";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << ringhits.size() << " ring hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << calohits.size() << " calo hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << inflectorhits.size() << " inflector hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << truthhits.size() << " truth hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << truthfinishedhits.size() << " truth hits in this event.";
    mf::LogInfo("ringTrackerAnalyzer") << "There are " << ringtrackerhits.size() << " virtual ring station hits in this event.";
  }

  h_RingTracker_Nhits->Fill(vringhits);
  if ( vringhits > 0.98 * maxturns_ ) { 
    //cout << "Ring Hits[" << vringhits << "] more than allowed [" << 0.98*maxturns_ << "]." << endl;
    //return;
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


  






  //-------------------
  // G4 Generated Stuff
  //-------------------
  
  double prhat_init = -9999.9;
  double pvhat_init = -9999.9;
  double rhat_init = -9999.9;
  double vhat_init = -9999.9;
  double pmom_init = -9999.9;
  double pol_init = -9999.9;
  double xe_init = -9999.9;
  double theta_init = -9999.9;
  double xe_rms_init = -9999.9;
  //double t0_init = -9999.9;
  int radbin = -1;
  TrackingActionArtRecordCollection thits;
  
  for ( int i = 0; i < 2; i++ ) {
    if ( i == 0 ) {
      thits = *truthhitDataHandle;
    }
    else {
      thits = *truthfinishedhitDataHandle;
    }

    for ( auto htruthdata : thits) {  
      int trackID = htruthdata.trackID;
      if ( trackID != 1 ) { continue; }
      int status = htruthdata.status;
      if ( status != gm2ringsim::kBirth ) { continue; }
      double rhat = htruthdata.rhat;
      double vhat = htruthdata.vhat;
      double time = htruthdata.time;
      double p = htruthdata.p;
      double prhat = htruthdata.prhat;
      double pvhat = htruthdata.pvhat;
      double polx_pre = htruthdata.polx;
      double poly_pre = htruthdata.poly;
      double polz_pre = htruthdata.polz;
      double xe_truth = ComputeXe(p, 0.137);
      double theta = htruthdata.theta;

      
      TVector3 pol_pre(polx_pre, poly_pre, polz_pre);
      double polx = pol_pre.Dot(xAxis);      
      double poly = pol_pre.Dot(yAxis);
      double polz = pol_pre.Dot(zAxis);    
      double pol_azimuth_truth, pol_radial_truth, pol_vertical_truth;
      ComputePolarization(&pol_azimuth_truth, &pol_radial_truth, &pol_vertical_truth, polx, poly, polz);
            
      prhat_init = prhat;
      pvhat_init = pvhat;
      rhat_init  = rhat;
      vhat_init  = vhat;
      pmom_init  = p;	
      pol_init   = pol_azimuth_truth; 
      xe_init    = xe_truth;
      theta_init = theta;
      xe_rms_init    = 0.0;
      //t0_init    = time;
      radbin     = Rbin(rhat_init);
//       double x = (rhat + 7112) * TMath::Cos(theta);
//       double z = (rhat + 7112) * TMath::Sin(theta);
//       double y = vhat;

//       cout << theta_init << "\t" << x << "\t" << y << "\t" << z << endl;
    }
  }










  
  //-------------------------
  // Ring Tracker Information
  //-------------------------
  double t0 = 0.0;
  double t1 = 0.0;
  double theta1 = -1.0;
  
  int pmytrack_vol = -1;
  int mytrack_vol = 0;
  int global_turn = 0;
  double degree_at_r0 = -1.0;
  
  int station = 0;
  double mom_init = -1.0;
  
  double previous_rhat = 9999.9;
  double previous_prhat = 999.9;
  double previous_degree = 999.9;
  
  double degree_at_quad = -9999.9;
  double degree_r_at_r0 = -9999.9;
  double kick_at_r0 = -9999.9;

  double tauc = 0.0;

  double sum_EqRad = 0.0;
  double sum_EqRadRMS = 0.0;
  int N_EqRad = 0;
  
  if ( SaveVRingHits_ || SaveVRing1PlaneHits_ ) {
    for ( auto hdata : ringtrackerhits) {
      int track_ID   = hdata.trackID;
      int track_volumeUID = hdata.volumeUID; 
      string track_volname = pvs.stringGivenID(track_volumeUID);
      IsVirtualRingStation(track_volname, &mytrack_vol);
      NewFixTurnCounter(&global_turn, mytrack_vol, pmytrack_vol);
      pmytrack_vol = mytrack_vol;
      if ( SaveVRing1PlaneHits_ == true && SaveVRingHits_ == false ) {
	if ( mytrack_vol != vring1p ) { continue; }
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
      double x = (rhat+7112) * cos(theta);
      double z = (rhat+7112) * sin(theta);    



      //---------------
      // Keep running sum of equilibrium radius
      //---------------
      sum_EqRad += rhat;
      sum_EqRadRMS += rhat*rhat;
      N_EqRad++;
      double xe = sum_EqRad / N_EqRad;
      double xe_rms = TMath::Sqrt(sum_EqRadRMS/N_EqRad);



      cout << global_turn << "\t" << mytrack_vol << "\t" << theta << "\t" << x << "\t" << z << endl;
      if ( global_turn == 0 && mytrack_vol == vring1p ) { tauc = time; }
      if ( global_turn == 1 && mytrack_vol == vring1p ) { 
	tauc = time - tauc; 
	//cout << "Rhat,Bin,Tau_{c} = " << rhat << "\t" << radbin << "\t" << tauc << endl;
	if ( TMath::Abs(mom/Pmagic()-1) < 0.001 || 1 ) {
	  h_RingTracker_OmegaC_Radbin->Fill(tauc, rhat_init);
	  h_RingTracker_OmegaC_Mombin->Fill(tauc, mom_init/Pmagic());
	  h_RingTracker_OmegaC[radbin]->Fill(tauc);
	}
	tauc = time;
      }


      double pol_azimuth, pol_radial, pol_vertical;
      ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, polx, poly, polz, theta);


      if ( track_ID == 1 ) {      
	if ( t0 <= 0.0 ) { 
// 	  SetInitial(&t0, &t1, &theta1, &prhat_init, &pvhat_init, &xe_init, &rhat_init, &vhat_init, &pmom_init, &pol_init, 
// 		     time, theta, prhat, pvhat, xe, rhat, vhat, mom, pol_azimuth);

	  SetInitial(&t0, &t1, &theta1, &prhat_init, &pvhat_init, &xe_init, &xe_rms_init, &rhat_init, &vhat_init, &pmom_init, &pol_init, 
		     time, theta, prhat_init, pvhat_init, xe_init, xe_rms_init, rhat_init, vhat_init, pmom_init, pol_init);
	}
      }
      
      t1 = time;
      theta1 = theta;

      double zhat = ComputeZhat(time - t0, global_turn);
    
      if ( debug_ ) { cout << "Made it past the track/turn definitions for turn/track: " << global_turn << " , " << mytrack_vol << "." << endl; }
    
      //if ( global_turn < 5 ) { cout << "init: " << track_volname << "\t" << mytrack_vol << endl; }

    
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

	  
	  if ( global_turn == 0 && mytrack_vol == vring1p ) {	  
	    FillRingTracker(kBegin, kRemainingDist, theta, xe, xe_rms, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical, zhat);  
	    FillRingTracker(kBegin, kGeneratedDist, theta_init, xe_init, xe_rms, rhat_init, vhat_init, prhat_init, pvhat_init, pmom_init, pol_azimuth, pol_radial, pol_vertical, zhat);
	  }
      
	  if ( ( global_turn >= maxturnsreal_ - 150 - 1 && global_turn > 100 ) ||
	       ( maxturnsreal_ < 125 && global_turn > 50 ) ) {
	    Npass_finalavg++;
	    FillRingTracker(kFinal, kRemainingDist, theta, xe, xe_rms, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical, zhat);
	    if ( SaveVRingHits_ == true ) {
	      h_RingTracker_dNdXe->Fill(xe);
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
      
	  //cout << global_turn << "\t" << maxturnsreal_ << "\t" << pass_final << endl;
	  if ( global_turn >= maxturnsreal_ - 1 ) {
	    if ( pass_final == false ) {
	      pass_final = true;
	      Npass_final++;
	      FillRingTracker(kFinal, kGeneratedDist, theta_init, xe_init, xe_rms_init, rhat_init, vhat_init, prhat_init, pvhat_init, mom_init, pol_azimuth, pol_radial, pol_vertical, zhat);
	    }
	  }
	}
    
	if ( pass_final ) {
	  if ( fill ) { FillKickDegree(1, degree_at_r0, kick_at_r0, mom); }
	}
      }

    
      if ( debug_ ) { cout << "Made it past the stored fills." << endl; }
    


      if ( track_ID == 1 && mytrack_vol == vring1p && global_turn == 0 ) { Nstart_ring++; }
      if ( track_ID == 1 && mytrack_vol == vring1p && global_turn == 1 ) { Nstored1_ring++; }
      if ( track_ID == 1 && mytrack_vol == vring1p && global_turn == 10 ) { Nstored10_ring++; }
      if ( track_ID == 1 && mytrack_vol == vring1p && global_turn == 100-1 ) { Nstored100_ring++; }       
    
      if ( debug_ ) { cout << "Made it past all of the stored fills." << endl; }
    


      if ( track_ID == 1 && global_turn == 0 ) {
	if ( mom_init < 0 ) { 
	  mom_init = momentum;
	}
	station = -1;

	if ( global_turn == 0 ) {
	  if ( mytrack_vol >= 1 && mytrack_vol <= 7 ) { station = mytrack_vol - 1; }
	  if ( mytrack_vol == 10 ) { station = 7; }
	}
	else if ( global_turn == 1 ) {
	  if ( mytrack_vol == vring1p ) { station = 8; }
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
      double omegaC = -1.0;
      if ( global_turn > 0 ) { omegaC = (time-t0)/(global_turn); }
      if ( global_turn < 10 && 0 ) { 
	cout << "GlobalTurn: " << global_turn << "\t" << t0 << "\t" << time << "\t" << omegaC << "\t";
      }
      
      if ( fill ) {
	//if ( track_ID == 1 ) { G4cout << "Trk: " << pdgid << "\t" << track_ID << "\t" << dT << "\t" << mytrack << "\t" << track_volname << G4endl; }
	//if ( pdgid > -13 && pdgid < 13 ) { G4cout << "Trk-e: " << pdgid << "\t" << track_ID << "\t" << dT << "\t" << mytrack_vol << "\t" << track_volname << G4endl; }
	FillTurnTime(dT, time, mytrack_vol, track_ID, zhat, xe, xe_rms, rhat, y, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical, e);
      }


      if ( debug_ ) { cout << "Made it past the time/turn fills." << endl; }
    

    } // loop over ring tracker hits
  } // if ( SaveVRingHits_ )


  if ( debug_ ) { cout << "Made it past the ring trackers." << endl; }


      
  //-----------------
  // Ring Information
  //-----------------  
  if ( debug_ ) { cout << " -- Ring -- " << endl; }
  int systemhits[30];
  //int numringhits[30];
  
  for ( int sh = 0; sh < 30; sh++ ) { systemhits[sh] = 0; }
  //for ( int rh = 0; rh < 30; rh++ ) { numringhits[rh] = 0; }

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
      double deltaE = ringhit.deltaE;
      if ( deltaE < 0 ) { deltaE = -1*ringhit.deltaE; }
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
//       if ( rhat < 45 && rhat > -45 ) {
// 	cout << system << "\t" << ringname << "\t" << endl;
//       }
      
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
    }

    for ( int system = 0; system < Nsystemtrackers; system++ ) {
      for ( int st = 0; st < 2; st++ ) {
	if ( st == 1 && pass_final == false ) { continue; }
	h_SystemHitTracker_Nhits[system][st]->Fill(systemhits[system]);
      }
    }

//     for ( int rhit = 0; rhit < Nringhits; rhit++ ) {
//       for ( int st = 0; st < 2; st++ ) {
// 	if ( st == 1 && pass_final == false ) { continue; }
// 	h_RingHitTracker_Nhits[rhit][st]->Fill(numringhits[rhit]);
//       }
//     }
  }

  if ( debug_ ) { cout << "Made it past the ring hits." << endl; }





  //-------------------
  // G4 Generated Stuff
  //-------------------
  //double x_gen, y_gen, xprime_gen, yprime_gen;

  TLorentzVector mu, el, nu_mu, nu_el, mu_decay, el_detector, mu_lost, mu_store;
  TVector3 mu_pol, el_pol, mu_boost;
  double Ldecay = 0.0;
  double tDecay = 0.0;
  double xDecay = 0.0;
  double zDecay = 0.0;
  t0 = 0.0;
  //double CosTheta = -99.9;
  int Npart[20];
  double Npart_Rhat[20];
  double Npart_Yhat[20];
  double Npart_Mom[20];
  double Npart_Time[20];
  double Npart_PDGID[20];
  double Npart_Status[20];
  double BirthElectronData[20];
  double DecayElectronData[20];
  double BirthMuonData[20];  
  double DecayMuonData[20];  
  double StoredMuonData[20];  
  double LostMuonData[20];  
  for ( int i = 0; i < 20; i++ ) {
    BirthElectronData[i] = -9999.9;
    DecayElectronData[i] = -9999.9;
    BirthMuonData[i] = -9999.9;
    DecayMuonData[i] = -9999.9;
    StoredMuonData[i] = -9999.9;
    LostMuonData[i] = -9999.9;
  }
  for ( int i = 0; i < Nparticles; i++ ) { Npart[i] = 0; Npart_Rhat[i] = -99.9; Npart_Yhat[i] = -99.9; Npart_Mom[i] = -99.9; Npart_Time[i] = 0.0; Npart_PDGID[i] = 0.0; Npart_Status[i] = -1.0; }
  if ( SaveTruthHits_ ) {
    
    //TrackingActionArtRecordCollection const & truthhits = *truthhitDataHandle;
    //TrackingActionArtRecordCollection const & truthfinishedhits = *truthfinishedhitDataHandle;
    TrackingActionArtRecordCollection thits;
    bool anal = false;

    bool plot_birth_muon = true;
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
	double loc_theta = htruthdata.theta;
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
	double x = (rhat + 7112) * TMath::Cos(loc_theta);
	double z = (rhat + 7112) * TMath::Sin(loc_theta);
	double r = rhat + 7112;
	int pdgid = 0;
	double xe_truth = ComputeXe(p, 0.137);
	double theta = ComputeTheta(x, z);
	double Polarization = -99.9;
	
	int counter = 0;
	counter = (int)(Nstat*(theta/TMath::TwoPi()));
	//cout << Nstat << "\t" << counter << "\t" << theta << endl;
	
	turn = (int)(time-t0)/(149.15*ns);
	double dT = turn + theta/TMath::TwoPi();

	if ( turn-1 >= maxturnsreal_ ) {
	  cout << "Turn [" << turn << "] greater than what's expected" << endl;
	  //return;
	}


	TVector3 pol_pre(polx_pre, poly_pre, polz_pre);
	double polx = pol_pre.Dot(xAxis);      
	double poly = pol_pre.Dot(yAxis);
	double polz = pol_pre.Dot(zAxis);    
	double pol_azimuth_truth, pol_radial_truth, pol_vertical_truth;
	ComputePolarization(&pol_azimuth_truth, &pol_radial_truth, &pol_vertical_truth, polx, poly, polz);


	if ( trackType == "mu+" ) { pdgid = kAntiMuonID; }
	if ( trackType == "e+" ) { pdgid = kPositronID; }
	if ( trackType == "mu-" ) { pdgid = kMuonID; }
	if ( trackType == "e-" ) { pdgid = kElectronID; }


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
	  if ( pdgid == kMuonID || pdgid == kAntiMuonID ) {
	    mu_store.SetPxPyPzE(px, py, pz, e);
	  }
	}
	if ( status == gm2ringsim::kLost ) {
	  if ( pdgid == kMuonID || pdgid == kAntiMuonID ) {
	    mu_lost.SetPxPyPzE(px, py, pz, e);
	  }
	}

	if ( status == gm2ringsim::kDecay ) {
	  if ( pdgid == kMuonID || pdgid == kAntiMuonID ) {
	    mu_pol.SetXYZ(pol_radial_truth, pol_vertical_truth, pol_azimuth_truth);
	    mu_decay.SetPxPyPzE(px, py, pz, e);
	  }
	  if ( pdgid == kElectronID || pdgid == kPositronID ) {
	    el_detector.SetPxPyPzE(px, py, pz, e);
	    el_pol.SetXYZ(px, py, pz);
	    
// 	    cout << "Muon Polarization: " << mu_pol.X() << "\t" << mu_pol.Y() << "\t" << mu_pol.Z() << endl;
// 	    cout << "Electron Momentum: " << el_pol.X() << "\t" << el_pol.Y() << "\t" << el_pol.Z() << endl;
// 	    cout << "S-dot-P:           " << el_pol.Dot(mu_pol)/el_pol.Mag() << endl;
	    Polarization = el_pol.Dot(mu_pol)/el_pol.Mag();
	  }
	}

	if ( status == gm2ringsim::kBirth ) {
	  if ( pdgid == kElectronID || pdgid == kPositronID ) {
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

	int particle = FindParticle(pdgid, status);
	if ( particle == truth_particle_nums["DecayElectron"] ) { continue; }

	if ( particle == truth_particle_nums["StoredMuon"] || status == gm2ringsim::kStore ) {
	  if ( fabs(rhat_truth) > 50 || fabs(y_truth) > 50 ) {
	    status = gm2ringsim::kLost;
	    particle = truth_particle_nums["LostMuon"];
	    //cout << "Stored -> Lost" << endl;
	  }
	}

	if ( particle > -1 ) {
	  Npart[particle]++;
	  Npart_Rhat[particle] = rhat_truth;
	  Npart_Yhat[particle] = y_truth;
	  Npart_Mom[particle] = p;
	  Npart_PDGID[particle] = pdgid;
	  Npart_Status[particle] = status;
	  Npart_Time[particle] = time;
	}

	if ( particle == truth_particle_nums["DecayMuon"] ) {
	  tDecay = time;
	  xDecay = x;
	  zDecay = z;
	  if ( tDecay-t0 > maxturnsreal_*150.0 ) {
	    ;//cout << "Time [" << tDecay/1000 << "] greater than what's expected [" << maxturnsreal_*0.14915 << "]" << endl;
	    return;
	  }
	}

	if ( particle == truth_particle_nums["StoredMuon"] || status == gm2ringsim::kStore ) {
// 	  if ( time < 1000 ) { 
// 	    anal = true; 
// 	    cout << "Time = " << time << endl;
// 	    cout << "Rhat-Y: " << rhat_truth << "\t" << y_truth << endl;

// 	  }
	}
	if ( fabs(rhat_truth) > 50 || fabs(y_truth) > 50 ) {
	  if ( particle == truth_particle_nums["StoredMuon"] || status == gm2ringsim::kStore ) {
	    cout << "Rhat-Y: " << rhat_truth << "\t" << y_truth << endl;
	    cout << "Time = " << time << endl;
	    anal = true;
	  }
	}
	  
	if ( particle == truth_particle_nums["BirthMuon"] ) {
	  int nd = 0;
	  BirthMuonData[nd++] = xe_truth; 
	  BirthMuonData[nd++] = rhat_truth; 
	  BirthMuonData[nd++] = y_truth; 
	  BirthMuonData[nd++] = rprime_truth; 
	  BirthMuonData[nd++] = yprime_truth; 
	  BirthMuonData[nd++] = p; 
	  BirthMuonData[nd++] = x; 
	  BirthMuonData[nd++] = z; 
	  BirthMuonData[nd++] = theta; 
	  BirthMuonData[nd++] = t0; 
	  BirthMuonData[nd++] = time; 
	  BirthMuonData[nd++] = tDecay; 
	  BirthMuonData[nd++] = pol_azimuth_truth; 
	  BirthMuonData[nd++] = pol_radial_truth; 
	  BirthMuonData[nd++] = pol_vertical_truth; 
	  BirthMuonData[nd++] = Polarization;
	}
	  
	if ( particle == truth_particle_nums["LostMuon"] ) {
	  int nd = 0;
	  LostMuonData[nd++] = xe_truth; 
	  LostMuonData[nd++] = rhat_truth; 
	  LostMuonData[nd++] = y_truth; 
	  LostMuonData[nd++] = rprime_truth; 
	  LostMuonData[nd++] = yprime_truth; 
	  LostMuonData[nd++] = p; 
	  LostMuonData[nd++] = x; 
	  LostMuonData[nd++] = z; 
	  LostMuonData[nd++] = theta; 
	  LostMuonData[nd++] = t0; 
	  LostMuonData[nd++] = time; 
	  LostMuonData[nd++] = tDecay; 
	  LostMuonData[nd++] = pol_azimuth_truth; 
	  LostMuonData[nd++] = pol_radial_truth; 
	  LostMuonData[nd++] = pol_vertical_truth; 
	  LostMuonData[nd++] = Polarization;
	}
	  
	if ( particle == truth_particle_nums["StoredMuon"] ) {
	  int nd = 0;
	  StoredMuonData[nd++] = xe_truth; 
	  StoredMuonData[nd++] = rhat_truth; 
	  StoredMuonData[nd++] = y_truth; 
	  StoredMuonData[nd++] = rprime_truth; 
	  StoredMuonData[nd++] = yprime_truth; 
	  StoredMuonData[nd++] = p; 
	  StoredMuonData[nd++] = x; 
	  StoredMuonData[nd++] = z; 
	  StoredMuonData[nd++] = theta; 
	  StoredMuonData[nd++] = t0; 
	  StoredMuonData[nd++] = time; 
	  StoredMuonData[nd++] = tDecay; 
	  StoredMuonData[nd++] = pol_azimuth_truth; 
	  StoredMuonData[nd++] = pol_radial_truth; 
	  StoredMuonData[nd++] = pol_vertical_truth; 
	  StoredMuonData[nd++] = Polarization;
	}
	  
	if ( particle == truth_particle_nums["DecayMuon"] ) {
	  int nd = 0;
	  DecayMuonData[nd++] = xe_truth; 
	  DecayMuonData[nd++] = rhat_truth; 
	  DecayMuonData[nd++] = y_truth; 
	  DecayMuonData[nd++] = rprime_truth; 
	  DecayMuonData[nd++] = yprime_truth; 
	  DecayMuonData[nd++] = p; 
	  DecayMuonData[nd++] = x; 
	  DecayMuonData[nd++] = z; 
	  DecayMuonData[nd++] = theta; 
	  DecayMuonData[nd++] = t0; 
	  DecayMuonData[nd++] = time; 
	  DecayMuonData[nd++] = tDecay; 
	  DecayMuonData[nd++] = pol_azimuth_truth; 
	  DecayMuonData[nd++] = pol_radial_truth; 
	  DecayMuonData[nd++] = pol_vertical_truth; 
	  DecayMuonData[nd++] = Polarization;
	}

	if ( particle == truth_particle_nums["BirthElectron"] ) {
	  int nd = 0;
	  BirthElectronData[nd++] = xe_truth; 
	  BirthElectronData[nd++] = rhat_truth; 
	  BirthElectronData[nd++] = y_truth; 
	  BirthElectronData[nd++] = rprime_truth; 
	  BirthElectronData[nd++] = yprime_truth; 
	  BirthElectronData[nd++] = p; 
	  BirthElectronData[nd++] = x; 
	  BirthElectronData[nd++] = z; 
	  BirthElectronData[nd++] = theta; 
	  BirthElectronData[nd++] = t0; 
	  BirthElectronData[nd++] = time; 
	  BirthElectronData[nd++] = tDecay; 
	  BirthElectronData[nd++] = pol_azimuth_truth; 
	  BirthElectronData[nd++] = pol_radial_truth; 
	  BirthElectronData[nd++] = pol_vertical_truth; 
	  BirthElectronData[nd++] = Polarization;
	}

	if ( fill ) {
	  FillTruthTurnTime(dT, counter, status, pdgid, time, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, theta, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, e);
	}
      } // loop over G4 track
    } // loop over track types
    
    
    for ( int i = 0; i < Nparticles; i++ ) {
      if ( Npart[i] > 1 ) { cout << "You have more than one particle of " << truth_particle_names[i] << endl; }
    }

//     if ( Npart[truth_particle_nums["StoredMuon"]] == 1 ) {
//       for ( int i = 0; i < Nparticles; i++ ) {
// 	cout << truth_particle_names[i] << " : " << Npart[i] << endl;
//       }
//     }


    if ( anal ) {
      for ( int i = 0; i < Nparticles; i++ ) {
	cout << truth_particle_names[i] << " : " << Npart[i] << "\tRhat=" << Npart_Rhat[i] << "\tY=" << Npart_Yhat[i] << "\tMom=" << Npart_Mom[i] << "\tID=" << Npart_PDGID[i] << "\tStatus=" << Npart_Status[i] << "\tTime=" << Npart_Time[i] << endl;
      }   
      
      cout << "Ring Hits: " << ringtrackerhits.size() << endl;
      for ( auto hdata : ringtrackerhits) {
	int track_ID   = hdata.trackID;
	int track_volumeUID = hdata.volumeUID; 
	string track_volname = pvs.stringGivenID(track_volumeUID);
	IsVirtualRingStation(track_volname, &mytrack_vol);
	NewFixTurnCounter(&global_turn, mytrack_vol, pmytrack_vol);
	pmytrack_vol = mytrack_vol;	
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
	double xe = ComputeXe(mom, 0.0);

	if ( (global_turn > 0 && mytrack_vol != vring1p) || 1  ) {
	  cout << "\tP = " << mom << "\tTime=" << time << "\tRhat=" << rhat << endl;
	}
      }
      exit(1);
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

      double mom_x = el.Px() / el.P();
      double mom_y = el.Py() / el.P();
      double mom_z = el.Pz() / el.P();
      double theta = ComputeTheta(xDecay, zDecay);
      
      double mom_azimuth, mom_radial, mom_vertical;
      ComputePolarization(&mom_azimuth, &mom_radial, &mom_vertical, mom_x, mom_y, mom_z, theta);
      
      
      double x = el.E() / Emax;
      double pol = 1.0;
      double cosT = 1.0;

      for ( int mytime = 0; mytime < Ntruthtimes; mytime++ ) {
	//if ( mytime == 5 ) { cout << "pass_final = " << pass_final << endl; }

	if ( mytime == 1 && pass_final == false ) { continue; }
	if ( mytime == 2 && pass_final == false ) { continue; }
      
	for ( int part = 0; part < Nparticles; part++ ) {
	  string truth_part_name = truth_particle_names[part];
	  if ( truth_part_name.find("Electron") ) {;
// 	    h_G4Tracker_dX[mytime][part]->Fill(x);
// 	    h_G4Tracker_Polarization[mytime][part]->Fill(pol);
	  }
	}
      }
    }
  } // if ( SaveTruthHits_ )


  int calonum = -1;
  if ( SaveCaloHits_ ) {
    //for ( auto ringhit : ringhits) {
    bool plot_birth_electron = true;
    for ( unsigned int i = 0; i < calohits.size(); ++i ) {
      CaloArtRecord calohit = calohits[i];
      double time = calohit.time;
      double x = calohit.x;
      double y = calohit.y;
      double z = calohit.z;
      double px = calohit.px;
      double py = calohit.py;
      double pz = calohit.pz;
      double num = calohit.caloNum;
      int trackID = calohit.trackID;


      TVector3 mom_calo(px, py, pz);
      double mom = mom_calo.Mag();
      TVector3 pos_calo(x, y, z);
      double rhat = ComputeRhat(&pos_calo);
      double vhat = ComputeVhat(&pos_calo);
      G4double prhat = ComputePrhat(&pos_calo, &mom_calo);
      G4double pvhat = ComputePvhat(&pos_calo, &mom_calo);
      double theta = ComputeTheta(x, z);
      //cout << "CaloHit(): ID, x,y,z: " << trackID << "\t" << x << " , " << y << " , " << z << endl;
      //cout << "Rhat = " << rhat << "\t" << "Vhat = " << vhat << "\tP = " << mom << endl;

      TrackingActionArtRecord thit;
      bool found_trk = false;
      //cout << "Tracks" << endl;
      int nt = 0;
      for ( auto htruthdata : *truthhitDataHandle) {
	//cout << "   " << nt++ << "\t" << htruthdata.trackType << "\t" << htruthdata.trackID << "\t" << htruthdata.p << "\t" << htruthdata.time << endl;
	if ( htruthdata.trackID == trackID && htruthdata.trackType == "e+" ) {	  
	  thit = htruthdata;
	  found_trk = true;
	}
      }

      //TrackingActionArtRecord thit = (*truthhitDataHandle)[trackID];
      if ( found_trk ) {
	std::string trk_trackType = thit.trackType;
	//cout << "CaloHit(): " << "Found " << trk_trackType << endl;	
	int trk_trackID = thit.trackID;
	int trk_status = thit.status;
	double trk_rhat = thit.rhat;
	cout.precision(4);
	//cout << "TrackID = " << trackID << "\tTrkType = " << trk_trackType << "\tTrkStatus = " << trk_status << "\tTime = " << time << "\tCaloNum = " << num << "\tRhat = " << rhat << "\t" << "Vhat = " << vhat << "\tP = " << mom << "\ttDrift = " << time - tDecay << endl;
	
	int pdgid = kPositronID;
	int particle = truth_particle_nums["DecayElectron"];
	int status = gm2ringsim::kDecay;
	double pol_azimuth_truth = 0.0;
	double pol_radial_truth = 0.0;
	double pol_vertical_truth = 0.0;
	double Polarization = 0.0;
	double y_truth = vhat;
	double rhat_truth = rhat;
	double xe_truth = 0.0;
	double p = mom;
	double rprime_truth = prhat;
	double yprime_truth = pvhat;
	double e = p;
	  
	  
	if ( particle == truth_particle_nums["DecayElectron"] ) {
	  int nd = 0;
	  DecayElectronData[nd++] = xe_truth; 
	  DecayElectronData[nd++] = rhat_truth; 
	  DecayElectronData[nd++] = y_truth; 
	  DecayElectronData[nd++] = rprime_truth; 
	  DecayElectronData[nd++] = yprime_truth; 
	  DecayElectronData[nd++] = p; 
	  DecayElectronData[nd++] = x; 
	  DecayElectronData[nd++] = z; 
	  DecayElectronData[nd++] = theta; 
	  DecayElectronData[nd++] = t0;
	  DecayElectronData[nd++] = time; 
	  DecayElectronData[nd++] = tDecay; 
	  DecayElectronData[nd++] = pol_azimuth_truth; 
	  DecayElectronData[nd++] = pol_radial_truth; 
	  DecayElectronData[nd++] = pol_vertical_truth; 
	  DecayElectronData[nd++] = Polarization;
	  calonum = num+1;
	}
	
	Npart[particle]++;
	Npart_Rhat[particle] = rhat_truth;
	Npart_Yhat[particle] = y_truth;
	Npart_Mom[particle] = p;
	Npart_PDGID[particle] = pdgid;
	Npart_Status[particle] = status;
	Npart_Time[particle] = time;

	int counter = num;
	int turn = (int)(time-t0)/(149.15*ns);
	double dT = turn + theta/TMath::TwoPi();
	
	FillTruthTurnTime(dT, counter, status, pdgid, time, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, theta, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, e);
      }
    }
  }


  //-----------------------------
  // Fill Truth Information
  //-----------------------------

  // Birth Muon:       GenDist from Birth Muon       ----> RemainingDist from Birth Muon (if Decay or Stored)
  // Stored Muon:      GenDist from Birth Muon       ----> RemainingDist from Stored Muon
  // Decay Muon:       GenDist from Birth Muon       ----> RemainingDist from Decay Muon
  // Lost Muon:        GenDist from Birth Muon       ----> RemainingDist from Lost Muon
  // Birth Electron:   GenDist from Birth Electron   ----> RemainingDist from Birth Electron (if Decay)
  // Decay Electron:   GenDist from Birth Electron   ----> RemainingDist from Decay Electron
  int pdgid = 0;
  int status = 0;
  int calostation = 0;
  int particle;
  
  //
  // Birth Muon
  //

  bool debug_truthtracker = false;
  particle = truth_particle_nums["BirthMuon"];
  if ( Npart[particle] > 0 ) {
    if ( debug_truthtracker ) { cout << "BirthMuon: " << BirthMuonData[5] << endl; }
    FillTruthTracker(kGeneratedDist, particle, calostation, BirthMuonData[0], BirthMuonData[1], BirthMuonData[2], BirthMuonData[3], BirthMuonData[4], BirthMuonData[5], BirthMuonData[6], BirthMuonData[7], BirthMuonData[8], BirthMuonData[9], BirthMuonData[10], BirthMuonData[11], BirthMuonData[12], BirthMuonData[13], BirthMuonData[14], BirthMuonData[15]);
    if ( Npart[truth_particle_nums["DecayMuon"]] > 0 || Npart[truth_particle_nums["StoredMuon"]] > 0 ) {
      FillTruthTracker(kRemainingDist, particle, calostation, BirthMuonData[0], BirthMuonData[1], BirthMuonData[2], BirthMuonData[3], BirthMuonData[4], BirthMuonData[5], BirthMuonData[6], BirthMuonData[7], BirthMuonData[8], BirthMuonData[9], BirthMuonData[10], BirthMuonData[11], BirthMuonData[12], BirthMuonData[13], BirthMuonData[14], BirthMuonData[15]);
    }
  }

  //
  // Stored Muon
  //
  particle = truth_particle_nums["StoredMuon"];
  if ( Npart[particle] > 0 ) {
    if ( debug_truthtracker ) { cout << "StoredMuon: " << StoredMuonData[5] << endl; }
    FillTruthTracker(kGeneratedDist, particle, calostation, BirthMuonData[0], BirthMuonData[1], BirthMuonData[2], BirthMuonData[3], BirthMuonData[4], BirthMuonData[5], BirthMuonData[6], BirthMuonData[7], BirthMuonData[8], BirthMuonData[9], BirthMuonData[10], BirthMuonData[11], BirthMuonData[12], BirthMuonData[13], BirthMuonData[14], BirthMuonData[15]);
    FillTruthTracker(kRemainingDist, particle, calostation, StoredMuonData[0], StoredMuonData[1], StoredMuonData[2], StoredMuonData[3], StoredMuonData[4], StoredMuonData[5], StoredMuonData[6], StoredMuonData[7], StoredMuonData[8], StoredMuonData[9], StoredMuonData[10], StoredMuonData[11], StoredMuonData[12], StoredMuonData[13], StoredMuonData[14], StoredMuonData[15]);
  }

  //
  // Lost Muon
  //
  particle = truth_particle_nums["LostMuon"];
  if ( Npart[particle] > 0 ) {
    if ( debug_truthtracker ) { cout << "LostMuon: " << LostMuonData[5] << endl; }
    FillTruthTracker(kGeneratedDist, particle, calostation, BirthMuonData[0], BirthMuonData[1], BirthMuonData[2], BirthMuonData[3], BirthMuonData[4], BirthMuonData[5], BirthMuonData[6], BirthMuonData[7], BirthMuonData[8], BirthMuonData[9], BirthMuonData[10], BirthMuonData[11], BirthMuonData[12], BirthMuonData[13], BirthMuonData[14], BirthMuonData[15]);
    FillTruthTracker(kRemainingDist, particle, calostation, LostMuonData[0], LostMuonData[1], LostMuonData[2], LostMuonData[3], LostMuonData[4], LostMuonData[5], LostMuonData[6], LostMuonData[7], LostMuonData[8], LostMuonData[9], LostMuonData[10], LostMuonData[11], LostMuonData[12], LostMuonData[13], LostMuonData[14], LostMuonData[15]);
  }

  //
  // Decay Muon
  //
  particle = truth_particle_nums["DecayMuon"];
  if ( Npart[particle] > 0 ) {
    if ( debug_truthtracker ) { cout << "DecayMuon: " << DecayMuonData[5] << endl; }
    FillTruthTracker(kGeneratedDist, particle, calostation, BirthMuonData[0], BirthMuonData[1], BirthMuonData[2], BirthMuonData[3], BirthMuonData[4], BirthMuonData[5], BirthMuonData[6], BirthMuonData[7], BirthMuonData[8], BirthMuonData[9], BirthMuonData[10], BirthMuonData[11], BirthMuonData[12], BirthMuonData[13], BirthMuonData[14], BirthMuonData[15]);
    FillTruthTracker(kRemainingDist, particle, calostation, DecayMuonData[0], DecayMuonData[1], DecayMuonData[2], DecayMuonData[3], DecayMuonData[4], DecayMuonData[5], DecayMuonData[6], DecayMuonData[7], DecayMuonData[8], DecayMuonData[9], DecayMuonData[10], DecayMuonData[11], DecayMuonData[12], DecayMuonData[13], DecayMuonData[14], DecayMuonData[15]);
  }

  //
  // Birth Electron
  //
  particle = truth_particle_nums["BirthElectron"];
  if ( Npart[particle] > 0 ) {
    if ( debug_truthtracker ) { cout << "BirthElectron: " << BirthElectronData[5] << endl; }
    FillTruthTracker(kGeneratedDist, particle, calostation, BirthElectronData[0], BirthElectronData[1], BirthElectronData[2], BirthElectronData[3], BirthElectronData[4], BirthElectronData[5], BirthElectronData[6], BirthElectronData[7], BirthElectronData[8], BirthElectronData[9], BirthElectronData[10], BirthElectronData[11], BirthElectronData[12], BirthElectronData[13], BirthElectronData[14], BirthElectronData[15]);
    if ( Npart[truth_particle_nums["DecayElectron"]] > 0 ) {
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData[0], BirthElectronData[1], BirthElectronData[2], BirthElectronData[3], BirthElectronData[4], BirthElectronData[5], BirthElectronData[6], BirthElectronData[7], BirthElectronData[8], BirthElectronData[9], BirthElectronData[10], BirthElectronData[11], BirthElectronData[12], BirthElectronData[13], BirthElectronData[14], BirthElectronData[15]);
    }    
  }

  //
  // Decay Electron
  //
  particle = truth_particle_nums["DecayElectron"];
  if ( Npart[particle] > 0 ) {
    calostation = calonum;
    if ( debug_truthtracker ) { cout << "DecayElectron: " << DecayElectronData[5] << endl; }
    FillTruthTracker(kGeneratedDist, particle, calostation, BirthElectronData[0], BirthElectronData[1], BirthElectronData[2], BirthElectronData[3], BirthElectronData[4], BirthElectronData[5], BirthElectronData[6], BirthElectronData[7], BirthElectronData[8], BirthElectronData[9], BirthElectronData[10], BirthElectronData[11], BirthElectronData[12], BirthElectronData[13], BirthElectronData[14], BirthElectronData[15]);
    FillTruthTracker(kRemainingDist, particle, calostation, DecayElectronData[0], DecayElectronData[1], DecayElectronData[2], DecayElectronData[3], DecayElectronData[4], DecayElectronData[5], DecayElectronData[6], DecayElectronData[7], DecayElectronData[8], DecayElectronData[9], DecayElectronData[10], DecayElectronData[11], DecayElectronData[12], DecayElectronData[13], DecayElectronData[14], DecayElectronData[15]);
  }
  
	  
  
  if ( debug_truthtracker ) {
    for ( int i = 0; i < Nparticles; i++ ) {
      if  ( Npart[i] > 0 ) {
	cout << truth_particle_names[i] << " : " << Npart[i] << "\tRhat=" << Npart_Rhat[i] << "\tY=" << Npart_Yhat[i] << "\tMom=" << Npart_Mom[i] << "\tID=" << Npart_PDGID[i] << "\tStatus=" << Npart_Status[i] << "\tTime=" << Npart_Time[i] << endl;
      }
    }
    cout << endl;
  }
  
  if ( debug_ ) { cout << "Made it past the truth info." << endl; }


  if ( debug_ ) { std::cout<<"Ngen: " << Ngen << " \t" << "Event ID: "<<e.id()<< " -- Done." << std::endl; }

} // analyze

using gm2ringsim::ringTrackerAnalyzer;
DEFINE_ART_MODULE(ringTrackerAnalyzer)
