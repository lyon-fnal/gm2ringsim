// This is the ringTrackerAnalyzer Art module.

//  This is an analyzer that will read HitData from the event and make some plots and a root tree

// Art includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

// Root + Art include
#include "TH1F.h"
#include "TGraph.h"
#include "TProfile.h"
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
#include "Geant4/Randomize.hh"

// Hit includes
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include "gm2ringsim/common/virtualringstation/VirtualRingStationArtRecord.hh"
#include "gm2ringsim/common/ring/RingArtRecord.hh"
#include "gm2ringsim/actions/track/TrackingActionArtRecord.hh"
#include "gm2ringsim/inflector/InflectorArtRecord.hh"
#include "gm2ringsim/calo/CaloArtRecord.hh"

#include "gm2geom/inflector/InflectorGeom.hh"

#include "gm2dataproducts/strawtracker/WireID.hh"

using gm2strawtracker::WireID;
using gm2strawtracker::StrawView;
using gm2strawtracker::u_view;
using gm2strawtracker::v_view;
using gm2strawtracker::na_view;


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

#define DeltaRbin 750
#define DeltaRmin 0
#define DeltaRmax 7500.0

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

#define YPrimetimebin_Ring 63
#define YPrimetimemin_Ring -10.5
#define YPrimetimemax_Ring 10.5
#define XPrimetimebin_Ring 63
#define XPrimetimemin_Ring -10.5
#define XPrimetimemax_Ring 10.5

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

#define Rbin_Beam 361
#define Rmin_Beam -45.5
#define Rmax_Beam 45.5
#define Xbin_Beam 361
#define Xmin_Beam -45.5
#define Xmax_Beam 45.5
#define Ybin_Beam 361
#define Ymin_Beam -45.5
#define Ymax_Beam 45.5

#define YPrimebin_Beam 201
#define YPrimemin_Beam -20.5
#define YPrimemax_Beam 20.5
#define XPrimebin_Beam 201
#define XPrimemin_Beam -20.5
#define XPrimemax_Beam 20.5

#define Rbin_Decay 501
#define Rmin_Decay -400.5
#define Rmax_Decay 100.5
#define Xbin_Decay 501
#define Xmin_Decay -400.5
#define Xmax_Decay 100.5
#define Ybin_Decay 200
#define Ymin_Decay -100.5
#define Ymax_Decay 100.5

#define YPrimebin_Decay 101
#define YPrimemin_Decay -50.5
#define YPrimemax_Decay 50.5
#define XPrimebin_Decay 101
#define XPrimemin_Decay -50.5
#define XPrimemax_Decay 50.5


#define SpinPhasebin 101
#define SpinPhasemin 0
#define SpinPhasemax TMath::Pi()

#define Polbin 101
#define Polmin -1
#define Polmax 1

#define PolYbin 101
#define PolYmin -1
#define PolYmax 1

#define Poltimebin 21
#define Poltimemin -1
#define Poltimemax 1

#define PoltimeYbin 21
#define PoltimeYmin -1
#define PoltimeYmax 1

#define Mombin 2001
#define Mommin 0.0
#define Mommax 2.0

#define MuMombin 2000
#define MuMommin 0.98
#define MuMommax 1.02

#define Momtimebin 100
#define Momtimemin 0.98
#define Momtimemax 1.02

#define Ebin 350
#define Emin 0.0
#define Emax 3.5

#define Etimebin 35
#define Etimemin 0.0
#define Etimemax 3.5

#define Thetabin 630
#define Thetamin 0.0
#define Thetamax TMath::TwoPi()

#define Thetatimebin 25
#define Thetatimemin 0.0
#define Thetatimemax TMath::TwoPi()

#define Rhatbin 121
#define Rhatmin -60.5
#define Rhatmax 60.5

#define Rhattimebin 61
#define Rhattimemin -45.0
#define Rhattimemax 45.0

#define XPSbin 91
#define XPSmin -45.5
#define XPSmax 45.5

#define YPSbin 91
#define YPSmin -45.5
#define YPSmax 45.5

#define XprimePSbin 85
#define XprimePSmin -8.5
#define XprimePSmax 8.5

#define YprimePSbin 55
#define YprimePSmin -5.5
#define YprimePSmax 5.5

#define vring1p 11
#define vring2p 8

#define konestraw 17
#define ktwostraw 23

#define Nstations 12*1
#define Nstations_spin 72*1
#define Nstations_nospin 12

#define Nringtimestamps 5
#define Ntruthtimestamps 4

#define MaxNparticles 20

#define kProtonID 2212
#define kElectronID 11
#define kPositronID -11
#define kMuonID 13
#define kAntiMuonID -13

using namespace std;

enum {kGeneratedDist, kRemainingDist};
enum {kBegin, kFinal};

enum {kXe, kRhat, kVhat, kXprime, kYprime, kP, kX, kZ, kTheta, kt0, kTime, ktDecay, kPol_azimuth, kPol_radial, kPol_vertical, kPolarization, kSystemHit, kSpinPhase};

enum {kAllTime, kPerTimeSingleHit, kPerTimeOncePerTurn, kPerTimeTwicePerTurn};

enum {kEarly, kMiddle, kLate, kAll};
enum {k1tracker, k2trackers};

enum {kAllStations, kAllStationsNoTimeOffset, kAllStationsNoFastRotation, kAllStationsNoTimeOffsetNoFastRotation, kQuadStations, kKickerStations, kOtherStations, kEmptyStations};

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
  
  
  std::string strawtrackerhitModuleLabel_;
  std::string strawtrackerinstanceName_;
  std::string ringtrackerhitModuleLabel_;
  std::string ringtrackerinstanceName_;
  std::string truthhitModuleLabel_;
  std::string truthinstanceName_;
  std::string truthbeamscanhitModuleLabel_;
  std::string truthbeamscaninstanceName_;
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
  double maxdecaytime_;
  double LaunchAngle_;
  double InflectorAngle_;
  double StorageOffset_;

  ofstream out_;
  
  bool SaveInfHits_;
  bool SaveRingHits_;
  bool SaveCaloHits_;
  bool SaveStrawHits_;
  bool SaveTruthHits_;
  bool SaveBeamScanHits_;
  bool SaveVRingHits_;
  bool SaveVRing1PlaneHits_;
  
  bool debug_;
  bool fill_;
  bool muongas_;

  bool SavePhaseSpace_;

  gm2geom::InflectorGeom infGeom_;
  
  bool IsCaloElectron(int particle);
  bool IsStrawElectron(int particle);

  int IsSomething(string name);
  bool IsSomething(string name, int comp);
  bool IsKicker(string name);
  bool IsQuad(string name);
  bool IsCryostat(string name);
  bool IsCollimator(string name);
  bool IsInflector(string name);
  bool IsStrawTracker(string name);
  bool IsVacuumChamber(string name);
  bool IsVirtualRingStation(string name);
  bool IsVirtualRingStation(double theta, int *num);
  bool IsVirtualRingStation(string name, int *num);

  double Pmagic();
  int Rbin(double rhat);

  double ComputeQuadIntAngle(double r1, double r2, double d1, double d2);
  double ComputeRmagicIntAngle(double r1, double r2, double d1, double d2);
  double ComputeKickAtRmagicInt(double pr1, double pr2, double d1, double d2, double dr0);
  double ComputeSpinPhase(double pol_azimuth, double pol_radial, double pol_vertical, double mom_azimuth, double mom_radial, double mom_vertical);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z);
  void ComputeMomentum(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz);
  void ComputeMomentum(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta);
  void ComputeMomentum(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z);

  double CaloSection(int calonum);

  bool FillByVRingHist();
  bool FillByVRingHist(int timestamp);
  bool FillByVRingHist(int timestamp, int counter);
  bool FillByTimestamp(int timestamp);
  bool FillByTimestamp(int timestamp, int counter);
  double TimeOffset(int timestamp, int counter);  
  string TimestampName(int timestamp);
  string TrackerTimestampName(int timestamp);

  void FillRingTrackerPhaseSpace(double time, int counter, double x, double y, double xprime, double yprime);
  void FillRingTrackerPhaseSpace(int timehist, int trackerhist, double x, double y, double xprime, double yprime);
  void FillRingTrackerPhaseSpace(int timehist, int trackerhist, double time, double x, double y);

  void FillRingTracker(int hist, int type, double theta, double xe, double xe_rms, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly, double zhat, double spinphase);
  void FillTimeRingTracker(int hist, double time, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly);


  void FillTruthData(double *data, double xe, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double theta, double t0, double time, double tDecay, double polz, double polx, double poly, double Polarization, int syshit, double spinphase);
  void FillTruthTracker(int hist, int particle, int counter, double *data, double *data_decay);
  void FillTruthTracker(int hist, int particle, int counter, int calonum, double *data, double *data_decay);

  void FillBeamScan(double time, double rhat, double y, double xprime, double yprime, double theta, double mom, double polx, double poly, double polz);
  void FillBeamScan(int hist, double xval, double rhat, double y, double xprime, double yprime, double theta, double mom, double polx, double poly, double polz);

  void FillTruthTurnTime(int particle, int calonum, double *data);
  void FillTruthTurnTime(double dT, int counter, int status, int pdgid, double time, double xe, double rhat, double y, double prhat, double pvhat, double mom, double theta, double polz, double polx, double poly, double e);
  void FillTruthTurnTime(double xval, int hist, int particle, double xe, double rhat, double y, double prhat, double pvhat, double mom, double theta, double polz, double polx, double poly);
  void FillNumTruthTurnTime(double xval, int hist, int particle, int counter, double y, double e);

  void FillTurnTime(double dT, double time, double t0, int counter, int trackID, double theta, double zhat, double xe, double xe_rms, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e, double spinphase);
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
  int Nexp = 0;
  int Ngen = 0;
  int Nstored = 0;
  int Nlost = 0;
  int Nlostdecayed = 0;
  int Ndecayed = 0;
  int Npositrons = 0;
  int Nstrawpositrons = 0;
  int Ngoodstrawpositrons = 0;
  int Nstrawcalopositrons = 0;
  int Ngoodstrawcalopositrons = 0;
  int Ncalopositrons = 0;
  int NcalopositronsEgtEth = 0;
  int Nstart_inflector = 0;
  int Nexit_inflector = 0;
  int Nstart_ring = 0;
  int Nsurvived[10000];    
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

  
  TTree *tree_, *tree_lost_, *bs_tree_;

  float lost_time;
  float lost_dist;

  float bs_time;
  float bs_rhat;
  float bs_vhat;
  float bs_pol;
  float bs_mom;

  float mu_rhat;
  float mu_vhat;
  float mu_pol;
  float mu_mom;  
  float pt_time;
  float pt_tmod;
  float pt_faoff;
  float pt_energy;
  float pt_vhat;
  int pt_calo;

  int Npart[MaxNparticles];
  double Npart_Rhat[MaxNparticles];
  double Npart_Yhat[MaxNparticles];
  double Npart_Mom[MaxNparticles];
  double Npart_Time[MaxNparticles];
  double Npart_PDGID[MaxNparticles];
  double Npart_Status[MaxNparticles];
  
  //-------------------------------
  // Kicker/Quad/Launch Information
  //-------------------------------
  TH2F *h_RingTracker_PrhatPvhatEntrance;
  TH1F *h_RingTracker_KickTime;
  TH1F *h_RingTracker_QuadTime;
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
  string ringtrackernames[24];
  TH2F *h_RingTracker_Snapshot_RhatY[24];
  TH2F *h_RingTracker_Snapshot_XprimeX[24];
  TH2F *h_RingTracker_Snapshot_YprimeY[24];  
  TH1F *h_RingTracker_Snapshot_Mom[24]; 
  TH1F *h_RingTracker_Snapshot_Time[24];
  TH1F *h_RingTracker_Snapshot_Rhat[24];
  TH1F *h_RingTracker_Snapshot_Vhat[24];
  TH1F *h_RingTracker_Snapshot_Pol[24];
  TH1F *h_RingTracker_Snapshot_PolX[24];
  TH1F *h_RingTracker_Snapshot_PolY[24];
  TH2F *h_RingTracker_Snapshot_PolXY[24];



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
  TH1F *h_RingTracker_SpinPhase[3][2];
  TH1F *h_RingTracker_PolX[3][2];
  TH2F *h_RingTracker_PolXY[3][2];
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
  TH1F *h_Nparticles;
  TH1F *h_CaloStation_Nhits;

  TH2F *h_RingTracker_OmegaC_Radbin;
  TH2F *h_RingTracker_OmegaC_Mombin;
  TH1F *h_RingTracker_OmegaC[10];
  //int Nstoredtrackers;
  //string rtsnames[11];
  

  TH2F *h_RingTracker_Zhat_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_Rhat_vs_Time[Nringtimestamps];
  TProfile *h_RingTracker_AvgRhat_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_EqRad_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_EqRadRMS_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_Prhat_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_Pvhat_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_Mom_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_Y_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_Y2_vs_Time[Nringtimestamps];
  TProfile *h_RingTracker_AvgY_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_SpinPhase_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_Pol_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_PolX_vs_Time[Nringtimestamps];
  TH2F *h_RingTracker_PolY_vs_Time[Nringtimestamps];
  TH1F *h_RingTracker_NgtEth_vs_Time[Nringtimestamps];
  TH1F *h_RingTracker_NwghtE_vs_Time[Nringtimestamps];
  TH1F *h_RingTracker_Num_vs_Time[Nringtimestamps];

  TH1F *h_VirtualRingTracker_X_PhaseSpace_vs_Time[4][2];
  TH1F *h_VirtualRingTracker_Xprime_PhaseSpace_vs_Time[4][2];
  TH2F *h_VirtualRingTracker_XprimeX_PhaseSpace_vs_Time[4][2];
  TH1F *h_VirtualRingTracker_Y_PhaseSpace_vs_Time[4][2];
  TH1F *h_VirtualRingTracker_Yprime_PhaseSpace_vs_Time[4][2];
  TH2F *h_VirtualRingTracker_YprimeY_PhaseSpace_vs_Time[4][2];
  TGraph *g_VirtualRingTracker_X_PhaseSpace_vs_Time[4][2];
  TGraph *g_VirtualRingTracker_Y_PhaseSpace_vs_Time[4][2];



  //------------------
  // Straw Information
  //------------------
  TH2F *h_StrawTracker_XZ;
  TH1F *h_StrawTracker_NumStations;


  //------------------
  // Truth Information
  //------------------
  int Ntruthtimes;
  int Nparticles;
  map<string, int> truth_particle_nums;
  string truth_particle_names[20];
  map<int, int> truth_particle_multiplicity;
  TH1F *h_G4Tracker_Xe[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Rhat[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_ThetaRhat[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Vhat[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_SpinPhase[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Pol[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_PolX[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_PolY[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_PolXY[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_Polt0[Ntruthtimestamps][24][MaxNparticles];
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
  TH1F *h_G4Tracker_Energy[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_t0[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_SystemHits[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Time[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Polarization[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_dYprimedY[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_dXprimedX[Ntruthtimestamps][24][MaxNparticles];
  
  // Straw Electron Specific Histograms
  TH1F *h_G4Tracker_NumStrawStations[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumStraw[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumStraw_with_SystemHits[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumStraw_without_SystemHits[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumStraw_with_Mom[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_StrawNum_vs_Energy[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_StrawNum_vs_Theta[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_StrawNum_vs_DeltaP[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_NumStrawStations_vs_Energy[Ntruthtimestamps][24][MaxNparticles];


  // Decay Electron Specific Histograms
  TH1F *h_G4Tracker_NumCalo[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumCalo_with_SystemHits[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumCalo_without_SystemHits[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_NumCalo_with_Mom[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_CaloNum_vs_Energy[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_CaloNum_vs_Theta[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_CaloNum_vs_DeltaP[Ntruthtimestamps][24][MaxNparticles];

  // Decay & Straw Electron Specific Histograms
  TH1F *h_G4Tracker_DriftTime[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_DriftTime_vs_DeltaR[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_DeltaR[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_DeltaR_vs_Energy[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Theta_with_SystemHits[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_Theta_without_SystemHits[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_DeltaTheta[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_DeltaTheta_vs_Energy[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_DeltaR_vs_DeltaP[Ntruthtimestamps][24][MaxNparticles];
  TH1F *h_G4Tracker_DeltaP[Ntruthtimestamps][24][MaxNparticles];
  TH2F *h_G4Tracker_Rhat_vs_DeltaP[Ntruthtimestamps][24][MaxNparticles];



  TH2F *h_G4Tracker_R_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Rhat_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Xe_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Prhat_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Pvhat_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Mom_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Energy_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Y_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Y2_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Pol_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_SpinPhase_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_PolX_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_PolY_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_Theta_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_E_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_NumCalo_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH2F *h_G4Tracker_NumStraw_vs_Time[Ntruthtimestamps][MaxNparticles];
  
  TH1F *h_G4Tracker_TimeOffset_vs_Time[Ntruthtimestamps][MaxNparticles];
  TH1F *h_G4Tracker_NwghtE_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Num_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Ndown_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Nup_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_Nud_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_IntNud_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_IntNud30us_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_IntNup_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_IntNdown_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  TH1F *h_G4Tracker_IntDiffNud_vs_Time[Ntruthtimestamps][26][MaxNparticles];
  int G4Tracker_Nu[MaxNparticles];
  int G4Tracker_Nd[MaxNparticles];



  TH2F *h_BeamScan_R_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_Rhat_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_Xprime_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_Yprime_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_Mom_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_Y_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_Y2_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_SpinPhase_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_PolX_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_PolY_vs_Time[Ntruthtimestamps][4];
  TH2F *h_BeamScan_Theta_vs_Time[Ntruthtimestamps][4];



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
  bool start_perfect_;
  TLorentzVector firstdecay;  
  TLorentzVector firstdecay_spin;  
  TLorentzVector firstdecay_spin_500ns; 
  TLorentzVector firstdecay_spin_1us;  
  TLorentzVector firstdecay_spin_2us;  
  TLorentzVector firstdecay_spin_3us;  

  int calosyshits[24][6];


  double BirthElectronData[20];
  double BirthElectronEgtEthData[20];
  double StrawElectronData[20];
  double StrawCaloElectronData[20];
  double GoodStrawElectronData[20];
  double GoodStrawCaloElectronData[20];
  double GoodOneStrawCaloElectronData[20];
  double GoodTwoStrawCaloElectronData[20];
  double DecayElectronData[20];
  double DecayElectronQuadSectionData[20];
  double DecayElectronKickerSectionData[20];
  double DecayElectronOtherSectionData[20];
  double DecayElectronEmptySectionData[20];
  double DecayElectronEltEthData[20];
  double DecayElectronEgtEthData[20];
  double DecayElectronEgtHghEthData[20];
  double DecayElectronLongDriftTimeData[20];
  double DecayElectronShortDriftTimeData[20];
  double BirthMuonData[20];  
  double DecayMuonData[20];    
  double StoredMuonData[20];  
  double LostMuonData[20];  
  double LostDecayMuonData[20];  
  

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

  cout << "Exposed                   : " << Nexp << endl;
  cout << "Generator                 : " << Ngen << endl;
  cout << "Stored                    : " << Nstored << endl;
  cout << "Lost                      : " << Nlost << endl;
  cout << "Lost-Decayed              : " << Nlostdecayed << endl;
  cout << "Decayed                   : " << Ndecayed << endl;
  cout << "Positrons                 : " << Npositrons << endl;
  cout << "Positrons (Straw)         : " << Nstrawpositrons << endl;
  cout << "Positrons (Straw/calo)    : " << Nstrawcalopositrons << endl;
  cout << "Positrons (Straw+5)       : " << Ngoodstrawpositrons << endl;
  cout << "Positrons (Straw+5/calo)  : " << Ngoodstrawcalopositrons << endl;
  cout << "Positrons (Calo)          : " << Ncalopositrons << endl;
  cout << "Positrons (Calo, E>1.8)   : " << NcalopositronsEgtEth << endl;
  for ( int i = 0; i < Nparticles; i++ ) {
    string name = truth_particle_names[i];
    int particle = truth_particle_nums[name];
    h_Nparticles->Fill(particle, truth_particle_multiplicity[particle]);
  }

  if ( debug_ ) {
    cout << "Filling Nparticles." << endl;
  }
  cout << "---------------------------------" << endl;
  for ( int i = 0; i < Nparticles; i++ ) {
    string name = truth_particle_names[i];
    int particle = truth_particle_nums[name];
    h_Nparticles->GetXaxis()->SetBinLabel(i+1, name.c_str());
    cout << "| " << truth_particle_multiplicity[particle] << " \t" << name << endl;
  }
  cout << "---------------------------------" << endl;
  if ( debug_ ) {
    cout << "Filling Nparticles. - Done" << endl;
  }
  
  //Nsurvived[0] = Ngen - h_RingTracker_Nhits->Integral();
  for ( int i = 0; i <= maxturnsreal_; i++ ) {
    if ( i % 100 == 0 || i >= maxturnsreal_ - 1) { 
      if ( Nsurvived[i] == 0 ) { break; }
      if ( i < 10 ) { cout << "Muons Surviving " << i << " turns:    " << Nsurvived[i] << endl; }
      else if ( i < 100 ) { cout << "Muons Surviving " << i << " turns:   " << Nsurvived[i] << endl; }
      else if ( i < 1000 ) { cout << "Muons Surviving " << i << " turns:  " << Nsurvived[i] << endl; }
      else if ( i < 10000 ) { cout << "Muons Surviving " << i << " turns: " << Nsurvived[i] << endl; }
    }
  }


  if ( FillByVRingHist() && 0 ) {
    for ( int t = 0; t < Nringtimestamps; t++ ) {
      // Loop over time
      h_RingTracker_AvgRhat_vs_Time[t] = h_RingTracker_Rhat_vs_Time[t]->ProfileX(h_RingTracker_AvgRhat_vs_Time[t]->GetName(), 1, h_RingTracker_Rhat_vs_Time[t]->GetNbinsX(), "s");
      h_RingTracker_AvgY_vs_Time[t] = h_RingTracker_Y_vs_Time[t]->ProfileX(h_RingTracker_AvgY_vs_Time[t]->GetName(), 1, h_RingTracker_Y_vs_Time[t]->GetNbinsX(), "s");
    }
  }


  if ( SavePhaseSpace_ ) {
    out_.close();
  }

//   if ( FillByVRingHist() ) {
//     for ( int nt = 0; nt < 2; nt++ ) {
//       for ( int n = 0; n < 4; n++ ) {
// 	if ( g_VirtualRingTracker_X_PhaseSpace_vs_Time[n][nt] ) {
// 	  TString hname = h_VirtualRingTracker_X_PhaseSpace_vs_Time[n][nt]->GetName();
// 	  TString gname = "Data_" + hname;
// 	  g_VirtualRingTracker_X_PhaseSpace_vs_Time[n][nt]->Write(gname);
// 	}
// 	if ( g_VirtualRingTracker_Y_PhaseSpace_vs_Time[n][nt] ) {
// 	  TString hname = h_VirtualRingTracker_Y_PhaseSpace_vs_Time[n][nt]->GetName();
// 	  TString gname = "Data_" + hname;
// 	  g_VirtualRingTracker_Y_PhaseSpace_vs_Time[n][nt]->Write(gname);
// 	}
//       }
//     }
//   }

  if ( FillByVRingHist() ) {
    cout << "Inflector \t" << Nstart_inflector << "\t" << Nexit_inflector << endl;
    cout << "RingStart \t" << Nstart_ring << endl;
    cout << "RingStored\t" << Nstored1_ring << "\t" << Nstored10_ring << "\t" << Nstored100_ring << endl;
    cout << "<Theta @ x=0>   = " << Ndegat0/(Ninside_midkicker+Ninside_allkicker) << endl;
    cout << "<x' @ Theta>    = " << 1000*Nxpat0/(Ninside_midkicker+Ninside_allkicker) << endl;
    cout << "[K2, K1-K3, !K] = " << Ninside_midkicker << " , " << Ninside_allkicker << " , " << Noutside_kickers << endl;
  }
//   ofstream out;
//   string outputname = "Eff/" + basename_ + ".dat";
//   out.open(outputname.c_str());
//   out << start_inflector << "\t" << Nexit_inflector << endl;
//   out << Nstart_ring << endl;
//   out << Nstored1_ring << "\t" << Nstored10_ring << "\t" << Nstored100_ring << endl;
//   out << Ngen << endl;
//   out << Ndegat0/(Ninside_midkicker+Ninside_allkicker) << "  0.0" << endl;
//   out << 1000*Nxpat0/(Ninside_midkicker+Ninside_allkicker) << "  0.0" << endl;
//   out.close();
  
  if ( 0 ) {
  for ( int particle = 0; particle < MaxNparticles; particle++ ) {
    for ( int hist = 0; hist < Ntruthtimestamps; hist++ ) {
      for ( int s = 0; s < 4; s++ ) {
	int bin30us = -1;
	if ( h_G4Tracker_Nud_vs_Time[hist][s][particle] ) {
	  bin30us = h_G4Tracker_Nud_vs_Time[hist][s][particle]->FindBin(30);
	}
	if ( h_G4Tracker_Nud_vs_Time[hist][s][particle] && h_G4Tracker_IntNud_vs_Time[hist][s][particle] ) {
	  for ( int bin = 1; bin <= h_G4Tracker_Nud_vs_Time[hist][s][particle]->GetNbinsX(); bin++ ) {
	    Double_t sum, err;
	    sum = h_G4Tracker_Nud_vs_Time[hist][s][particle]->IntegralAndError(0, bin, err);
	    h_G4Tracker_IntNud_vs_Time[hist][s][particle]->SetBinContent(bin, sum);
	    h_G4Tracker_IntNud_vs_Time[hist][s][particle]->SetBinError(bin, err);

	    if ( bin >= bin30us && bin30us > -1 ) {
	      sum = h_G4Tracker_Nud_vs_Time[hist][s][particle]->IntegralAndError(bin30us, bin, err);
	      h_G4Tracker_IntNud30us_vs_Time[hist][s][particle]->SetBinContent(bin, sum);
	      h_G4Tracker_IntNud30us_vs_Time[hist][s][particle]->SetBinError(bin, err);
	    }
	    else {
	      h_G4Tracker_IntNud30us_vs_Time[hist][s][particle]->SetBinContent(bin, 0.0);
	      h_G4Tracker_IntNud30us_vs_Time[hist][s][particle]->SetBinError(bin, 0.0);
	    }
	  }
	}
	if ( h_G4Tracker_Ndown_vs_Time[hist][s][particle] && h_G4Tracker_IntNdown_vs_Time[hist][s][particle] ) {
	  for ( int bin = 1; bin <= h_G4Tracker_Ndown_vs_Time[hist][s][particle]->GetNbinsX(); bin++ ) {
	    Double_t sum, err;
	    sum = h_G4Tracker_Ndown_vs_Time[hist][s][particle]->IntegralAndError(0, bin, err);
	    h_G4Tracker_IntNdown_vs_Time[hist][s][particle]->SetBinContent(bin, sum);
	    h_G4Tracker_IntNdown_vs_Time[hist][s][particle]->SetBinError(bin, err);
	  }
	}
	if ( h_G4Tracker_Nup_vs_Time[hist][s][particle] && h_G4Tracker_IntNup_vs_Time[hist][s][particle] ) {
	  for ( int bin = 1; bin <= h_G4Tracker_Nup_vs_Time[hist][s][particle]->GetNbinsX(); bin++ ) {
	    Double_t sum, err;
	    sum = h_G4Tracker_Nup_vs_Time[hist][s][particle]->IntegralAndError(0, bin, err);
	    h_G4Tracker_IntNup_vs_Time[hist][s][particle]->SetBinContent(bin, sum);
	    h_G4Tracker_IntNup_vs_Time[hist][s][particle]->SetBinError(bin, err);
	  }
	}
	if ( h_G4Tracker_IntNup_vs_Time[hist][s][particle] && h_G4Tracker_IntNdown_vs_Time[hist][s][particle] && 
	     h_G4Tracker_IntDiffNud_vs_Time[hist][s][particle] ) {
	  h_G4Tracker_IntDiffNud_vs_Time[hist][s][particle]->Add(h_G4Tracker_IntNup_vs_Time[hist][s][particle], h_G4Tracker_IntNdown_vs_Time[hist][s][particle], 1, 1);
	}
      }
    }
  }
  }


  if ( fill_ && 0 ) {
  cout << "============ TRUTH INFORMATION ============" << endl;

  int tmp_particle = truth_particle_nums["DecayMuon"];
  if ( tmp_particle >= 0 ) {
  int tmp_timestamp = kGeneratedDist;
  int s = 0;
  G4cout.precision(4);
  G4cout << "  Min/Max (xe):      " << h_G4Tracker_Xe[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Xe[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_Xe[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Xe[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " mm." << G4endl;
  G4cout << "  Min/Max (x):       " << h_G4Tracker_Rhat[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Rhat[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_Rhat[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Rhat[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " mm." << G4endl;
  G4cout << "  Min/Max (y):       " << h_G4Tracker_Vhat[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Vhat[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_Vhat[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Vhat[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " mm." << G4endl;
  G4cout << "  Min/Max (x'):      " << h_G4Tracker_Xprime[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Xprime[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_Xprime[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Xprime[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " mrad." << G4endl;
  G4cout << "  Min/Max (y'):      " << h_G4Tracker_Yprime[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Yprime[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_Yprime[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Yprime[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " mrad." << G4endl;
  G4cout << "  Min/Max (t0):      " << h_G4Tracker_t0[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_t0[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_t0[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_t0[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " ns." << G4endl;
  tmp_timestamp = kPerTimeSingleHit;
  s = 1;
  G4cout << "  Min/Max (time):    " << h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " us." << G4endl;
  tmp_particle = truth_particle_nums["DecayElectron"];
  G4cout << "  Min/Max (caltime): " << h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->FindFirstBinAbove(1)) << " / " << h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->GetBinCenter(h_G4Tracker_Num_vs_Time[tmp_timestamp][s][tmp_particle]->FindLastBinAbove(1)) << " us." << G4endl;
  }
  G4cout << "First Decay Information:" << G4endl;
  G4double rhat = TMath::Sqrt(firstdecay.X()*firstdecay.X() + firstdecay.Z()*firstdecay.Z()) - R_magic();
  G4double theta = ComputeTheta(firstdecay.X(), firstdecay.Z());
  theta = ConvertTheta(theta);
  G4double vhat = firstdecay.Y();
  G4double time = firstdecay.T();
  G4cout.precision(3);
  G4cout << "  Rhat  = " << rhat << " mm." << G4endl;
  G4cout << "  Vhat  = " << vhat << " mm." << G4endl;
  G4cout << "  Theta = " << theta*180/TMath::Pi() << " deg." << G4endl;
  G4cout << "  Time  = " << time << " ns." << G4endl;
  G4cout << G4endl;
  G4cout << "First Decay Spin Information:" << G4endl;
  double pol_azimuth, pol_radial, pol_vertical;
  ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, firstdecay_spin.X(), firstdecay_spin.Y(), firstdecay_spin.Z());
  G4cout << "  sX    = " << pol_radial << G4endl;
  G4cout << "  xY    = " << pol_vertical << G4endl;
  G4cout << "  sZ    = " << pol_azimuth << G4endl;
  G4cout << G4endl;
  G4cout << "Spin Information for [0-3] microsecs:" << G4endl;
  G4cout << "  sZ (0us)   = " << pol_azimuth << "  (t=" << firstdecay_spin.T() << ")" << G4endl;
  ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, firstdecay_spin_500ns.X(), firstdecay_spin_500ns.Y(), firstdecay_spin_500ns.Z());
  G4cout << "  sZ (500ns) = " << pol_azimuth << "  (t=" << firstdecay_spin_500ns.T() << ")" << G4endl;
  ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, firstdecay_spin_1us.X(), firstdecay_spin_1us.Y(), firstdecay_spin_1us.Z());
  G4cout << "  sZ (1us)   = " << pol_azimuth << "  (t=" << firstdecay_spin_1us.T() << ")" << G4endl;
  ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, firstdecay_spin_2us.X(), firstdecay_spin_2us.Y(), firstdecay_spin_2us.Z());
  G4cout << "  sZ (2us)   = " << pol_azimuth << "  (t=" << firstdecay_spin_2us.T() << ")" << G4endl;
  ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, firstdecay_spin_3us.X(), firstdecay_spin_3us.Y(), firstdecay_spin_3us.Z());
  G4cout << "  sZ (3us)   = " << pol_azimuth << "  (t=" << firstdecay_spin_3us.T() << ")" << G4endl;
  G4cout << G4endl;
  }


  if ( FillByVRingHist() ) {	    
    if ( debug_ ) { cout << "Filling Ring Tracker SFs." << endl; }
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
	  h_RingTracker_SpinPhase[i][t]->Scale(finalavg_sf);
	  h_RingTracker_PolX[i][t]->Scale(finalavg_sf);
	  h_RingTracker_PolXY[i][t]->Scale(finalavg_sf);
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
    if ( debug_ ) { cout << "Filling Ring Tracker SFs. - Done" << endl; }
  }


  cout << "======================" << endl;
  for ( int i = 0; i <= maxturnsreal_; i++ ) {
    if ( i == 5 || i == 10 || i == 50 || i == 100 || i == 500 || i == 1000 || i == maxturnsreal_ ) {
      if ( Nsurvived[i] == 0 ) { break; }
      if ( i < 10 ) { cout << "Muons Surviving " << i << " turns:    " << Nsurvived[i] << endl; }
      else if ( i < 100 ) { cout << "Muons Surviving " << i << " turns:   " << Nsurvived[i] << endl; }
      else if ( i < 1000 ) { cout << "Muons Surviving " << i << " turns:  " << Nsurvived[i] << endl; }
      else if ( i < 10000 ) { cout << "Muons Surviving " << i << " turns: " << Nsurvived[i] << endl; }
    }
  }
  cout << "======================" << endl;
  cout << endl;

  
  cout << "Calo\tQuad\tKicker\tColl\tVac\tOther\tNothing" << endl;
  for ( int i = 0; i < 24; i++ ) {
    int sum = 0;
    for ( int j = 0; j < 6; j++ ) { sum += calosyshits[i][j]; }
    if ( sum > 0 ) {
      cout << i << "\t" << calosyshits[i][0] << "\t" << calosyshits[i][1] << "\t" << calosyshits[i][2] << "\t" << calosyshits[i][3] << "\t" << calosyshits[i][4] << "\t" << calosyshits[i][5] << endl;    
    }
  }

  if ( debug_ ) { cout << "Done with the job." << endl; }
}

  
gm2ringsim::ringTrackerAnalyzer::ringTrackerAnalyzer(fhicl::ParameterSet const &p) :
  art::EDAnalyzer(p),
  strawtrackerhitModuleLabel_ ( p.get<std::string>("strawtrackerhitModuleLabel",  "artg4"   ) ),
  strawtrackerinstanceName_   ( p.get<std::string>("strawtrackerinstanceName",    "straws"     ) ),
  ringtrackerhitModuleLabel_ ( p.get<std::string>("ringtrackerhitModuleLabel",  "artg4"   ) ),
  ringtrackerinstanceName_   ( p.get<std::string>("ringtrackerinstanceName",    "VirtualRingStation"     ) ),
  truthhitModuleLabel_       ( p.get<std::string>("truthhitModuleLabel",  "artg4"   ) ),
  truthinstanceName_         ( p.get<std::string>("truthinstanceName",    ""     ) ),
  truthbeamscanhitModuleLabel_       ( p.get<std::string>("truthbeamscanhitModuleLabel",  "artg4"   ) ),
  truthbeamscaninstanceName_         ( p.get<std::string>("truthbeamscaninstanceName",    "BeamScanTracks"     ) ),
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
  maxturnsreal_       ( p.get<int>("maxturns", 10000) ),
  maxdecaytime_       ( p.get<double>("maxdecaytime", 149.15) ),
  LaunchAngle_        ( p.get<double>("LaunchAngle", -9999.9)),
  InflectorAngle_     ( p.get<double>("InflectorAngle", 0.0)),
  StorageOffset_      ( p.get<double>("StorageOffset", 0.0)),
  SaveInfHits_      ( p.get<bool>("SaveInfHits", false)),
  SaveRingHits_      ( p.get<bool>("SaveRingHits", false)),
  SaveCaloHits_      ( p.get<bool>("SaveCaloHits", false)),
  SaveStrawHits_      ( p.get<bool>("SaveStrawHits", false)),
  SaveTruthHits_      ( p.get<bool>("SaveTruthHits", false)),
  SaveBeamScanHits_      ( p.get<bool>("SaveBeamScanHits", false)),
  SaveVRingHits_      ( p.get<bool>("SaveVRingHits", false)),
  SaveVRing1PlaneHits_      ( p.get<bool>("SaveVRing1PlaneHits", false)),
  debug_                    ( p.get<bool>("debug", false)),
  fill_                    ( p.get<bool>("fill", true)),	
  muongas_                    ( p.get<bool>("muongas", true)),	
  SavePhaseSpace_                    ( p.get<bool>("SavePhaseSpace", false))
{
  //truthinstanceName_   ( p.get<std::string>("truthinstanceName",    "TrackingAction"     ) ),
  // You could require that hist_dir and tree_dir have some real strings, but what if the
  // user just leaves them blank. We then want to use the top level directory.
  // Note that the TFileService handle dereferences to an object that inherits from
  // TFileDirectory. This will be a little tricky, so pay close attention

  if ( basename_.find("MuonGas") != string::npos ) { muongas_ = true; }
  else if ( basename_.find("BeamTransport") != string::npos ) { muongas_ = true; }
  else { muongas_ = false; }

  if ( StorageOffset_ <= 0.0 ) { StorageOffset_ = 0.0; }
  if ( muongas_ ) { SaveCaloHits_ = true; }
  if ( muongas_ ) { SaveVRingHits_ = false; }
  if ( muongas_ ) { SaveVRing1PlaneHits_ = false; }
//   if ( !muongas_ ) {
//     SaveVRing1PlaneHits_ = true;
//     SaveVRingHits_ = true;
//   }
  //SaveVRingHits_ = false;

  cout << "==========================================" << endl;
  cout << "ringTrackerAnalyzer::ringTrackerAnalyzer()" << endl;
  cout << "  -> MuonGas:   " << muongas_ << endl;
  cout << "  -> Basename:  " << basename_ << endl;
  cout << "  -> Beamstart: " << beamstart_ << endl;
  cout << "  -> Max Turns: " << maxturnsreal_ << endl;
  cout << "  -> Max Decay Time: " << maxdecaytime_ << endl;
  cout << "  -> Launch:    " << LaunchAngle_ << " mrad." << endl;
  cout << "  -> Inflector: " << InflectorAngle_ << " mrad." << endl;
  cout << "  -> Offset:    " << StorageOffset_ << " mm." << endl;
  if ( fill_ ) { cout << "  -> Filling histograms." << endl; }
  if ( SavePhaseSpace_ ) { cout << "  -> Saving Phase Space." << endl; }
  if ( SaveInfHits_ ) { cout << "  -> Saving inflector hits." << endl; }
  if ( SaveRingHits_ ) { cout << "  -> Saving ring hits." << endl; }
  if ( SaveCaloHits_ ) { cout << "  -> Saving calo hits." << endl; }
  if ( SaveStrawHits_ ) { cout << "  -> Saving straw hits." << endl; }
  if ( SaveTruthHits_ ) { cout << "  -> Saving truth hits." << endl; }
  if ( SaveBeamScanHits_ ) { cout << "  -> Saving beam scan hits." << endl; }
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

  for ( int i = 0; i < 24; i++ ) {
    for ( int j = 0; j < 6; j++ ) {
      calosyshits[i][j] = 0;
    }
  }


  gm2geom::InflectorGeom infGeom_("inflector");

  firstdecay.SetT(9999.9);
  maxturns_ = maxturnsreal_;
  Ndegat0 = 0.0;
  Nxpat0 = 0.0;
  Nexp = 0;
  Ngen = 0;
  Nstored = 0;
  Nstart_inflector = 0;
  Nexit_inflector = 0;
  Nstart_ring = 0;
  Nstored1_ring = 0;
  Nstored10_ring = 0;
  Nstored100_ring = 0;

  for ( int i = 0; i < 10000; i++ ) {
    Nsurvived[i] = 0;
  }

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


  Nringtrackers = 12;
  ringtrackernames[0] = "0deg";  
  ringtrackernames[1] = "30deg";  
  ringtrackernames[2] = "60deg";  
  ringtrackernames[3] = "90deg";  
  ringtrackernames[4] = "120deg";  
  ringtrackernames[5] = "150deg";  
  ringtrackernames[6] = "180deg";  
  ringtrackernames[7] = "210deg";  
  ringtrackernames[8] = "240deg";  
  ringtrackernames[9] = "270deg"; 
  ringtrackernames[10] = "300deg";
  ringtrackernames[11] = "330deg";

  
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

  timestamp_map[Ntimestamps] = kAllTime;
  timestamps[Ntimestamps++] = "Time";

  timestamp_map[Ntimestamps] = kPerTimeSingleHit;
  timestamps[Ntimestamps++] = "TimeSingleHit";

  timestamp_map[Ntimestamps] = kPerTimeOncePerTurn;
  timestamps[Ntimestamps++] = "TimeOncePerTurn";

  timestamp_map[Ntimestamps] = kPerTimeTwicePerTurn;
  timestamps[Ntimestamps++] = "TimeTwicePerTurn";


  truth_particle_nums[""] = -1;
  truth_particle_nums["BirthMuon"] = -1;
  truth_particle_nums["DecayMuon"] = -1;
  truth_particle_nums["LostMuon"] = -1;
  truth_particle_nums["LostDecayMuon"] = -1;
  truth_particle_nums["StoredMuon"] = -1;
  truth_particle_nums["BirthElectron"] = -1;
  truth_particle_nums["BirthElectronEgtEth"] = -1;
  truth_particle_nums["DecayElectron"] = -1;
  truth_particle_nums["StrawElectron"] = -1;
  truth_particle_nums["StrawCaloElectron"] = -1;
  truth_particle_nums["GoodStrawElectron"] = -1;
  truth_particle_nums["GoodStrawCaloElectron"] = -1;
  truth_particle_nums["GoodOneStrawCaloElectron"] = -1;
  truth_particle_nums["GoodTwoStrawCaloElectron"] = -1;
  truth_particle_nums["DecayElectronQuadSection"] = -1;
  truth_particle_nums["DecayElectronOtherSection"] = -1;
  truth_particle_nums["DecayElectronKickerSection"] = -1;
  truth_particle_nums["DecayElectronEmptySection"] = -1;
  truth_particle_nums["DecayElectronEltEth"] = -1;
  truth_particle_nums["DecayElectronEgtEth"] = -1;
  truth_particle_nums["DecayElectronEgtHghEth"] = -1;
  truth_particle_nums["DecayElectronLongDriftTime"] = -1;
  truth_particle_nums["DecayElectronShortDriftTime"] = -1;
  for ( int i = 0; i < MaxNparticles; i++ ) { 
    truth_particle_names[i] = "";
    truth_particle_multiplicity[i] = 0;
  }

  
  if ( basename_.find("NoSpinTracking") != string::npos ) {
    spin_decay = true;
  }
  else {
    spin_decay = true;
  }
  keep_edm_plots = false;

  Nstat = Nstations;
  //if ( spin_decay ) { Nstat = Nstations_spin; }
  //else { Nstat = Nstations_nospin; }
  
  cout << "------- Spin Info -------" << endl;
  cout << "Spin Decay:   " << spin_decay << endl;
  cout << "EDM Plots:    " << keep_edm_plots << endl;
  cout << "Num Stations: " << Nstat << endl;

  if ( basename_.find("NoMuonDecay") != string::npos ) {
    Nparticles = 0;
    truth_particle_names[Nparticles++] = "BirthMuon";
    truth_particle_names[Nparticles++] = "LostMuon";
    truth_particle_names[Nparticles++] = "LostDecayMuon";
    truth_particle_names[Nparticles++] = "StoredMuon";
    truth_particle_names[Nparticles++] = "DecayMuon";
  }
  else {
    if ( muongas_ ) {
      truth_particle_names[Nparticles++] = "BirthMuon";
      truth_particle_names[Nparticles++] = "DecayMuon";
      //truth_particle_names[Nparticles++] = "BirthElectron";
      truth_particle_names[Nparticles++] = "BirthElectronEgtEth";
      //truth_particle_names[Nparticles++] = "StrawElectron";
      //truth_particle_names[Nparticles++] = "GoodStrawElectron";
      //truth_particle_names[Nparticles++] = "StrawCaloElectron";
      truth_particle_names[Nparticles++] = "GoodStrawCaloElectron";
      //truth_particle_names[Nparticles++] = "GoodOneStrawCaloElectron";
      //truth_particle_names[Nparticles++] = "GoodTwoStrawCaloElectron";
      truth_particle_names[Nparticles++] = "DecayElectron";
      truth_particle_names[Nparticles++] = "DecayElectronEgtEth";
    }
    else {
      Nparticles = 0;
      truth_particle_names[Nparticles++] = "BirthMuon";
      truth_particle_names[Nparticles++] = "DecayMuon";
      truth_particle_names[Nparticles++] = "BirthElectron";
      truth_particle_names[Nparticles++] = "BirthElectronEgtEth";
      //truth_particle_names[Nparticles++] = "StrawElectron";
      //truth_particle_names[Nparticles++] = "GoodStrawElectron";
      //truth_particle_names[Nparticles++] = "StrawCaloElectron";
      truth_particle_names[Nparticles++] = "GoodStrawCaloElectron";
      //truth_particle_names[Nparticles++] = "GoodOneStrawCaloElectron";
      //truth_particle_names[Nparticles++] = "GoodTwoStrawCaloElectron";
      truth_particle_names[Nparticles++] = "DecayElectron";
      truth_particle_names[Nparticles++] = "DecayElectronEgtEth";

      if ( 0 ) {
	truth_particle_names[Nparticles++] = "LostDecayMuon";
	truth_particle_names[Nparticles++] = "LostMuon";
	truth_particle_names[Nparticles++] = "StoredMuon";
      }

      if ( 0 ) {
	truth_particle_names[Nparticles++] = "DecayElectronQuadSection";
	truth_particle_names[Nparticles++] = "DecayElectronKickerSection";
	truth_particle_names[Nparticles++] = "DecayElectronOtherSection";
	truth_particle_names[Nparticles++] = "DecayElectronEmptySection";
      }
      //truth_particle_names[Nparticles++] = "DecayElectronEltEth";
      //truth_particle_names[Nparticles++] = "DecayElectronEgtHghEth";
      //truth_particle_names[Nparticles++] = "DecayElectronLongDriftTime";
      //truth_particle_names[Nparticles++] = "DecayElectronShortDriftTime";
    }
  }

  if ( SavePhaseSpace_ ) {
    out_.open("phase_space.dat");
  }

  cout << "Using These: " << endl;
  for ( int i = 0; i < MaxNparticles; i++ ) { 
    string name = truth_particle_names[i];
    if ( name.size() > 0 ) {
      truth_particle_nums[name] = i;
      cout << "  " << i << "\t" << name << "\t" << truth_particle_nums[name] << endl;
    }
  }
  cout << "Check of all possibilities" << endl;
  for ( int i = 0; i < MaxNparticles; i++ ) {
    string name = truth_particle_names[i];
    int num = truth_particle_nums[name];
    if ( num >= 0 ) {
      cout << "  " << i << "\t" << name << "\t" << num << endl;
    }
  }

  fill = fill_;


  tree_lost_ = histDir.make<TTree>("lost", "Tree of hits");
  tree_lost_->Branch("time", &lost_time, "time/F");
  tree_lost_->Branch("dist", &lost_dist, "dist/F");

  tree_ = histDir.make<TTree>("positrons", "Tree of hits");
  tree_->Branch("time", &pt_time, "time/F");
  tree_->Branch("tmod", &pt_tmod, "tmod/F");
  tree_->Branch("faoff", &pt_faoff, "faoff/F");
  tree_->Branch("energy", &pt_energy, "energy/F");
  tree_->Branch("vhat", &pt_vhat, "vhat/F");
  tree_->Branch("calo", &pt_calo, "calo/I");
  tree_->Branch("mu_vhat", &mu_vhat, "mu_vhat/F");
  tree_->Branch("mu_rhat", &mu_rhat, "mu_rhat/F");
  tree_->Branch("mu_pol", &mu_pol, "mu_pol/F");
  tree_->Branch("mu_mom", &mu_mom, "mu_mom/F");


  bs_tree_ = histDir.make<TTree>("beamscan", "Tree of hits");
  bs_tree_->Branch("bs_time", &bs_time, "bs_time/F");
  bs_tree_->Branch("bs_vhat", &bs_vhat, "bs_vhat/F");
  bs_tree_->Branch("bs_rhat", &bs_rhat, "bs_rhat/F");
  bs_tree_->Branch("bs_pol", &bs_pol, "bs_pol/F");
  bs_tree_->Branch("bs_mom", &bs_mom, "bs_mom/F");
 
  stringstream hname;

  hname << "Nparticles";
  h_Nparticles = histDir.make<TH1F>(hname.str().c_str(), "", MaxNparticles, -0.5, MaxNparticles - 0.5);
  h_Nparticles->SetXTitle("Particle Multiplicity");
  hname.str("");

  hname << "RingTracker_Nhits";
  h_RingTracker_Nhits = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_ + 2, 0 - 0.5, maxturns_ + 1.5);
  h_RingTracker_Nhits->SetXTitle("Number of VRS Hits");
  hname.str("");

  hname << "CaloStation_Nhits";
  h_CaloStation_Nhits = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
  h_CaloStation_Nhits->SetXTitle("Number of Calo Station Hits");
  hname.str("");


  

  if ( FillByVRingHist() ) {
    
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
	  h_RingTracker_Theta[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Thetabin, Thetamin, Thetamax);
	  h_RingTracker_Theta[tt][t]->SetXTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	  hname.str("");

	  hname << "RingTracker_Pol_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_Pol[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_RingTracker_Pol[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	  hname.str("");

	  hname << "RingTracker_SpinPhase_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_SpinPhase[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", SpinPhasebin, SpinPhasemin, SpinPhasemax);
	  h_RingTracker_SpinPhase[tt][t]->SetXTitle("cos^{-1}(#hat{s}#bullet#hat{p}_{z})");
	  hname.str("");

	  hname << "RingTracker_PolX_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_PolX[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	  h_RingTracker_PolX[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	  hname.str("");

	  hname << "RingTracker_PolY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_PolY[tt][t] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	  h_RingTracker_PolY[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
	  hname.str("");

	  hname << "RingTracker_PolXY_" << ringtracker_time_name << "_" << typname;
	  h_RingTracker_PolXY[tt][t] = histDir.make<TH2F>(hname.str().c_str(), "", Polbin, Polmin, Polmax, PolYbin, PolYmin, PolYmax);
	  h_RingTracker_PolXY[tt][t]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	  h_RingTracker_PolXY[tt][t]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	  hname.str("");
	}
      }
    }

    if ( fill ) {
      hname.str("");
      hname << "RingTracker_PrhatPvhatEntrance";
      h_RingTracker_PrhatPvhatEntrance = histDir.make<TH2F>(hname.str().c_str(), "", XPrimebin_Ring/10+1, XPrimemin_Ring/100, XPrimemax_Ring/100, YPrimebin_Ring/10+1, YPrimemin_Ring/100, YPrimemax_Ring/100);
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

      
      hname << "RingTracker_KickTime";
      h_RingTracker_KickTime = histDir.make<TH1F>(hname.str().c_str(), "", 300, 0.0, 300.0);
      h_RingTracker_KickTime->SetXTitle("time [ns]");
      hname.str(""); 
      
      hname << "RingTracker_QuadTime";
      h_RingTracker_QuadTime = histDir.make<TH1F>(hname.str().c_str(), "", 300, 0.0, 300.0);
      h_RingTracker_QuadTime->SetXTitle("time [ns]");
      hname.str(""); 

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
	h_RingTracker_Snapshot_RhatY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin*2, Rhatmax*2, Rhatbin, Rhatmin*2, Rhatmax*2);
	h_RingTracker_Snapshot_RhatY[i]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_Snapshot_RhatY[i]->SetYTitle("y_{ring} [mm]");
	hname.str(""); 
    
	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_YprimeY";
	h_RingTracker_Snapshot_YprimeY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin*2, Rhatmax*2, YPrimebin_Ring, YPrimemin_Ring, YPrimemax_Ring);
	h_RingTracker_Snapshot_YprimeY[i]->SetXTitle("y_{ring} [mm]");
	h_RingTracker_Snapshot_YprimeY[i]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_XprimeX";
	h_RingTracker_Snapshot_XprimeX[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin, Rhatmin*2, Rhatmax*2, XPrimebin_Ring, XPrimemin_Ring, XPrimemax_Ring);
	h_RingTracker_Snapshot_XprimeX[i]->SetXTitle("x_{ring} [mm]");
	h_RingTracker_Snapshot_XprimeX[i]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Mom";
	h_RingTracker_Snapshot_Mom[i] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	h_RingTracker_Snapshot_Mom[i]->SetXTitle("P_{#mu} / P_{m}");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Time";
	h_RingTracker_Snapshot_Time[i] = histDir.make<TH1F>(hname.str().c_str(), "", 300, 0.0, 300);
	h_RingTracker_Snapshot_Time[i]->SetXTitle("time [ns]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Rhat";
	h_RingTracker_Snapshot_Rhat[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin*4, Rhatmin*2, Rhatmax*2);
	h_RingTracker_Snapshot_Rhat[i]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	hname.str("");

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_Vhat";
	h_RingTracker_Snapshot_Vhat[i] = histDir.make<TH1F>(hname.str().c_str(), "", Rhatbin*4, Rhatmin*2, Rhatmax*2);
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

	hname << "RingTracker_Snapshot_" << ring_tracker_name << "_PolXY";
	h_RingTracker_Snapshot_PolXY[i] = histDir.make<TH2F>(hname.str().c_str(), "", Polbin, Polmin, Polmax, PolYbin, PolYmin, PolYmax);
	h_RingTracker_Snapshot_PolXY[i]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	h_RingTracker_Snapshot_PolXY[i]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	  hname.str("");
      }
    } 
  } // if ( SaveVRingHits_ )


  if ( SaveStrawHits_ ) {
    stringstream hname;
    hname << "StrawTracker_XZ";
    h_StrawTracker_XZ = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring, Rmin_Ring, Rmax_Ring, Rbin_Ring, Rmin_Ring, Rmax_Ring);
    h_StrawTracker_XZ->SetXTitle("z_{ring} [mm]");
    h_StrawTracker_XZ->SetYTitle("x_{ring} [mm]");    
    hname.str("");

    hname << "StrawTracker_NumStations";
    h_StrawTracker_NumStations = histDir.make<TH1F>(hname.str().c_str(), "", 30, -0.5, 29.5);
    h_StrawTracker_NumStations->SetXTitle("Number of Stations w/ 4+ Hits");
    hname.str("");
  }


  if ( FillByVRingHist() ) {

    if ( debug_ ) { cout << "Creating ring tracker histogram vs time." << endl; }

    if ( fill ) {
      for ( int t = 0; t < Nringtimestamps; t++ ) {
	string timename, xtitle;
	
	if ( !FillByVRingHist(t) ) { continue; }

	if ( timestamp_map[t] == kAllTime ) { xtitle = "Time [#mus]"; }
	if ( timestamp_map[t] == kPerTimeSingleHit ) { xtitle = "Time [#mus]"; }
	if ( timestamp_map[t] == kPerTimeOncePerTurn ||
	     timestamp_map[t] == kPerTimeTwicePerTurn ) { xtitle = "Time [#mus]"; }
	timename = timestamps[t];
	
	int rbin  = Rhattimebin;
	int rmin  = Rhattimemin;
	int rmax  = Rhattimemax;
	int xpbin = XPrimetimebin_Ring;
	int xpmin = XPrimetimemin_Ring;
	int xpmax = XPrimetimemax_Ring;
	int ypbin = YPrimetimebin_Ring;
	int ypmin = YPrimetimemin_Ring;
	int ypmax = YPrimetimemax_Ring;

	double timeperturn = 0.16;
	double timebinning = 0.005; // microseconds

	double maxt = maxturns_;
	if ( timestamp_map[t] == kAllTime ) {
	  maxt = maxturns_* timeperturn;
	  timebinning = 0.16; // microseconds
	}
	if ( timestamp_map[t] == kPerTimeOncePerTurn || timestamp_map[t] == kPerTimeSingleHit ) {
	  maxt = maxturns_* timeperturn;
	  timebinning = 0.16; // microseconds
	}
	if ( timestamp_map[t] == kPerTimeTwicePerTurn ) { 
	  maxt = maxturns_* timeperturn;
	  timebinning = 0.16; // microseconds
	}
	int binsperturn = (int)(timeperturn/timebinning);

	int Nbins = maxturns_ * binsperturn;
	
	if ( debug_ ) { cout << "  Creating ring tracker histogram vs time [" << timename << "]" << endl; }
	
	hname << "Tracker_EqRad" << "_vs_" << timename;
	h_RingTracker_EqRad_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
	h_RingTracker_EqRad_vs_Time[t]->SetYTitle("x_{e} #equiv <R - R_{m}> [mm]");
	h_RingTracker_EqRad_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_EqRadRMS" << "_vs_" << timename;
	h_RingTracker_EqRadRMS_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin/10+1, rmin, rmax);
	h_RingTracker_EqRadRMS_vs_Time[t]->SetYTitle("x_{e} #equiv <R - R_{m}>^{2} [mm]");
	h_RingTracker_EqRadRMS_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Rhat" << "_vs_" << timename;
	h_RingTracker_Rhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
	h_RingTracker_Rhat_vs_Time[t]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_Rhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_AvgRhat" << "_vs_" << timename;
	h_RingTracker_AvgRhat_vs_Time[t] = histDir.make<TProfile>(hname.str().c_str(), "", Nbins, 0.0, maxt);
	h_RingTracker_AvgRhat_vs_Time[t]->SetYTitle("#bar{x}_{ring} #equiv R - R_{m} [mm]");
	h_RingTracker_AvgRhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Zhat" << "_vs_" << timename;
	h_RingTracker_Zhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Zhatbin, Zhatmin, Zhatmax);
	h_RingTracker_Zhat_vs_Time[t]->SetYTitle("z_{ring} #equiv #omega_{c}[t-t_{0}] - 2#piN_{turns}");
	h_RingTracker_Zhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Mom" << "_vs_" << timename;
	h_RingTracker_Mom_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Momtimebin, Momtimemin, Momtimemax);
	h_RingTracker_Mom_vs_Time[t]->SetYTitle("P_{#mu} / P_{m}");
	h_RingTracker_Mom_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Prhat" << "_vs_" << timename;
	h_RingTracker_Prhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, xpbin, xpmin, xpmax);
	h_RingTracker_Prhat_vs_Time[t]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	h_RingTracker_Prhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
	
	hname << "Tracker_Pvhat" << "_vs_" << timename;
	h_RingTracker_Pvhat_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, ypbin, ypmin, ypmax);
	h_RingTracker_Pvhat_vs_Time[t]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	h_RingTracker_Pvhat_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Y2" << "_vs_" << timename;
	h_RingTracker_Y2_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, 0.0, rmax*rmax);
	h_RingTracker_Y2_vs_Time[t]->SetYTitle("y^{2}_{ring} [mm]");
	h_RingTracker_Y2_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_Y" << "_vs_" << timename;
	h_RingTracker_Y_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
	h_RingTracker_Y_vs_Time[t]->SetYTitle("y_{ring} [mm]");
	h_RingTracker_Y_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_AvgY" << "_vs_" << timename;
	h_RingTracker_AvgY_vs_Time[t] = histDir.make<TProfile>(hname.str().c_str(), "", Nbins, 0.0, maxt);
	h_RingTracker_AvgY_vs_Time[t]->SetYTitle("y_{ring} [mm]");
	h_RingTracker_AvgY_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Pol" << "_vs_" << timename;
	h_RingTracker_Pol_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Poltimebin, Poltimemin, Poltimemax);
	h_RingTracker_Pol_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{z}");
	h_RingTracker_Pol_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_SpinPhase" << "_vs_" << timename;
	h_RingTracker_SpinPhase_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, SpinPhasebin, SpinPhasemin, SpinPhasemax);
	h_RingTracker_SpinPhase_vs_Time[t]->SetYTitle("cos^{-1}(#hat{s}#bullet#hat{p}_{z})");
	h_RingTracker_SpinPhase_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_PolX" << "_vs_" << timename;
	h_RingTracker_PolX_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Poltimebin, Poltimemin, Poltimemax);
	h_RingTracker_PolX_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{r}");
	h_RingTracker_PolX_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_PolY" << "_vs_" << timename;
	h_RingTracker_PolY_vs_Time[t] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, PoltimeYbin, PoltimeYmin, PoltimeYmax);
	h_RingTracker_PolY_vs_Time[t]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	h_RingTracker_PolY_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	hname << "Tracker_Num" << "_vs_" << timename;
	h_RingTracker_Num_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", Nbins, 0.0, maxt);
	h_RingTracker_Num_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");

	//cout << "Bin Width = " << h_RingTracker_Num_vs_Time[t]->GetBinWidth(1) << endl;
      
	hname << "Tracker_NgtEth" << "_vs_" << timename;
	h_RingTracker_NgtEth_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", Nbins, 0.0, maxt);
	h_RingTracker_NgtEth_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      
	hname << "Tracker_NwghtE" << "_vs_" << timename;
	h_RingTracker_NwghtE_vs_Time[t] = histDir.make<TH1F>(hname.str().c_str(), "", Nbins, 0.0, maxt);
	h_RingTracker_NwghtE_vs_Time[t]->SetXTitle(xtitle.c_str());
	hname.str("");
      }
    }

    if ( debug_ ) { cout << "Creating ring tracker histogram vs time. - Done" << endl; }

    if ( fill ) {
      for ( int nt = 0; nt < 2; nt++ ) {
	string ntrackers = "";
	if ( nt == 0 ) { ntrackers = "1Tracker"; }
	if ( nt == 1 ) { ntrackers = "2Trackers"; }

	for ( int t = 0; t < 4; t++ ) {
	  string tname = "";
	  if ( t == kEarly ) { tname = "Early"; }
	  if ( t == kMiddle ) { tname = "Middle"; }
	  if ( t == kLate ) { tname = "Late"; }
	  if ( t == kAll ) { tname = "All"; }
	  
	  hname << "VirtualRingTracker_X_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	  h_VirtualRingTracker_X_PhaseSpace_vs_Time[t][nt] = histDir.make<TH1F>(hname.str().c_str(), "", XPSbin, XPSmin, XPSmax);
	  h_VirtualRingTracker_X_PhaseSpace_vs_Time[t][nt]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  hname.str("");
	  
	  hname << "VirtualRingTracker_Y_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	  h_VirtualRingTracker_Y_PhaseSpace_vs_Time[t][nt] = histDir.make<TH1F>(hname.str().c_str(), "", YPSbin, YPSmin, YPSmax);
	  h_VirtualRingTracker_Y_PhaseSpace_vs_Time[t][nt]->SetXTitle("y_{ring} [mm]");
	  hname.str("");
	  
	  hname << "VirtualRingTracker_Xprime_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	  h_VirtualRingTracker_Xprime_PhaseSpace_vs_Time[t][nt] = histDir.make<TH1F>(hname.str().c_str(), "", XprimePSbin, XprimePSmin, XprimePSmax);
	  h_VirtualRingTracker_Xprime_PhaseSpace_vs_Time[t][nt]->SetXTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
	  
	  hname << "VirtualRingTracker_Yprime_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	  h_VirtualRingTracker_Yprime_PhaseSpace_vs_Time[t][nt] = histDir.make<TH1F>(hname.str().c_str(), "", YprimePSbin, YprimePSmin, YprimePSmax);
	  h_VirtualRingTracker_Yprime_PhaseSpace_vs_Time[t][nt]->SetXTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");
	  
	  hname << "VirtualRingTracker_XprimeX_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	  h_VirtualRingTracker_XprimeX_PhaseSpace_vs_Time[t][nt] = histDir.make<TH2F>(hname.str().c_str(), "", XPSbin, XPSmin, XPSmax, XprimePSbin, XprimePSmin, XprimePSmax);
	  h_VirtualRingTracker_XprimeX_PhaseSpace_vs_Time[t][nt]->SetXTitle("x_{ring} [mm]");
	  h_VirtualRingTracker_XprimeX_PhaseSpace_vs_Time[t][nt]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  hname.str("");
	  
	  hname << "VirtualRingTracker_YprimeY_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	  h_VirtualRingTracker_YprimeY_PhaseSpace_vs_Time[t][nt] = histDir.make<TH2F>(hname.str().c_str(), "", YPSbin, YPSmin, YPSmax, YprimePSbin, YprimePSmin, YprimePSmax);
	  h_VirtualRingTracker_YprimeY_PhaseSpace_vs_Time[t][nt]->SetXTitle("y_{ring} [mm]");
	  h_VirtualRingTracker_YprimeY_PhaseSpace_vs_Time[t][nt]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  hname.str("");

	  g_VirtualRingTracker_X_PhaseSpace_vs_Time[t][nt] = NULL;
	  g_VirtualRingTracker_Y_PhaseSpace_vs_Time[t][nt] = NULL;
	}
      }
    }

  } // if ( SaveVRing1PlaneHits_ )  

  if ( debug_ ) { cout << "Created beam scan histograms." << endl; }

  
  if ( SaveBeamScanHits_ && fill_ ) {

	  
    int rbin  = Rhattimebin;
    int rmin  = Rhattimemin;
    int rmax  = Rhattimemax;
    int xbin  = Rhattimebin;
    int xmin  = Rhattimemin;
    int xmax  = Rhattimemax;
    int ybin  = Rhattimebin;
    int ymin  = Rhattimemin;
    int ymax  = Rhattimemax;
    int xpbin = XPrimetimebin_Ring;
    int xpmin = XPrimetimemin_Ring;
    int xpmax = XPrimetimemax_Ring;
    int ypbin = YPrimetimebin_Ring;
    int ypmin = YPrimetimemin_Ring;
    int ypmax = YPrimetimemax_Ring;


    for ( int t = 0; t < Ntruthtimestamps; t++ ) {

      if ( debug_ ) { cout << "Creating Beam Scan Histograms for time " << timestamps[t] << endl; }

      
      if ( !FillByTimestamp(t) ) { continue; }

      string timename, xtitle;
      if ( timestamp_map[t] == kAllTime ) { xtitle = "Time [#mus]"; }
      if ( timestamp_map[t] == kPerTimeOncePerTurn ||
	   timestamp_map[t] == kPerTimeSingleHit || 
	   timestamp_map[t] == kPerTimeTwicePerTurn ) { xtitle = "Time [#mus]"; }
      timename = timestamps[t];
      

      double timeperturn = 0.14915;
      int hitsperturn = 12;
      double maxt = maxturns_;
      double timebinning = 0.005;
      if ( timestamp_map[t] == kAllTime ) {
	timebinning = timeperturn;
	maxt = maxturns_* timeperturn;
      }
      if ( timestamp_map[t] == kPerTimeSingleHit ) {
	maxt = maxturns_* timeperturn;
	timebinning = 0.14915;
	//timebinning = 0.0025;
      }
      if ( timestamp_map[t] == kPerTimeOncePerTurn ) {
	maxt = maxturns_* timeperturn;
	timebinning = timeperturn / 4;
      }
      if ( timestamp_map[t] == kPerTimeTwicePerTurn ) { 
	maxt = maxturns_* timeperturn;
	timebinning = timeperturn / 4;
      }
      
      if ( maxturns_ > 200 ) { timebinning = TMath::Max(0.14915/4, timebinning); }
      
      
      int binsperturn = (int)(timeperturn/timebinning);
      int Nbins = maxturns_ * binsperturn;

      for ( int m = 0; m < 4; m++ ) {
	string momname = "";
	if ( m == 1 ) { momname = "_HighMom"; }
	if ( m == 2 ) { momname = "_LowMom"; }
	if ( m == 3 ) { momname = "_MagicMom"; }
      
      stringstream hname;
      hname << "BeamScan_Rhat_vs_" << timename << momname;
    h_BeamScan_Rhat_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
    h_BeamScan_Rhat_vs_Time[t][m]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
    h_BeamScan_Rhat_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_Y_vs_" << timename << momname;
    h_BeamScan_Y_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
    h_BeamScan_Y_vs_Time[t][m]->SetYTitle("y_{ring} [mm]");
    h_BeamScan_Y_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_Y2_vs_" << timename << momname;
    h_BeamScan_Y2_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, 0.0, rmax*rmax);
    h_BeamScan_Y2_vs_Time[t][m]->SetYTitle("y^{2}_{ring} [mm]");
    h_BeamScan_Y2_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_R_vs_" << timename << momname;
    h_BeamScan_R_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, 0.0, rmax);
    h_BeamScan_R_vs_Time[t][m]->SetYTitle("R = |#hat{r} + #hat{y}| [mm]");
    h_BeamScan_R_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");   
      
    hname << "BeamScan_Xprime_vs_" << timename << momname;
    h_BeamScan_Xprime_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, xpbin, xpmin, xpmax);
    h_BeamScan_Xprime_vs_Time[t][m]->SetYTitle("x'_{gen} #equiv P_{x}/P_{z} [mrad]");
    h_BeamScan_Xprime_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_Yprime_vs_" << timename << momname;
    h_BeamScan_Yprime_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, ypbin, ypmin, ypmax);
    h_BeamScan_Yprime_vs_Time[t][m]->SetYTitle("y'_{gen} #equiv P_{y}/P_{z} [mrad]");
    h_BeamScan_Yprime_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");	  
    
    hname << "BeamScan_Mom_vs_" << timename << momname;
    h_BeamScan_Mom_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Momtimebin, Momtimemin, Momtimemax);
    h_BeamScan_Mom_vs_Time[t][m]->SetYTitle("P_{#mu} / P_{m}");
    h_BeamScan_Mom_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_SpinPhase_vs_" << timename << momname;
    h_BeamScan_SpinPhase_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, SpinPhasebin/5+1, SpinPhasemin, SpinPhasemax);
    h_BeamScan_SpinPhase_vs_Time[t][m]->SetYTitle("cos^{-1}(#hat{s}#bullet#hat{p}_{z})");
    h_BeamScan_SpinPhase_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_PolX_vs_" << timename << momname;
    h_BeamScan_PolX_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Poltimebin, Poltimemin, Poltimemax);
    h_BeamScan_PolX_vs_Time[t][m]->SetYTitle("#hat{s}#bullet#hat{p}_{r}");
    h_BeamScan_PolX_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_PolY_vs_" << timename << momname;
    h_BeamScan_PolY_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, PoltimeYbin, PoltimeYmin, PoltimeYmax);
    h_BeamScan_PolY_vs_Time[t][m]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
    h_BeamScan_PolY_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
    
    hname << "BeamScan_Theta_vs_" << timename << momname;
    h_BeamScan_Theta_vs_Time[t][m] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Thetatimebin, Thetatimemin, Thetatimemax);
    h_BeamScan_Theta_vs_Time[t][m]->SetYTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
    h_BeamScan_Theta_vs_Time[t][m]->SetXTitle(xtitle.c_str());
    hname.str("");
  }
  }
  }



  if ( SaveTruthHits_ ) {

    stringstream hname;

    for ( int part = 0; part < Nparticles; part++ ) {
      string truth_part_name = truth_particle_names[part];

      for ( int t = 0; t < Ntruthtimes; t++ ) {
	string truth_time_name = timevals[t];

	if ( debug_ ) { cout << "Creating " << truth_part_name << " histograms for time " << truth_time_name << endl; }
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

	  
	  int rbin  = Rbin_Beam;
	  double rmin  = Rmin_Beam;
	  double rmax  = Rmax_Beam;
	  int xbin  = Xbin_Beam;
	  double xmin  = Xmin_Beam;
	  double xmax  = Xmax_Beam;
	  int ybin  = Ybin_Beam;
	  double ymin  = Ymin_Beam;
	  double ymax  = Ymax_Beam;
	  int xpbin = XPrimebin_Beam;
	  double xpmin = XPrimemin_Beam;
	  double xpmax = XPrimemax_Beam;
	  int ypbin = YPrimebin_Beam;
	  double ypmin = YPrimemin_Beam;
	  double ypmax = YPrimemax_Beam;

	  int particle = truth_particle_nums[truth_part_name];
	  if ( IsCaloElectron(particle) || IsStrawElectron(particle) ) {
	    if ( truth_time_name == "GeneratedDist" ) {
	      rbin  = Rbin_Decay;
	      rmin  = Rmin_Decay;
	      rmax  = Rmax_Decay;
	      xbin  = Xbin_Decay;
	      xmin  = Xmin_Decay;
	      xmax  = Xmax_Decay;
	      ybin  = Ybin_Decay;
	      ymin  = Ymin_Decay;
	      ymax  = Ymax_Decay;
	      xpbin = XPrimebin_Decay;
	      xpmin = XPrimemin_Decay;
	      xpmax = XPrimemax_Decay;
	      ypbin = YPrimebin_Decay;
	      ypmin = YPrimemin_Decay;
	      ypmax = YPrimemax_Decay;
	    }
	  }

	  if ( fill ) {
	    hname << "G4Track_Polarization_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_Polarization[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", SpinPhasebin, SpinPhasemin, SpinPhasemax);
	    h_G4Tracker_Polarization[t][s][part]->SetXTitle("cos^{-1}(#hat{s}_{#mu}#bullet#hat{p}_{#mu}) [rad]");
	    hname.str("");
	
	    hname << "G4Track_dXprimedX_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_dXprimedX[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", xbin, xmin, xmax, xpbin, xpmin, xpmax);
	    h_G4Tracker_dXprimedX[t][s][part]->SetXTitle("x_{gen} [mm]");
	    h_G4Tracker_dXprimedX[t][s][part]->SetYTitle("x'_{gen} #equiv P_{x}/P_{z} (p_{#mu}=0) [mrad]");
	    hname.str("");
	
	    hname << "G4Track_dYprimedY_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_dYprimedY[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", ybin, ymin, ymax, ypbin, ypmin, ypmax);
	    h_G4Tracker_dYprimedY[t][s][part]->SetYTitle("y_{gen} [mm]");
	    h_G4Tracker_dYprimedY[t][s][part]->SetYTitle("y'_{gen} #equiv P_{y}/P_{z} (p_{#mu}=0) [mrad]");
	    hname.str("");      
      
	    hname << "G4Track_XprimeX_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_XprimeX[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", xbin, xmin, xmax, xpbin, xpmin, xpmax);
	    h_G4Tracker_XprimeX[t][s][part]->SetXTitle("x_{gen} [mm]");
	    h_G4Tracker_XprimeX[t][s][part]->SetYTitle("x'_{gen} #equiv P_{x}/P_{z} [mrad]");
	    hname.str("");
      
	    hname << "G4Track_YprimeY_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_YprimeY[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", ybin, ymin, ymax, ypbin, ypmin, ypmax);
	    h_G4Tracker_YprimeY[t][s][part]->SetXTitle("y_{gen} [mm]");
	    h_G4Tracker_YprimeY[t][s][part]->SetYTitle("y'_{gen} #equiv P_{y}/P_{z} [mrad]");
	    hname.str("");

	    hname << "G4Track_Rhat_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_Rhat[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", rbin, rmin, rmax);
	    h_G4Tracker_Rhat[t][s][part]->SetXTitle("x_{gen} #equiv R - R_{m} [mm]");
	    hname.str("");

	    hname << "G4Track_ThetaRhat_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_ThetaRhat[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", Thetabin, Thetamin, Thetamax, rbin, rmin, rmax);
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
	    h_G4Tracker_R[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", rbin/2+1, 0.0, rmax);
	    h_G4Tracker_R[t][s][part]->SetXTitle("R = |#hat{r} + #hat{y}| [mm]");
	    hname.str("");
      
	    hname << "G4Track_Pol_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_Pol[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	    h_G4Tracker_Pol[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	    hname.str("");
      
	    hname << "G4Track_SpinPhase_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_SpinPhase[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", SpinPhasebin, SpinPhasemin, SpinPhasemax);
	    h_G4Tracker_SpinPhase[t][s][part]->SetXTitle("cos^{-1}(#hat{s}_{#mu}#bullet#hat{p}_{#mu}) [rad]");	   
	    hname.str("");
      
	    hname << "G4Track_PolX_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_PolX[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Polbin, Polmin, Polmax);
	    h_G4Tracker_PolX[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	    hname.str("");
      
	    hname << "G4Track_PolY_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_PolY[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", PolYbin, PolYmin, PolYmax);
	    h_G4Tracker_PolY[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{y}");
	    hname.str("");
      
	    hname << "G4Track_PolXY_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_PolXY[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", Polbin, Polmin, Polmax, Polbin, Polmin, Polmax);
	    h_G4Tracker_PolXY[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{r}");
	    h_G4Tracker_PolXY[t][s][part]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	    hname.str("");
      
	    hname << "G4Track_Polt0_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_Polt0[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", Polbin, Polmin, Polmax, 300, -100, 200);
	    h_G4Tracker_Polt0[t][s][part]->SetXTitle("#hat{s}#bullet#hat{p}_{z}");
	    h_G4Tracker_Polt0[t][s][part]->SetYTitle("t_{0} [ns]");
	    hname.str("");
      
	    hname << "G4Track_Theta_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_Theta[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Thetabin, Thetamin, Thetamax);
	    h_G4Tracker_Theta[t][s][part]->SetXTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	    hname.str("");
      
	    hname << "G4Track_ThetaR_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_ThetaR[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", Thetabin, Thetamin, Thetamax, rbin/2+1, 0.0, rmax);
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
	    if ( truth_part_name.find("Muon") != string::npos ) {
	      h_G4Tracker_Mom[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", MuMombin, MuMommin, MuMommax);
	    }
	    else {
	      h_G4Tracker_Mom[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Mombin, Mommin, Mommax);
	    }
	    h_G4Tracker_Mom[t][s][part]->SetXTitle("P / P_{magic}");
	    hname.str("");
    
	    hname << "G4Track_Energy_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_Energy[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Ebin, Emin, Emax);
	    h_G4Tracker_Energy[t][s][part]->SetXTitle("Energy [GeV]");
	    hname.str("");
    
	    hname << "G4Track_t0_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_t0[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 300, -100, 200);
	    h_G4Tracker_t0[t][s][part]->SetXTitle("t_{0} [ns]");
	    hname.str("");
    
	    hname << "G4Track_SystemHits_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_SystemHits[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 10, -0.5, 9.5);
	    h_G4Tracker_SystemHits[t][s][part]->SetXTitle("System Hits");
	    hname.str("");
    
	    hname << "G4Track_Time_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", (int)(maxturns_*0.2)*150, 0.0, maxturns_*0.2);
	    h_G4Tracker_Time[t][s][part]->SetXTitle("t_{Birth/Lost/Decay/Store} [#mus]");
	    hname.str("");
    

	    //-----------------
	    // Straw Electrons
	    //-----------------
	    if ( IsStrawElectron(particle) ) {
	      hname << "G4Track_NumStrawStations_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumStrawStations[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 10, -0.5, 9.5);
	      h_G4Tracker_NumStrawStations[t][s][part]->SetXTitle("Number of Straw Station  Hits");
	      hname.str("");
    
	      hname << "G4Track_NumStraw_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumStraw[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumStraw[t][s][part]->SetXTitle("Straw Station");
	      hname.str("");
    
	      hname << "G4Track_NumStraw_with_SystemHits_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumStraw_with_SystemHits[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumStraw_with_SystemHits[t][s][part]->SetXTitle("Straw Station (w/ System Hit)");
	      hname.str("");
    
	      hname << "G4Track_NumStraw_without_SystemHits_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumStraw_without_SystemHits[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumStraw_without_SystemHits[t][s][part]->SetXTitle("Straw Station (w/o System Hit)");
	      hname.str("");
    	    
	      hname << "G4Track_NumStraw_with_Mom_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumStraw_with_Mom[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumStraw_with_Mom[t][s][part]->SetXTitle("Straw Station (w/ |p| weight)");
	      hname.str("");
    
	      hname << "G4Track_NumStrawStations_vs_Energy_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumStrawStations_vs_Energy[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 10, -0.5, 9.5, Ebin/5+1, Emin, Emax);
	      h_G4Tracker_NumStrawStations_vs_Energy[t][s][part]->SetXTitle("Number of Straw Station Hits");
	      h_G4Tracker_NumStrawStations_vs_Energy[t][s][part]->SetYTitle("|P_{e^{+}}| [GeV]");
	      hname.str("");
    
	      hname << "G4Track_StrawNum_vs_Energy_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_StrawNum_vs_Energy[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 26, -0.5, 25.5, Ebin/5+1, Emin, Emax);
	      h_G4Tracker_StrawNum_vs_Energy[t][s][part]->SetXTitle("Straw Station");
	      h_G4Tracker_StrawNum_vs_Energy[t][s][part]->SetYTitle("|P_{e^{+}}| [GeV]");
	      hname.str("");
    
	      //Double_t dPbins[14]={-1500, -1000, -500, -250, -150, -100, -50, -40, -30, -20, -15, -10, -5, 0.1};	    
	      hname << "G4Track_StrawNum_vs_DeltaP_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_StrawNum_vs_DeltaP[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 26, -0.5, 25.5, 50, -500, 0.1);
	      h_G4Tracker_StrawNum_vs_DeltaP[t][s][part]->SetXTitle("Straw Station");
	      h_G4Tracker_StrawNum_vs_DeltaP[t][s][part]->SetYTitle("#DeltaP_{e^{+}} [GeV]");
	      hname.str("");
    
	      hname << "G4Track_StrawNum_vs_Theta_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_StrawNum_vs_Theta[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 26, -0.5, 25.5, Thetabin, Thetamin, Thetamax);
	      h_G4Tracker_StrawNum_vs_Theta[t][s][part]->SetXTitle("Straw Station");
	      h_G4Tracker_StrawNum_vs_Theta[t][s][part]->SetYTitle("#theta_{ring} = tan^{-1}(#frac{x}{z}) [rad]");
	      hname.str("");
	    }

	    //-----------------
	    // Decay Electrons
	    //-----------------    
	    if ( IsCaloElectron(particle) ) {
	      hname << "G4Track_NumCalo_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumCalo[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumCalo[t][s][part]->SetXTitle("Calo Station");
	      hname.str("");
    
	      hname << "G4Track_NumCalo_with_SystemHits_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumCalo_with_SystemHits[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumCalo_with_SystemHits[t][s][part]->SetXTitle("Calo Station (w/ System Hit)");
	      hname.str("");
    
	      hname << "G4Track_NumCalo_without_SystemHits_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumCalo_without_SystemHits[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumCalo_without_SystemHits[t][s][part]->SetXTitle("Calo Station (w/o System Hit)");
	      hname.str("");
    	    
	      hname << "G4Track_NumCalo_with_Mom_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_NumCalo_with_Mom[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 26, -0.5, 25.5);
	      h_G4Tracker_NumCalo_with_Mom[t][s][part]->SetXTitle("Calo Station (w/ |p| weight)");
	      hname.str("");
    
	      hname << "G4Track_CaloNum_vs_Energy_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_CaloNum_vs_Energy[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 26, -0.5, 25.5, Ebin/5+1, Emin, Emax);
	      h_G4Tracker_CaloNum_vs_Energy[t][s][part]->SetXTitle("Calo Station");
	      h_G4Tracker_CaloNum_vs_Energy[t][s][part]->SetYTitle("|P_{e^{+}}| [GeV]");
	      hname.str("");
    
	      //Double_t dPbins[14]={-1500, -1000, -500, -250, -150, -100, -50, -40, -30, -20, -15, -10, -5, 0.1};	    
	      hname << "G4Track_CaloNum_vs_DeltaP_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_CaloNum_vs_DeltaP[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 26, -0.5, 25.5, 50, -500, 0.1);
	      h_G4Tracker_CaloNum_vs_DeltaP[t][s][part]->SetXTitle("Calo Station");
	      h_G4Tracker_CaloNum_vs_DeltaP[t][s][part]->SetYTitle("#DeltaP_{e^{+}} [GeV]");
	      hname.str("");
    
	      hname << "G4Track_CaloNum_vs_Theta_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_CaloNum_vs_Theta[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 26, -0.5, 25.5, Thetabin, Thetamin, Thetamax);
	      h_G4Tracker_CaloNum_vs_Theta[t][s][part]->SetXTitle("Calo Station");
	      h_G4Tracker_CaloNum_vs_Theta[t][s][part]->SetYTitle("#theta_{ring} = tan^{-1}(#frac{x}{z}) [rad]");
	      hname.str("");
	    }


	    //-----------------
	    // Decay or Straw Electrons
	    //-----------------
	    if ( IsCaloElectron(particle) || IsStrawElectron(particle) ) {
	      hname << "G4Track_DriftTime_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DriftTime[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 101, -0.5, 100.5);
	      h_G4Tracker_DriftTime[t][s][part]->SetXTitle("t_{Meas} - t_{Decay/Store} [ns]");
	      hname.str("");
    
	      hname << "G4Track_DeltaTheta_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DeltaTheta[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Thetabin, -Thetamax/2.0, Thetamax/2.0);
	      h_G4Tracker_DeltaTheta[t][s][part]->SetXTitle("#theta_{calo} - #theta_{ring}");
	      hname.str("");
    
	      hname << "G4Track_DeltaTheta_vs_Energy_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DeltaTheta_vs_Energy[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", Thetabin/10+1, -Thetamax/2.0, Thetamax/2.0, Ebin/5+1, Emin, Emax);
	      h_G4Tracker_DeltaTheta_vs_Energy[t][s][part]->SetXTitle("#theta_{calo} - #theta_{ring}");
	      h_G4Tracker_DeltaTheta_vs_Energy[t][s][part]->SetYTitle("|P_{e^{+}}| [GeV]");
	      hname.str("");
    
	      hname << "G4Track_Theta_with_SystemHits_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_Theta_with_SystemHits[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Thetabin, Thetamin, Thetamax);
	      h_G4Tracker_Theta_with_SystemHits[t][s][part]->SetYTitle("#theta_{ring} = tan^{-1}(#frac{x}{z}) (w/ System Hit) [rad]");
	      hname.str("");
    
	      hname << "G4Track_Theta_without_SystemHits_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_Theta_without_SystemHits[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", Thetabin, Thetamin, Thetamax);
	      h_G4Tracker_Theta_without_SystemHits[t][s][part]->SetYTitle("#theta_{ring} = tan^{-1}(#frac{x}{z}) (w/o System Hit) [rad]");
	      hname.str("");
    
	      hname << "G4Track_DeltaP_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DeltaP[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", 50, -500, 0.1);
	      h_G4Tracker_DeltaP[t][s][part]->SetXTitle("#DeltaP_{e^{+}} [GeV]");
	      hname.str("");
    
	      hname << "G4Track_DeltaR_vs_DeltaP_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DeltaR_vs_DeltaP[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", DeltaRbin/10+1, DeltaRmin, DeltaRmax, 50, -500, 0.1);	 
	      h_G4Tracker_DeltaR_vs_DeltaP[t][s][part]->SetXTitle("|r_{calo} - r_{ring}| [mm]");
	      h_G4Tracker_DeltaR_vs_DeltaP[t][s][part]->SetYTitle("#DeltaP_{e^{+}} [GeV]");
	      hname.str("");
	      
	      hname << "G4Track_Rhat_vs_DeltaP_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_Rhat_vs_DeltaP[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", rbin, rmin, rmax, 50, -500, 0.1);
	      h_G4Tracker_Rhat_vs_DeltaP[t][s][part]->SetXTitle("x_{gen} #equiv R - R_{m} [mm]");	 
	      h_G4Tracker_Rhat_vs_DeltaP[t][s][part]->SetYTitle("#DeltaP_{e^{+}} [GeV]");
	      hname.str("");

	      hname << "G4Track_DriftTime_vs_DeltaR_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DriftTime_vs_DeltaR[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", 101, -0.5, 100.5, DeltaRbin/10+1, DeltaRmin, DeltaRmax);
	      h_G4Tracker_DriftTime_vs_DeltaR[t][s][part]->SetXTitle("t_{Meas} - t_{Decay/Store} [#mus]");
	      h_G4Tracker_DriftTime_vs_DeltaR[t][s][part]->SetYTitle("|r_{calo} - r_{ring}| [mm]");
	      hname.str("");    

	      hname << "G4Track_DeltaR_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DeltaR[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", DeltaRbin, DeltaRmin, DeltaRmax);	    
	      h_G4Tracker_DeltaR[t][s][part]->SetXTitle("|r_{calo} - r_{ring}| [mm]");
	      hname.str("");      

	      hname << "G4Track_DeltaR_vs_Energy_" << truth_part_name << "_" << truth_time_name << statname;
	      h_G4Tracker_DeltaR_vs_Energy[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", DeltaRbin/10+1, DeltaRmin, DeltaRmax, Ebin/5+1, Emin, Emax);
	      h_G4Tracker_DeltaR_vs_Energy[t][s][part]->SetXTitle("|r_{calo} - r_{ring}| [mm]");
	      h_G4Tracker_DeltaR_vs_Energy[t][s][part]->SetYTitle("|P_{e^{+}}| [GeV]");
	      hname.str("");    
	    }
	    else {
	      h_G4Tracker_DriftTime[t][s][part] = NULL;
	    }
    
	    hname << "G4Track_RhatY_" << truth_part_name << "_" << truth_time_name << statname;
	    h_G4Tracker_RhatY[t][s][part] = histDir.make<TH2F>(hname.str().c_str(), "", xbin, xmin, xmax, ybin, ymin, ymax);
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

	if ( debug_ ) { cout << "Creating " << truth_part_name << " histograms for time " << truth_time_name << " -- Done." << endl; }
	
      } // loop over times
    } // loop over 7 particles




    if ( fill ) {
      for ( int part = 0; part < Nparticles; part++ ) {
	string truth_part_name = truth_particle_names[part];

	for ( int t = 0; t < Ntruthtimestamps; t++ ) {

	  if ( !FillByTimestamp(t) ) { continue; }

	  if ( truth_part_name == "BirthMuon" ) { continue; }
	

	  string timename, xtitle;
	  if ( timestamp_map[t] == kAllTime ) { xtitle = "Time [#mus]"; }
	  if ( timestamp_map[t] == kPerTimeOncePerTurn ||
	       timestamp_map[t] == kPerTimeSingleHit || 
	       timestamp_map[t] == kPerTimeTwicePerTurn ) { xtitle = "Time [#mus]"; }
	  timename = timestamps[t];


	  if ( debug_ ) { cout << "Creating " << truth_part_name << " histograms for (vs) time " << timename << "." << endl; }
	
	  
	  int rbin  = Rhattimebin;
	  int rmin  = Rhattimemin;
	  int rmax  = Rhattimemax;
	  int xpbin = XPrimetimebin_Ring;
	  int xpmin = XPrimetimemin_Ring;
	  int xpmax = XPrimetimemax_Ring;
	  int ypbin = YPrimetimebin_Ring;
	  int ypmin = YPrimetimemin_Ring;
	  int ypmax = YPrimetimemax_Ring;
	  

	  double timeperturn = 0.14915;
	  int hitsperturn = 12;
	  double maxt = maxturns_;
	  double timebinning = 0.005;
	  if ( timestamp_map[t] == kAllTime ) {
	    timebinning = timeperturn;
	    maxt = maxturns_* timeperturn;
	  }
	  if ( timestamp_map[t] == kPerTimeSingleHit ) {
	    maxt = maxturns_* timeperturn;
	    timebinning = 0.14915;
	    //timebinning = 0.0025;
	  }
	  if ( timestamp_map[t] == kPerTimeOncePerTurn ) {
	    maxt = maxturns_* timeperturn;
	    timebinning = timeperturn / 4;
	  }
	  if ( timestamp_map[t] == kPerTimeTwicePerTurn ) { 
	    maxt = maxturns_* timeperturn;
	    timebinning = timeperturn / 4;
	  }

	  if ( maxturns_ > 200 ) { timebinning = TMath::Max(0.14915/4, timebinning); }
	  // 	  if ( truth_part_name == "DecayMuon" ) { 
	  // 	    if ( timestamp_map[t] != kPerTimeSingleHit ) {	      
	  // 	      timebinning = TMath::Min(timeperturn / Nstations, timebinning);
	  // 	    }
	  // 	  }
	  // 	  if ( truth_part_name.find("DecayElectron") != string::npos ) { 
	  // 	    if ( timestamp_map[t] != kPerTimeSingleHit ) {
	  // 	      timebinning = 0.14915/24;
	  // 	    }
	  // 	  }
	  
	  
	  int binsperturn = (int)(timeperturn/timebinning);
	  int Nbins = maxturns_ * binsperturn;

	  // 100 ns bins
	  //if ( maxturns_ > 500 ) { hitsperturn = 1; }

	  if ( debug_ ) {
	    cout << "  Time bins: " << Nbins << " for 0 < t < " << maxt << endl;
	    cout << "  Rhat bins: " << rbin << " for " << rmin << " < r < " << rmax << endl;
	    cout << "  x' bins:   " << xpbin << " for " << xpmin << " < r < " << xpmax << endl;
	    cout << "  y' bins:   " << ypbin << " for " << ypmin << " < r < " << ypmax << endl;
	    cout << "  Engy bins: " << Etimebin << " for " << Etimemin << " < r < " << Etimemax << endl;
	    cout << "  Mom bins:  " << Momtimebin << " for " << Momtimemin << " < r < " << Momtimemax << endl;
	  }

	  hname << "G4Track_Xe" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Xe_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
	  h_G4Tracker_Xe_vs_Time[t][part]->SetYTitle("x_{e} #equiv #frac{(p - p_{m})R_{m}}{p_{m}(1-n)} [mm]");
	  h_G4Tracker_Xe_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4Track_Rhat" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Rhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
	  h_G4Tracker_Rhat_vs_Time[t][part]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_G4Tracker_Rhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Y" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Y_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, rmin, rmax);
	  h_G4Tracker_Y_vs_Time[t][part]->SetYTitle("y_{ring} [mm]");
	  h_G4Tracker_Y_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Y2" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Y2_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, 0.0, rmax*rmax);
	  h_G4Tracker_Y2_vs_Time[t][part]->SetYTitle("y^{2}_{ring} [mm]");
	  h_G4Tracker_Y2_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4Track_R" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_R_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, rbin, 0.0, rmax);
	  h_G4Tracker_R_vs_Time[t][part]->SetYTitle("R = |#hat{r} + #hat{y}| [mm]");
	  h_G4Tracker_R_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");	  
	  
	  hname << "G4Track_Mom" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Mom_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Momtimebin, Momtimemin, Momtimemax);
	  h_G4Tracker_Mom_vs_Time[t][part]->SetYTitle("P_{#mu} / P_{m}");
	  h_G4Tracker_Mom_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Energy" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Energy_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Etimebin, Etimemin, Etimemax);
	  h_G4Tracker_Energy_vs_Time[t][part]->SetYTitle("Energy [GeV]");
	  h_G4Tracker_Energy_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
	  
	  hname << "G4Track_Prhat" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Prhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, xpbin, xpmin, xpmax);
	  h_G4Tracker_Prhat_vs_Time[t][part]->SetYTitle("x'_{ring} #equiv P_{x}/P_{z} [mrad]");
	  h_G4Tracker_Prhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Pvhat" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Pvhat_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, ypbin, ypmin, ypmax);
	  h_G4Tracker_Pvhat_vs_Time[t][part]->SetYTitle("y'_{ring} #equiv P_{y}/P_{z} [mrad]");
	  h_G4Tracker_Pvhat_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_Pol" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Pol_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Poltimebin, Poltimemin, Poltimemax);
	  h_G4Tracker_Pol_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{z}");
	  h_G4Tracker_Pol_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      
	  hname << "G4Track_SpinPhase" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_SpinPhase_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, SpinPhasebin/5+1, SpinPhasemin, SpinPhasemax);
	  h_G4Tracker_SpinPhase_vs_Time[t][part]->SetYTitle("cos^{-1}(#hat{s}#bullet#hat{p}_{z})");
	  h_G4Tracker_SpinPhase_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4Track_PolX" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_PolX_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Poltimebin, Poltimemin, Poltimemax);
	  h_G4Tracker_PolX_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{r}");
	  h_G4Tracker_PolX_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4Track_PolY" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_PolY_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, PoltimeYbin, PoltimeYmin, PoltimeYmax);
	  h_G4Tracker_PolY_vs_Time[t][part]->SetYTitle("#hat{s}#bullet#hat{p}_{y}");
	  h_G4Tracker_PolY_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");

	  hname << "G4Track_Theta" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_Theta_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, Thetatimebin, Thetatimemin, Thetatimemax);
	  h_G4Tracker_Theta_vs_Time[t][part]->SetYTitle("#theta = tan^{-1}(#frac{x}{z}) [rad]");
	  h_G4Tracker_Theta_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");
      

	  int particle = truth_particle_nums[truth_part_name];

	  if ( IsCaloElectron(particle) ) {
	    hname << "G4Track_NumCalo" << "_" << truth_part_name << "_vs_" << timename;
	    h_G4Tracker_NumCalo_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, 24, -0.5, 23.5);
	    h_G4Tracker_NumCalo_vs_Time[t][part]->SetYTitle("Calo Station");
	    h_G4Tracker_NumCalo_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	    hname.str("");
	  }
	  
	  if ( IsStrawElectron(particle) ) {
	    hname << "G4Track_NumStraw" << "_" << truth_part_name << "_vs_" << timename;
	    h_G4Tracker_NumStraw_vs_Time[t][part] = histDir.make<TH2F>(hname.str().c_str(), "", Nbins, 0.0, maxt, 24, -0.5, 23.5);
	    h_G4Tracker_NumStraw_vs_Time[t][part]->SetYTitle("Straw Station");
	    h_G4Tracker_NumStraw_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	    hname.str("");
	  }
	        
      
	  hname << "G4Track_TimeOffset" << "_" << truth_part_name << "_vs_" << timename;
	  h_G4Tracker_TimeOffset_vs_Time[t][part] = histDir.make<TH1F>(hname.str().c_str(), "", 200, 0.0, 0.200);
	  h_G4Tracker_TimeOffset_vs_Time[t][part]->SetXTitle(xtitle.c_str());
	  hname.str("");



	  if ( debug_ ) { cout << "Creating " << truth_part_name << " histograms for (vs) time " << timename << " for Num Counts." << endl; }
	  
	  
	  int NstatHist = 4;
	  if ( IsStrawElectron(particle) || IsCaloElectron(particle) ) { NstatHist += 3; }
	  else { NstatHist += 0; }
	  
	  for ( int s = 0; s < NstatHist; s++ ) {
	    string statname = "";
	    
	    //
	    // Everyone has thsese
	    // 
	    if ( s == 0 ) { statname = "AllStations"; }
	    else if ( s == 1 ) { statname = "AllStationsNoTimeOffset"; }
	    else if ( s == 2 ) { statname = "AllStationsNoFastRotation"; }
	    else if ( s == 3 ) { statname = "AllStationsNoTimeOffsetNoFastRotation"; }
	    else {
	      
	      //
	      // If not Decay electron
	      //
	      if ( IsCaloElectron(particle) ) { 
		if ( s == 4 ) { statname = "QuadStations"; }
		else if ( s == 5 ) { statname = "KickerStations"; }
		else if ( s == 6 ) { statname = "EmptyStations"; }
		else {
		  continue; 
		}
	      }
	      else {
		continue;
	      }
	    }

	    int numhitsperturn = Nstations;
	    
	    double tbin = 0.002;
	    double tturn = 0.15;
	    numhitsperturn = (int)(tturn/tbin);
	    double Nmaxt = tturn * maxturns_;
	    
	    
	    if ( debug_ ) {
	      cout << "  Num bins:  " << maxturns_*numhitsperturn << " for 0 < t < " << maxt << endl;
	    }


	    hname << "G4Track_Num" << statname << "_" << truth_part_name << "_vs_" << timename;
	    h_G4Tracker_Num_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	    h_G4Tracker_Num_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	    hname.str("");

	    if ( fabs(h_G4Tracker_Num_vs_Time[t][s][part]->GetBinWidth(1) - tbin) > 0.001 ) {
	      cout << "Not the correct binning." << endl;
	      cout << "W    = " << h_G4Tracker_Num_vs_Time[t][s][part]->GetBinWidth(1) << endl;
	      cout << "Tbin = " << tbin << endl;
	      exit(1);
	    }
	    
	    if ( keep_edm_plots ) {
	      if ( s >= 4 ) { continue; }
	      hname << "G4Track_NwghtE" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_NwghtE_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_NwghtE_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      hname.str("");
	      
	      hname << "G4Track_Nup" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_Nup_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_Nup_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_Nup_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	      
	      hname << "G4Track_IntNup" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_IntNup_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_IntNup_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_IntNup_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	      
	      hname << "G4Track_Ndown" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_Ndown_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_Ndown_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_Ndown_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	      
	      hname << "G4Track_IntNdown" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_IntNdown_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_IntNdown_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_IntNdown_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	      
	      hname << "G4Track_Nud" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_Nud_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_Nud_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_Nud_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	      
	      hname << "G4Track_IntNud" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_IntNud_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_IntNud_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_IntNud_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	      
	      hname << "G4Track_IntNud30us" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_IntNud30us_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_IntNud30us_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_IntNud30us_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	      
	      hname << "G4Track_IntDiffNud" << statname << "_" << truth_part_name << "_vs_" << timename;
	      h_G4Tracker_IntDiffNud_vs_Time[t][s][part] = histDir.make<TH1F>(hname.str().c_str(), "", maxturns_*numhitsperturn, 0.0, Nmaxt);
	      h_G4Tracker_IntDiffNud_vs_Time[t][s][part]->SetXTitle(xtitle.c_str());
	      h_G4Tracker_IntDiffNud_vs_Time[t][s][part]->Sumw2();
	      hname.str("");
	    }
	    else {
	      h_G4Tracker_IntNud30us_vs_Time[t][s][part] = NULL;
	      h_G4Tracker_IntDiffNud_vs_Time[t][s][part] = NULL;
	      h_G4Tracker_Nud_vs_Time[t][s][part] = NULL;
	      h_G4Tracker_IntNud_vs_Time[t][s][part] = NULL;
	      h_G4Tracker_Nup_vs_Time[t][s][part] = NULL;
	      h_G4Tracker_IntNup_vs_Time[t][s][part] = NULL;
	      h_G4Tracker_Ndown_vs_Time[t][s][part] = NULL;
	      h_G4Tracker_IntNdown_vs_Time[t][s][part] = NULL;
	    }
	  }
	  
	  if ( debug_ ) { cout << "Creating " << truth_part_name << " histograms for time " << timename << ". -- Done" << endl; }
	  
	} // Loop over hits per turn
      } // Loop over particles
    } // fill
  } // if ( SaveTruthHits_ )

  if ( debug_ ) { cout << "Creating truth histograms." << endl; }



  
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
	h_InflectorTracker_XZ[i] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring/10, Rmin_Ring, Rmax_Ring, Rbin_Ring/10, Rmin_Ring, Rmax_Ring);
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
      h_InflectorTracker_PrhatPvhatEntrance = histDir.make<TH2F>(hname.str().c_str(), "", YPrimebin_Ring/10+1, YPrimemin_Ring/100, YPrimemax_Ring/100, YPrimebin_Ring/10+1, YPrimemin_Ring/100, YPrimemax_Ring/100);
      h_InflectorTracker_PrhatPvhatEntrance->SetYTitle("#hat{P}_{v}");
      h_InflectorTracker_PrhatPvhatEntrance->SetXTitle("#hat{P}_{r}");

      hname.str("");  
      hname << "InflectorTracker_PrhatPvhatExit";
      h_InflectorTracker_PrhatPvhatExit = histDir.make<TH2F>(hname.str().c_str(), "", YPrimebin_Ring/10+1, YPrimemin_Ring/100, YPrimemax_Ring/100, YPrimebin_Ring/10+1, YPrimemin_Ring/100, YPrimemax_Ring/100);
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


  if ( debug_ ) { cout << "Creating inflector histograms." << endl; }



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
	  h_SystemHitTracker_RhatY[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin_Ring/2+1, Rhatmin_Ring, Rhatmax_Ring, Ybin_Ring/2+1, Ymin_Ring, Ymax_Ring);
	  h_SystemHitTracker_RhatY[i][st]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_SystemHitTracker_RhatY[i][st]->SetYTitle("y_{ring} [mm]");
	  hname.str("");
	
	  hname << rhitnames[i] << "Hits" << stname << "_RhatTime";
	  h_SystemHitTracker_RhatTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Rhatbin_Ring/2+1, Rhatmin_Ring, Rhatmax_Ring);
	  h_SystemHitTracker_RhatTime[i][st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_SystemHitTracker_RhatTime[i][st]->SetXTitle("t_{ring} [#mus]");
	  hname.str("");
	
	  hname << rhitnames[i] << "Hits" << stname << "_YTime";
	  h_SystemHitTracker_YTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Ybin_Ring/2+1, Ymin_Ring, Ymax_Ring);
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
	  h_RingHitTracker_XZ[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", Rbin_Ring/10+1, Rmin_Ring, Rmax_Ring, Rbin_Ring/10+1, Rmin_Ring, Rmax_Ring);
	  h_RingHitTracker_XZ[i][st]->SetYTitle("x_{ring} [mm]");
	  h_RingHitTracker_XZ[i][st]->SetXTitle("z_{ring} [mm]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_RhatY";
	  h_RingHitTracker_RhatY[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", Rhatbin_Ring/2+1, Rhatmin_Ring, Rhatmax_Ring, Ybin_Ring/2+1, Ymin_Ring, Ymax_Ring);
	  h_RingHitTracker_RhatY[i][st]->SetXTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_RingHitTracker_RhatY[i][st]->SetYTitle("y_{ring} [mm]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_RhatTime";
	  h_RingHitTracker_RhatTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Rhatbin_Ring/2+1, Rhatmin_Ring, Rhatmax_Ring);
	  h_RingHitTracker_RhatTime[i][st]->SetYTitle("x_{ring} #equiv R - R_{m} [mm]");
	  h_RingHitTracker_RhatTime[i][st]->SetXTitle("t_{ring} [#mus]");
	  hname.str("");
	
	  hname << inames[i] << "Hits" << stname << "_YTime";
	  h_RingHitTracker_YTime[i][st] = histDir.make<TH2F>(hname.str().c_str(), "", 20000, 0.0, 200, Ybin_Ring/2+1, Ymin_Ring, Ymax_Ring);
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


  if ( debug_ ) { cout << "Creating ring histograms." << endl; }


  gm2geom::inflectorGeometry const& ig = infGeom_.ig ;//inflectorGeometry::getInstance();
  G4ThreeVector upstream(0., 0., -ig.mandrel_length());  
  upstream.rotateX(-infGeom_.tiltAngle); // why negative?  WTF?
  upstream.rotateY(infGeom_.swingAngle);
  upstream += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream.rotateY(infGeom_.rotAngle);
  G4ThreeVector downstream(R_magic()+ig.aperture_off(), 0., 0.);
  downstream.rotateY(infGeom_.rotAngle);
  G4ThreeVector dir = (downstream - upstream).unit();
  
  start_perfect_ = false;
  if ( beamstart_ == "UC" || beamstart_ == "upstream_cryo" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart_.SetXYZ(6989 , 0 , -2394);
    rhat_offset = 275.7;
    yhat_offset = 0;
  }
  if ( beamstart_ == "UM" || beamstart_ == "upstream" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    beamStart_.SetXYZ(6990 , 0 , -2388);
    rhat_offset = 247;
    yhat_offset = 0;
  }
  if ( beamstart_ == "DM" || beamstart_ == "downstream" ) {
    xAxis.SetXYZ(0.9952 , 0 , -0.09736);
    yAxis.SetXYZ(0 , 1 , 0);
    //zAxis.SetXYZ(0.09736 , 0 , 0.9952);
    zAxis.SetXYZ(dir.x(), dir.y(), dir.z());
    xAxis = yAxis.Cross(zAxis);
    beamStart_.SetXYZ(downstream.x(), downstream.y(), downstream.z());
    //beamStart_.SetXYZ(7155 , 0 , -699.9);
    rhat_offset = 77;
    yhat_offset = 0;
  }
  if ( beamstart_ == "Perfect" || beamstart_ == "CO" ) {
    xAxis.SetXYZ(1 , 0 , 0);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0 , 0 , 1);
    beamStart_.SetXYZ(7112 + StorageOffset_ , 0 , 0);
    rhat_offset = StorageOffset_;
    yhat_offset = 0;
    start_perfect_ = true;
  }
  if ( beamstart_ == "CO_Offset77" ) {
    xAxis.SetXYZ(1 , 0 , 0);
    yAxis.SetXYZ(0 , 1 , 0);
    zAxis.SetXYZ(0 , 0 , 1);
    beamStart_.SetXYZ(7112 + StorageOffset_ , 0 , 0);
    rhat_offset = StorageOffset_;
    yhat_offset = 0;
  }

  if ( beamstart_ == "UC" || beamstart_ == "upstream_cryo" ) {;
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

double gm2ringsim::ringTrackerAnalyzer::ComputeSpinPhase(double pol_x, double pol_y, double pol_z, double mom_x, double mom_y, double mom_z)
{
  double mom_azimuth, mom_radial, mom_vertical;
  ComputeMomentum(&mom_azimuth, &mom_radial, &mom_vertical, mom_x, mom_y, mom_z);
  double pol_azimuth, pol_radial, pol_vertical;
  ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, pol_x, pol_y, pol_z);

  double dot = pol_azimuth * mom_azimuth;
  double dot_full = pol_azimuth * mom_azimuth + pol_radial * mom_radial + pol_vertical * mom_vertical;
  if ( dot < 1 || dot > 1 ) {
    ;//cout << "MomZ = " << mom_z << "\tPolZ = " << pol_z << endl;
  }
  double phase = TMath::ACos(dot);
  //cout << "MomZ = " << mom_azimuth << "\tPolZ = " << pol_azimuth << "\t";
  //cout << phase << "\t" << dot << "\t" << dot_full << endl;

  return( phase );
}

void gm2ringsim::ringTrackerAnalyzer::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz)
{  
  *pol_vertical = poly;
  *pol_radial   = polx;
  *pol_azimuth  = polz; 
}

void gm2ringsim::ringTrackerAnalyzer::ComputeMomentum(double *mom_azimuth, double *mom_radial, double *mom_vertical, double momx, double momy, double momz)
{
  TVector3 mom_ring(momx, momy, momz);
  *mom_vertical = momy / mom_ring.Mag();
  *mom_radial   = momx / mom_ring.Mag();
  *mom_azimuth  = momz / mom_ring.Mag(); 
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

void gm2ringsim::ringTrackerAnalyzer::ComputeMomentum(double *mom_azimuth, double *mom_radial, double *mom_vertical, double momx, double momy, double momz, double theta)
{
  // There is defined going counter clockwise around the ring starting at x = +1 and z = 0;
  RotateRingToBeamCoordinates(theta);
  TVector3 mom_ring(momx, momy, momz);
  *mom_vertical = mom_ring.Dot(yAxis_beam_) / mom_ring.Mag();
  *mom_radial   = mom_ring.Dot(xAxis_beam_) / mom_ring.Mag();
  *mom_azimuth  = mom_ring.Dot(zAxis_beam_) / mom_ring.Mag();
}

void gm2ringsim::ringTrackerAnalyzer::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z)
{
  double theta_nom = TMath::ATan2(x, z);
  double theta = TMath::Pi()/2.0 - theta_nom;

  ComputePolarization(pol_azimuth, pol_radial, pol_vertical, polx, poly, polz, theta);
}

void gm2ringsim::ringTrackerAnalyzer::ComputeMomentum(double *mom_azimuth, double *mom_radial, double *mom_vertical, double momx, double momy, double momz, double x, double z)
{
  double theta_nom = TMath::ATan2(x, z);
  double theta = TMath::Pi()/2.0 - theta_nom;

  ComputeMomentum(mom_azimuth, mom_radial, mom_vertical, momx, momy, momz, theta);
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

double gm2ringsim::ringTrackerAnalyzer::CaloSection(int calonum)
{
  if ( calonum == 21 || calonum == 22 || calonum == 23 ) { return( kQuadStations ); }
  if ( calonum == 15 || calonum == 16 || calonum == 17 ) { return( kQuadStations ); }
  if ( calonum == 9 || calonum == 10 || calonum == 11 ) { return( kQuadStations ); }
  if ( calonum == 3 || calonum == 4 || calonum == 5 ) { return( kKickerStations ); }

  return( kEmptyStations );
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
  double t_mod = 0.0;;

  bool is_time = false;
  if ( timestamp == kPerTimeOncePerTurn || timestamp == kPerTimeSingleHit || timestamp == kPerTimeTwicePerTurn ) { is_time = true; }
  if ( timestamp == 10*kPerTimeOncePerTurn || timestamp == 10*kPerTimeSingleHit || timestamp == 10*kPerTimeTwicePerTurn ) { is_time = true; }
  
  double t_scale = 1.0;
  if ( is_time ) { t_scale = 0.14915; }

  if ( is_time ) {
    if ( timestamp == 10*kPerTimeOncePerTurn || timestamp == 10*kPerTimeSingleHit || timestamp == 10*kPerTimeTwicePerTurn ) {
      t_mod = t_scale*((float)counter/24);
    }
    else {
      t_mod = t_scale*((float)counter/Nstat);
    }
  }
  
  
  return( t_mod );
}

bool gm2ringsim::ringTrackerAnalyzer::FillByVRingHist()
{
  if ( !fill_ ) { return( false ); }
  if ( SaveVRingHits_ || SaveVRing1PlaneHits_ ) { return( true ); }

  return( false );
}

bool gm2ringsim::ringTrackerAnalyzer::FillByVRingHist(int timestamp)
{
  if ( !fill_ ) { return( false ); }
  if ( debug_ ) { cout << "    FillByVRingHist(timestamp=" << timestamp << ")" << endl; }
  
  bool retval = true;

  if ( SaveVRingHits_ == false ) { retval = false; }
  if ( SaveVRing1PlaneHits_ == true ) {
    if ( timestamp_map[timestamp] == kAllTime ) { retval = false; }
    else if ( timestamp_map[timestamp] == kPerTimeSingleHit ) { retval = true; }
    else if ( timestamp_map[timestamp] == kPerTimeOncePerTurn ) { retval = true; }
    else if ( timestamp_map[timestamp] == kPerTimeTwicePerTurn ) { retval = false; }
    else { retval = false; }
  }

  if ( debug_ ) { cout << "    FillByVRingHist(timestamp=" << timestamp << ") --> " << retval << endl; }
  
  return( retval );
}

bool gm2ringsim::ringTrackerAnalyzer::FillByVRingHist(int timestamp, int counter)
{
  if ( !fill_ ) { return( false ); }
  if ( debug_ ) { cout << "    FillByVRingHist(timestamp=" << timestamp << " , counter=" << counter << ")" << endl; }

  bool retval = true;

  if ( SaveVRingHits_ == false ) { retval = false; }
  if ( SaveVRing1PlaneHits_ == true ) {
    if ( timestamp_map[timestamp] == kAllTime ) { 
      retval = false;
    }
    else if ( timestamp_map[timestamp] == kPerTimeOncePerTurn ) { 
      if ( counter != vring1p ) { retval = false; }
      else { retval = true; }
    }
    else if ( timestamp_map[timestamp] == kPerTimeSingleHit ) {
      if ( counter != vring1p ) { retval = false; }
      else { retval = true; }
    }
    else if ( timestamp_map[timestamp] == kPerTimeTwicePerTurn ) { 
      if ( counter != vring2p ) { retval = false; }
      else { retval = false; }
    }
    else { 
      retval = false;
    }
  }
  
  if ( debug_ ) { cout << "    FillByVRingHist(timestamp=" << timestamp << " , counter=" << counter << ") --> " << retval << endl; }
  
  return( retval );
}

string gm2ringsim::ringTrackerAnalyzer::TimestampName(int timestamp)
{
  if ( timestamp_map[timestamp] == kAllTime ) { return( "PerTime" ); }
  if ( timestamp_map[timestamp] == kPerTimeTwicePerTurn ) { return( "PerTimeTwicePerTurn" ); }
  if ( timestamp_map[timestamp] == kPerTimeOncePerTurn ) { return( "PerTimeOncePerTurn" ); }
  if ( timestamp_map[timestamp] == kPerTimeSingleHit ) { return( "PerTimeSingleHit" ); }
  
  return( "NONE" );
}

string gm2ringsim::ringTrackerAnalyzer::TrackerTimestampName(int timestamp)
{
  if ( timestamp == kGeneratedDist ) { return( "GeneratedDist" ); }
  if ( timestamp == kRemainingDist ) { return( "RemainingDist" ); }
  
  return( "NONE" );
}

bool gm2ringsim::ringTrackerAnalyzer::FillByTimestamp(int timestamp)
{
  if ( debug_ ) { cout << "    FillByTimestamp(timestamp=" << timestamp << ")" << endl; }
  int hist = timestamp;
  
  bool retval = true;

  if ( timestamp_map[hist] == kAllTime ) { return( false ); }
  if ( timestamp_map[hist] == kPerTimeTwicePerTurn ) { retval = false; }

  if ( debug_ ) { cout << "    FillByTimestamp(timestamp=" << timestamp << ") --> " << retval << endl; }
  
  return( retval );
}


bool gm2ringsim::ringTrackerAnalyzer::FillByTimestamp(int timestamp, int counter)
{
  if ( debug_ ) { cout << "    FillByTimestamp(timestamp=" << timestamp << " , counter=" << counter << ")" << endl; }
  
  int hist = timestamp;

  if ( !FillByTimestamp(timestamp) ) { return( false ); }
  
  if ( timestamp_map[hist] == kAllTime ) {
    return( false );
  }
  if ( timestamp_map[hist] == kPerTimeSingleHit ) {
    if ( counter == vring1p || counter == vring1p-1 || counter == vring1p+1 ) { return( true ); }
    return( false );
  }

  if ( timestamp_map[hist] == kPerTimeOncePerTurn ) { 
    if ( counter != vring1p ) { return( true ); }
    return( true );
  }
  
  if ( timestamp_map[hist] == kPerTimeTwicePerTurn ) { 
    if ( counter != vring1p && counter != vring2p ) { return( false ); }
    return( false );
  }

  return( true );
}


void gm2ringsim::ringTrackerAnalyzer::FillTimeRingTracker(int hist, double time, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly)
{

  if ( debug_ ) { cout << "FillTimeRingTracker(" << hist << ")" << endl; }
  if ( SaveVRingHits_ == false ) { return; }
  if ( hist < 0 ) { return; }
  if ( !fill_ ) { return; }

  if ( hist < Nringtrackers ) {
    h_RingTracker_Snapshot_Time[hist]->Fill(time);
    h_RingTracker_Snapshot_Rhat[hist]->Fill(rhat);
    h_RingTracker_Snapshot_Vhat[hist]->Fill(vhat);
    h_RingTracker_Snapshot_XprimeX[hist]->Fill(rhat, 1000*prhat);
    h_RingTracker_Snapshot_YprimeY[hist]->Fill(vhat, 1000*pvhat);
    h_RingTracker_Snapshot_Mom[hist]->Fill(mom/Pmagic());
    h_RingTracker_Snapshot_RhatY[hist]->Fill(rhat, vhat);
    h_RingTracker_Snapshot_Pol[hist]->Fill(polz);
    h_RingTracker_Snapshot_PolX[hist]->Fill(polx);
    h_RingTracker_Snapshot_PolY[hist]->Fill(poly);
    h_RingTracker_Snapshot_PolXY[hist]->Fill(polx, poly);
  }  
  if ( debug_ ) { cout << "FillTimeRingTracker(" << hist << ") - Done" << endl; }
}

void gm2ringsim::ringTrackerAnalyzer::FillTruthData(double *data, double xe_truth, double r_truth, double y_truth, double rprime_truth, double yprime_truth, double p, double x, double z, double theta, double t0, double time, double tDecay, double polz, double polx, double poly, double Polarization, int syshit, double spinphase)
{
  data[kXe] = xe_truth;
  data[kRhat] = r_truth;
  data[kVhat] = y_truth;
  data[kXprime] = rprime_truth;
  data[kYprime] = yprime_truth;
  data[kP] = p;
  data[kX] = x;
  data[kZ] = z;
  data[kTheta] = theta;
  data[kt0] = t0;
  data[kTime] = time;
  data[ktDecay] = tDecay;  
  TVector3 spin(polx, poly, polz);
  data[kPol_azimuth] = spin.Z()/spin.Mag();
  data[kPol_radial] = spin.X()/spin.Mag();
  data[kPol_vertical] = spin.Y()/spin.Mag();
  data[kPolarization] = Polarization;
  data[kSystemHit] = syshit;
  data[kSpinPhase] = spinphase;
}
  
void gm2ringsim::ringTrackerAnalyzer::FillTruthTracker(int hist, int particle, int counter, double *data, double *data_decay)
{
  if ( debug_ ) { cout << "FillTruthTracker(hist=" << TrackerTimestampName(hist) << ", " << truth_particle_names[particle] << ", counter=" << counter << ")" << endl; }
  if ( SaveTruthHits_ == false ) { return; }
  if ( particle == -1 ) { return; }
  
  int calonum = 0;
  if ( IsCaloElectron(particle) || IsStrawElectron(particle) ) { 
    calonum = counter;
    counter = 0;
  }
  else {
    calonum = -1;
    counter = 0;
  }

  FillTruthTracker(hist, particle, counter, calonum, data, data_decay);
  if ( debug_ ) { cout << "FillTruthTracker(hist=" << TrackerTimestampName(hist) << ", " << truth_particle_names[particle] << ", counter=" << counter << ") - Done" << endl; }

}

void gm2ringsim::ringTrackerAnalyzer::FillTruthTracker(int hist, int particle, int counter, int calonum, double *data, double *data_decay)
{
  if ( debug_ ) { cout << "FillTruthTracker(hist=" << TrackerTimestampName(hist) << " , " << truth_particle_names[particle] << ", counter=" << counter << ", calonum" << calonum << ")" << endl; }
  if ( hist >= Ntruthtimes ) { return; }
  if ( particle >= Nparticles ) { return; }
  if ( hist < 0 || particle < 0 ) { return; }
  if ( !fill_ ) { return; }
  if ( counter < 0 ) {
    cout << "FillTruthTracker(hist=" << hist << " , " << truth_particle_names[particle] << ", counter=" << counter << ", calonum" << calonum << ")::: Counter is less than zero!!!!!" << endl;
    return;
  }

  if ( debug_ ) {
    if ( data_decay ) { cout << "More particles." << endl; }
  }

  //-----------------
  // Decay Particle
  //-----------------
  double rhat = data[kRhat];
  double vhat = data[kVhat];
  double xprime = 1000*data[kXprime];
  double yprime = 1000*data[kYprime];
  double R = std::sqrt(data[kRhat]*data[kRhat] + data[kVhat]*data[kVhat]);
  double xe = data[kXe];
  double mom = data[kP]/Pmagic();
  double theta = data[kTheta];
  double pol_azimuth = data[kPol_azimuth];
  double pol_vertical = data[kPol_vertical];
  double pol_radial = data[kPol_radial];
  double spinphase = data[kSpinPhase];
  if ( 0 ) {
    if ( truth_particle_names[particle].find("StrawElectron") != string::npos ) {
      cout << truth_particle_names[particle] << "\t";
      cout << data[kP] << "\t";
      if ( data_decay ) {
	cout << data_decay[kP];
      }
      cout << endl;
    }
  }


  if ( data_decay ) {
    rhat = data_decay[kRhat];
    vhat = data_decay[kVhat];
    xprime = 1000*data_decay[kXprime];
    yprime = 1000*data_decay[kYprime];
    R = std::sqrt(rhat*rhat + vhat*vhat);
    xe = data_decay[kXe];
    mom = data_decay[kP]/Pmagic();
    theta = data_decay[kTheta];
    pol_azimuth = data_decay[kPol_azimuth];
    pol_vertical = data_decay[kPol_vertical];
    pol_radial = data_decay[kPol_radial];
    spinphase = data_decay[kSpinPhase];
  }
  

  //if ( particle == truth_particle_nums["StoredMuon"] ) {
//     cout << "==> (HIST=" << hist << ") , Found stored muon: " << "rhat=" << r_truth << "\ty=" << y_truth << "\t" << "p=" << p << endl;
//   }

 

  h_G4Tracker_Xe[hist][counter][particle]->Fill(xe);
  h_G4Tracker_Rhat[hist][counter][particle]->Fill(rhat);
  h_G4Tracker_Vhat[hist][counter][particle]->Fill(vhat);
  h_G4Tracker_R[hist][counter][particle]->Fill(R);
  h_G4Tracker_Xprime[hist][counter][particle]->Fill(xprime);
  h_G4Tracker_Yprime[hist][counter][particle]->Fill(yprime);
  h_G4Tracker_XprimeX[hist][counter][particle]->Fill(rhat, xprime);
  h_G4Tracker_YprimeY[hist][counter][particle]->Fill(vhat, yprime);
  h_G4Tracker_Mom[hist][counter][particle]->Fill(mom);
  h_G4Tracker_Energy[hist][counter][particle]->Fill(data[kP]/GeV);
  h_G4Tracker_RhatY[hist][counter][particle]->Fill(rhat, vhat);
  h_G4Tracker_XZ[hist][counter][particle]->Fill(data[kZ], data[kX]);
  h_G4Tracker_t0[hist][counter][particle]->Fill(data[kt0]);
  h_G4Tracker_Time[hist][counter][particle]->Fill((data[kTime])/1000);
  h_G4Tracker_Pol[hist][counter][particle]->Fill(data[kPol_azimuth]);
  h_G4Tracker_SpinPhase[hist][counter][particle]->Fill(spinphase);
  h_G4Tracker_PolX[hist][counter][particle]->Fill(pol_radial);
  h_G4Tracker_PolY[hist][counter][particle]->Fill(pol_vertical);
  h_G4Tracker_PolXY[hist][counter][particle]->Fill(pol_radial, pol_vertical);
  h_G4Tracker_Polt0[hist][counter][particle]->Fill(pol_azimuth, data[kt0]);
  h_G4Tracker_Theta[hist][counter][particle]->Fill(theta);
  h_G4Tracker_ThetaR[hist][counter][particle]->Fill(theta, R);
  h_G4Tracker_ThetaRhat[hist][counter][particle]->Fill(theta, rhat);
  h_G4Tracker_Polarization[hist][counter][particle]->Fill(spinphase);
  h_G4Tracker_SystemHits[hist][counter][particle]->Fill(data[kSystemHit]);

  if ( IsCaloElectron(particle) ) {
    double DeltaR = std::sqrt( (data[kX]-BirthElectronData[kX])*(data[kX]-BirthElectronData[kX]) + 
			       (data[kVhat]-BirthElectronData[kVhat])*(data[kVhat]-BirthElectronData[kVhat]) +
			       (data[kZ]-BirthElectronData[kZ])*(data[kZ]-BirthElectronData[kZ]) );    
    double drift_time = data[kTime]-BirthElectronData[kTime];
    double DeltaP = data[kP] - BirthElectronData[kP];

    double dTheta = data[kTheta]-BirthElectronData[kTheta];
    if ( dTheta > TMath::Pi() )  { dTheta -= TMath::TwoPi(); }
    if ( dTheta < -TMath::Pi() ) { dTheta += TMath::TwoPi(); }
    h_G4Tracker_NumCalo[hist][counter][particle]->Fill(calonum);
    h_G4Tracker_NumCalo_with_Mom[hist][counter][particle]->Fill(calonum, data[kP]/GeV);
    if ( data[kSystemHit] > 0 ) {
      h_G4Tracker_NumCalo_with_SystemHits[hist][counter][particle]->Fill(calonum);
      h_G4Tracker_Theta_with_SystemHits[hist][counter][particle]->Fill(data[kTheta]);
    }
    else {
      h_G4Tracker_NumCalo_without_SystemHits[hist][counter][particle]->Fill(calonum);
      h_G4Tracker_Theta_without_SystemHits[hist][counter][particle]->Fill(data[kTheta]);
    }

    h_G4Tracker_CaloNum_vs_DeltaP[hist][counter][particle]->Fill(calonum, DeltaP);
    h_G4Tracker_DeltaP[hist][counter][particle]->Fill(DeltaP);
    h_G4Tracker_DeltaR_vs_DeltaP[hist][counter][particle]->Fill(DeltaR, DeltaP);
    h_G4Tracker_Rhat_vs_DeltaP[hist][counter][particle]->Fill(data[kRhat], DeltaP);

    h_G4Tracker_CaloNum_vs_Energy[hist][counter][particle]->Fill(calonum, data[kP]/GeV);
    h_G4Tracker_CaloNum_vs_Theta[hist][counter][particle]->Fill(calonum, BirthElectronData[kTheta]);

    h_G4Tracker_DriftTime[hist][counter][particle]->Fill(drift_time);
    h_G4Tracker_DriftTime_vs_DeltaR[hist][counter][particle]->Fill(drift_time, DeltaR);
    h_G4Tracker_DeltaR[hist][counter][particle]->Fill(DeltaR);
    h_G4Tracker_DeltaR_vs_Energy[hist][counter][particle]->Fill(DeltaR, data[kP]/GeV);
    h_G4Tracker_DeltaTheta[hist][counter][particle]->Fill(dTheta);
    h_G4Tracker_DeltaTheta_vs_Energy[hist][counter][particle]->Fill(dTheta, data[kP]/GeV);
   
    if ( debug_ ) {
      if ( hist == kGeneratedDist ) {
	cout << truth_particle_names[particle] << "\t" << hist << "\t" << data[kP] << "\t" << drift_time << "\t" << DeltaR << "\t" << dTheta << endl;
      }
    }
  }



  if ( IsStrawElectron(particle) ) {
    int strawnum = calonum;
    //cout << "STnum = " << strawnum << endl;
    double DeltaR = std::sqrt( (data[kX]-BirthElectronData[kX])*(data[kX]-BirthElectronData[kX]) + 
			       (data[kVhat]-BirthElectronData[kVhat])*(data[kVhat]-BirthElectronData[kVhat]) +
			       (data[kZ]-BirthElectronData[kZ])*(data[kZ]-BirthElectronData[kZ]) );    
    double drift_time = data[kTime]-BirthElectronData[kTime];
    double DeltaP = data[kP] - BirthElectronData[kP];

    double dTheta = data[kTheta]-BirthElectronData[kTheta];
    if ( dTheta > TMath::Pi() )  { dTheta -= TMath::TwoPi(); }
    if ( dTheta < -TMath::Pi() ) { dTheta += TMath::TwoPi(); }
    h_G4Tracker_NumStraw[hist][counter][particle]->Fill(strawnum);
    h_G4Tracker_NumStraw_with_Mom[hist][counter][particle]->Fill(strawnum, data[kP]/GeV);
    h_G4Tracker_NumStrawStations[hist][counter][particle]->Fill(data[kSystemHit]);
    if ( data[kSystemHit] > 0 ) {
      h_G4Tracker_NumStraw_with_SystemHits[hist][counter][particle]->Fill(strawnum);
      h_G4Tracker_Theta_with_SystemHits[hist][counter][particle]->Fill(data[kTheta]);
    }
    else {
      h_G4Tracker_NumStraw_without_SystemHits[hist][counter][particle]->Fill(strawnum);
      h_G4Tracker_Theta_without_SystemHits[hist][counter][particle]->Fill(data[kTheta]);
    }

    h_G4Tracker_NumStrawStations_vs_Energy[hist][counter][particle]->Fill(data[kSystemHit], data[kP]/GeV);
    h_G4Tracker_StrawNum_vs_DeltaP[hist][counter][particle]->Fill(strawnum, DeltaP);
    h_G4Tracker_DeltaP[hist][counter][particle]->Fill(DeltaP);
    h_G4Tracker_DeltaR_vs_DeltaP[hist][counter][particle]->Fill(DeltaR, DeltaP);
    h_G4Tracker_Rhat_vs_DeltaP[hist][counter][particle]->Fill(data[kRhat], DeltaP);

    h_G4Tracker_StrawNum_vs_Energy[hist][counter][particle]->Fill(strawnum, data[kP]/GeV);
    h_G4Tracker_StrawNum_vs_Theta[hist][counter][particle]->Fill(strawnum, BirthElectronData[kTheta]);

    h_G4Tracker_DriftTime[hist][counter][particle]->Fill(drift_time);
    h_G4Tracker_DriftTime_vs_DeltaR[hist][counter][particle]->Fill(drift_time, DeltaR);
    h_G4Tracker_DeltaR[hist][counter][particle]->Fill(DeltaR);
    h_G4Tracker_DeltaR_vs_Energy[hist][counter][particle]->Fill(DeltaR, data[kP]/GeV);
    h_G4Tracker_DeltaTheta[hist][counter][particle]->Fill(dTheta);
    h_G4Tracker_DeltaTheta_vs_Energy[hist][counter][particle]->Fill(dTheta, data[kP]/GeV);
   
    if ( debug_ ) {
      if ( hist == kGeneratedDist ) {
	cout << truth_particle_names[particle] << "\t" << hist << "\t" << data[kP] << "\t" << drift_time << "\t" << DeltaR << "\t" << dTheta << endl;
      }
    }
  }

  if ( debug_ ) { cout << "FillTruthTracker(hist=" << TrackerTimestampName(hist) << " , " << truth_particle_names[particle] << ", counter=" << counter << ", calonum" << calonum << ") - Done" << endl; }
}


void gm2ringsim::ringTrackerAnalyzer::FillRingTrackerPhaseSpace(double time, int counter, double x, double y, double xprime, double yprime)
{
  if ( counter == vring1p ) {
    FillRingTrackerPhaseSpace(kAll, k1tracker, x, y, xprime, yprime);
    if ( time < 1*microsecond ) { FillRingTrackerPhaseSpace(kEarly, k1tracker, x, y, xprime, yprime); }
    if ( time < 5*microsecond ) { FillRingTrackerPhaseSpace(kMiddle, k1tracker, x, y, xprime, yprime); }
    if ( time < 20*microsecond ) { FillRingTrackerPhaseSpace(kLate, k1tracker, x, y, xprime, yprime); }
    
    if ( SavePhaseSpace_ ) {
      if ( time < 25*microsecond ) {
	out_ << time << "\t" << k1tracker << "\t" << x << "\t" << y << endl;
      }
    }
    //     if ( time < 1*microsecond ) { FillRingTrackerPhaseSpace(kEarly, k1tracker, time, x, y); }
    //     if ( time < 5*microsecond ) { FillRingTrackerPhaseSpace(kMiddle, k1tracker, time, x, y); }
    //     if ( time < 20*microsecond ) { FillRingTrackerPhaseSpace(kLate, k1tracker, time, x, y); }
  }
  
  if ( counter == vring1p || counter == vring2p ) {
    FillRingTrackerPhaseSpace(kAll, k2trackers, x, y, xprime, yprime);
    if ( time < 1*microsecond ) { FillRingTrackerPhaseSpace(kEarly, k2trackers, x, y, xprime, yprime); }
    if ( time < 5*microsecond ) { FillRingTrackerPhaseSpace(kMiddle, k2trackers, x, y, xprime, yprime); }
    if ( time < 20*microsecond ) { FillRingTrackerPhaseSpace(kLate, k2trackers, x, y, xprime, yprime); }

    if ( SavePhaseSpace_ ) {
      if ( time < 25*microsecond ) {
	out_ << time << "\t" << k2trackers << "\t" << x << "\t" << y << endl;
      }
    }
//     if ( time < 1*microsecond ) { FillRingTrackerPhaseSpace(kEarly, k2trackers, time, x, y); }
//     if ( time < 5*microsecond ) { FillRingTrackerPhaseSpace(kMiddle, k2trackers, time, x, y); }
//     if ( time < 20*microsecond ) { FillRingTrackerPhaseSpace(kLate, k2trackers, time, x, y); }
  } 
}


void gm2ringsim::ringTrackerAnalyzer::FillRingTrackerPhaseSpace(int timehist, int trackerhist, double time, double x, double y)
{
  if ( !fill_ ) { return; }
  return;
  int npx = g_VirtualRingTracker_X_PhaseSpace_vs_Time[timehist][trackerhist]->GetN();
  g_VirtualRingTracker_X_PhaseSpace_vs_Time[timehist][trackerhist]->SetPoint(npx, time, x);
  int npy = g_VirtualRingTracker_Y_PhaseSpace_vs_Time[timehist][trackerhist]->GetN();
  g_VirtualRingTracker_Y_PhaseSpace_vs_Time[timehist][trackerhist]->SetPoint(npy, time, y);
}


void gm2ringsim::ringTrackerAnalyzer::FillRingTrackerPhaseSpace(int timehist, int trackerhist, double x, double y, double xprime, double yprime)
{
  if ( !fill_ ) { return; }
  h_VirtualRingTracker_X_PhaseSpace_vs_Time[timehist][trackerhist]->Fill(x);  
  h_VirtualRingTracker_Xprime_PhaseSpace_vs_Time[timehist][trackerhist]->Fill(xprime*1000);  
  h_VirtualRingTracker_Y_PhaseSpace_vs_Time[timehist][trackerhist]->Fill(y);  
  h_VirtualRingTracker_Yprime_PhaseSpace_vs_Time[timehist][trackerhist]->Fill(yprime*1000);  
  h_VirtualRingTracker_XprimeX_PhaseSpace_vs_Time[timehist][trackerhist]->Fill(x, xprime*1000);  
  h_VirtualRingTracker_YprimeY_PhaseSpace_vs_Time[timehist][trackerhist]->Fill(y, yprime*1000);
}

  
void gm2ringsim::ringTrackerAnalyzer::FillRingTracker(int hist, int type, double theta, double xe, double xe_rms, double rhat, double vhat, double prhat, double pvhat, double mom, double polz, double polx, double poly, double zhat, double spinphase)
{
  if ( debug_ ) { cout << "FillRingTracker(hist=" << hist << " , type=" << type << ")" << endl; }
  if ( !FillByVRingHist(hist) ) { return; }
  if ( hist >= Nringtrackertimes ) { return; }
  if ( type >= Ntimevals ) { return; }
  if ( hist < 0 || type < 0 ) { return; }
  if ( !fill_ ) { return; }

  // hist [] ; 0 <=> Begin (t0) , 1 <=> End (t_{inf})
  // type [] ; 0 <=> GeneratedDist , 1 <=> RemainingDist
  
  if ( h_RingTracker_EqRad[hist][type] == NULL ) {
    cout << "h_RingTracker_EqRad[hist][type] is NULL" << endl;
    return;
  }
  
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
  h_RingTracker_SpinPhase[hist][type]->Fill(spinphase);
  h_RingTracker_Pol[hist][type]->Fill(polz);
  h_RingTracker_PolX[hist][type]->Fill(polx);
  h_RingTracker_PolY[hist][type]->Fill(poly);
  h_RingTracker_PolXY[hist][type]->Fill(polx, poly);
  h_RingTracker_Zhat[hist][type]->Fill(zhat);

  if ( debug_ ) { cout << "FillRingTracker(hist=" << hist << " , type=" << type << ") - Done" << endl; }
}


void gm2ringsim::ringTrackerAnalyzer::FillBeamScan(double time, double rhat, double vhat, double prhat, double pvhat, double theta, double p, double polx, double poly, double polz)
{
  if ( !fill_ ) { return; }
  int counter = 0;
  counter = (int)(Nstat*(theta/TMath::TwoPi()));
  int turn = (int)(time)/(149.15*ns);
  double dT = turn + theta/TMath::TwoPi();
  
  double xval = time/1000;
  for ( int hist = 0; hist < Ntruthtimestamps; hist++ ) {
    if ( !FillByTimestamp(hist) ) { continue; }
    
    if ( timestamp_map[hist] == kAllTime ||
	 timestamp_map[hist] == kPerTimeOncePerTurn ||
	 timestamp_map[hist] == kPerTimeSingleHit ||
	 timestamp_map[hist] == kPerTimeTwicePerTurn ) {
      xval = time/1000;
    }

    if ( FillByTimestamp(hist, counter) ) {
      FillBeamScan(hist, xval, rhat, vhat, prhat, pvhat, theta, p, polx, poly, polz);
    }
  }
}

void gm2ringsim::ringTrackerAnalyzer::FillBeamScan(int hist, double xval, double rhat, double y, double xprime, double yprime, double theta, double mom, double polx, double poly, double polz)
{
  if ( timestamp_map[hist] == kPerTimeSingleHit && Npart[truth_particle_nums["DecayMuon"]] > 0 && bs_tree_ ) {
    bs_time = xval;
    bs_vhat = y;
    bs_pol  = polz;
    bs_mom  = mom/Pmagic();
    bs_rhat = rhat;
    bs_tree_->Fill();
  }



  if ( !fill_ ) { return; }
  int m = 0;
  //cout << "Filling BeamScan: " << hist << endl;
  h_BeamScan_R_vs_Time[hist][m]->Fill(xval, TMath::Sqrt(rhat*rhat + y*y));
  h_BeamScan_Rhat_vs_Time[hist][m]->Fill(xval, rhat);
  h_BeamScan_Y_vs_Time[hist][m]->Fill(xval, y);
  h_BeamScan_Y2_vs_Time[hist][m]->Fill(xval, y*y);
  h_BeamScan_SpinPhase_vs_Time[hist][m]->Fill(xval, polz);
  h_BeamScan_PolX_vs_Time[hist][m]->Fill(xval, polx);
  h_BeamScan_PolY_vs_Time[hist][m]->Fill(xval, poly);
  h_BeamScan_Theta_vs_Time[hist][m]->Fill(xval, theta);
  h_BeamScan_Xprime_vs_Time[hist][m]->Fill(xval, 1000*xprime);
  h_BeamScan_Yprime_vs_Time[hist][m]->Fill(xval, 1000*yprime);
  h_BeamScan_Mom_vs_Time[hist][m]->Fill(xval, mom/Pmagic());

  if ( mom/Pmagic() > 1 ) { m = 1; }
  else { m = 2; }

  h_BeamScan_R_vs_Time[hist][m]->Fill(xval, TMath::Sqrt(rhat*rhat + y*y));
  h_BeamScan_Rhat_vs_Time[hist][m]->Fill(xval, rhat);
  h_BeamScan_Y_vs_Time[hist][m]->Fill(xval, y);
  h_BeamScan_Y2_vs_Time[hist][m]->Fill(xval, y*y);
  h_BeamScan_SpinPhase_vs_Time[hist][m]->Fill(xval, polz);
  h_BeamScan_PolX_vs_Time[hist][m]->Fill(xval, polx);
  h_BeamScan_PolY_vs_Time[hist][m]->Fill(xval, poly);
  h_BeamScan_Theta_vs_Time[hist][m]->Fill(xval, theta);
  h_BeamScan_Xprime_vs_Time[hist][m]->Fill(xval, 1000*xprime);
  h_BeamScan_Yprime_vs_Time[hist][m]->Fill(xval, 1000*yprime);
  h_BeamScan_Mom_vs_Time[hist][m]->Fill(xval, mom/Pmagic());

  if ( fabs(mom/Pmagic() - 1) < 0.0015 / 100 ) {
    m = 3;
    h_BeamScan_R_vs_Time[hist][m]->Fill(xval, TMath::Sqrt(rhat*rhat + y*y));
    h_BeamScan_Rhat_vs_Time[hist][m]->Fill(xval, rhat);
    h_BeamScan_Y_vs_Time[hist][m]->Fill(xval, y);
    h_BeamScan_Y2_vs_Time[hist][m]->Fill(xval, y*y);
    h_BeamScan_SpinPhase_vs_Time[hist][m]->Fill(xval, polz);
    h_BeamScan_PolX_vs_Time[hist][m]->Fill(xval, polx);
    h_BeamScan_PolY_vs_Time[hist][m]->Fill(xval, poly);
    h_BeamScan_Theta_vs_Time[hist][m]->Fill(xval, theta);
    h_BeamScan_Xprime_vs_Time[hist][m]->Fill(xval, 1000*xprime);
    h_BeamScan_Yprime_vs_Time[hist][m]->Fill(xval, 1000*yprime);
    h_BeamScan_Mom_vs_Time[hist][m]->Fill(xval, mom/Pmagic());
  }

}


void gm2ringsim::ringTrackerAnalyzer::FillTruthTurnTime(int particle, int calonum, double *data)
{
  bool debug = debug_;
  if ( debug ) { cout << "FillTruthTurnTime(particle=" << truth_particle_names[particle] << ", calonum=" << calonum << ")" << endl; }
  
  int counter = 0;
  counter = (int)(Nstat*(data[kTheta]/TMath::TwoPi()));
  if ( calonum >= 0 ) { counter = calonum; }
  int turn = (int)(data[kTime]-data[kt0])/(149.15*ns);
  double dT = turn + data[kTheta]/TMath::TwoPi();
  
  //cout << truth_particle_names[particle] << "\t" << calonum << "\t" << Nstat << "\t" << counter << "\t" << Nstat*(data[kTheta]/TMath::TwoPi()) << "\t" << vring1p << "\t" << data[kTheta] << endl;
  
  double xval;
  for ( int hist = 0; hist < Ntruthtimestamps; hist++ ) {
    //if ( truth_particle_names[particle] == "DecayMuon" ) {
    //  cout << "  Trying hist: " << hist << endl;
    //}
    if ( !FillByTimestamp(hist) ) { continue; }
    
    xval = dT;
    if ( timestamp_map[hist] == kAllTime ||
	 timestamp_map[hist] == kPerTimeOncePerTurn ||
	 timestamp_map[hist] == kPerTimeSingleHit ||
	 timestamp_map[hist] == kPerTimeTwicePerTurn ) {
      xval = (data[kTime])/1000;
    }

    if ( debug_ ) {
      if ( truth_particle_names[particle] == "DecayMuon" ) {
	cout << truth_particle_names[particle] << "\t" << TimestampName(hist) << "\t" << calonum << "\t" << Nstat << "\t" << counter << "\t" << Nstat*(data[kTheta]/TMath::TwoPi()) << "\t" << vring1p << "\t" << data[kTheta] << endl;
      }
    }
    if ( FillByTimestamp(hist, counter) ) {
      if ( debug_ ) {
	if ( truth_particle_names[particle] == "DecayMuon" ) {
	  cout << "  --> Filling." << endl;
	}      
      }
      FillTruthTurnTime(xval, hist, particle, data[kXe], data[kRhat], data[kVhat], data[kXprime], data[kYprime], data[kP], data[kTheta], data[kPol_azimuth], data[kPol_radial], data[kPol_vertical]);
    }
    else {
      if ( debug_ ) {
	if ( truth_particle_names[particle] == "DecayMuon" ) {
	  cout << "  --> Not Filling." << endl;
	}      
      }
    }
    FillNumTruthTurnTime(xval, hist, particle, counter, data[kVhat], data[kP]);
  }

  if ( debug ) { cout << "FillTruthTurnTime(particle=" << truth_particle_names[particle] << ", calonum=" << calonum << ") - Done" << endl; }

}

void gm2ringsim::ringTrackerAnalyzer::FillTruthTurnTime(double xval, int hist, int particle, double xe, double rhat, double y, double prhat, double pvhat, double mom, double theta, double polz, double polx, double poly)
{
  bool debug = debug_;
  if ( !FillByTimestamp(hist) ) { return; }
  if ( debug ) { cout << "  FillTruthTurnTime(hist=" << hist << " , particle=" << particle << ")" << endl; }
  if ( hist < 0 ) { return; }
  if ( particle < 0 ) { return; }
  if ( !fill_ ) { return; }

  if ( h_G4Tracker_Xe_vs_Time[hist][particle] == NULL ) {
    if ( debug ) { cout << "h_G4Tracker_Xe_vs_Time is NULL for " << truth_particle_names[particle] << " and hist=" << hist << endl; }
    return;
  }

  // Fix timestamp so it looks like we are looking at a single point in time
  //xval -= TimeOffset(timestamp_map[hist], counter);

  if ( 0 ) {
    if ( truth_particle_names[particle] == "DecayMuon" ) {
      cout << "Filling DecayMuon: " << hist << endl;
    }
  }

  h_G4Tracker_Xe_vs_Time[hist][particle]->Fill(xval, xe);
  h_G4Tracker_R_vs_Time[hist][particle]->Fill(xval, TMath::Sqrt(rhat*rhat + y*y));
  h_G4Tracker_Rhat_vs_Time[hist][particle]->Fill(xval, rhat);
  h_G4Tracker_Y_vs_Time[hist][particle]->Fill(xval, y);
  h_G4Tracker_Y2_vs_Time[hist][particle]->Fill(xval, y*y);
  h_G4Tracker_Pol_vs_Time[hist][particle]->Fill(xval, polz);
  h_G4Tracker_PolX_vs_Time[hist][particle]->Fill(xval, polx);
  h_G4Tracker_PolY_vs_Time[hist][particle]->Fill(xval, poly);
  h_G4Tracker_Theta_vs_Time[hist][particle]->Fill(xval, theta);
  h_G4Tracker_Prhat_vs_Time[hist][particle]->Fill(xval, 1000*prhat);
  h_G4Tracker_Pvhat_vs_Time[hist][particle]->Fill(xval, 1000*pvhat);
  h_G4Tracker_Mom_vs_Time[hist][particle]->Fill(xval, mom/Pmagic());
  h_G4Tracker_Energy_vs_Time[hist][particle]->Fill(xval, mom/GeV);  
}

void gm2ringsim::ringTrackerAnalyzer::FillNumTruthTurnTime(double xval, int hist, int particle, int counter, double y, double e)
{
  bool debug = debug_;
  if ( !FillByTimestamp(hist) ) { return; }
  if ( debug ) { cout << "  FillNumTruthTurnTime(hist=" << hist << " , particle=" << truth_particle_names[particle] << ")" << endl; }
  if ( hist < 0 ) { return; }
  if ( particle < 0 ) { return; }

  double tmod = 0.0;
  if ( IsStrawElectron(particle) || IsCaloElectron(particle) ) {
    tmod = TimeOffset(timestamp_map[hist]*10, counter);
  }
  else {
    tmod = TimeOffset(timestamp_map[hist], counter);
  }

  if ( debug ) { cout << "    tmod=" << tmod << endl; }

  double xvals[4];
  xvals[kAllStations] = xval - tmod;
  xvals[kAllStationsNoTimeOffset] = xval;
  double fa_offset = (2*G4UniformRand()-1) * 0.1492;
  xvals[kAllStationsNoFastRotation] = xvals[kAllStations] + fa_offset;
  xvals[kAllStationsNoTimeOffsetNoFastRotation] = xvals[kAllStationsNoTimeOffset] + fa_offset;

  if ( particle == truth_particle_nums["DecayElectron"] && timestamp_map[hist] == kPerTimeSingleHit && Npart[truth_particle_nums["DecayMuon"]] > 0 && tree_ ) {    
    pt_time = xval;
    pt_tmod = tmod;
    pt_faoff = fa_offset;
    pt_energy = e;
    pt_calo = counter;
    pt_vhat = DecayElectronData[kVhat];

    mu_vhat = DecayMuonData[kVhat];
    mu_pol  = DecayMuonData[kPol_azimuth];
    mu_mom  = DecayMuonData[kP];
    mu_rhat = DecayMuonData[kRhat];
    tree_->Fill();
  }

  if ( !fill_ ) { return; }

  if ( h_G4Tracker_TimeOffset_vs_Time[hist][particle] ) {
    h_G4Tracker_TimeOffset_vs_Time[hist][particle]->Fill(xvals[1] - xvals[0]);
  }
  if ( debug ) { cout << "    Filled toff." << endl; }
  if ( h_G4Tracker_NumCalo_vs_Time[hist][particle] ) {
    h_G4Tracker_NumCalo_vs_Time[hist][particle]->Fill(xval, counter);
  }
  if ( h_G4Tracker_NumStraw_vs_Time[hist][particle] ) {
    h_G4Tracker_NumStraw_vs_Time[hist][particle]->Fill(xval, counter);
  }
  if ( debug ) { cout << "    NumCalos." << endl; }
  
  

  // Do this for all "calos" or "stations"
  for ( int s = 0; s < 4; s++ ) {
    if ( debug_ ) { cout <<"    s=" << s << endl; }
    if ( h_G4Tracker_Num_vs_Time[hist][s][particle] ) {
      h_G4Tracker_Num_vs_Time[hist][s][particle]->Fill(xvals[s]);
    }

    if ( debug_ ) { cout << "    Filling EDM plots." << endl; }
    if ( keep_edm_plots ) {
      if ( y > 0 ) { 
	h_G4Tracker_Nup_vs_Time[hist][s][particle]->Fill(xvals[s], 1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(xvals[s], 1);
      }
      else if ( y < 0 ) { 
	h_G4Tracker_Ndown_vs_Time[hist][s][particle]->Fill(xvals[s], -1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(xvals[s], -1);
      }
      
      h_G4Tracker_NwghtE_vs_Time[hist][s][particle]->Fill(xvals[s], e/Pmagic());
    }
    if ( debug_ ) { cout << "    Filling EDM plots - Done." << endl; }
  }


  // Do this for 1 station
  if ( IsStrawElectron(particle) || IsCaloElectron(particle) ) {
    int s = CaloSection(counter);
    if ( debug ) { cout << "Filling single station [" << counter << "]" << endl; }
    if ( debug_ ) { cout <<"    s=" << s << endl; }
    if ( h_G4Tracker_Num_vs_Time[hist][s][particle] ) {
      h_G4Tracker_Num_vs_Time[hist][s][particle]->Fill(xvals[s-2]);
    }
    
    if ( keep_edm_plots ) {
      if ( debug ) { cout << "Filling single station EDM plots." << endl; }
      if ( y > 0 ) { 
	h_G4Tracker_Nup_vs_Time[hist][s][particle]->Fill(xvals[s-2], 1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(xvals[s-2], 1);
      }
      else if ( y < 0 ) { 
	h_G4Tracker_Ndown_vs_Time[hist][s][particle]->Fill(xvals[s-2], -1);
	h_G4Tracker_Nud_vs_Time[hist][s][particle]->Fill(xvals[s-2], -1);
      }
      
      h_G4Tracker_NwghtE_vs_Time[hist][s][particle]->Fill(xvals[s-2], e/Pmagic());
    }

    if ( debug ) { cout << "Filling single station. - Done" << endl; }
  }

  if ( debug ) { cout << "  FillNumTruthTurnTime(hist=" << hist << " , particle=" << truth_particle_names[particle] << ") - Done" << endl; }
}

void gm2ringsim::ringTrackerAnalyzer::FillTurnTime(double dT, double time, double t0, int counter, int trackID, double theta, double zhat, double xe, double xe_rms, double rhat, double y, double prhat, double pvhat, double mom, double polz, double polx, double poly, double e, double spinphase)
{
  bool debug = debug_;

  if ( !fill_ ) { return; }


  if ( debug ) { cout << "FillTurnTime(trackID=" << trackID << " , counter=" << counter << ")" << endl; }

  double xval;

  if ( trackID != 1 ) { return; }
  
  for ( int hist = 0; hist < Nringtimestamps; hist++ ) {
    if ( !FillByVRingHist(hist) ) { continue; }
    
    xval = dT;

    double tmod = 0.14915 * theta / TMath::TwoPi();


    if ( timestamp_map[hist] == kPerTimeOncePerTurn ||
	 timestamp_map[hist] == kPerTimeSingleHit ||
	 timestamp_map[hist] == kPerTimeTwicePerTurn ) { xval = (time)/1000;}

    

    if ( timestamp_map[hist] == kPerTimeTwicePerTurn ) {
      //xval -= (0.14915)*(0.25);
      if ( debug_ ) { cout << t0 << endl; }
      if ( counter > 0 && counter < 5 && 0 ) {
	cout << counter << "\t" << dT << "\t" << theta << "\t" << time/1000 << "\t\t";
	//cout << h_RingTracker_Rhat_vs_Time[hist]->FindBin(xval, rhat) << "\t";
	cout << h_RingTracker_Num_vs_Time[hist]->GetBinWidth(h_RingTracker_Num_vs_Time[hist]->FindBin(xval)) << "\t";
	cout << h_RingTracker_Num_vs_Time[hist]->GetBinCenter(h_RingTracker_Num_vs_Time[hist]->FindBin(xval)-1) << "\t";
	cout << h_RingTracker_Num_vs_Time[hist]->GetBinCenter(h_RingTracker_Num_vs_Time[hist]->FindBin(xval)) << "\t";
	cout << h_RingTracker_Num_vs_Time[hist]->GetBinCenter(h_RingTracker_Num_vs_Time[hist]->FindBin(xval)+1) << "\t";
	cout << h_RingTracker_Num_vs_Time[hist]->FindBin(xval) << "\t" << endl;
      }
    }

    if ( !FillByVRingHist(hist, counter) ) { continue; }
    
    h_RingTracker_EqRad_vs_Time[hist]->Fill(xval, xe);
    h_RingTracker_EqRadRMS_vs_Time[hist]->Fill(xval, xe_rms);
    h_RingTracker_Rhat_vs_Time[hist]->Fill(xval, rhat);
    h_RingTracker_Y_vs_Time[hist]->Fill(xval, y);
    h_RingTracker_Y2_vs_Time[hist]->Fill(xval, y*y);
    h_RingTracker_SpinPhase_vs_Time[hist]->Fill(xval, spinphase);
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

  if ( debug ) { cout << "FillTurnTime(trackID=" << trackID << " , counter=" << counter << ") - Done" << endl; }
}

void gm2ringsim::ringTrackerAnalyzer::FillKickDegree(int histtype, double degree_at_r0, double kick_at_r0, double mom)
{
  if ( !FillByVRingHist() ) { return; }

  if ( !fill_ ) { return; }


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
  bool debug = false;
  if ( debug ) { cout << "ComputeRmagicIntAngle(" << r1 << " , " << r2 << " , " << d1 << " , " << d2 << ")" << endl; }
  
  double rmagic_int_angle = -1.0;
  
  // Fit x @ theta0 to [ a - b*theta^{2} ]
  double a_r = ( r1 * d2*d2 ) - ( r2 * d1*d1 );
  double b_r = r2 - r1;
  if ( debug ) { cout << "  a - b = " << a_r << "  -  " << b_r << endl; }
  if ( debug ) { cout << "  d2*d2 - d1*d1 = " << d2*d2 << "  -  " << d1*d1 << " = " << d2*d2 - d1*d1 << endl; }
  a_r /= ( d2*d2 - d1*d1 );
  b_r /= ( d2*d2 - d1*d1 );
  if ( debug ) { cout << "  a - b = " << a_r << "  -  " << b_r << endl; }
  rmagic_int_angle = TMath::Sqrt(a_r/b_r);


  double slope_r_over_deg = (r1 - r2)/(d1 - d2);
  rmagic_int_angle = (0 - r1)/slope_r_over_deg + d1;
  if ( debug ) { cout << "Slope Method = " << rmagic_int_angle << endl; }
  
  if ( debug ) { cout << "ComputeRmagicIntAngle(" << r1 << " , " << r2 << " , " << d1 << " , " << d2 << ") = " << rmagic_int_angle << endl; }
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
  if ( debug_ ) { cout << "NewFixTurnCounter(" << *global_turn << " , " << mytrack_vol << " , " << ptrack_vol << ")" << endl; }
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
  if ( pdgid == kProtonID ) {
    if ( status == gm2ringsim::kBirth ) { particle = truth_particle_nums["BirthMuon"]; }
    if ( status == gm2ringsim::kDecay ) { particle = truth_particle_nums["LostMuon"]; }
    if ( status == gm2ringsim::kLost )  { particle = truth_particle_nums["LostMuon"]; }
    if ( status == gm2ringsim::kStore ) { particle = truth_particle_nums["StoredMuon"]; }
  }
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
  if ( pdgid == kProtonID ) { id_name = "proton"; }
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

bool gm2ringsim::ringTrackerAnalyzer::IsCaloElectron(int particle)
{
  if ( particle < 0 ) { return( false ); }

  if ( particle == truth_particle_nums["DecayElectron"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronQuadSection"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronOtherSection"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronKickerSection"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronEmptySection"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronEltEth"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronEgtEth"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronEgtHghEth"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronLongDriftTime"] ) { return( true ); }
  if ( particle == truth_particle_nums["DecayElectronShortDriftTime"] ) { return( true ); }

  return( false );
}

bool gm2ringsim::ringTrackerAnalyzer::IsStrawElectron(int particle)
{
  if ( particle < 0 ) { return( false ); }

  if ( particle == truth_particle_nums["StrawElectron"] ) { return( true ); }
  if ( particle == truth_particle_nums["StrawCaloElectron"] ) { return( true ); }
  if ( particle == truth_particle_nums["GoodStrawElectron"] ) { return( true ); }
  if ( particle == truth_particle_nums["GoodStrawCaloElectron"] ) { return( true ); }
  if ( particle == truth_particle_nums["GoodOneStrawCaloElectron"] ) { return( true ); }
  if ( particle == truth_particle_nums["GoodTwoStrawCaloElectron"] ) { return( true ); }

  return( false );
}

int gm2ringsim::ringTrackerAnalyzer::IsSomething(string name)
{
  if ( gm2ringsim::ringTrackerAnalyzer::IsKicker(name) ) { return( kKickerHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsQuad(name) ) { return( kQuadHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsCollimator(name) ) { return( kCollimatorHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsCryostat(name) ) { return( kCryostatHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsInflector(name) ) { return( kInflectorHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsVacuumChamber(name) ) { return( kVacuumHit ); }
  if ( gm2ringsim::ringTrackerAnalyzer::IsStrawTracker(name) ) { return( kStrawSystemHit ); }
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
  if ( gm2ringsim::ringTrackerAnalyzer::IsStrawTracker(name) && comp == kStrawSystemHit ) { return( true ); }
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


bool gm2ringsim::ringTrackerAnalyzer::IsVirtualRingStation(double theta, int *num)
{
  *num = (int)((12*theta/TMath::TwoPi()));
  double check = (*num)/12.0 * TMath::TwoPi();
  if ( fabs(check - theta) > 0.1 ) { 
    cout << "Bad Virtual Ring Station: " << theta << endl;
    return( false );
  }
  
  return( true );
}

bool gm2ringsim::ringTrackerAnalyzer::IsVirtualRingStation(string name, int *num)
{
  *num = -1;

  //cout << "Real:   " << name << endl;

  std::string ttLVName = artg4::addNumberToName("VirtualRingStationPV", vring1p);
  if ( name == ttLVName ) { *num = vring1p; return( true ); }

  std::string ttLVName2 = artg4::addNumberToName("VirtualRingStationPV", vring2p);
  if ( name == ttLVName2 ) { *num = vring2p; return( true ); }



  for(int arc=0; arc!=12; ++arc){
    if ( arc == vring1p ) { continue; }
    if ( arc == vring2p ) { continue; }
    std::string ttLVName = artg4::addNumberToName("VirtualRingStationPV", arc);
    //G4cout << ttLVName << G4endl;
    if ( name == ttLVName ) { *num = arc; return( true ); }
  }

  return ( false );
}

bool gm2ringsim::ringTrackerAnalyzer::IsStrawTracker(string name)
{
  if ( name.find("SingleStraw") != string::npos ) { return( true ); }

  return( false );
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
  if ( debug_ ) { cout << "New Event: analyze(): " << Nexp << endl; }
  Nexp++;
  //if ( Nexp < 33340 ) { return; }
  
  // Let's get the physical volume store. It's either in the run or the service
  auto const & pvs = artg4::dataFromRunOrService<artg4::PhysicalVolumeStoreData, artg4::PhysicalVolumeStoreService>(e.getRun(), pvsProducerLabel_, pvsInstanceLabel_);
  
  // Print it out
  //   for ( unsigned int i = 0; i < pvs.size(); ++i ) {
  //     mf::LogInfo("ParticleTrackAnalyzer") << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i);
  //     cout << "\t" << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i) << endl;
  //   }
  
  //return;
  
  
  //Ngen++;
  
  //if ( Ngen > 2000 ) { return; }
  
  // Extract the hits
  
  // Make the handle
  art::Handle< StrawArtRecordCollection > strawtrackerhitDataHandle;
  art::Handle< VirtualRingStationArtRecordCollection > ringtrackerhitDataHandle;
  art::Handle< TrackingActionArtRecordCollection > truthhitDataHandle;
  art::Handle< TrackingActionArtRecordCollection > truthbeamscanhitDataHandle;
  art::Handle< InflectorArtRecordCollection > inflectorhitDataHandle;
  art::Handle< RingArtRecordCollection > ringhitDataHandle;
  art::Handle< CaloArtRecordCollection > calohitDataHandle;
  
  // Fill the handle (note the use of the member data)
  if ( debug_ ) { std::cout<<"Ngen: " << Ngen << " \t" << "Event ID: "<<e.id()<<std::endl; }
  
  //e.getByLabel(hitModuleLabel_, instanceName_, hitDataHandle);
  if ( SaveStrawHits_ ) {
    e.getByLabel(strawtrackerhitModuleLabel_, strawtrackerinstanceName_, strawtrackerhitDataHandle);
  }
  if ( SaveVRingHits_  ) {
    e.getByLabel(ringtrackerhitModuleLabel_, ringtrackerinstanceName_, ringtrackerhitDataHandle);
  }
  if ( SaveTruthHits_ || 1 ) {
    e.getByLabel(truthhitModuleLabel_, truthinstanceName_, truthhitDataHandle);
  }
  if ( SaveBeamScanHits_ ) {
    e.getByLabel(truthbeamscanhitModuleLabel_, truthbeamscaninstanceName_, truthbeamscanhitDataHandle);
  }
  if ( SaveInfHits_ ) {
    e.getByLabel(inflectorhitModuleLabel_, inflectorinstanceName_, inflectorhitDataHandle);
  }
  if ( SaveRingHits_ ) {
    e.getByLabel(ringhitModuleLabel_, ringinstanceName_, ringhitDataHandle);
  }
  if ( SaveCaloHits_ ) {
    e.getByLabel(calohitModuleLabel_, caloinstanceName_, calohitDataHandle);
  }

  if ( debug_ ) { cout << "Got all handles." << endl; }
  
  // Resolve the handle

  StrawArtRecordCollection const & strawhits = *strawtrackerhitDataHandle;
  VirtualRingStationArtRecordCollection const & ringtrackerhits = *ringtrackerhitDataHandle;
  TrackingActionArtRecordCollection const & truthhits = *truthhitDataHandle;
  TrackingActionArtRecordCollection const & truthbeamscanhits = *truthbeamscanhitDataHandle;
  InflectorArtRecordCollection const & inflectorhits = *inflectorhitDataHandle;
  RingArtRecordCollection const & ringhits = *ringhitDataHandle;
  CaloArtRecordCollection const & calohits = *calohitDataHandle;



  if ( debug_ ) { cout << "Got all collections." << endl; }
  
  
  // Let's use the nice C++11 vector iteration
  //int i = 0;

  if ( debug_ ) { cout << "Getting VR hits." << G4endl; }
  unsigned int vringhits = -1;
  if ( SaveVRingHits_ ) { vringhits = ringtrackerhits.size(); }
  if ( debug_ ) { cout << "Getting VR hits - Done." << G4endl; }
  if ( debug_ ) {
    cout << "  ringTrackerAnalyzer: " << "There are " << pvs.size() << " entries in the PVS" << endl;
    if ( SaveRingHits_ ) {
      cout << "  ringTrackerAnalyzer: " << "There are " << ringhits.size() << " ring hits in this event." << endl;
    }
    if ( SaveCaloHits_ ) {
      cout << "  ringTrackerAnalyzer: " << "There are " << calohits.size() << " calo hits in this event." << endl;
    }
    if ( SaveStrawHits_ ) {
      cout << "  ringTrackerAnalyzer: " << "There are " << strawhits.size() << " straw hits in this event." << endl;
    }
    if ( SaveInfHits_ ) {
      cout << "  ringTrackerAnalyzer: " << "There are " << inflectorhits.size() << " inflector hits in this event." << endl;
    }
    cout << "  ringTrackerAnalyzer: " << "There are " << truthhits.size() << " truth hits in this event." << endl;
    if ( SaveBeamScanHits_ ) {
      cout << "  ringTrackerAnalyzer: " << "There are " << truthbeamscanhits.size() << " truth hits in this event." << endl;
    }
    if ( SaveVRingHits_  ) {
      cout << "  ringTrackerAnalyzer: " << "There are " << ringtrackerhits.size() << " virtual ring station hits in this event." << endl;
    }
  }


  //h_RingTracker_Nhits->Fill(vringhits);
  if ( vringhits > 0.98 * maxturns_ ) { 
    //cout << "Ring Hits[" << vringhits << "] more than allowed [" << 0.98*maxturns_ << "]." << endl;
    //return;
  }
  //h_CaloStation_Nhits->Fill(calohits.size());

  if ( debug_ ) { cout << "Entering event loop." << G4endl; }

  for ( int i = 0; i < 20; i++ ) {
    BirthElectronData[i] = -9999.9;
    StrawElectronData[i] = -9999.9;
    StrawCaloElectronData[i] = -9999.9;
    GoodStrawElectronData[i] = -9999.9;
    GoodStrawCaloElectronData[i] = -9999.9;
    GoodOneStrawCaloElectronData[i] = -9999.9;
    GoodTwoStrawCaloElectronData[i] = -9999.9;
    DecayElectronData[i] = -9999.9;
    DecayElectronOtherSectionData[i] = -9999.9;
    DecayElectronQuadSectionData[i] = -9999.9;
    DecayElectronKickerSectionData[i] = -9999.9;
    DecayElectronEmptySectionData[i] = -9999.9;
    BirthElectronEgtEthData[i] = -9999.9;
    DecayElectronEltEthData[i] = -9999.9;
    DecayElectronEgtHghEthData[i] = -9999.9;
    DecayElectronLongDriftTimeData[i] = -9999.9;
    DecayElectronShortDriftTimeData[i] = -9999.9;
    BirthMuonData[i] = -9999.9;
    DecayMuonData[i] = -9999.9;
    StoredMuonData[i] = -9999.9;
    LostMuonData[i] = -9999.9;
    LostDecayMuonData[i] = -9999.9;
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
	    if ( fill_ ) {
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
  double t0_init = -9999.9;
  //double t0_init = -9999.9;
  int radbin = -1;
  //TrackingActionArtRecordCollection thits;
  if ( SaveTruthHits_ ) {
  for ( int i = 0; i < 1; i++ ) {
    if ( i == 0 ) {
      ;//thits = *truthhitDataHandle;
    }
    else {
      ;//thits = *truthfinishedhitDataHandle;
    }

    for ( auto htruthdata : truthhits) {  
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
      t0_init    = time;
      radbin     = Rbin(rhat_init);
//       double x = (rhat + 7112) * TMath::Cos(theta);
//       double z = (rhat + 7112) * TMath::Sin(theta);
//       double y = vhat;

//       cout << theta_init << "\t" << x << "\t" << y << "\t" << z << endl;
    }
  }
  }

  if ( debug_ ) { cout << "Made it past saving initial truth information." << endl; }
  







  
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

  double kicktime = -1.0;
  double quadtime = -1.0;
  
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
  
  if ( FillByVRingHist() ) { 

    if ( debug_ ) { cout << "Looping over ring hits." << endl; }
    if ( debug_ ) { cout << "  Size = " << ringtrackerhits.size() << endl; }
    for ( auto hdata : ringtrackerhits) {
      if ( debug_ ) { cout << "Inside ring tracker hits." << endl; }
      int track_ID   = hdata.trackID;
      if ( debug_ ) { cout << "  trkID: " << track_ID << endl; }
      int track_volumeUID = hdata.volumeUID; 
      if ( debug_ ) { cout << "  volID: " << track_volumeUID << endl; }
      double theta = hdata.theta;
      if ( !IsVirtualRingStation(theta, &mytrack_vol) ) {      
	if ( debug_ ) { cout << "  theta: " << theta << endl; }
	if ( debug_ ) {
	  for ( unsigned int i = 0; i < pvs.size(); ++i ) {
	    mf::LogInfo("ParticleTrackAnalyzer") << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i);
	    cout << "\t" << "PhysicalVolume #" << i << " = " << pvs.stringGivenID(i) << endl;
	  }
	}
	string track_volname = pvs.stringGivenID(track_volumeUID);
	if ( debug_ ) { cout << "  Checking VRS: " << track_volname << endl; }
	IsVirtualRingStation(track_volname, &mytrack_vol);
	if ( debug_ ) { cout << "  Check Turn Counter: " << global_turn << " , " << mytrack_vol << " , " << pmytrack_vol << endl; }
      }
      NewFixTurnCounter(&global_turn, mytrack_vol, pmytrack_vol);
      pmytrack_vol = mytrack_vol;
      if ( debug_ ) { cout << "  Counter/Turn: " << mytrack_vol << " / " << global_turn << endl; }
      if ( SaveVRing1PlaneHits_ == true && SaveVRingHits_ == false ) {
	if ( mytrack_vol != vring1p ) { continue; }
      }


      if ( debug_ ) { cout << "  Counter/Turn: " << mytrack_vol << " / " << global_turn << endl; }
      if ( mytrack_vol == vring1p ) {
	Nsurvived[global_turn]++;
	if ( debug_ ) { cout << "\t" << Nsurvived[global_turn] << G4endl; }
      }
      
      double time = hdata.time;
      double mom = hdata.p;
      double momentum = mom;
      double rhat = hdata.rhat;
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
      double py = pvhat * mom;
      double px = prhat * mom;
      double pz = TMath::Sqrt(mom*mom - px*px - py*py);  


      //---------------
      // Keep running sum of equilibrium radius
      //---------------
      sum_EqRad += rhat;
      sum_EqRadRMS += rhat*rhat;
      N_EqRad++;
      double xe = sum_EqRad / N_EqRad;
      double xe_rms = TMath::Sqrt(sum_EqRadRMS/N_EqRad);



      //cout << global_turn << "\t" << mytrack_vol << "\t" << theta << "\t" << x << "\t" << z << endl;
      if ( global_turn == 0 && mytrack_vol == vring1p ) { tauc = time; }
      if ( global_turn == 1 && mytrack_vol == vring1p ) { 
	tauc = time - tauc; 
	//cout << "Rhat,Bin,Tau_{c} = " << rhat << "\t" << radbin << "\t" << tauc << endl;
	if ( debug_ ) { cout << "  Fill OmegaC-Radbin" << endl; }
	if ( TMath::Abs(mom/Pmagic()-1) < 0.001 || 1 ) {
	  if ( fill_ ) {
	    h_RingTracker_OmegaC_Radbin->Fill(tauc, rhat_init);
	    h_RingTracker_OmegaC_Mombin->Fill(tauc, mom_init/Pmagic());
	    if ( radbin >= 0 && radbin < 10 ) {
	      h_RingTracker_OmegaC[radbin]->Fill(tauc);
	    }
	  }
	  
	  if ( debug_ ) { cout << "  Fill OmegaC-Radbin - Done" << endl; }
	}
	tauc = time;
      }
      

      double pol_azimuth, pol_radial, pol_vertical;
      ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, polx, poly, polz, theta);
      double spinphase = TMath::ACos(pol_azimuth); //ComputeSpinPhase(polx, poly, polz, px, py, pz);

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
      if ( FillByVRingHist() ) { 
	double degree = 360 * theta / TMath::TwoPi();
	bool debug_kick = false;
	if ( track_ID == 1 ) {
	  if ( global_turn == 0 && degree_at_quad < 0 ) {	
	    //cout << degree << "\t" << rhat << endl;
	    if ( rhat < 50 && previous_rhat >= 50 ) {
	      degree_at_quad = ComputeQuadIntAngle(rhat, previous_rhat, degree, previous_degree);
	      quadtime = time - t0_init;
	      if ( fill ) { h_RingTracker_DegreeAtQuad[0]->Fill(degree_at_quad); }
	      if ( debug_kick ) { cout << "Deg@Quad = " << degree_at_quad << endl; }
	    }
	  }
    
	  if ( global_turn == 0 && degree_r_at_r0 < 0 ) {
	    if ( fill ) {
	      h_RingTracker_FirstTurnX[0]->Fill(degree, rhat);
	      h_RingTracker_FirstTurnY[0]->Fill(degree, vhat);
	    }
	    if ( rhat < 0 && previous_rhat >= 0 ) {
	      degree_r_at_r0 = ComputeRmagicIntAngle(rhat, previous_rhat, degree, previous_degree);
	      Ndegat0 += degree_r_at_r0;

	      if ( debug_kick ) { cout << "Deg = " << degree << " \tRhat/Rhat-1 = " << rhat << " , " << previous_rhat << endl; }
	      if ( debug_kick ) { cout << "Deg@R0 = " << degree_r_at_r0 << "\t"; }
	      
	      if ( degree_r_at_r0 > 75.2 && degree_r_at_r0 < 89.3 ) { Ninside_midkicker++; }
	      else if ( degree_r_at_r0 > 60.3 && degree_r_at_r0 < 104.3 ) { Ninside_allkicker++; }
	      else { Noutside_kickers++; }
	
	      kick_at_r0 = ComputeKickAtRmagicInt(prhat, previous_prhat, degree, previous_degree, degree_r_at_r0);
	      Nxpat0 += kick_at_r0;

	      if ( debug_kick ) { cout << "Kick@R0 = " << kick_at_r0 << endl; }

	      //if ( degree_r_at_r0 > 60.3 && degree_r_at_r0 < 104.3 ) {
		kicktime = time - t0_init;
		//}
	      if ( fill ) { FillKickDegree(0, degree_r_at_r0, kick_at_r0, mom); }
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
	    FillRingTracker(kBegin, kRemainingDist, theta, xe, xe_rms, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical, zhat, spinphase);  
	    FillRingTracker(kBegin, kGeneratedDist, theta_init, xe_init, xe_rms, rhat_init, vhat_init, prhat_init, pvhat_init, pmom_init, pol_azimuth, pol_radial, pol_vertical, zhat, spinphase);
	  }


	  FillRingTrackerPhaseSpace(time, mytrack_vol, rhat, vhat, prhat, pvhat);

      
	  if ( ( global_turn >= maxturnsreal_ - 150 - 1 && global_turn > 100 ) ||
	       ( maxturnsreal_ < 125 && global_turn > 50 ) ) {
	    Npass_finalavg++;
	    FillRingTracker(kFinal, kRemainingDist, theta, xe, xe_rms, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical, zhat, spinphase);
	    if ( FillByVRingHist() ) {	      
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
	      FillRingTracker(kFinal, kGeneratedDist, theta_init, xe_init, xe_rms_init, rhat_init, vhat_init, prhat_init, pvhat_init, mom_init, pol_azimuth, pol_radial, pol_vertical, zhat, spinphase);
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
	station = mytrack_vol;

// 	if ( global_turn == 0 ) {
// 	  if ( mytrack_vol >= 1 && mytrack_vol <= 7 ) { station = mytrack_vol - 1; }
// 	  if ( mytrack_vol == 10 ) { station = 7; }
// 	}
// 	else if ( global_turn == 1 ) {
// 	  if ( mytrack_vol == vring1p ) { station = 8; }
// 	}
	
	if ( station >= 0 ) {
	  if ( fill ) { 
	    if ( station < Nringtrackers ) {
	      FillTimeRingTracker(station, time, rhat, vhat, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical);
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
	FillTurnTime(dT, time, t0, mytrack_vol, track_ID, theta, zhat, xe, xe_rms, rhat, y, prhat, pvhat, mom, pol_azimuth, pol_radial, pol_vertical, e, spinphase);
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
      if ( debug_ ) { cout << "  RingHit[" << i << "/" << ringhits.size() << "]" << endl; }
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

      
      //
      // Check for positron hit in system
      //
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
  int positron_trackID = -1;
  for ( int i = 0; i < Nparticles; i++ ) { Npart[i] = 0; Npart_Rhat[i] = -99.9; Npart_Yhat[i] = -99.9; Npart_Mom[i] = -99.9; Npart_Time[i] = 0.0; Npart_PDGID[i] = 0.0; Npart_Status[i] = -1.0; }
  if ( SaveTruthHits_ ) {
    
    //TrackingActionArtRecordCollection const & truthhits = *truthhitDataHandle;
    TrackingActionArtRecordCollection thits;

    bool plot_birth_muon = true;
    //cout << endl;
    for ( int i = 0; i < 1; i++ ) {
      if ( i == 0 ) {
	thits = *truthhitDataHandle;
      }

      //cout << i << "\t" << thits.size() << endl;

      //for ( auto htruthdata : truthhits) { 
      for ( auto htruthdata : thits) {  
	//cout << "i = " << i << endl;
	std::string trackType = htruthdata.trackType;
	int trackID = htruthdata.trackID;
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
	
	if ( debug_ ) { cout << "  Truth Track: " << trackType << " w/ status = " << status << endl; }

	turn = (int)(time-t0)/(149.15*ns);
	double dT = turn + theta/TMath::TwoPi();

	if ( turn-1 >= maxturnsreal_ ) {;
	  //cout << "Turn [" << turn << "] greater than what's expected" << endl;
	  //return;
	}


	TVector3 pol_pre(polx_pre, poly_pre, polz_pre);
	double polx = pol_pre.Dot(xAxis);
	double poly = pol_pre.Dot(yAxis);
	double polz = pol_pre.Dot(zAxis);
	double pol_azimuth_truth, pol_radial_truth, pol_vertical_truth;
	ComputePolarization(&pol_azimuth_truth, &pol_radial_truth, &pol_vertical_truth, polx_pre, poly_pre, polz_pre, theta);
	double spinphase = TMath::ACos(pol_azimuth_truth); //ComputeSpinPhase(polx, poly, polz, px, py, pz);
	//double spinphase = ComputeSpinPhase(polx_pre, poly_pre, polz_pre, px, py, pz);
	

	if ( trackType == "proton" ) { pdgid = kProtonID; }
	if ( trackType == "mu+" ) { pdgid = kAntiMuonID; }
	if ( trackType == "e+" ) { pdgid = kPositronID; }
	if ( trackType == "mu-" ) { pdgid = kMuonID; }
	if ( trackType == "e-" ) { pdgid = kElectronID; }


	if ( debug_ ) { cout << "  i = " << i << "\t" << trackType << "\t" << time << "\t" << pdgid << "\t" << status << endl; }

	int part = -1;
    
	if ( trackType == "pi+" || trackType == "pi-" ) { part = 0; }
	else if ( trackType == "mu+" || trackType == "mu-" ) { part = 0; }
	else if ( trackType == "proton" ) { part = 0; }
	else if ( trackType == "e+" || trackType == "e-" ) { part = 1; }
	else { part = 2; }
    
	if ( status == gm2ringsim::kBirth ) {
	  if ( trackType == "mu+" || trackType == "mu-" || trackType == "proton" ) { 
	    mu.SetPxPyPzE(px, py, pz, e);
	    t0 = time;
	    if ( muongas_ ) { t0 = 0.0; }
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
	  
	
	int particle = FindParticle(pdgid, status);
	if ( debug_ ) { cout << "  Particle ID = " << particle << endl; }
	if ( particle == truth_particle_nums["DecayElectron"] ) { continue; }
	if ( particle < 0 ) { continue; }


    
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

	
	
	if ( 0 ) {
	  if ( particle == truth_particle_nums["DecayMuon"] ) {
	    double spinphase2 = ComputeSpinPhase(polx_pre, poly_pre, polz_pre, px, py, pz);
	    cout << "Spin Phase = " << spinphase2 << "\t" << pol_azimuth_truth << "\t" << TMath::ACos(pol_azimuth_truth) << endl;
	  }
	}

	//	cout << start_perfect_ << endl;
	if ( particle == truth_particle_nums["BirthMuon"] && !start_perfect_ ) {

	  TVector3 pol_ring_inf(polx_pre, poly_pre, polz_pre);
	  TVector3 mom_ring_inf(px, py, pz);
	  TVector3 pos_ring_inf(x, y, z);
	  pos_ring_inf -= beamStart_;
    
	  double x_inf = pos_ring_inf.Dot(xAxis);
	  double y_inf = pos_ring_inf.Dot(yAxis);
	  double r_inf = TMath::Sqrt(x_inf*x_inf + y_inf*y_inf);
	  double z_inf = pos_ring_inf.Dot(zAxis);
    
      //cout << inum << "\t" << x_acc << "\t" << y_acc << "\t" << z_acc << endl;

	  if ( 0 ) {
	    cout.precision(3);
	    cout << "px,py,pz = " << mom_ring_inf.X() << "\t" << mom_ring_inf.Y() << "\t" << mom_ring_inf.Z() << endl;
	    cout << "[x]   = " << xAxis.x() << "\t" << xAxis.y() << "\t" << xAxis.z() << endl;
	    cout << "[y]   = " << "0" << "\t1\t" << "0" << endl;
	    cout << "[z]   = " << zAxis.x() << "\t" << zAxis.y() << "\t" << zAxis.z() << endl;
	  }
    
	  double px_inf = mom_ring_inf.Dot(xAxis);
	  double py_inf = mom_ring_inf.Dot(yAxis);
	  double pz_inf = mom_ring_inf.Dot(zAxis);
	  //cout << "px',py',pz' = " << px_inf << "\t" << py_inf << "\t" << pz_inf << endl;

	  double xprime_inf = px_inf / pz_inf;
	  double yprime_inf = py_inf / pz_inf;
	  double zprime_inf = pz_inf / pz_inf;

	  pol_vertical_truth = pol_ring_inf.Dot(yAxis)/pol_ring_inf.Mag();
	  pol_radial_truth   = pol_ring_inf.Dot(xAxis)/pol_ring_inf.Mag();
	  pol_azimuth_truth  = pol_ring_inf.Dot(zAxis)/pol_ring_inf.Mag();


	  //cout << x_inf << "\t" << y_inf << endl;
	  //cout << xprime_inf << "\t" << yprime_inf << endl;

	  rhat_truth   = x_inf;
	  rprime_truth = xprime_inf;
	  if ( 0 ) {
	    cout << "[x', 1000*x'] = [" << xprime_inf << " , " << 1000*xprime_inf << "]" << endl;
	    cout << "[y', 1000*y'] = [" << yprime_inf << " , " << 1000*yprime_inf << "]" << endl;
	    cout << "[z', 1000*z'] = [" << zprime_inf << " , " << 1000*zprime_inf << "]" << endl;
	  }
	  y_truth      = y_inf;
	  yprime_truth = yprime_inf;
	  x_truth = x;
	  z_truth = z;
	}	  
	else {
	  //G4cout << "\tRhat = " << rhat << "\tRt = " << rhat_truth << "\tR = " << r << "\t" << y << "\t" << y_truth << "\t" << rprime_truth << "\t" << yprime_truth << G4endl;
	  rhat_truth = rhat;
	  rprime_truth = prhat;
	  yprime_truth = pvhat;
	  y_truth = y;
	  x_truth = x;
	  z_truth = z;
	  //TVector3 pol_ring(polx_pre, poly_pre, polz_pre);
	  //pol_vertical_truth = pol_ring.Dot(yAxis)/pol_ring.Mag();
	  //pol_radial_truth   = pol_ring.Dot(xAxis)/pol_ring.Mag();
	  //pol_azimuth_truth  = pol_ring.Dot(zAxis)/pol_ring.Mag();
	}	
	
	
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


	if ( particle == truth_particle_nums["StoredMuon"] ) {
	  if ( fabs(rhat_truth) > 50 || fabs(y_truth) > 50 ) {
	    status = gm2ringsim::kLost;
	    particle = truth_particle_nums["LostMuon"];
	    //cout << "Stored -> Lost" << endl;
	  }
	}
	if ( particle == truth_particle_nums["DecayMuon"] ) {
	  if ( fabs(rhat_truth) > 50 || fabs(y_truth) > 50 ) {
	    status = gm2ringsim::kLost;
	    particle = truth_particle_nums["LostDecayMuon"];
	    //cout << "Decay Muon at time [" << time << "] ---> is Lost because x,y = [" << rhat_truth << " , " << y_truth << "]" << endl;
	  }
	}

	if ( particle < 0 ) { continue; }

	if ( particle > -1 ) {
	  if ( debug_ ) { cout << "  Saving particle in Npart list w/ id = " << particle << endl; }
	  Npart[particle]++;
	  Npart_Rhat[particle] = rhat_truth;
	  Npart_Yhat[particle] = y_truth;
	  Npart_Mom[particle] = p;
	  Npart_PDGID[particle] = pdgid;
	  Npart_Status[particle] = status;
	  Npart_Time[particle] = time;
	}

	if ( particle == truth_particle_nums["DecayMuon"] ) {
	  if ( !FillByVRingHist() ) {
	    int turn_approx = 0;
	    while ( turn_approx <= maxturnsreal_ ) {
	      if ( time < turn_approx * 149.15*ns ) { break; }
	      Nsurvived[turn_approx]++;
	      turn_approx += 100;
	    }
	  }
	  tDecay = time;
	  xDecay = x;
	  zDecay = z;
	  // 149.15
	  if ( tDecay-t0 > maxturnsreal_*maxdecaytime_ ) {
	    ;//cout << "Time [" << tDecay/1000 << "] greater than what's expected [" << maxturnsreal_*maxdecaytime_ << "]" << endl;
	    return;
	  }
	}
	  

	if ( fabs(rhat_truth) > 50 || fabs(y_truth) > 50 ) {
	  if ( particle == truth_particle_nums["StoredMuon"] ) {
	    cout << "Rhat-Y: " << rhat_truth << "\t" << y_truth << endl;
	    cout << "Time = " << time << endl;
	  }
	}

	
	int syshits = 0;
	//bool kickerhit = false;
	//bool quadhit = false;
	if ( particle == truth_particle_nums["LostMuon"] ) {
	  if ( SaveRingHits_ ) {
	    //cout << "Found lost mu+, looking for ring hit from col size = " << ringhits.size() << "." << endl;
	    for ( unsigned int i = 0; i < ringhits.size(); ++i ) {
	      RingArtRecord ringhit = ringhits[i];
	      int ringtrackID   = ringhit.trackID;
	      //cout << "  " << ringtrackID << "  ?  " << trackID << endl;
	      if ( ringtrackID == trackID && syshits == 0 ) {
		int ringvolumeUID = ringhit.volumeUID;
		string ringname = pvs.stringGivenID(ringvolumeUID);
		int syshit = IsSomething(ringname);
		//if ( syshit == kKickerHit ) { kickerhit = true; }
		//if ( syshit == kQuadHit ) { quadhit = true; }	      
		syshits = syshit + 1;
	      }
	    }
	  }
	}

	if ( debug_ ) { cout << "Filling Truth data for " << truth_particle_names[particle] << endl; }

	if ( particle == truth_particle_nums["BirthMuon"] ) {
	  Ngen++;
	  FillTruthData(BirthMuonData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	  
	}
	if ( particle == truth_particle_nums["LostMuon"] ) {
	  Nlost++;
	  FillTruthData(LostMuonData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	  
	  lost_time = time;
	  lost_dist = TMath::Sqrt(y_truth*y_truth + rhat_truth*rhat_truth);
	  tree_lost_->Fill();
	}
	if ( particle == truth_particle_nums["LostDecayMuon"] ) {
	  Nlostdecayed++;
	  FillTruthData(LostDecayMuonData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	  
	  lost_time = time;
	  lost_dist = TMath::Sqrt(y_truth*y_truth + rhat_truth*rhat_truth);
	  tree_lost_->Fill();
	}
	if ( particle == truth_particle_nums["StoredMuon"] ) {
	  Nstored++;
	  FillTruthData(StoredMuonData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	  
	}
	if ( particle == truth_particle_nums["DecayMuon"] ) {
	  Ndecayed++;
	  FillTruthData(DecayMuonData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	  
	}
	if ( particle == truth_particle_nums["BirthElectron"] ) {
	  FillTruthData(BirthElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	  Npositrons++;
	  if ( p > 1.8*GeV ) {
	    int tmp_particle = truth_particle_nums["BirthElectronEgtEth"];	
	    Npart[tmp_particle]++;
	    Npart_Rhat[tmp_particle] = rhat_truth;
	    Npart_Yhat[tmp_particle] = y_truth;
	    Npart_Mom[tmp_particle] = p;
	    Npart_PDGID[tmp_particle] = pdgid;
	    Npart_Status[tmp_particle] = status;
	    Npart_Time[tmp_particle] = time;
	    FillTruthData(BirthElectronEgtEthData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	 
	  }

	  positron_trackID = trackID;
	}

	if ( debug_ ) { cout << "Filling Truth data for " << truth_particle_names[particle] << " --- Done. " << endl; }
	
	
	if ( fill ) {
	  ;//FillTruthTurnTime(dT, counter, status, pdgid, time, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, theta, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, e);
	}
      } // loop over G4 track
    } // loop over track types
    
    
    bool show_particle = false;
    if ( debug_ ) { show_particle = true; }
    for ( int i = 0; i < Nparticles; i++ ) {
      if ( Npart[i] > 1 ) { cout << "You have more than one particle of " << truth_particle_names[i] << endl; show_particle = true; }
    }
    if ( show_particle ) {
      for ( int i = 0; i < Nparticles; i++ ) {
	if  ( Npart[i] > 0 ) {
	  cout << truth_particle_names[i] << " : " << Npart[i] << "\tRhat=" << Npart_Rhat[i] << "\tY=" << Npart_Yhat[i] << "\tMom=" << Npart_Mom[i] << "\tID=" << Npart_PDGID[i] << "\tStatus=" << Npart_Status[i] << "\tTime=" << Npart_Time[i] << endl;
	}
      }      
      cout << endl;
    }


//     if ( Npart[truth_particle_nums["StoredMuon"]] == 1 ) {
//       for ( int i = 0; i < Nparticles; i++ ) {
// 	cout << truth_particle_names[i] << " : " << Npart[i] << endl;
//       }
//     }


    if ( 0 ) {
      for ( int i = 0; i < Nparticles; i++ ) {
	cout << truth_particle_names[i] << " : " << Npart[i] << "\tRhat=" << Npart_Rhat[i] << "\tY=" << Npart_Yhat[i] << "\tMom=" << Npart_Mom[i] << "\tID=" << Npart_PDGID[i] << "\tStatus=" << Npart_Status[i] << "\tTime=" << Npart_Time[i] << endl;
      }   
      
      cout << "Ring Hits: " << ringtrackerhits.size() << endl;
      for ( auto hdata : ringtrackerhits) {
	int track_ID   = hdata.trackID;
	double theta = hdata.theta;
	if ( !IsVirtualRingStation(theta, &mytrack_vol) ) {  
	  int track_volumeUID = hdata.volumeUID; 
	  string track_volname = pvs.stringGivenID(track_volumeUID);    
	  IsVirtualRingStation(track_volname, &mytrack_vol);
	}
	NewFixTurnCounter(&global_turn, mytrack_vol, pmytrack_vol);
	
	pmytrack_vol = mytrack_vol;	
	double time = hdata.time;
	double mom = hdata.p;
	double momentum = mom;
	double rhat = hdata.rhat;
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
      
      double Enmax = (105.7*105.7 + 0.511*0.511)/(2*105.7);

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
      
      
      double x = el.E() / Enmax;
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

  if ( debug_ ) { cout << "Made it past truth polarization hits." << endl; }







  //-------------------
  //
  // Calo Hits
  //
  //-------------------
  int calonum = -1;
  int ncalomuhits = 0;
  if ( SaveCaloHits_ ) {
    //for ( auto ringhit : ringhits) {
    bool plot_birth_electron = true;
    if ( debug_ ) { cout << "Getting calo hits" << endl; }
    if ( debug_ ) { cout << "  Calo Hits Size = " << calohits.size() << endl; }
    
    for ( unsigned int i = 0; i < calohits.size(); ++i ) {
      CaloArtRecord calohit = calohits[i];
      if ( debug_ ) { cout << "  Got Calo Hit [" << i << "]" << endl; }
      double time = calohit.time;
      double x = calohit.x;
      double y = calohit.y;
      double z = calohit.z;
      double px = calohit.px;
      double py = calohit.py;
      double pz = calohit.pz;
      double num = calohit.caloNum;
      int trackID = calohit.trackID;
      int syshits = 0;


      TVector3 mom_calo(px, py, pz);
      double mom = mom_calo.Mag();
      TVector3 pos_calo(x, y, z);
      double rhat = ComputeRhat(&pos_calo);
      double vhat = ComputeVhat(&pos_calo);
      G4double prhat = ComputePrhat(&pos_calo, &mom_calo);
      G4double pvhat = ComputePvhat(&pos_calo, &mom_calo);
      double theta = ComputeTheta(x, z);
      //cout << "CaloHit(" << num << ", " << i << "/" << calohits.size() << "): ID, x,y,z: " << trackID << "(" << ")\t" << x << " , " << y << " , " << z << endl;
      //continue;
      //cout << "Rhat = " << rhat << "\t" << "Vhat = " << vhat << "\tP = " << mom << endl;

      TrackingActionArtRecord poshit, muhit;
      bool found_positron_trk = false;
      bool found_muon_trk = false;
      //cout << "Tracks" << endl;
      int nt = 0;

      if ( debug_ ) { cout << "  Checking for e+ or mu+." << endl; }

      
      for ( auto htruthdata : truthhits ) {
	if ( debug_ ) { cout << "     " << nt++ << "\t" << htruthdata.trackType << "\t" << htruthdata.trackID << "\t" << htruthdata.p << "\t" << htruthdata.time << endl; }
	if ( htruthdata.trackID == trackID && htruthdata.trackType == "e+" ) {	  
	  poshit = htruthdata;
	  found_positron_trk = true;
	  if ( debug_ ) { cout << "      Found e+" << endl; }
	}
	if ( htruthdata.trackID == trackID && htruthdata.trackType == "mu+" ) {	  
	  muhit = htruthdata;
	  found_muon_trk = true;
	  if ( debug_ ) { cout << "      Found mu+" << endl; }
	}
      }

      if ( debug_ ) { cout << "  Checking for e+ or mu+. Done." << endl; }

      bool kickerhit = false;
      bool quadhit = false;
      bool collhit = false;
      bool vachit = false;
      bool otherhit = false;
      bool nohit = true;
      if ( found_positron_trk ) {
	if ( SaveRingHits_ ) {
	  if ( debug_ ) { cout << "  Looping over ring hits since we found the positron." << endl; }
	  int pvsize = (int)pvs.size();
	  if ( debug_ ) {
	    cout << "Found e+, looking for ring hit from col size = " << ringhits.size() << ". PVs = " << pvsize << endl;
	  }
	  if ( 1 ) {
	    for ( unsigned int i = 0; i < ringhits.size(); ++i ) {
	      RingArtRecord ringhit = ringhits[i];
	      int ringtrackID   = ringhit.trackID;
	      double deltaE = ringhit.deltaE;
	      if ( deltaE < 0 ) { deltaE = -1*ringhit.deltaE; }
	      if ( deltaE <= 0.0 ) { deltaE = 1e-10; }
	      if ( deltaE < 0.1 ) { continue; }
	      
	      if ( debug_ ) { cout << "        RingTrack=" << ringtrackID << "  ?  CaloTrack=" << trackID << endl; }
	      if ( ringtrackID == trackID ) {
		if ( debug_ ) { cout << "          Found ring track." << endl; }
		int ringvolumeUID = ringhit.volumeUID;
		if ( ringvolumeUID >= pvsize ) { continue; }
		if ( debug_ ) { cout << "          Found ring track w/ UID = " << ringvolumeUID << endl; }
		string ringname = pvs.stringGivenID(ringvolumeUID);
		if ( debug_ ) { cout << "          Found ring track w/ ringname = " << ringname << " and dE = " << deltaE << endl; }
		int syshit = IsSomething(ringname);
		if ( syshit == kKickerHit ) { kickerhit = true; nohit = false; }
		if ( syshit == kQuadHit ) { quadhit = true; nohit = false; }
		if ( syshit == kCollimatorHit ) { collhit = true; otherhit = true; nohit = false; }
		if ( syshit == kVacuumHit ) { vachit = true; otherhit = true; nohit = false; }	    
		if ( syshit == kCryostatHit ) { otherhit = true; nohit = false; }	 	    	    
		if ( syshit == kStrawSystemHit ) { otherhit = true; nohit = false; }		    	    
		if ( syshit == kCaloSystemHit ) { otherhit = true; nohit = false; }	 	    
		if ( syshit == kInflectorHit ) { otherhit = true; nohit = false; }
		//if ( nohit && deltaE > 1 ) { otherhit = true; 
		syshits++;
		
		if ( nohit ) {
		  if ( debug_ ) { cout << "          Found ring track w/ ringname = " << ringname << " and dE = " << deltaE << endl; }
		}
	      }
	    }
	  }

	  if ( debug_ ) {
	    if ( quadhit ) { cout << "  Found e+ in quads." << endl; }
	    if ( kickerhit ) { cout << "  Found e+ in kicker." << endl; }
	    if ( collhit ) { cout << "  Found e+ in collimator." << endl; }
	  }


	}


	if ( quadhit ) { calosyshits[(int)num][0]++; }
	if ( kickerhit ) { calosyshits[(int)num][1]++; }
	if ( collhit ) { calosyshits[(int)num][2]++; }
	if ( vachit ) { calosyshits[(int)num][3]++; }
	if ( otherhit ) { calosyshits[(int)num][4]++; }
	if ( nohit ) { calosyshits[(int)num][5]++; }
      }
      
      //TrackingActionArtRecord thit = (*truthhitDataHandle)[trackID];
      if ( found_muon_trk ) {
	if ( debug_ ) { 
	  cout << "  Found muon in calo[" << num << "] at time [" << time << "] w/ |P|" << mom << endl;
	}
	ncalomuhits++;
	int pdgid = kPositronID;
	int particle = truth_particle_nums["LostDecayMuon"];
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
	double spinphase = TMath::ACos(pol_azimuth_truth); //ComputeSpinPhase(polx, poly, polz, px, py, pz);
	//double spinphase = ComputeSpinPhase(0, 0, 0, px, py, pz);
	FillTruthData(LostDecayMuonData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	
	Npart[particle]++;
	Npart_Rhat[particle] = rhat_truth;
	Npart_Yhat[particle] = y_truth;
	Npart_Mom[particle] = p;
	Npart_PDGID[particle] = pdgid;
	Npart_Status[particle] = status;
	Npart_Time[particle] = time;
      }

      if ( found_positron_trk ) {
	std::string trk_trackType = poshit.trackType;
	if ( debug_ ) { cout << "CaloHit(): " << "Found " << trk_trackType << endl; }
	int trk_trackID = poshit.trackID;
	int trk_status = poshit.status;
	double trk_rhat = poshit.rhat;
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
	double spinphase = TMath::ACos(pol_azimuth_truth); //ComputeSpinPhase(polx, poly, polz, px, py, pz);
	//double spinphase = ComputeSpinPhase(0, 0, 0, px, py, pz);
	  
	
	if ( particle >= 0 ) {
	  if ( particle == truth_particle_nums["DecayElectron"] ) {
	    //cout << "SYSHITS = " << syshits << endl;
	    Ncalopositrons++;
	    FillTruthData(DecayElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	    if ( time - BirthElectronData[kTime] > 40 ) {
	      FillTruthData(DecayElectronLongDriftTimeData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	      int tmp_particle = truth_particle_nums["DecayElectronLongDriftTime"];
	      if ( tmp_particle >= 0 ) {
		Npart[tmp_particle]++;
		Npart_Rhat[tmp_particle] = rhat_truth;
		Npart_Yhat[tmp_particle] = y_truth;
		Npart_Mom[tmp_particle] = p;
		Npart_PDGID[tmp_particle] = pdgid;
		Npart_Status[tmp_particle] = status;
		Npart_Time[tmp_particle] = time;
	      }
	    }
	    else {
	      FillTruthData(DecayElectronShortDriftTimeData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	      int tmp_particle = truth_particle_nums["DecayElectronShortDriftTime"];
	      if ( tmp_particle >= 0 ) {	
		Npart[tmp_particle]++;
		Npart_Rhat[tmp_particle] = rhat_truth;
		Npart_Yhat[tmp_particle] = y_truth;
		Npart_Mom[tmp_particle] = p;
		Npart_PDGID[tmp_particle] = pdgid;
		Npart_Status[tmp_particle] = status;
		Npart_Time[tmp_particle] = time;
	      }
	    }
	    if ( p > 1.8*GeV ) {
	      NcalopositronsEgtEth++;
	      int tmp_particle = truth_particle_nums["DecayElectronEgtEth"];	
	      if ( tmp_particle >= 0 ) {
		Npart[tmp_particle]++;
		Npart_Rhat[tmp_particle] = rhat_truth;
		Npart_Yhat[tmp_particle] = y_truth;
		Npart_Mom[tmp_particle] = p;
		Npart_PDGID[tmp_particle] = pdgid;
		Npart_Status[tmp_particle] = status;
		Npart_Time[tmp_particle] = time;
		FillTruthData(DecayElectronEgtEthData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	 
	      }
	    }
	    else {
	      int tmp_particle = truth_particle_nums["DecayElectronEltEth"];	
	      if ( tmp_particle >= 0 ) {
		Npart[tmp_particle]++;
		Npart_Rhat[tmp_particle] = rhat_truth;
		Npart_Yhat[tmp_particle] = y_truth;
		Npart_Mom[tmp_particle] = p;
		Npart_PDGID[tmp_particle] = pdgid;
		Npart_Status[tmp_particle] = status;
		Npart_Time[tmp_particle] = time;
		FillTruthData(DecayElectronEltEthData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	      }
	    }
	    if ( p > 2.6*GeV ) {
	      int tmp_particle = truth_particle_nums["DecayElectronEgtHghEth"];	
	      if ( tmp_particle >= 0 ) {
		Npart[tmp_particle]++;
		Npart_Rhat[tmp_particle] = rhat_truth;
		Npart_Yhat[tmp_particle] = y_truth;
		Npart_Mom[tmp_particle] = p;
		Npart_PDGID[tmp_particle] = pdgid;
		Npart_Status[tmp_particle] = status;
		Npart_Time[tmp_particle] = time;
		FillTruthData(DecayElectronEgtHghEthData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	 
	      }
	    }
	    calonum = num;
	  }
	  
	  if ( particle >= 0 ) {
	    Npart[particle]++;
	    Npart_Rhat[particle] = rhat_truth;
	    Npart_Yhat[particle] = y_truth;
	    Npart_Mom[particle] = p;
	    Npart_PDGID[particle] = pdgid;
	    Npart_Status[particle] = status;
	    Npart_Time[particle] = time;
	  }
	}

	if ( kickerhit ) {
	  int tmp_particle = truth_particle_nums["DecayElectronKickerSection"];	
	  if ( tmp_particle >= 0 ) {
	    Npart[tmp_particle]++;
	    Npart_Rhat[tmp_particle] = rhat_truth;
	    Npart_Yhat[tmp_particle] = y_truth;
	    Npart_Mom[tmp_particle] = p;
	    Npart_PDGID[tmp_particle] = pdgid;
	    Npart_Status[tmp_particle] = status;
	    Npart_Time[tmp_particle] = time;
	    FillTruthData(DecayElectronKickerSectionData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	 
	  }
	}
	if ( otherhit ) {
	  int tmp_particle = truth_particle_nums["DecayElectronOtherSection"];	
	  if ( tmp_particle >= 0 ) {
	    Npart[tmp_particle]++;
	    Npart_Rhat[tmp_particle] = rhat_truth;
	    Npart_Yhat[tmp_particle] = y_truth;
	    Npart_Mom[tmp_particle] = p;
	    Npart_PDGID[tmp_particle] = pdgid;
	    Npart_Status[tmp_particle] = status;
	    Npart_Time[tmp_particle] = time;
	    FillTruthData(DecayElectronOtherSectionData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	 
	  }
	}
	if ( quadhit ) {
	  int tmp_particle = truth_particle_nums["DecayElectronQuadSection"];	
	  if ( tmp_particle >= 0 ) {
	    Npart[tmp_particle]++;
	    Npart_Rhat[tmp_particle] = rhat_truth;
	    Npart_Yhat[tmp_particle] = y_truth;
	    Npart_Mom[tmp_particle] = p;
	    Npart_PDGID[tmp_particle] = pdgid;
	    Npart_Status[tmp_particle] = status;
	    Npart_Time[tmp_particle] = time;
	    FillTruthData(DecayElectronQuadSectionData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	 
	  }
	}
	if ( nohit ) {
	  int tmp_particle = truth_particle_nums["DecayElectronEmptySection"];	
	  if ( tmp_particle >= 0 ) {
	    Npart[tmp_particle]++;
	    Npart_Rhat[tmp_particle] = rhat_truth;
	    Npart_Yhat[tmp_particle] = y_truth;
	    Npart_Mom[tmp_particle] = p;
	    Npart_PDGID[tmp_particle] = pdgid;
	    Npart_Status[tmp_particle] = status;
	    Npart_Time[tmp_particle] = time;
	    FillTruthData(DecayElectronEmptySectionData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);	 
	  }
	}
      }

      if ( debug_ ) { cout << "  Got Calo Hit [" << i << "] - Done" << endl; }
    }
  }

  if ( debug_ ) { cout << "Made it past calo hits." << endl; }




  //-------------------
  //
  // Straw Hits
  //
  //-------------------
  int stationnum = -1;
  int nstrawmuhits = 0;
  if ( SaveStrawHits_ ) {
    if ( debug_ ) { cout << "Getting straw hits" << endl; }
    if ( debug_ ) { cout << "  Straw Hits Size = " << strawhits.size() << endl; }

    int Nplanes = 0;
    int lastplane = -1;
    vector<int> stations;
    int uv_layers[30];
    for ( int s = 0; s < 30; s++ ) {
      uv_layers[s] = 0;
    }
    double lastPos[3] = {0, 0, 0};
    double lastMom[3] = {0, 0, 0};
    double lastTime = 0.0;   
    //cout << "Straw Hits Size = " << strawhits.size() << endl;
    for ( unsigned int i = 0; i < strawhits.size(); ++i ) {
      StrawArtRecord strawhit = strawhits[i];

      int trackID = strawhit.trackID;
      int volumeUID = strawhit.volumeUID;      
      string strawname = "dummy";
      if ( volumeUID < (int)pvs.size() ) {
	strawname = pvs.stringGivenID(volumeUID);
      }

      if ( debug_ ) { cout << "     Strawname: " << strawname << "\t" << "Track: " << trackID << "\t" << positron_trackID << endl; }
      if ( trackID != positron_trackID ) { continue; }
      
      //gm2strawtracker::WireID id = gm2strawtracker::wireIDfromString(strawname);

      Nplanes++;
      
      //       // Tracker number should range from 0-1 and describes which of the
      //       // tracking stations contains this wire: 0 for the one directly opposite
      //       // the inflector, and 1 for the 270 degree one.
      //       short trackerNumber = id.getTrackerNumber();
      
      //       // Station number (describes which of the straw station/boxes this wire
      //       // is in) should range from 0 to around 20 at most.
      //       short station = id.getStation();
      
      //       // View is a member of the StrawView enum and describes whether the
      //       // wire in question is a u-view or a v-view wire. We still need to
      //       // figure out or define which direction is u-view.
      //       StrawView view = id.getView();
      
      //       // Layer describes which of the two layers of that view this wire is in
      //       // (see the 3d model). It should range from 0-1.
      //       short layer = id.getLayer();
      
      //       // Wire gives the wire number within the previously-specified layer. It
      //       // will range from 0-n, depending on the size of the station in
      //       // question, and wire 0 is (I think?) the one closest to the scallop
      //       // edge.
      //       short wire = id.getWire();

      
      //cout << "  " << Nplanes << "\t" << trackerNumber << "\t" << station << "\t" << view << "\t" << layer << "\t" << wire << endl;


      float x_global = strawhit.x_global;
      float y_global = strawhit.y_global;
      float z_global = strawhit.z_global;
      float r_global = strawhit.r_global;
      float px_global = strawhit.px_global;
      float py_global = strawhit.py_global;
      float pz_global = strawhit.pz_global;

      lastPos[0] = x_global;
      lastPos[1] = y_global;
      lastPos[2] = z_global;
      lastMom[0] = px_global;
      lastMom[1] = py_global;
      lastMom[2] = pz_global;

      if ( h_StrawTracker_XZ && fill_ ) {
	h_StrawTracker_XZ->Fill(x_global, z_global);
      }

      float x_local = strawhit.x_local;
      float y_local = strawhit.y_local;
      float z_local = strawhit.z_local;
      float px_local = strawhit.px_local;
      float py_local = strawhit.py_local;
      float pz_local = strawhit.pz_local;

      float x_station = strawhit.x_module;
      float y_station = strawhit.y_module;
      float z_station = strawhit.z_module;

      float x_scallop = strawhit.x_scallop;
      float y_scallop = strawhit.y_scallop;
      float z_scallop = strawhit.z_scallop;

      float time = strawhit.time;
      lastTime = time;

      int strawInRow = strawhit.strawInRow;
      int layerNumber = strawhit.layerNumber;
      int viewNumber = strawhit.viewNumber;
      int moduleNumber = strawhit.moduleNumber;
      int strawNumber = strawhit.strawNumber;
      int stationNumber = strawhit.stationNumber;
      stationnum = stationNumber;
      //cout << "  Got Stationnum: " << stationnum << endl;
      //cout << moduleNumber << endl;

      uv_layers[moduleNumber]++;

      if ( debug_ ) { cout << "    " << Nplanes << "\t" << strawNumber << "\t" << moduleNumber << "\t" << viewNumber << "\t" << layerNumber << "\t" << strawInRow << endl; }
    }

    int Nstrawstations = 0;
    for ( int s = 0; s < 30; s++ ) {
      if ( uv_layers[s] >= 4 ) { Nstrawstations++; }
    }

    if ( debug_ ) { 
      cout << "  Particle in straws hit " << Nplanes << " planes." << endl;
      cout << "  Particle in straws hit " << Nstrawstations << " stations." << endl;
    }

    if ( fill_ ) {
      h_StrawTracker_NumStations->Fill(Nstrawstations);
    }
    if ( Nstrawstations >= 1 ) {
      int pdgid = kPositronID;
      int particle = truth_particle_nums["StrawElectron"];
      int status = gm2ringsim::kDecay;

      double x = lastPos[0];
      double y = lastPos[1];
      double z = lastPos[2];
      double px = lastMom[0];
      double py = lastMom[1];
      double pz = lastMom[2];

      double pol_azimuth_truth = 0.0;
      double pol_radial_truth = 0.0;
      double pol_vertical_truth = 0.0;
      double Polarization = 0.0;
      double xe_truth = 0.0;      
      double spinphase = TMath::ACos(pol_azimuth_truth); //ComputeSpinPhase(polx, poly, polz, px, py, pz);
      //double spinphase = ComputeSpinPhase(0, 0, 0, px, py, pz);
      int syshits = Nstrawstations;

      TVector3 mom_straw(px, py, pz);
      double mom = mom_straw.Mag();
      double p = mom;
      TVector3 pos_straw(x, y, z);
      double rhat_truth = ComputeRhat(&pos_straw);
      double y_truth = ComputeVhat(&pos_straw);
      G4double rprime_truth = ComputePrhat(&pos_straw, &mom_straw);
      G4double yprime_truth = ComputePvhat(&pos_straw, &mom_straw);
      double theta = ComputeTheta(x, z);
      double time = lastTime;


      if ( particle >= 0 ) {
	if ( particle == truth_particle_nums["StrawElectron"] ) {
	  Nstrawpositrons++;
	  FillTruthData(StrawElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	  Npart[particle]++;
	  Npart_Rhat[particle] = rhat_truth;
	  Npart_Yhat[particle] = y_truth;
	  Npart_Mom[particle] = p;
	  Npart_PDGID[particle] = pdgid;
	  Npart_Status[particle] = status;
	  Npart_Time[particle] = time;
	}
      }

      if ( Nstrawstations >= 5 ) {
	int particle = truth_particle_nums["GoodStrawElectron"];
	if ( particle >= 0 ) {
	  Ngoodstrawpositrons++;
	  FillTruthData(GoodStrawElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	  Npart[particle]++;
	  Npart_Rhat[particle] = rhat_truth;
	  Npart_Yhat[particle] = y_truth;
	  Npart_Mom[particle] = p;
	  Npart_PDGID[particle] = pdgid;
	  Npart_Status[particle] = status;
	  Npart_Time[particle] = time;
	}
      }

      int tmp_particle = truth_particle_nums["DecayElectron"];
      if ( tmp_particle >= 0 ) {
	if ( Npart[tmp_particle] > 0 ) {	  
	  int particle_straw_calo = truth_particle_nums["StrawCaloElectron"];
	  if ( particle_straw_calo >= 0 ) {
	    Nstrawcalopositrons++;
	    FillTruthData(StrawCaloElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	    Npart[particle_straw_calo]++;
	    Npart_Rhat[particle_straw_calo] = rhat_truth;
	    Npart_Yhat[particle_straw_calo] = y_truth;
	    Npart_Mom[particle_straw_calo] = p;
	    Npart_PDGID[particle_straw_calo] = pdgid;
	    Npart_Status[particle_straw_calo] = status;
	    Npart_Time[particle_straw_calo] = time;
	  }

	  if ( Nstrawstations >= 5 ) {
	    int particle = truth_particle_nums["GoodStrawCaloElectron"];
	    if ( particle >= 0 ) {
	      Ngoodstrawcalopositrons++;
	      FillTruthData(GoodStrawCaloElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
	      Npart[particle]++;
	      Npart_Rhat[particle] = rhat_truth;
	      Npart_Yhat[particle] = y_truth;
	      Npart_Mom[particle] = p;
	      Npart_PDGID[particle] = pdgid;
	      Npart_Status[particle] = status;
	      Npart_Time[particle] = time;
	    }

	    if ( stationnum == konestraw ) {
	      particle = truth_particle_nums["GoodOneStrawCaloElectron"];
	      if ( particle >= 0 ) {
		FillTruthData(GoodOneStrawCaloElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
		Npart[particle]++;
		Npart_Rhat[particle] = rhat_truth;
		Npart_Yhat[particle] = y_truth;
		Npart_Mom[particle] = p;
		Npart_PDGID[particle] = pdgid;
		Npart_Status[particle] = status;
		Npart_Time[particle] = time;
	      }
	    }

	    if ( stationnum == konestraw || stationnum == ktwostraw ) {
	      particle = truth_particle_nums["GoodTwoStrawCaloElectron"];
	      if ( particle >= 0 ) {
		FillTruthData(GoodTwoStrawCaloElectronData, xe_truth, rhat_truth, y_truth, rprime_truth, yprime_truth, p, x, z, theta, t0, time, tDecay, pol_azimuth_truth, pol_radial_truth, pol_vertical_truth, Polarization, syshits, spinphase);
		Npart[particle]++;
		Npart_Rhat[particle] = rhat_truth;
		Npart_Yhat[particle] = y_truth;
		Npart_Mom[particle] = p;
		Npart_PDGID[particle] = pdgid;
		Npart_Status[particle] = status;
		Npart_Time[particle] = time;
	      }
	    }
	  }
	}
      }
    }
  }
  if ( debug_ ) { cout << "Made it past straw hits." << endl; }


  if ( fill_ ) {
    h_CaloStation_Nhits->Fill(ncalomuhits);
  }



  //-------------------
  // Beam Scan
  //-------------------
  if ( SaveBeamScanHits_ ) {
    int particle = truth_particle_nums["DecayMuon"];
    if ( particle >= 0 ) {
      if ( Npart[particle] > 0 || 1 ) {
	double maxvhat=0;
	double maxrhat=0;
	int randbs = (int)truthbeamscanhits.size()*G4UniformRand();
	if ( randbs == (int)truthbeamscanhits.size() ) { randbs--; }       
	
	int c = 0;
	for ( auto hbeamscandata : truthbeamscanhits ) {
	  if ( c != randbs && 0 ) { continue; }
	  int trackID = hbeamscandata.trackID;
	  double rhat = hbeamscandata.rhat;
	  double vhat = hbeamscandata.vhat;
	  double time = hbeamscandata.time;
	  double p = hbeamscandata.p;
	  double prhat = hbeamscandata.prhat;
	  double pvhat = hbeamscandata.pvhat;
	  double polx_pre = hbeamscandata.polx;
	  double poly_pre = hbeamscandata.poly;
	  double polz_pre = hbeamscandata.polz;
	  double xe_truth = ComputeXe(p, 0.137);
	  double theta = hbeamscandata.theta;
	  
	  TVector3 pol_pre(polx_pre, poly_pre, polz_pre);
	  double polx = pol_pre.Dot(xAxis);      
	  double poly = pol_pre.Dot(yAxis);
	  double polz = pol_pre.Dot(zAxis);    
	  double pol_azimuth_truth, pol_radial_truth, pol_vertical_truth;
	  ComputePolarization(&pol_azimuth_truth, &pol_radial_truth, &pol_vertical_truth, polx, poly, polz);
	  

	  if ( fabs(vhat) > maxvhat ) { maxvhat = fabs(vhat); }
	  if ( fabs(rhat) > maxrhat ) { maxrhat = fabs(rhat); }

	  FillBeamScan(time, rhat, vhat, prhat, pvhat, theta, p, pol_radial_truth, pol_vertical_truth, pol_azimuth_truth);
	}

	if ( 0 ) {
	  double decay_vhat = DecayMuonData[kVhat];
	  double decay_time = DecayMuonData[kTime];
	  double decay_rhat = DecayMuonData[kRhat];
	  if ( fabs(decay_rhat) > maxrhat ) {
	    cout << "Decay outside of BeamScan: " << truthbeamscanhits.size() << endl;
	    if ( truthbeamscanhits.size() > 20 ) {
	      for ( auto hbeamscandata : truthbeamscanhits ) {
		int trackID = hbeamscandata.trackID;
		double rhat = hbeamscandata.rhat;
		double time = hbeamscandata.time;
		//cout << "BeamScan:  " << rhat << " @ time = " << time << endl;
	      }
	    }
	    cout << "MuonDecay: " << decay_rhat << " @ time = " << decay_time << endl;

	  }
	}
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
  int calostation = -1;
  int particle;

  bool debug_truthtracker = false;
	  
  
  if ( debug_truthtracker || debug_ ) {
    cout << "Particle Dump: " << endl;
    for ( int i = 0; i < Nparticles; i++ ) {
      if  ( Npart[i] > 0 ) {
	cout << truth_particle_names[i] << " : " << Npart[i] << "\tRhat=" << Npart_Rhat[i] << "\tY=" << Npart_Yhat[i] << "\tMom=" << Npart_Mom[i] << "\tID=" << Npart_PDGID[i] << "\tStatus=" << Npart_Status[i] << "\tTime=" << Npart_Time[i] << endl;
      }
    }
    cout << endl;
  }
  if ( debug_ ) { cout << "Made it past particle dump." << endl; }
  
  

  
  //
  // Birth Muon
  //
  particle = truth_particle_nums["BirthMuon"];
  if ( particle >= 0 ) {  
    if ( debug_truthtracker || debug_ ) { cout << "Part=" << particle << "\tname=" << truth_particle_names[particle] << " : " << Npart[particle] << endl; }
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "BirthMuon: " << BirthMuonData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, BirthMuonData, NULL);
      bool fillremain = false;
      if ( truth_particle_nums["DecayMuon"] >= 0 ) {
	if ( Npart[truth_particle_nums["DecayMuon"]] > 0 ) { fillremain = true; }
      }
      if ( truth_particle_nums["StoredMuon"] >= 0 ) {
	if ( Npart[truth_particle_nums["StoredMuon"]] > 0 ) { fillremain = true; }
      }
      if ( fillremain ) {
	FillTruthTracker(kRemainingDist, particle, calostation, BirthMuonData, NULL);

	if ( FillByVRingHist() ) {
	  if ( debug_ ) { cout << t0_init << "\t" << quadtime - t0_init << "\t" << kicktime - t0_init << endl; }
	  if ( fill && h_RingTracker_QuadTime ) { h_RingTracker_QuadTime->Fill(quadtime); }
	  if ( fill && h_RingTracker_KickTime ) { h_RingTracker_KickTime->Fill(kicktime); }
	  if ( debug_ ) { cout << t0_init << "\t" << quadtime - t0_init << "\t" << kicktime - t0_init << endl; }
	}
      }
      
      FillTruthTurnTime(particle, calostation, BirthMuonData);
    }
  }

  
  //
  // Stored Muon
  //
  particle = truth_particle_nums["StoredMuon"];
  if ( particle >= 0 ) {  
    if ( debug_truthtracker || debug_ ) { cout << "Part=" << particle << "\tname=" << truth_particle_names[particle] << " : " << Npart[particle] << endl; }
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "StoredMuon: " << StoredMuonData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, StoredMuonData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthMuonData, NULL);
      FillTruthTurnTime(particle, calostation, StoredMuonData);
    }
  }

  //
  // Lost Muon
  //
  particle = truth_particle_nums["LostMuon"];
  if ( particle >= 0 ) {  
    if ( debug_truthtracker || debug_ ) { cout << "Part=" << particle << "\tname=" << truth_particle_names[particle] << " : " << Npart[particle] << endl; }
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "LostMuon: " << LostMuonData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, LostMuonData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthMuonData, NULL);
      FillTruthTurnTime(particle, calostation, LostMuonData);
    }
  }

  //
  // Lost Muon (in calorimeter)
  //
  particle = truth_particle_nums["LostDecayMuon"];
  if ( particle >= 0 ) {  
    if ( debug_truthtracker || debug_ ) { cout << "Part=" << particle << "\tname=" << truth_particle_names[particle] << " : " << Npart[particle] << endl; }
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "LostDecayMuon: " << LostDecayMuonData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, LostDecayMuonData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthMuonData, NULL);
      FillTruthTurnTime(particle, calostation, LostDecayMuonData);
    }
  }
  
  //
  // Decay Muon
  //
  particle = truth_particle_nums["DecayMuon"];
  if ( particle >= 0 ) {
    if ( debug_truthtracker || debug_ ) { cout << "Part=" << particle << "\tname=" << truth_particle_names[particle] << " : " << Npart[particle] << endl; }
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayMuon: " << DecayMuonData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayMuonData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthMuonData, NULL);
      //cout << "Filling All DecayMuon" << endl;
      FillTruthTurnTime(particle, calostation, DecayMuonData);
      if ( DecayMuonData[kTime] < firstdecay.T() ) {
	double x = (DecayMuonData[kRhat]+R_magic())*TMath::Cos(DecayMuonData[kTheta]);
	double z = (DecayMuonData[kRhat]+R_magic())*TMath::Sin(DecayMuonData[kTheta]);
	double y = DecayMuonData[kVhat];
	double t = DecayMuonData[kTime];
	firstdecay.SetXYZT(x, y, z, t);
	double sx = DecayMuonData[kPol_radial];
	double sy = DecayMuonData[kPol_vertical];
	double sz = DecayMuonData[kPol_azimuth];
	firstdecay_spin.SetXYZT(sx, sy, sz, t);
      }
      if ( fabs(DecayMuonData[kTime] - 500*ns) < 1*ns ) {
	double sx = DecayMuonData[kPol_radial];
	double sy = DecayMuonData[kPol_vertical];
	double sz = DecayMuonData[kPol_azimuth];
	double t = DecayMuonData[kTime];
	firstdecay_spin_500ns.SetXYZT(sx, sy, sz, t);
      }
      if ( fabs(DecayMuonData[kTime] - 1000*ns) < 1*ns ) {
	double sx = DecayMuonData[kPol_radial];
	double sy = DecayMuonData[kPol_vertical];
	double sz = DecayMuonData[kPol_azimuth];
	double t = DecayMuonData[kTime];
	firstdecay_spin_1us.SetXYZT(sx, sy, sz, t);
      }
      if ( fabs(DecayMuonData[kTime] - 2000*ns) < 1*ns ) {
	double sx = DecayMuonData[kPol_radial];
	double sy = DecayMuonData[kPol_vertical];
	double sz = DecayMuonData[kPol_azimuth];
	double t = DecayMuonData[kTime];
	firstdecay_spin_2us.SetXYZT(sx, sy, sz, t);
      }
      if ( fabs(DecayMuonData[kTime] - 3000*ns) < 1*ns ) {
	double sx = DecayMuonData[kPol_radial];
	double sy = DecayMuonData[kPol_vertical];
	double sz = DecayMuonData[kPol_azimuth];
	double t = DecayMuonData[kTime];
	firstdecay_spin_3us.SetXYZT(sx, sy, sz, t);
      }
    }
  }
  

  //
  // Birth Electron
  //
  particle = truth_particle_nums["BirthElectron"];
  if ( particle >= 0 ) {
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "BirthElectron: " << BirthElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, BirthElectronData, NULL);
      if ( Npart[truth_particle_nums["DecayElectron"]] > 0 ) {
	FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, NULL);
      }
      FillTruthTurnTime(particle, calostation, BirthElectronData);
    }
  }

  
  //
  // Birth Electron, E > 1.8 GeV
  //
  particle = truth_particle_nums["BirthElectronEgtEth"];
  if ( particle >= 0 ) {
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "BirthElectronEgtEth: " << BirthElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, BirthElectronData, NULL);
      if ( Npart[truth_particle_nums["DecayElectron"]] > 0 ) {
	FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, NULL);
      }
      FillTruthTurnTime(particle, calostation, BirthElectronEgtEthData);
    }
  }


  //
  // Straw Electron
  //
  particle = truth_particle_nums["StrawElectron"];
  if ( particle >= 0 ) {
    calostation = stationnum;
    //cout << "TrackerNum = " << stationnum << endl;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "StrawElectron: " << StrawElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, stationnum, StrawElectronData, NULL);
      FillTruthTracker(kRemainingDist, particle, stationnum, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, stationnum, StrawElectronData);
    }
  }


  //
  // Straw Electron + 5 station hits
  //
  particle = truth_particle_nums["GoodStrawElectron"];
  if ( particle >= 0 ) {
    calostation = stationnum;
    //cout << "TrackerNum = " << stationnum << endl;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "GoodStrawElectron: " << GoodStrawElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, stationnum, GoodStrawElectronData, NULL);
      FillTruthTracker(kRemainingDist, particle, stationnum, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, stationnum, GoodStrawElectronData);
    }
  }


  //
  // Straw Electron w/ Calo
  //
  particle = truth_particle_nums["StrawCaloElectron"];
  if ( particle >= 0 ) {
    calostation = stationnum;
    //cout << "StationNum = " << stationnum << endl;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "StrawCaloElectron: " << StrawCaloElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, stationnum, StrawCaloElectronData, NULL);
      FillTruthTracker(kRemainingDist, particle, stationnum, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, stationnum, StrawCaloElectronData);
    }
  }


  //
  // Straw Electron w/ Calo + 5 station hits
  //
  particle = truth_particle_nums["GoodStrawCaloElectron"];
  if ( particle >= 0 ) {
    calostation = stationnum;
    //cout << "StationNum = " << stationnum << endl;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "GoodStrawCaloElectron: " << GoodStrawCaloElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, stationnum, GoodStrawCaloElectronData, NULL);
      FillTruthTracker(kRemainingDist, particle, stationnum, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, stationnum, GoodStrawCaloElectronData);
    }
  }


  //
  // Straw Electron w/ Calo + 5 station hits w/ 1 station
  //
  particle = truth_particle_nums["GoodOneStrawCaloElectron"];
  if ( particle >= 0 ) {
    calostation = stationnum;
    //cout << "StationNum = " << stationnum << endl;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "StrawElectron: " << StrawElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, stationnum, GoodOneStrawCaloElectronData, NULL);
      FillTruthTracker(kRemainingDist, particle, stationnum, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, stationnum, GoodOneStrawCaloElectronData);
    }
  }


  //
  // Straw Electron w/ Calo + 5 station hits w/ 2 stations
  //
  particle = truth_particle_nums["GoodTwoStrawCaloElectron"];
  if ( particle >= 0 ) {
    calostation = stationnum;
    //cout << "StationNum = " << stationnum << endl;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "StrawElectron: " << StrawElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, stationnum, GoodTwoStrawCaloElectronData, NULL);
      FillTruthTracker(kRemainingDist, particle, stationnum, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, stationnum, GoodTwoStrawCaloElectronData);
    }
  }


  //
  // Decay Electron
  //
  particle = truth_particle_nums["DecayElectron"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectron: " << DecayElectronData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronData);
    }
  }


  //
  // Decay Electron, E > 1.8 GeV
  //
  particle = truth_particle_nums["DecayElectronEgtEth"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronEgtEth: " << DecayElectronEgtEthData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronEgtEthData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronEgtEthData);
    }
  }


  //
  // Decay Electron, E < 1.8 GeV
  //
  particle = truth_particle_nums["DecayElectronEltEth"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronEltEth: " << DecayElectronEltEthData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronEltEthData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronEltEthData);
    }
  }


  //
  // Decay Electron, E > 2.6 GeV
  //
  particle = truth_particle_nums["DecayElectronEgtHghEth"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronEgtHghEth: " << DecayElectronEgtHghEthData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronEgtHghEthData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronEgtHghEthData);
    }
  }


  //
  // Decay Electron, Drift Time > 40 ns
  //
  particle = truth_particle_nums["DecayElectronLongDriftTime"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronLongDriftTime: " << DecayElectronLongDriftTimeData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronLongDriftTimeData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronLongDriftTimeData);
    }
  }


  //
  // Decay Electron, Drift Time < 40 ns
  //
  particle = truth_particle_nums["DecayElectronShortDriftTime"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronShortDriftTime: " << DecayElectronShortDriftTimeData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronShortDriftTimeData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronShortDriftTimeData);
    }
  }


  //
  // Decay Electron, Quad Section
  //
  particle = truth_particle_nums["DecayElectronQuadSection"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronQuadSection: " << DecayElectronQuadSectionData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronQuadSectionData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronQuadSectionData);
    }
  }


  //
  // Decay Electron, Kicker Section
  //
  particle = truth_particle_nums["DecayElectronKickerSection"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronKickerSection: " << DecayElectronKickerSectionData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronKickerSectionData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronKickerSectionData);
    }
  }


  //
  // Decay Electron, Other Section
  //
  particle = truth_particle_nums["DecayElectronOtherSection"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronOtherSection: " << DecayElectronOtherSectionData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronOtherSectionData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronOtherSectionData);
    }
  }


  //
  // Decay Electron, Empty Section
  //
  particle = truth_particle_nums["DecayElectronEmptySection"];
  if ( particle >= 0 ) {
    calostation = calonum;
    if ( Npart[particle] > 0 ) {
      if ( debug_truthtracker || debug_ ) { cout << "DecayElectronEmptySection: " << DecayElectronEmptySectionData[kP] << endl; }
      truth_particle_multiplicity[particle]++;
      FillTruthTracker(kGeneratedDist, particle, calostation, DecayElectronEmptySectionData, NULL);
      FillTruthTracker(kRemainingDist, particle, calostation, BirthElectronData, DecayMuonData);
      FillTruthTurnTime(particle, calostation, DecayElectronEmptySectionData);
    }
  }



  if ( fabs(DecayElectronData[kVhat]) > 100 && fabs(DecayElectronData[kVhat]) < 300 ) {
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
