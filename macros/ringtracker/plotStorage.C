#include <map>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

bool plot_dp_over_p = true;
bool read_from_file = true;

int dummy = -1;
string none = "";
string btype_str = "";

bool zoom = false;
double zoomval = 0.5;

int prhat;
double sigmap;
double dp_over_p;
int delta;

int kLCRKick = 0;
int kSquareKick = 1;
int kNoKick = 2;
int kGaussian = 0;
int kUniform = 1;
int kDownstreamMandrel = 0;
int kUpstreamMandrel = 1;
int kUpstreamCryo = 2;
int kCentralOrbit_Offset77 = 3;
int kCentralOrbit = 4;
int kVanish = 0;
int kMapped = 1;
int kOpen = 0;
int kPartiallyOpen = 1;
int kClosed = 2;
int kBeamsize0pi = 0;
int kBeamsize1pi = 1;
int kBeamsize20pi = 2;
int kBeamsize40pi = 3;
int kBeamsize42pi = 4;
int kScrapingON = 0;
int kScrapingOFF = 1;
int kNewSupports = 0;
int kNoQuads = 1;
int kNoQuadSupports = 2;
int kFullHighMom = 3;
int kFullHigherMom = 4;
int kFullHighestMom = 5;
int kBNLmatch = 0;
int kPerfectmatch = 1;

int btype_start, btype_end, btype_inc;
int field_start, field_end, field_inc;

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TLorentzVector.h"

#include "GetEffs.C"

#include "Includes/FillEffs_CO_Vanish_Perfect.C"
#include "Includes/FillEffs_CO_Vanish_Sneak.C"


//-------- Central Orbit ---------//
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit.C"
#include "Includes/FillEffs_Beamsize0pi_Gaussian_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit.C"
#include "Includes/FillEffs_Beamsize0pi_Gaussian_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit.C"
#include "Includes/FillEffs_Beamsize40pi_Gaussian_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit.C"
#include "Includes/FillEffs_Beamsize40pi_Gaussian_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit.C"

//-------- Downstream Mandrel --------//
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel.C"

#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel.C"

#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel.C"

#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel.C"

//--------- UpstreamCryo ---------//
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"

#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"

#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo.C"
#include "Includes/FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo.C"



#define Nquads 3
#define Ninfs 3
#define Nfields 2
#define Nbeamsizes 5
#define Nbeamtypes 2
#define Nkicks 3
#define Nmomtypes 2



void SetConstants(int *btype, int *field)
{
  //*btype = kGaussian;
  *btype = kUniform;
  if ( btype_str == "Gaussian" || btype_str == "gauss" || btype_str == "g" ) { *btype = kGaussian; }
  if ( btype_str == "Uniform" || btype_str == "uni" || btype_str == "u" ) { *btype = kUniform; }
  btype_start = *btype; btype_end = btype_start + 1; btype_inc = 1;

  //*field = kMapped;
  *field = kVanish;
  field_start = *field; field_end = field_start + 1; field_inc = 1;
  
}

void style() {

  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);

  gStyle->SetTitleColor(1);   // 0
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleBorderSize(1);

  /*
    gStyle->SetTitleX(0.10);
    gStyle->SetTitleY(0.94);
    gStyle->SetTitleW(0.5);
    gStyle->SetTitleH(0.06);

    gStyle->SetLabelOffset(1e-04);
    gStyle->SetLabelSize(0.2);
    gStyle->SetTitleSize(0.2);
  */
  gStyle->SetStatColor(0);
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatX(0.90);
  gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.30);
  gStyle->SetStatH(0.10);
  
  gStyle->SetErrorX(0.0);   // Horizontal error bar size
  gStyle->SetOptStat(0);
  //   gStyle->SetPaperSize(10.,12.);   // Printout size
}


void PlotBasic(int beamstart, int bs, int q, int inf, int kick, int momtype, int field, int btype, double start)
{	    
  if ( zoom || 1 ) { return; }

  stringstream textname;
  textname << "#mathcal{A} = " << bs << "#pi";
  //myText(0.67, start, 1, textname.str().c_str(), 0.03);

  double yscale = 0.05;
  
  if ( momtype == kPerfectmatch ) {
    textname.str("");
    textname << "#beta_{x} = 7.6 m ; #beta_{y} = 19.1 m";
    myText(0.62, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
    textname.str("");
    textname << "#alpha_{x} = 0 ; #alpha_{y} = 0";
    myText(0.62, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
  }
  else if ( momtype == kBNLmatch ) {
    textname.str("");
    textname << "#beta_{x} = 2.0 m ; #beta_{y} = 19.6 m";
    myText(0.62, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
    textname.str("");
    textname << "#alpha_{x} = -0.54 ; #alpha_{y} = 0.043";
    myText(0.62, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
  }


  if ( kick >= 0 ) {
    textname.str("");
    textname << NiceKickname(kick);
    myText(0.65, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
  }


  if ( bs >= 0 ) {
    textname.str("");
    textname << NiceBeamsizename(bs);
    myText(0.65, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
  }


  //   if ( btype >= 0 ) {
  //     textname.str("");
  //     textname << NiceBeamtypename(btype);
  //     myText(0.65, start - yscale, 1, textname.str().c_str(), 0.03);
  //     yscale += 0.05;
  //   }


  if ( q >= 0 ) {
    textname.str("");
    textname << NiceQuadname(q);
    myText(0.65, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
  }

  if ( inf >= 0 ) {
    textname.str("");
    textname << NiceInfname(inf);
    myText(0.65, start - yscale, 1, textname.str().c_str(), 0.03);
    yscale += 0.05;
  }


  //if ( field < 0 ) { c1name << "_AllFields"; }
  //else { c1name << "_" << Fieldname(field); }

  
  
}


{
  return( plot );
}

string Plotname(string subdir, string plottypename, int beamstart, int bs, int q, int inf, int kick, int momtype, int field, int btype)
{
  string basedir = "~/WWW/Inflector/" + Startname(beamstart);
  string dir = basedir + "/" + subdir + "/";

  stringstream c1name;

  c1name << dir;
  //<< "gr_dP_P";
  if ( plottypename.size() > 0 ) { c1name << "_" << plottypename; }
  else {
    if ( momtype < 0 ) { c1name << "AllMomtypes"; }
    else { c1name << Momtypename(momtype); }
  }

  if ( kick < 0 ) { c1name << "_AllKicks"; }
  else { c1name << "_" << Kickname(kick); }

  if ( bs < 0 ) { c1name << "_AllBeamsizes"; }
  else { c1name << "_" << Beamsizename(bs); }

  if ( btype < 0 ) { c1name << "_AllBeamtypes";  }
  else { c1name << "_" << Beamtypename(btype); }

  if ( q < 0 ) { c1name << "_AllQuads"; }
  else { c1name << "_" << Quadname(q); }

  if ( inf < 0 ) { c1name << "_AllInflectors"; }
  else { c1name << "_" << Infname(inf); }

  if ( field == kMapped ) { c1name << "_" << Fieldname(field); }

  if ( zoom ) { c1name << "_Zoom"; }


  c1name << ".eps";
  
  return( c1name.str() );
}

string Momname(int mom)
{
  string momname;
  if ( mom == 8 ) { momname = "PerfectMatch_dP05"; dp_over_p = 0.005; }
  if ( mom == 9 ) { momname = "PerfectMatch_dP005"; dp_over_p = 0.0005; }
  if ( mom == 10 ) { momname = "PerfectMatch_dP1"; dp_over_p = 0.01; }
  if ( mom == 11 ) { momname = "PerfectMatch_dP001"; dp_over_p = 0.001; }
  if ( mom == 12 ) { momname = "PerfectMatch_dP0001"; dp_over_p = 0.0001; }
  if ( mom == 13 ) { momname = "PerfectMatch_dP2"; dp_over_p = 0.02; }
  if ( mom == 14 ) { momname = "PerfectMatch_dP5"; dp_over_p = 0.05; }
  if ( mom == 15 ) { momname = "PerfectMatch_dP10"; dp_over_p = 0.10; }
  if ( mom == 16 ) { momname = "PerfectMatch_dP0075"; dp_over_p = 0.0075; }
  if ( mom == 17 ) { momname = "PerfectMatch_dP025"; dp_over_p = 0.025; }
  if ( mom == 18 ) { momname = "PerfectMatch_dP075"; dp_over_p = 0.075; }
  if ( mom == 19 ) { momname = "PerfectMatch_dP0025"; dp_over_p = 0.0025; }
  if ( mom == 20 ) { momname = "E821Match_dP05"; dp_over_p = 0.005; }
  if ( mom == 21 ) { momname = "E821Match_dP005"; dp_over_p = 0.0005; }
  if ( mom == 22 ) { momname = "E821Match_dP1"; dp_over_p = 0.01; }
  if ( mom == 23 ) { momname = "E821Match_dP001"; dp_over_p = 0.001; }
  if ( mom == 24 ) { momname = "E821Match_dP0001"; dp_over_p = 0.0001; }
  if ( mom == 25 ) { momname = "E821Match_dP2"; dp_over_p = 0.02; }
  if ( mom == 26 ) { momname = "E821Match_dP5"; dp_over_p = 0.05; }
  if ( mom == 27 ) { momname = "E821Match_dP10"; dp_over_p = 0.10; }
  if ( mom == 28 ) { momname = "E821Match_dP0075"; dp_over_p = 0.0075; }
  if ( mom == 29 ) { momname = "E821Match_dP025"; dp_over_p = 0.025; }
  if ( mom == 30 ) { momname = "E821Match_dP075"; dp_over_p = 0.075; }
  if ( mom == 31 ) { momname = "E821Match_dP0025"; dp_over_p = 0.0025; }
  return( momname );
}

string Kickname(int kick)
{
  if ( kick == kLCRKick ) { return( "LCRKick" ); }
  if ( kick == kSquareKick ) { return( "SquareKick" ); }
  if ( kick == kNoKick ) { return( "NoKick" ); }
  
  return( "" );
}

string NiceKickname(int kick)
{
  if ( kick == kLCRKick ) { return( "E821 Kick" ); }
  if ( kick == kSquareKick ) { return( "Ideal Kick" ); }
  if ( kick == kNoKick ) { return( "No Kick" ); }
      
  return( "" );
}

string Momtypename(int momtype)
{
  if ( momtype == kBNLmatch ) { return( "BNLmatch" ); }
  if ( momtype == kPerfectmatch ) { return( "Perfectmatch" ); }
  
  return( "" );
}

string NiceMomtypename(int momtype)
{
  if ( momtype == kBNLmatch ) { return( "BNL-match" ); }
  if ( momtype == kPerfectmatch ) { return( "Ideal-match" ); }
  return( "" );
}

string Beamtypename(int beamtype)
{
  if ( beamtype == 0 ) { return( "Gaussian" ); }
  if ( beamtype == 1 ) { return( "Uniform" ); }
  return( "" );
}

string NiceBeamtypename(int beamtype)
{
  if ( beamtype == kGaussian ) { return( "Gaussian PS" ); }
  if ( beamtype == kUniform ) { return( "Uniform PS" ); }
}

int Beamsize(int bs)
{
  if ( bs == kBeamsize0pi ) { return( 0 ); }
  if ( bs == kBeamsize1pi ) { return( 1 ); }
  if ( bs == kBeamsize20pi ) { return( 20 ); }
  if ( bs == kBeamsize40pi ) { return( 40 ); }
  if ( bs == kBeamsize42pi ) { return( 42 ); }
}

string Beamsizename(int bs)
{
  if ( bs == kBeamsize0pi ) { return( "Beamsize0pi" ); }
  if ( bs == kBeamsize1pi ) { return( "Beamsize1pi" ); }
  if ( bs == kBeamsize20pi ) { return( "Beamsize20pi" ); }
  if ( bs == kBeamsize40pi ) { return( "Beamsize40pi" ); }
  if ( bs == kBeamsize42pi ) { return( "Beamsize42pi" ); }
  return( "UnknownBS" );
}

string NiceBeamsizename(int bs)
{
  if ( bs == kBeamsize0pi ) { return( "A = 0#pi" ); }
  if ( bs == kBeamsize1pi ) { return( "A = 1#pi" ); }
  if ( bs == kBeamsize20pi ) { return( "A = 20#pi" ); }
  if ( bs == kBeamsize40pi ) { return( "A = 40#pi" ); }
  if ( bs == kBeamsize42pi ) { return( "A = 42#times56#pi" ); }
  return( "UnknownBS" );
}

string Scrapingname(int s)
{
  if ( s == 0 ) { return( "ScrapingON" ); }
  if ( s == 1 ) { return( "ScrapingOFF" ); }

  return("");
}

string NiceQuadname(int q)
{
  string quadname;
  if ( q == 0 ) { quadname = "E821 Quads"; }
  if ( q == 1 ) { quadname = "Massless Q1"; }
  if ( q == 2 ) { quadname = "Massless Supports"; }
  if ( q == 3 ) { quadname = "Full P_{m}+4"; }
  if ( q == 4 ) { quadname = "Full P_{m}+8"; }
  if ( q == 5 ) { quadname = "Full P_{m}+12"; }
  return( quadname );
}

string Quadname(int q)
{
  string quadname;
  if ( q == 0 ) { quadname = "NewSupports"; }
  if ( q == 1 ) { quadname = "NoQuads"; }
  if ( q == 2 ) { quadname = "NoQuadSupports"; }
  if ( q == 3 ) { quadname = "FullHighMom"; }
  if ( q == 4 ) { quadname = "FullHigherMom"; }
  if ( q == 5 ) { quadname = "FullHighestMom"; }
  return( quadname );
}

string Infname(int inf)
{
  string infname;
  if ( inf == kOpen ) { infname = "Open"; }
  if ( inf == kPartiallyOpen ) { infname = "PartiallyOpen"; }
  if ( inf == kClosed ) { infname = "Closed"; }
  return( infname );
}

string NiceInfname(int inf)
{
  string infname;
  if ( inf == kOpen ) { infname = "Open Inf. Ends"; }
  if ( inf == kPartiallyOpen ) { infname = "1 Open Inf. End"; }
  if ( inf == kClosed ) { infname = "Closed Inf. Ends"; }
  return( infname );
}

string Fieldname(int field)
{
  string fieldname;
  if ( field == kVanish ) { fieldname = "Vanish"; }
  if ( field == kMapped ) { fieldname = "Mapped"; }

  if ( fieldname.size() == 0 ) { cout << "Field not known! [" << field << "]" << endl; }
    
  return( fieldname );
}

double mRadToDeg(double mrad)
{
  return( mrad );
  double conv = 180.0/TMath::Pi();

  double deg = mrad * conv / 1000.0;

  return( deg );
}

double DegreeTomRad(double deg)
{
  return( deg );
  double conv = TMath::Pi() / 180.0;

  double mrad = deg * conv * 1000.0;

  return( mrad );
}

string Startname(int start)
{
  string startname;
  if ( start == 0 ) { startname = "DownstreamMandrel"; }
  if ( start == 1 ) { startname = "UpstreamMandrel"; }
  if ( start == 2 ) { startname = "UpstreamCryo"; }
  if ( start == 3 ) { startname = "CentralOrbit_Offset77"; }
  if ( start == 4 ) { startname = "CentralOrbit"; }
  return( startname );
}

void SetGraph(TGraph *gr, int color, int max)
{
  gr->SetLineColor(color);
  gr->SetMarkerSize(1.25);
  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(color);
  gr->SetMinimum(0.0);
  gr->SetMaximum(max);
}



int N_dP_Ideal[Nbeamsizes][Nmomtypes];
double dPOverP_Ideal[Nbeamsizes][Nmomtypes][20];
double eff_dP_Ideal[Nbeamsizes][Nmomtypes][20];
double err_dP_Ideal[Nbeamsizes][Nmomtypes][20];

void plotStorage()
{
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasStyle.C");
  SetAtlasStyle();
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasUtils.C");
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasLabels.C");

  style();


  ifstream in;
  in.open("plot_from_file");
  in >> plot_dp_over_p;
  in >> btype_str;
  in.close();

  //plot_dp_over_p = true;

  if ( plot_dp_over_p ) { read_from_file = false; }


  

  int N_dP[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes], N_dP_NoLaunch[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes];
  double dPOverP[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes][20], dPOverP_NoLaunch[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes][20];
  double eff_dP[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes][20], err_dP[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes][20];;
  double eff_dP_NoLaunch[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes][20], err_dP_NoLaunch[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes][20];

  int bs_start, bs_end, bs_inc;
  int inf_start, inf_end, inf_inc;
  int q_start, q_end, q_inc;
  int beamstart_start, beamstart_end, beamstart_inc;
  int mom_start, mom_end, mom_inc;
  int plottype_start, plottype_end, plottype_inc;
  int kick_start, kick_end, kick_inc;
  int momtype_start, momtype_end, momtype_inc;
  plottype_start = 1;
  plottype_end = 1 + 1;
  plottype_inc = 1;

  string param = "CO-plot";
  param = "DM-plot";
  param = "DM-2D";
  param = "UC-2D";
  
  //param = "DM";
  //param = "UC-plot";
  //param = "UC";
  
  

  if ( param == "CO" ) {
    bs_start = kBeamsize40pi; bs_end = bs_start + 1; bs_inc = 1;
    inf_start = kOpen; inf_end = kOpen + 1; inf_inc = 1;
    q_start = kNoQuads; q_end = kNoQuads + 1; q_inc = 1;
    beamstart_start = kCentralOrbit; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 11 + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kBNLmatch + 1; momtype_inc = 1;
    //momtype_start = kPerfectmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kNoKick; kick_end = kick_start + 1; kick_inc = 1;  
  }
  else if ( param == "CO-plot" ) {
    bs_start = kBeamsize0pi; bs_end = kBeamsize40pi + 1; bs_inc = 3;
    inf_start = kOpen; inf_end = kOpen + 1; inf_inc = 1;
    q_start = kNoQuads; q_end = kNoQuads + 1; q_inc = 1;
    beamstart_start = kCentralOrbit; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 11 + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kNoKick; kick_end = kick_start + 1; kick_inc = 1;  
  }
  else if ( param == "DM" ) {
    bs_start = kBeamsize0pi; bs_end = kBeamsize40pi + 1; bs_inc = 3;
    inf_start = kOpen; inf_end = kClosed + 1; inf_inc = 1;
    q_start = kNewSupports; q_end = kNoQuadSupports + 1; q_inc = 1;
    beamstart_start = kDownstreamMandrel; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 11 + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kSquareKick; kick_end = kSquareKick + 1; kick_inc = 1; 
    //kick_start = kLCRKick; kick_end = kLCRKick + 1; kick_inc = 1;  
  }
  else if ( param == "DM-plot" ) {
    bs_start = kBeamsize0pi; bs_end = kBeamsize40pi + 1; bs_inc = 3;
    inf_start = kOpen; inf_end = kClosed + 1; inf_inc = 1;
    q_start = kNewSupports; q_end = kNoQuadSupports + 1; q_inc = 1;
    beamstart_start = kDownstreamMandrel; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 11 + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kLCRKick; kick_end = kSquareKick + 1; kick_inc = 1;  
  }
  else if ( param == "DM-2D" ) {
    bs_start = kBeamsize40pi; bs_end = kBeamsize40pi + 1; bs_inc = 3;
    inf_start = kOpen; inf_end = kOpen + 1; inf_inc = 1;
    q_start = kNoQuads; q_end = kNoQuads + 1; q_inc = 1;
    beamstart_start = kDownstreamMandrel; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kSquareKick; kick_end = kSquareKick + 1; kick_inc = 1;  
    field_start = kMapped; field_end = field_start + 1; field_inc = 1;
  }
  else if ( param == "UC" ) {
    bs_start = kBeamsize40pi; bs_end = kBeamsize40pi + 1; bs_inc = 3;
    inf_start = kOpen; inf_end = kClosed + 1; inf_inc = 1;
    inf_start = kPartiallyOpen; inf_end = kClosed + 1; inf_inc = 1;
    //inf_start = kOpen; inf_end = kOpen + 1; inf_inc = 1;
    q_start = kNewSupports; q_end = kNoQuadSupports + 1; q_inc = 1;
    q_start = kNoQuadSupports; q_end = kNoQuadSupports + 1; q_inc = 1;
    q_start = kNoQuads; q_end = kNoQuads + 1; q_inc = 1;
    beamstart_start = kUpstreamCryo; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 11 + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kLCRKick; kick_end = kSquareKick + 1; kick_inc = 1;  
    kick_start = kLCRKick; kick_end = kLCRKick + 1; kick_inc = 1; 
    //kick_start = kSquareKick; kick_end = kSquareKick + 1; kick_inc = 1;   
  }
  else if ( param == "UC-plot" ) {
    bs_start = kBeamsize0pi; bs_end = kBeamsize40pi + 1; bs_inc = 3;
    inf_start = kOpen; inf_end = kClosed + 1; inf_inc = 1;
    q_start = kNewSupports; q_end = kNoQuadSupports + 1; q_inc = 1;
    beamstart_start = kUpstreamCryo; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 11 + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kLCRKick; kick_end = kSquareKick + 1; kick_inc = 1;  
  }
  else if ( param == "UC-2D" ) {
    bs_start = kBeamsize40pi; bs_end = kBeamsize40pi + 1; bs_inc = 3;
    inf_start = kOpen; inf_end = kOpen + 1; inf_inc = 1;
    q_start = kNoQuads; q_end = kNoQuads + 1; q_inc = 1;
    beamstart_start = kUpstreamCryo; beamstart_end = beamstart_start + 1; beamstart_inc = 1;
    mom_start = 0; mom_end = mom_start + 1; mom_inc = 1;
    momtype_start = kBNLmatch; momtype_end = kBNLmatch + 1; momtype_inc = 1;
    momtype_start = kPerfectmatch; momtype_end = kPerfectmatch + 1; momtype_inc = 1;
    plottype_start = 1; plottype_end = 2; plottype_inc = 1;
    kick_start = kSquareKick; kick_end = kSquareKick + 1; kick_inc = 1;  
    field_start = kVanish; field_end = field_start + 1; field_inc = 1;
  }
  else {
    int bs_start = kBeamsize0pi;
    int bs_end = kBeamsize0pi;
    //int bs_start = kBeamsize40pi;
    //int bs_end = kBeamsize40pi + 1;
    int bs_inc = 2;

    int inf_start = kOpen;
    int inf_end = kClosed + 1;
    //int inf_end = kOpen + 1;
    int inf_inc = 1;

    int q_start = kNewSupports;
    //int q_start = kNoQuadSupports;
    //int q_end = kNewSupports + 1;
    int q_end = kNoQuadSupports + 1;
    //int q_end = kNoQuads + 1;
    int q_inc = 1;

    //int field_start = kVanish;
    //int field_end = kVanish + 1;
  
    int field_start = kVanish;  
    int field_end = kMapped + 1;
    int field_inc = 1;
  
    int beamstart_start = kDownstreamMandrel;
    //int beamstart_start = kDownstreamMandrel;
    //int beamstart_end = kUpstreamMandrel + 1;
    //int beamstart_end = kDownstreamMandrel + 1;

    //int beamstart_start = kUpstreamMandrel;
    //int beamstart_end = kUpstreamMandrel + 1;

    //int beamstart_start = kUpstreamCryo;
    int beamstart_end = kUpstreamCryo + 1;
    int beamstart_inc = 2;

    int mom_start = 8;
    //int mom_end = 8 + 1;
    //int mom_start = 20;
    int mom_end = 20 + 1;
    int mom_inc = 12;
  }

  

  //
  // I must set btype and field before running....
  //
  int btype, field;
  SetConstants(&btype, &field);


  for ( int bs = 0; bs < Nbeamsizes; bs++ ) {
    for ( int inf = 0; inf < Ninfs; inf++ ) {
      for ( int q = 0; q < Nquads; q++ ) {
	for ( int kick = 0; kick < Nkicks; kick++ ) {
	  for ( int momtype = 0; momtype < Nmomtypes; momtype++ ) {
	    N_dP[bs][inf][q][kick][momtype] = 0;
	    N_dP_NoLaunch[bs][inf][q][kick][momtype] = 0;
	    for ( int dP = 0; dP < 20; dP++ ) {
	      dPOverP[bs][inf][q][kick][momtype][dP] = -1.0;
	      dPOverP_NoLaunch[bs][inf][q][kick][momtype][dP] = -1.0;
	      eff_dP[bs][inf][q][kick][momtype][dP] = -1.0;
	      err_dP[bs][inf][q][kick][momtype][dP] = -1.0;
	      eff_dP_NoLaunch[bs][inf][q][kick][momtype][dP] = -1.0;
	      err_dP_NoLaunch[bs][inf][q][kick][momtype][dP] = -1.0;
	    }
	  }
	}
      }
    }
  }
  
  //
  // Inflector (Kick vs Launch or Delta vs Launch)
  //
  
  cout << "Beamstart: " << Startname(beamstart_start) << "\t" << Startname(beamstart_end-1) << "\t" << beamstart_inc << endl;
  for ( int beamstart = beamstart_start; beamstart < beamstart_end; beamstart+=beamstart_inc ) {
    
    for ( int s = 1; s < 2; s++ ) {
      cout << "  Quads: " << Quadname(q_start) << "\t" << Quadname(q_end-1) << "\t" << q_inc << endl;
      for ( int q = q_start; q < q_end; q+=q_inc ) {

	cout << "    Moms: " << Momname(mom_start) << "\t" << Momname(mom_end-1) << "\t" << mom_inc << endl;
	for ( int mom = mom_start; mom < mom_end; mom+=mom_inc ) {

	  if ( read_from_file ) { continue; }

	  cout << "      Beamtype: " << Beamtypename(btype_start) << "\t" << Beamtypename(btype_end-1) << "\t" << btype_inc << endl;
	  for ( int btype = btype_start; btype < btype_end; btype += btype_inc ) {

	    cout << "        Beamsize: " << Beamsizename(bs_start) << "\t" << Beamsizename(bs_end-1) << "\t" << bs_inc << endl;
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      
	      int beamsize = Beamsize(bs);
	      //cout << beamsize << endl;
	      
	      cout << "          Infs: " << Infname(inf_start) << "\t" << Infname(inf_end-1) << "\t" << inf_inc << endl;
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		
		cout << "            Fields: " << Fieldname(field_start) << "\t" << Fieldname(field_end-1) << "\t" << field_inc << endl;
		for ( int field = field_start; field < field_end; field+=field_inc ) {
		  
		  //if ( beamstart == kUpstreamMandrel && field == kVanish ) { continue; }
		  //if ( beamstart == kUpstreamCryo && field == kVanish && beamsize != 0 ) { continue; }
		  //if ( field == kMapped && bs != kBeamsize40pi ) { continue; }
		  
		  for ( int sigmat = 25; sigmat <= 25; sigmat += 49 ) {

		    cout << "              Kicks: " << Kickname(kick_start) << "\t" << Kickname(kick_end-1) << "\t" << kick_inc << endl;
		    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {

		      cout << "                Momtype: " << Momtypename(momtype_start) << "\t" << Momtypename(momtype_end-1) << "\t" << momtype_inc << endl;		      

		      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
			int sigmap;
			if ( momtype == kBNLmatch ) { sigmap = 20 + mom; }
			if ( momtype == kPerfectmatch ) { sigmap = 8 + mom; }
			//cout << sigmap << "\t" << mom << "\t" << momtype << endl;
			
			for ( int plottype = plottype_start; plottype < plottype_end; plottype += plottype_inc ) {
			
			  double eff, err;
			  double max_eff = 0.0;
			  double max_err = 0.0;
			  double nol_eff = 0.0;
			  double nol_err = 0.0;
			  int max_eff_delta = 0;
			  int max_eff_launch = 0;
			
			  TGraph2D *dt = new TGraph2D();
			  int Np = 0;	    
			
			  for ( int N2d = 0; N2d < 600; N2d++ ) {
			    eff = 0.0; err = 0.0;
			    int launch = -99;
			    int delta = -99;
			    int kickval;
			    if ( kick == kLCRKick ) { kickval = 90; }
			    if ( kick == kSquareKick ) { kickval = 220; }
			    if ( kick == kNoKick ) { kickval = 0; }
			    
			    if ( plottype == 0 ) { GetEff_KL(&eff, &err, &launch, &kickval, s, beamstart, btype, inf, field, prhat, sigmap, sigmat, beamsize, q, momtype, N2d); }
			    if ( plottype == 1 ) { GetEff_DL(&eff, &err, &launch, &delta, kickval, s, beamstart, btype, sigmap, sigmat, field, inf, bs, q, momtype, N2d); }
			    if ( plottype == 2 ) { GetEff_DL_Inf(&eff, &err, &launch, &delta, kickval, s, beamstart, btype, sigmap, sigmat, field, inf, bs, q, momtype, N2d); }
			    if ( plottype == 3 ) { GetEff_DL_InfLength(&eff, &err, &launch, &delta, kickval, s, beamstart, btype, sigmap, sigmat, field, inf, bs, q, momtype, N2d); }
			    if ( plottype == 4 ) { GetEff_DL_RingAcc(&eff, &err, &launch, &delta, kickval, s, beamstart, btype, sigmap, sigmat, field, inf, bs, q, momtype, N2d); }
			    if ( plottype == 5 ) { GetEff_DL_InfAcc(&eff, &err, &launch, &delta, kickval, s, beamstart, btype, sigmap, sigmat, field, inf, bs, q, momtype, N2d); }

			    bool debug = false;
			    if ( debug ) { cout << N2d << "\t" << eff << " +/- " << err << "  (" << delta << " , " << launch << " , " << kickval << ")" << endl; }

			    //if ( launch == 1 || launch == 2 || launch == -1 || launch == -2 )  { continue; }
			    //if ( delta == 1 || delta == 2 || delta == -1 || delta == -2 )  { continue; }
			    //
			    // Cuts
			    //
			    if ( plottype == 1 || plottype == 2 || plottype == 3 || plottype == 4 ) {
			      if ( field == kMapped || field == kVanish ) {
				if ( beamstart == kUpstreamCryo ) {
				  if ( bs == kBeamsize40pi || bs == kBeamsize1pi ) {
				    if ( delta < -20 )  { continue; }
				    if ( delta >  50 )  { continue; }
				    if ( launch > 15 )  { continue; }
				    if ( launch < -20 ) { continue; }
				  }
				}
				if ( beamstart == kDownstreamMandrel ) {
				  if ( bs == kBeamsize40pi || bs == kBeamsize1pi ) {
				    if ( delta < -20 )  { continue; }
				    if ( delta >  50 )  { continue; }
				    if ( launch > 15 )  { continue; }
				    if ( launch < -20 ) { continue; }
				  }
				}
			      }
			    }

			    if ( plottype == 1 ) {
			      if ( field == kVanish ) {
				if ( beamstart == kUpstreamCryo ) {
				  if ( bs == kBeamsize40pi ) {
				    if ( delta < -12 )  { continue; }
				    if ( delta >  0 )  { continue; }
				    if ( launch > 7 )  { continue; }
				    if ( launch < -7 ) { continue; }
				  }
				}
				if ( beamstart == kDownstreamMandrel ) {
				  if ( bs == kBeamsize40pi ) {
				    if ( delta < -12 )  { continue; }
				    if ( delta >  3 )  { continue; }
				    if ( launch > 7 )  { continue; }
				    if ( launch < -7 ) { continue; }
				  }
				}
			      }
			      if ( field == kMapped ) {
				if ( beamstart == kDownstreamMandrel ) {
				  if ( bs == kBeamsize40pi ) {
				    if ( delta < -12 )  { continue; }
				    if ( delta >  3 )  { continue; }
				    if ( launch > 7 )  { continue; }
				    if ( launch < -7 ) { continue; }
				  }
				}
				if ( beamstart == kUpstreamCryo ) {
				  if ( bs == kBeamsize40pi || bs == kBeamsize0pi) {
				    if ( delta < -7 )  { continue; }
				    if ( delta >  0 )  { continue; }
				    if ( launch > -5 )  { continue; }
				    if ( launch < -15 ) { continue; }
				  }
				}
			      }
			    }

			    if ( plottype == 2 || plottype == 3 ) {
			      if ( field == kMapped || field == kVanish ) {
				if ( beamstart == kUpstreamCryo ) {
				  if ( bs == kBeamsize40pi || bs == kBeamsize1pi ) {
				    if ( delta < -20 )  { continue; }
				    if ( delta >  30 )  { continue; }
				    if ( launch > 15 )  { continue; }
				    if ( launch < -20 ) { continue; }
				  }
				}
			      }
			    }
			

			
			    //
			    // End of function calls
			    //			
			    if ( ( plottype == 0 && ( kickval != -99 && launch != -99 ) ) ||
				 ( plottype != 0 && ( delta != -99 && launch != -99 ) ) ) {
			      if ( eff <= 0.0 ) { eff = 1e-4; }
			      if ( eff > max_eff ) {
				max_eff = eff;
				max_err = err;
				max_eff_delta = delta;
				max_eff_launch = launch;
			      }
			      if ( plottype == 0 ) {
				if ( launch == 0 ) {
				  if ( eff > nol_eff ) {
				    nol_eff = eff;
				    nol_err = err;
				  }
				}
			      }
			      else {
				if ( launch == 0 && delta == 0 ) {
				  if ( eff > nol_eff ) {
				    nol_eff = eff;
				    nol_err = err;
				  }
				}
			      }
			  
			      if ( plottype == 0 ) { dt->SetPoint(Np++, kickval, launch, eff); }
			      else {
				if ( debug ) { cout << Np << "\t" << delta << "\t" << launch << "\t" << eff << endl; }
				dt->SetPoint(Np++, delta, launch, eff);
			      }
			      // 			  else if ( plottype == 2 || plottype == 3 || plottype == 5 ) {
			      // 			    if ( debug ) { cout << Np << "\t" << mRadToDeg(delta) << "\t" << mRadToDeg(launch) << endl; }
			      // 			    dt->SetPoint(Np++, mRadToDeg(delta), mRadToDeg(launch), eff);
			      // 			  }
			    }
			    else {
			      //cout << "Breaking at Np = " << Np << endl;
			      break;
			    }
			  }
		      
		      
		      
			  if ( Np == 0 ) { 
			    cout << "No entries for " << "\tScraping=" << Scrapingname(s) << "\tQuad=" << Quadname(q) << "\tStart=" << Startname(beamstart) << "\tBeamtype=" << Beamtypename(btype) << "\tInf=" << Infname(inf) << "\tField=" << Fieldname(field) << "\tsigmaT=" << sigmat << "\tMom=" << Momname(sigmap) << "\tBeamsize=" << beamsize << "\tKick = " << kickval << "\tPlotType=" << plottype << endl;
			    delete dt; continue;	      
			  }
			  else {
			    cout << "Found " << Np << " entries for " << "\tScraping=" << Scrapingname(s) << "\tQuad=" << Quadname(q) << "\tStart=" << Startname(beamstart) << "\tBeamtype=" << Beamtypename(btype) << "\tInf=" << Infname(inf) << "\tField=" << Fieldname(field) << "\tsigmaT=" << sigmat << "\tMom=" << Momname(sigmap) << "\tBeamsize=" << beamsize << "\tKick = " << kickval << "\tPlotType=" << plottype << endl;
			    cout << "MAX EFF       = " << max_eff << " +/- " << max_err << "  \t for dP/P = " << dp_over_p << endl;
			    //cout << "MAX EFF (l=0) = " << nol_eff << " +/- " << nol_err << "  \t for dP/P = " << dp_over_p << endl;
			    if ( plottype == 1 ) {
			      if ( dp_over_p > 0 ) {
				dPOverP[bs][inf][q][kick][momtype][N_dP[bs][inf][q][kick][momtype]] = dp_over_p;
				eff_dP[bs][inf][q][kick][momtype][N_dP[bs][inf][q][kick][momtype]] = max_eff;
				err_dP[bs][inf][q][kick][momtype][N_dP[bs][inf][q][kick][momtype]] = max_err;
				N_dP[bs][inf][q][kick][momtype]++;
				//cout << bs << "\t" << inf << "\t" << mom << "\t" << N_dP[bs][inf][q][kick][momtype] << endl;
			    
				dPOverP_NoLaunch[bs][inf][q][kick][momtype][N_dP_NoLaunch[bs][inf][q][kick][momtype]] = dp_over_p;
				eff_dP_NoLaunch[bs][inf][q][kick][momtype][N_dP_NoLaunch[bs][inf][q][kick][momtype]] = nol_eff;
				err_dP_NoLaunch[bs][inf][q][kick][momtype][N_dP_NoLaunch[bs][inf][q][kick][momtype]] = nol_err;
				N_dP_NoLaunch[bs][inf][q][kick][momtype]++;
			      }
			    }
			  }
		      
			  //continue;
			  if ( !plot_dp_over_p ) { continue; }

			  if ( Np <= 1 ) { continue; }

			  double maxtmp = -1.0;
			  FindMax(dt, &maxtmp, "tmp");
			  cout << "MAX = " << maxtmp << endl;
		      
			  //
			  // Add points to define everything
			  // 		      
		      
		      
		      
			  dt->SetMinimum(0.0);
			  //dt->SetMaximum(100.0);
			  //dt->GetXaxis()->SetRangeUser(-10, 2);
			  //dt->GetYaxis()->SetRangeUser(-22, 0);
		      
			  //dt->Draw();
			  //dt->Draw("COLZ");
			  dt->Draw("CONT4Z");
			  if ( plottype == 0 ) {
			    if ( kick == kLCRKick ) { 
			      dt->GetXaxis()->SetTitle("LCR Kick HV [kV]");
			    }
			    if ( kick == kSquareKick ) { 
			      dt->GetXaxis()->SetTitle("Square Kick [G]");
			    }
			    dt->GetYaxis()->SetTitle("Launch Angle (w.r.t Inf Angle) [mrad]");
			  }
			  if ( plottype == 1 || plottype == 4 ) {
			    dt->GetXaxis()->SetTitle("Inflector Angle (w.r.t Ring) [mrad]");
			    dt->GetYaxis()->SetTitle("Launch Angle (w.r.t Inf Angle) [mrad]");
			  }
			  if ( plottype == 2 || plottype == 3 || plottype == 5  ) {
			    dt->GetXaxis()->SetTitle("Inflector Angle (w.r.t Ring) [mrad]");
			    dt->GetYaxis()->SetTitle("Launch Angle (w.r.t Inf Angle) [mrad]");
			  }
			  dt->GetZaxis()->SetTitle("Muon Survival Rate [%]");
			  gPad->SetRightMargin(0.15);
		      
			  stringstream c1name;
			  string infname;
			  if ( inf == 2 ) { infname = "InflectorClosed"; }
			  if ( inf == 1 ) { infname = "InflectorPartiallyOpen"; }
			  if ( inf == 0 ) { infname = "InflectorOpen"; }
			  string beamstartname;
			  if ( beamstart == 0 ) { beamstartname = "DownstreamMandrel"; }
			  if ( beamstart == 1 ) { beamstartname = "UpstreamMandrel"; }
			  if ( beamstart == 2 ) { beamstartname = "UpstreamCryo"; }
		      

			  bool plotmax = false;
			  if ( plotmax ) {
			  stringstream ss_max;
			  string fracname = "#frac{Stored}{Inject}";
			  stringstream max_loc;
			  max_loc << max_eff_delta << "," << max_eff_launch;
			  if ( max_eff >= 100.0 ) {
			    ss_max << "f" << "^{(" << max_loc.str() << ")}_{max} = 100%";
			    //#equiv" << fracname << "=100%";
			  }
			  else {	    
			    ss_max.precision(2);
			    ss_max << "f" << "^{(" << max_loc.str() << ")}_{max} = " << max_eff << " #pm ";
			    //ss_max << "#varepsilon_{max}#equiv" << fracname << "=" << max_eff << " #pm ";
			    ss_max.precision(1);
			    ss_max << max_err;
			    //ss_max << "#varepsilon_{max} = " << max_eff << " #pm ";
			    //ss_max.precision(1);
			    //ss_max << max_err;
			  }
			  if ( field == 0 ) {
			    if ( beamstart == kUpstreamCryo ) {
			      myText(0.52, 0.86, 0, ss_max.str().c_str(), 0.06);
			    }
			    else {
			      myText(0.20, 0.22, 0, ss_max.str().c_str(), 0.08);
			    }
			  }
			  if ( field == 1 ) {
			    if ( beamstart == 0 ) {
			      myText(0.20, 0.22, 0, ss_max.str().c_str(), 0.08);
			    }
			    else {
			      myText(0.20, 0.82, 0, ss_max.str().c_str(), 0.08);
			    }
			  }

			  cout << ss_max.str() << endl;
			  }
		      
			  FindMax(dt, &maxtmp, "tmp");
		      
			  string plottypename;
			  if ( plottype == 0 ) { plottypename = "kick_launch"; }
			  if ( plottype == 1 ) { plottypename = "delta_launch"; }
			  if ( plottype == 2 ) { plottypename = "delta_launch_inflector"; }
			  if ( plottype == 3 ) { plottypename = "delta_launch_inflectorinternal"; }
			  if ( plottype == 4 ) { plottypename = "delta_launch_ringacceptance"; }
			  if ( plottype == 5 ) { plottypename = "delta_launch_inflectoracceptance"; }

			 
			  c1name << Plotname(plottypename, none, beamstart, bs, q, inf, kick, momtype, field, btype);

			  c1->SaveAs(c1name.str().c_str());
			  c1->Clear();   
			  delete dt;
			  cout << "Wrote: " << c1name.str() << endl;
			  cout << endl;
			  //return;
			} // plottype
		      } // momtype
		    } // kick
		  } // sigmat
		} // field
	      } // inf
	    } // beamsize
	  } // beamtype
	} // mom
      } // quads
    } // scraping
  } // beamstart
  
    //return;



  

  //
  // I must set btype and field before running....
  //
  SetConstants(&btype, &field);


 
  cout << "Creating TGraphErrors" << endl;

 
  TGraphErrors *dte[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes];  
  TGraphErrors *dte_NoLaunch[Nbeamsizes][Ninfs][Nquads][Nkicks][Nmomtypes];

  TGraphErrors *dte_Ideal[Nbeamsizes][Nmomtypes];
  for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
    for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
      dte_Ideal[bs][momtype] = new TGraphErrors();
    }
  }
  
  for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {  
      for ( int q = q_start; q < q_end; q+=q_inc ) {  
	for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	  for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
	    dte[bs][inf][q][kick][momtype] = NULL;
	    dte_NoLaunch[bs][inf][q][kick][momtype] = NULL;
	  }
	}
      }
    }
  }

  cout << "Done Creating TGraphErrors" << endl;
    
  if ( read_from_file ) {;

    //
    // Load Ideal TGraph
    //
    if ( btype == kUniform ) {
      FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
    }
    if ( btype == kGaussian ) {
      FillEffs_Beamsize0pi_Gaussian_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      FillEffs_Beamsize0pi_Gaussian_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      FillEffs_Beamsize40pi_Gaussian_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      FillEffs_Beamsize40pi_Gaussian_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
    }

    // I need to fix the 5e-3 point
    for ( int bs = 0; bs < Nbeamsizes; bs++ ) {
      for ( int momtype = 0; momtype < Nmomtypes; momtype++ ) {
	int lowest = -1;
	int fix = -1;
	for ( int i = 0; i < 20; i++ ) {
	  if ( dPOverP[bs][kOpen][kNoQuads][kNoKick][momtype][i] == 0.05 ) { fix = i; }
	  if ( dPOverP[bs][kOpen][kNoQuads][kNoKick][momtype][i] == 0.01 ) { lowest = i; }
	  if ( lowest >= 0 && fix >= 0 ) { break; }
	}
	if ( lowest >= 0 && fix >= 0 ) {
	  cout << "Setting " << eff_dP[bs][kOpen][kNoQuads][kNoKick][momtype][fix] << " --> " << eff_dP[bs][kOpen][kNoQuads][kNoKick][momtype][lowest] << endl;
	  eff_dP[bs][kOpen][kNoQuads][kNoKick][momtype][fix] = eff_dP[bs][kOpen][kNoQuads][kNoKick][momtype][lowest];
	  err_dP[bs][kOpen][kNoQuads][kNoKick][momtype][fix] = err_dP[bs][kOpen][kNoQuads][kNoKick][momtype][lowest];
	  cout << "Setting " << eff_dP_NoLaunch[bs][kOpen][kNoQuads][kNoKick][momtype][fix] << " --> " << eff_dP_NoLaunch[bs][kOpen][kNoQuads][kNoKick][momtype][lowest] << endl;
	  eff_dP_NoLaunch[bs][kOpen][kNoQuads][kNoKick][momtype][fix] = eff_dP_NoLaunch[bs][kOpen][kNoQuads][kNoKick][momtype][lowest];
	  err_dP_NoLaunch[bs][kOpen][kNoQuads][kNoKick][momtype][fix] = err_dP_NoLaunch[bs][kOpen][kNoQuads][kNoKick][momtype][lowest];
	}
      }
    }

    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	
	int np = 0;
	for ( int i = 0; i < 20; i++ ) {
	  if ( zoom ) {
	    if ( dPOverP[bs][kOpen][kNoQuads][kNoKick][momtype][i] < zoomval ) { continue; }
	    if ( dPOverP[bs][kOpen][kNoQuads][kNoKick][momtype][i] >= 5 ) { continue; }
	  }
	  if ( dPOverP[bs][kOpen][kNoQuads][kNoKick][momtype][i] > 0 ) {
	    dte_Ideal[bs][momtype]->SetPoint(np, dPOverP[bs][kOpen][kNoQuads][kNoKick][momtype][i], eff_dP[bs][kOpen][kNoQuads][kNoKick][momtype][i]);
	    dte_Ideal[bs][momtype]->SetPointError(np, 0.0, err_dP[bs][kOpen][kNoQuads][kNoKick][momtype][i]);
	    np++;
	  }
	}

	dte_Ideal[bs][momtype]->SetLineStyle(Style_Ideal());
	dte_Ideal[bs][momtype]->SetMarkerColor(Color_Ideal());
	dte_Ideal[bs][momtype]->SetLineColor(Color_Ideal());
      }
    }


    if ( beamstart_start == kCentralOrbit ) {
      if ( btype == kUniform ) {
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      }
      if ( btype == kGaussian ) {
	FillEffs_Beamsize0pi_Gaussian_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Gaussian_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Gaussian_Open_NoQuads_Vanish_NoKick_BNLmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Gaussian_Open_NoQuads_Vanish_NoKick_Perfectmatch_CentralOrbit(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      }
    }

    if ( beamstart_start == kDownstreamMandrel ) {
      if ( btype == kUniform ) {
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);


	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_DownstreamMandrel(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      }
      if ( btype == kGaussian ) {;}
    }
    if ( beamstart_start == kUpstreamCryo ) {
      if ( btype == kUniform ) {
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_SquareKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);


	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Open_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_Closed_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);

	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuads_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NewSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize0pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_BNLmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
	FillEffs_Beamsize40pi_Uniform_PartiallyOpen_NoQuadSupports_Vanish_LCRKick_Perfectmatch_UpstreamCryo(N_dP, N_dP_NoLaunch, dPOverP, dPOverP_NoLaunch, eff_dP, err_dP, eff_dP_NoLaunch, err_dP_NoLaunch);
      }
      if ( btype == kGaussian ) {;}
    }


    // I need to fix the 5e-3 line

    for ( int bs = 0; bs < Nbeamsizes; bs++ ) {
      for ( int inf = 0; inf < Ninfs; inf++ ) {
	for ( int q = 0; q < Nquads; q++ ) {
	  for ( int kick = 0; kick < Nkicks; kick++ ) {
	    for ( int momtype = 0; momtype < Nmomtypes; momtype++ ) {
	      int lowest = -1;
	      int fix = -1;
	      for ( int i = 0; i < 20; i++ ) {
		if ( dPOverP[bs][inf][q][kick][momtype][i] == 0.05 ) { fix = i; }
		if ( dPOverP[bs][inf][q][kick][momtype][i] == 0.01 ) { lowest = i; }
		if ( lowest >= 0 && fix >= 0 ) { break; }
	      }
	      if ( lowest >= 0 && fix >= 0 ) {
		cout << "Setting " << eff_dP[bs][inf][q][kick][momtype][fix] << " --> " << eff_dP[bs][inf][q][kick][momtype][lowest] << endl;
		eff_dP[bs][inf][q][kick][momtype][fix] = eff_dP[bs][inf][q][kick][momtype][lowest];
		err_dP[bs][inf][q][kick][momtype][fix] = err_dP[bs][inf][q][kick][momtype][lowest];
		cout << "Setting " << eff_dP_NoLaunch[bs][inf][q][kick][momtype][fix] << " --> " << eff_dP_NoLaunch[bs][inf][q][kick][momtype][lowest] << endl;
		eff_dP_NoLaunch[bs][inf][q][kick][momtype][fix] = eff_dP_NoLaunch[bs][inf][q][kick][momtype][lowest];
		err_dP_NoLaunch[bs][inf][q][kick][momtype][fix] = err_dP_NoLaunch[bs][inf][q][kick][momtype][lowest];
	      }
	    }
	  }
	}
      }
    }

    //     for ( int bs = 0; bs < Nbeamsizes; bs++ ) {
    //       for ( int inf = 0; inf < Ninfs; inf++ ) {
    // 	for ( int q = 0; q < Nquads; q++ ) {
    // 	  for ( int kick = 0; kick < Nkicks; kick++ ) {
    // 	    for ( int momtype = 0; momtype < Nmomtypes; momtype++ ) {
    // 	      for ( int i = 0; i < 20; i++ ) {
    // 		if ( dPOverP[bs][inf][q][kick][momtype][i] > 0 ) { cout << "dPOverP[" << Beamsizename(bs) << "][" << Infname(inf) << "][" << Quadname(q) << "][" << Kickname(kick) << "][" << Momtypename(momtype) << "][" << i << "] = " << dPOverP[bs][inf][q][kick][momtype][i] << endl; }
    // 	      }
    // 	    }
    // 	  }
    // 	}
    //       }
    //     }


    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	    for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
	      for ( int i = 0; i < 20; i++ ) {
		//if ( dPOverP[bs][inf][q][kick][momtype][i] < 0.1 ) { continue; }
		if ( dPOverP[bs][inf][q][kick][momtype][i] > 0 ) { cout << "dPOverP[" << Beamsizename(bs) << "][" << Infname(inf) << "][" << Quadname(q) << "][" << Kickname(kick) << "][" << Momtypename(momtype) << "][" << i << "] = " << dPOverP[bs][inf][q][kick][momtype][i] << endl; }
		if ( dPOverP[bs][inf][q][kick][momtype][i] > 0 ) { N_dP[bs][inf][q][kick][momtype]++; }
		if ( dPOverP_NoLaunch[bs][inf][q][kick][momtype][i] > 0 ) { N_dP_NoLaunch[bs][inf][q][kick][momtype]++; }
	      }
	    }
	  }
	}
      }
    }
  }
  

  for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
      for ( int q = q_start; q < q_end; q+=q_inc ) {
	for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	  for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
	    cout << "N[" << Beamsizename(bs) << "][" << Infname(inf) << "][" << Quadname(q) << "][" << Kickname(kick) << "][" << Momtypename(momtype) << "] = " << N_dP[bs][inf][q][kick][momtype] << endl;
	    if ( N_dP[bs][inf][q][kick][momtype] > 0 ) { dte[bs][inf][q][kick][momtype] = new TGraphErrors(); }
	    if ( N_dP_NoLaunch[bs][inf][q][kick][momtype] > 0 ) { dte_NoLaunch[bs][inf][q][kick][momtype] = new TGraphErrors(); }
	  }
	}
      }
    }
  }

  
  for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
      for ( int q = q_start; q < q_end; q+=q_inc ) {
	for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	  for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
	    if ( dte[bs][inf][q][kick][momtype] ) {
	      int np = 0;
	      for ( int i = 0; i < N_dP[bs][inf][q][kick][momtype]; i++ ) {
		
		if ( zoom ) {
		  if ( dPOverP[bs][inf][q][kick][momtype][i] < zoomval ) { continue; }
		  if ( dPOverP[bs][inf][q][kick][momtype][i] >= 5 ) { continue; }
		}

		dte[bs][inf][q][kick][momtype]->SetPoint(np, dPOverP[bs][inf][q][kick][momtype][i], eff_dP[bs][inf][q][kick][momtype][i]);
		dte[bs][inf][q][kick][momtype]->SetPointError(np, 0.0, err_dP[bs][inf][q][kick][momtype][i]);
		np++;
	      }
	    }
	    
	    
	    if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) {
	      int np = 0;
	      for ( int i = 0; i < N_dP_NoLaunch[bs][inf][q][kick][momtype]; i++ ) {
		
		if ( zoom ) {
		  if ( dPOverP_NoLaunch[bs][inf][q][kick][momtype][i] < zoomval ) { continue; }
		  if ( dPOverP_NoLaunch[bs][inf][q][kick][momtype][i] >= 5 ) { continue; }
		}

		dte_NoLaunch[bs][inf][q][kick][momtype]->SetPoint(np, dPOverP_NoLaunch[bs][inf][q][kick][momtype][i], eff_dP_NoLaunch[bs][inf][q][kick][momtype][i]);
		dte_NoLaunch[bs][inf][q][kick][momtype]->SetPointError(np, 0.0, err_dP_NoLaunch[bs][inf][q][kick][momtype][i]);
		np++;
	      }
	    }
	  }
	}
      }
    }
  }
  

  cout << "HI THOMAS" << endl;


    
  if ( !read_from_file ) {
    cout << "I SHOULD BE HERE" << endl;
    stringstream outname;
    outname << "Includes/FillEffs_";
    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
      outname << Beamsizename(bs) << "_";
      for ( int btype = btype_start; btype < btype_end; btype += btype_inc ) {
	outname << Beamtypename(btype) << "_";
	for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {  
	  outname << Infname(inf) << "_";
	  for ( int q = q_start; q < q_end; q+=q_inc ) {  
	    outname << Quadname(q) << "_";
	    for ( int field = field_start; field < field_end; field+=field_inc ) {
	      outname << Fieldname(field) << "_";
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		outname << Kickname(kick) << "_";
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  outname << Momtypename(momtype) << "_";
		  for ( int beamstart = beamstart_start; beamstart < beamstart_end; beamstart+=beamstart_inc ) {
		    outname << Startname(beamstart) << ".C";
		
		    ofstream out;
		    outname.str("");
		    outname << "Includes/";
		
		    stringstream funcname;
		    funcname << "FillEffs_";
		    funcname << Beamsizename(bs) << "_";
		    funcname << Beamtypename(btype) << "_";
		    funcname << Infname(inf) << "_";
		    funcname << Quadname(q) << "_";
		    funcname << Fieldname(field) << "_";
		    funcname << Kickname(kick) << "_";
		    funcname << Momtypename(momtype) << "_";
		    funcname << Startname(beamstart);
		
		    outname << funcname.str() << ".C";
		    cout << funcname.str() << endl;
		
		    out.open(outname.str().c_str());
		
		    stringstream dPsize;
		    dPsize << "[" << Nbeamsizes << "][" << Ninfs << "][" << Nquads << "][" << Nkicks << "][" << Nmomtypes << "]";
		
		    out << "void " << funcname.str() << "(int N_dP" << dPsize.str() << ", int N_dP_NoLaunch" << dPsize.str() << ", double dPOverP" << dPsize.str() << "[20], double dPOverP_NoLaunch" << dPsize.str() << "[20], double eff_dP" << dPsize.str() << "[20], double err_dP" << dPsize.str() << "[20], double eff_dP_NoLaunch" << dPsize.str() << "[20], double err_dP_NoLaunch" << dPsize.str() << "[20]) {" << endl;
		    out << endl;
		
		    out << "  int kGaussian = 0;" << endl;
		    out << "  int kUniform = 1;" << endl;
		    out << "  int kDownstreamMandrel = 0;" << endl;
		    out << "  int kUpstreamMandrel = 1;" << endl;
		    out << "  int kUpstreamCryo = 2;" << endl;
		    out << "  int kVanish = 0;" << endl;
		    out << "  int kMapped = 1;" << endl;
		    out << "  int kOpen = 0;" << endl;
		    out << "  int kPartiallyOpen = 1;" << endl;
		    out << "  int kClosed = 2;" << endl;
		    out << "  int kBeamsize0pi = 0;" << endl;
		    out << "  int kBeamsize1pi = 1;" << endl;
		    out << "  int kBeamsize20pi = 2;" << endl;
		    out << "  int kBeamsize40pi = 3;" << endl;
		    out << "  int kBeamsize42pi = 4;" << endl;
		    out << "  int kScrapingON = 0;" << endl;
		    out << "  int kScrapingOFF = 1;" << endl;
		    out << "  int kNewSupports = 0;" << endl;
		    out << "  int kNoQuads = 1;" << endl;
		    out << "  int kNoQuadSupports = 2;" << endl;
		    out << "  int kFullHighMom = 3;" << endl;
		    out << "  int kFullHigherMom = 4;" << endl;
		    out << "  int kFullHighestMom = 5;" << endl;
		    out << "  int kBNLmatch = 0;" << endl;
		    out << "  int kPerfectmatch = 1;" << endl;
		    out << endl;
		
		
		    int j = 0;
		    double dp_min = 99.9;
		    int dp_min_pos = -1;
		    cout << N_dP[bs][inf][q][kick][momtype] << endl;
		    while ( j < N_dP[bs][inf][q][kick][momtype] ) {
		      double dp_min_tmp = 99.9;
		      for ( int i = 0; i < N_dP[bs][inf][q][kick][momtype]; i++ ) {
			if ( j == 0 ) {
			  if ( dPOverP[bs][inf][q][kick][momtype][i] < dp_min ) { dp_min = dPOverP[bs][inf][q][kick][momtype][i]; dp_min_pos = i; }
			}
			else {
			  if ( dPOverP[bs][inf][q][kick][momtype][i] < dp_min_tmp &&
			       dPOverP[bs][inf][q][kick][momtype][i] > dp_min ) { dp_min_tmp = dPOverP[bs][inf][q][kick][momtype][i]; dp_min_pos = i; }
			}
		      }
		      if ( j != 0 ) { dp_min = dp_min_tmp; }	
		  
		      string quad_name = "k" + Quadname(q);
		      string inf_name = "k" + Infname(inf);
		      string bs_name = "k" + Beamsizename(bs);
		      string btype_name = "k" + Beamtypename(btype);
		      string field_name = "k" + Fieldname(field);
		      string kick_name = "k" + Kickname(kick);
		      string momtype_name = "k" + Momtypename(momtype);

		      out << "  " << "dPOverP[" << bs_name << "][" << inf_name << "][" << quad_name << "][" << kick_name << "][" << momtype_name << "][" << j << "] = " << dPOverP[bs][inf][q][kick][momtype][dp_min_pos]*100 << "; \t";
		      out << "  " << "eff_dP[" << bs_name << "][" << inf_name << "][" << quad_name << "][" << kick_name << "][" << momtype_name << "][" << j << "] = " << eff_dP[bs][inf][q][kick][momtype][dp_min_pos] << "; \t";
		      out << "  " << "err_dP[" << bs_name << "][" << inf_name << "][" << quad_name << "][" << kick_name << "][" << momtype_name << "][" << j << "] = " << err_dP[bs][inf][q][kick][momtype][dp_min_pos] << ";" << endl;
		      j++;	
		    }

		
		    j = 0;
		    dp_min = 99.9;
		    dp_min_pos = -1;
		    while ( j < N_dP_NoLaunch[bs][inf][q][kick][momtype] ) {
		      double dp_min_tmp = 99.9;
		      for ( int i = 0; i < N_dP_NoLaunch[bs][inf][q][kick][momtype]; i++ ) {
			if ( j == 0 ) {
			  if ( dPOverP_NoLaunch[bs][inf][q][kick][momtype][i] < dp_min ) { dp_min = dPOverP_NoLaunch[bs][inf][q][kick][momtype][i]; dp_min_pos = i; }
			}
			else {
			  if ( dPOverP_NoLaunch[bs][inf][q][kick][momtype][i] < dp_min_tmp &&
			       dPOverP_NoLaunch[bs][inf][q][kick][momtype][i] > dp_min ) { dp_min_tmp = dPOverP_NoLaunch[bs][inf][q][kick][momtype][i]; dp_min_pos = i; }
			}
		      }
		      if ( j != 0 ) { dp_min = dp_min_tmp; }	
		
		      string quad_name = "k" + Quadname(q);
		      string inf_name = "k" + Infname(inf);
		      string bs_name = "k" + Beamsizename(bs);
		      string btype_name = "k" + Beamtypename(btype);
		      string field_name = "k" + Fieldname(field);
		      string kick_name = "k" + Kickname(kick);
		      string momtype_name = "k" + Momtypename(momtype);
		  
		      out << "  " << "dPOverP_NoLaunch[" << bs_name << "][" << inf_name << "][" << quad_name << "][" << kick_name << "][" << momtype_name << "][" << j << "] = " << dPOverP_NoLaunch[bs][inf][q][kick][momtype][dp_min_pos]*100 << "; \t";
		      out << "  " << "eff_dP_NoLaunch[" << bs_name << "][" << inf_name << "][" << quad_name << "][" << kick_name << "][" << momtype_name << "][" << j << "] = " << eff_dP_NoLaunch[bs][inf][q][kick][momtype][dp_min_pos] << "; \t";
		      out << "  " << "err_dP_NoLaunch[" << bs_name << "][" << inf_name << "][" << quad_name << "][" << kick_name << "][" << momtype_name << "][" << j << "] = " << err_dP_NoLaunch[bs][inf][q][kick][momtype][dp_min_pos] << ";" << endl;
		      j++;
		    }
		
		    out << "}" << endl;
		    out.close();
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  if ( !read_from_file ) { return; }
  cout << "AGAIN" << endl;
  

  cout << "Making plots" << endl;

  //
  // Set generic properties
  //
 
  for ( int q = q_start; q < q_end; q+=q_inc ) {
    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	  for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	    
	    if ( dte[bs][inf][q][kick][momtype] ) {	    
	      dte[bs][inf][q][kick][momtype]->SetLineWidth(3);
	      //if ( inf == kOpen ) { dte[bs][inf][q][kick][momtype]->SetLineStyle(1); }
	      //if ( inf == kClosed ) { dte[bs][inf][q][kick][momtype]->SetLineStyle(2); }
	      //if ( inf == kPartiallyOpen ) { dte[bs][inf][q][kick][momtype]->SetLineStyle(3); }
	      dte[bs][inf][q][kick][momtype]->SetMinimum(0.0);
	      dte[bs][inf][q][kick][momtype]->GetXaxis()->SetTitle("#deltaP/P (%) of Incoming Muon Beam");
	      dte[bs][inf][q][kick][momtype]->GetYaxis()->SetTitle("Muon Survival Rate [%]");
	    }
	    if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) {
	      dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineWidth(3);
	      //if ( inf == kOpen ) { dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(1); }
	      //if ( inf == kClosed ) { dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(2); }
	      //if ( inf == kPartiallyOpen ) { dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(3); }
	      dte_NoLaunch[bs][inf][q][kick][momtype]->SetMinimum(0.0);
	      dte_NoLaunch[bs][inf][q][kick][momtype]->GetXaxis()->SetTitle("#deltaP/P (%) of Incoming Muon Beam");
	      dte_NoLaunch[bs][inf][q][kick][momtype]->GetYaxis()->SetTitle("Muon Survival Rate [%]");
	    }
	  }
	}
      }
    }
  }



  for ( int beamstart = beamstart_start; beamstart < beamstart_end; beamstart+=beamstart_inc ) {

    int plot_start = 0;
    int plot_end = 2;
    if ( beamstart == kCentralOrbit ) { plot_start = 1; plot_end = 2; }
    else { plot_start = 0; plot_end = 1; }

  
    //
    // Plotting for all state
    //
    cout << endl;
    cout << "Plot individual states" << endl;
    cout << endl;

    if ( 0 ) {
      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      int beamsize = Beamsize(bs);
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		if ( dte[bs][inf][q][kick][momtype] == NULL ) { continue; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] == NULL ) { continue; }
		
		for ( int plot = plot_start; plot < plot_end; plot++ ) {
		  bool first = true;
		  
		  if ( plot == 0 ) {
		    if ( dte[bs][inf][q][kick][momtype] ) {
		      dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color(plot+1));
		      dte[bs][inf][q][kick][momtype]->SetLineColor(Color(plot+1));
		      dte[bs][inf][q][kick][momtype]->Draw("ALP");
		    }
		  }
		  if ( plot == 1 ) {
		    if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		      dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color(plot+1));
		      dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color(plot+1));
		      dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP");
		    }
		  }
	      
		  gPad->SetRightMargin(0.15);
		  PlotBasic(beamsize, mom_start, 0.70);
	      
		  TLegend *leg = new TLegend(0.47, 0.75, 0.85, 0.88);
		  leg->SetTextFont(42);
		  leg->SetTextSize(0.03);
		  leg->SetFillColor(0);
	      
		  stringstream name;
		  name << "(" << Quadname(q) << ") (" << Infname(inf) << ") (" << Fieldname(field) << ")" << endl;
	      
		  if ( plot == 0 ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  leg->Draw("same");
	      
		  stringstream c1name;
		  string pname = Plotname("Individual", none, bs, q, inf, kick, momtype, field, btype);
		  //c1name << Plotname("Individual", none, bs, q, inf, kick, momtype, field, btype);
		  cout << "Trying to save file: " << c1name.str() << endl;
		  c1->SetLogx(1);
		  c1->SaveAs(c1name.str().c_str());
		  c1->Clear();   
		  cout << "Wrote: " << c1name.str() << endl;
		  cout << endl;
		}
	      }
	    }
	  }
	}
      }




      //
      // Plotting for all kick states together 
      //
      cout << endl;
      cout << "Plot kick states together" << endl;
      cout << endl;

  
      //
      // Plotting for all beamtypes states together 
      //
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      int beamsize = Beamsize(bs);
	    
	      bool found = false;
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	  
	      for ( int plot = plot_start; plot < plot_end; plot++ ) {
		bool first = true;
	    
		double max = 0.0;
		for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		}
	    
	    
		for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	    
	    
		for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Kick(kick));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Kick(kick));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Kick(kick));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Kick(kick));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
	    
		gPad->SetRightMargin(0.15);
		PlotBasic(beamstart, bs, q, inf, -1, momtype, field, btype, 0.7);
	    
		TLegend *leg = new TLegend(0.60, 0.75, 0.85, 0.88);
		leg->SetTextFont(42);
		leg->SetTextSize(0.03);
		leg->SetFillColor(0);
	    
		for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		  stringstream name;
		  name << Kickname(kick);
	      
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		}
		leg->Draw("same");
	    
		stringstream c1name;
		c1name << Plotname("AllKicks", "", beamstart, bs, q, inf, -1, momtype, field, btype);
		cout << "Trying to save file: " << c1name.str() << endl;
		if ( zoom ) { c1->SetLogx(0); }
		else { c1->SetLogx(1); }
		c1->SaveAs(c1name.str().c_str());
		c1->Clear();   
		cout << "Wrote: " << c1name.str() << endl;
		cout << endl;
		delete leg;
	      }
	    }
	  }
	}
      }



      //
      // Plotting for all momtype states together 
      //
      cout << endl;
      cout << "Plot momtype states together" << endl;
      cout << endl;

  
      //
      // Plotting for all beamtypes states together 
      //
      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      int beamsize = Beamsize(bs);
	    
	      bool found = false;
	      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	  
	      for ( int plot = plot_start; plot < plot_end; plot++ ) {
		bool first = true;
	    
		double max = 0.0;
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		}
	    
	    
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	    
	    
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Momtype(momtype));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Momtype(momtype));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Momtype(momtype));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Momtype(momtype));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
	    
		gPad->SetRightMargin(0.15);
		PlotBasic(beamstart, bs, q, inf, kick, -1, field, btype, 0.7);
	    
		TLegend *leg = new TLegend(0.60, 0.75, 0.85, 0.88);
		leg->SetTextFont(42);
		leg->SetTextSize(0.03);
		leg->SetFillColor(0);
	    
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  stringstream name;
		  name << NiceMomtypename(momtype);
	      
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		}
		leg->Draw("same");
	    
		stringstream c1name;
		c1name << Plotname("AllMomtypes", "", beamstart, bs, q, inf, kick, -1, field, btype);
		cout << "Trying to save file: " << c1name.str() << endl;
		if ( zoom ) { c1->SetLogx(0); }
		else { c1->SetLogx(1); }
		c1->SaveAs(c1name.str().c_str());
		c1->Clear();   
		cout << "Wrote: " << c1name.str() << endl;
		cout << endl;
		delete leg;
	      }
	    }
	  }
	}
      }

    



      //
      // Plotting for all momtype states together 
      //
      cout << endl;
      cout << "Plot momtype and kick states together" << endl;
      cout << endl;

  
      //
      // Plotting for all beamtypes states together 
      //
      for ( int q = q_start; q < q_end; q+=q_inc ) {
	for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	  for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	    int beamsize = Beamsize(bs);
	  
	    bool found = false;
	    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	    }

	    for ( int plot = plot_start; plot < plot_end; plot++ ) {
	      bool first = true;
	    
	      double max = 0.0;
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		}
	      }
	    
	    
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	      }
	    
	    
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Momtype_X(momtype, kick));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Momtype_X(momtype, kick));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Momtype_X(momtype, kick));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Momtype_X(momtype, kick));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
	      }
	    
	      gPad->SetRightMargin(0.15);
	      PlotBasic(beamstart, bs, q, inf, -1, -1, field, btype, 0.5);
	    
	      TLegend *leg = new TLegend(0.45, 0.50, 0.75, 0.88);
	      leg->SetTextFont(42);
	      leg->SetTextSize(0.03);
	      leg->SetFillColor(0);
	    
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		  stringstream name;
		  name << "#splitline{" << NiceMomtypename(momtype) << "}{" << NiceKickname(kick) << "}";
		
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		}
	      }
	      leg->Draw("same");
	    
	      stringstream c1name;
	      c1name << Plotname("AllKicksMomtypes", "", beamstart, bs, q, inf, -1, -1, field, btype);
	      cout << "Trying to save file: " << c1name.str() << endl;
	      if ( zoom ) { c1->SetLogx(0); }
	      else { c1->SetLogx(1); }
	      c1->SaveAs(c1name.str().c_str());
	      c1->Clear();   
	      cout << "Wrote: " << c1name.str() << endl;
	      cout << endl;
	      delete leg;
	    }
	  }
	}
      }



  
      //
      // Plotting for all beamsize states together 
      //
      cout << endl;
      cout << "Plot beamsize and kick states together" << endl;
      cout << endl;
  
      //
      // Plotting for all beamsize states together 
      //
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	  
	    bool found = false;
	    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	    }
	  
	    for ( int plot = plot_start; plot < plot_end; plot++ ) {
	      bool first = true;
	    
	      double max = 0.0;
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		}
	      }
	    
	    
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	      }
	    
	    
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Beamsize_X(bs, kick));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Beamsize_X(bs, kick));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Beamsize_X(bs, kick));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Beamsize_X(bs, kick));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
	      }
	    
	      gPad->SetRightMargin(0.15);
	      PlotBasic(beamstart, -1, q, inf, -1, momtype, field, btype, 0.5);
	    
	      TLegend *leg = new TLegend(0.60, 0.55, 0.85, 0.88);
	      leg->SetTextFont(42);
	      leg->SetTextSize(0.025);
	      leg->SetFillColor(0);
	    
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  stringstream name;
		  int beamsize = Beamsize(bs);
		  name << "#splitline{" << NiceBeamsizename(bs) << "}{" << NiceKickname(kick) << "}";
		
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		}
	      }
	      leg->Draw("same");
	    
	      stringstream c1name;
	      c1name << Plotname("AllKicksBeamsizes", "", beamstart, -1, q, inf, -1, momtype, field, btype);
	      cout << "Trying to save file: " << c1name.str() << endl;
	      if ( zoom ) { c1->SetLogx(0); }
	      else { c1->SetLogx(1); }
	      c1->SaveAs(c1name.str().c_str());
	      c1->Clear();   
	      cout << "Wrote: " << c1name.str() << endl;
	      cout << endl;
	      delete leg;
	    }
	  }
	}
      }


    } // if ( 0 ) {}




  
  
      //
      // Plotting for all beamsize states together 
      //
      cout << endl;
      cout << "Plot beamsize states together" << endl;
      cout << endl;
  
      //
      // Plotting for all beamsize states together 
      //
      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {

	      bool found = false;
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	  
	      for ( int plot = plot_start; plot < plot_end; plot++ ) {
		bool first = true;
	    
		double max = 0.0;
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		}
	    
	    
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	    
	    
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Beamsize(bs));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Beamsize(bs));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Beamsize(bs));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Beamsize(bs));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
	    
		gPad->SetRightMargin(0.15);
		PlotBasic(beamstart, -1, q, inf, kick, momtype, field, btype, 0.55);
	    
		TLegend *leg = new TLegend(0.55, 0.55, 0.85, 0.88);
		leg->SetTextFont(42);
		leg->SetTextSize(0.03);
		leg->SetFillColor(0);
	    
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  stringstream name;
		  int beamsize = Beamsize(bs);
		  name << NiceBeamsizename(bs);
	      
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		}
		leg->Draw("same");
	    
		stringstream c1name;
		c1name << Plotname("AllBeamsizes", "", beamstart, -1, q, inf, kick, momtype, field, btype);
		cout << "Trying to save file: " << c1name.str() << endl;
		if ( zoom ) { c1->SetLogx(0); }
		else { c1->SetLogx(1); }
		c1->SaveAs(c1name.str().c_str());
		c1->Clear();   
		cout << "Wrote: " << c1name.str() << endl;
		cout << endl;
		delete leg;
	      }
	    }
	  }
	}
      }



  
  
      //
      // Plotting for all quad states together 
      //
      cout << endl;
      cout << "Plot quad states together" << endl;
      cout << endl;

      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      int beamsize = Beamsize(bs);
	  
	      bool found = false;
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	  
	      for ( int plot = plot_start; plot < plot_end; plot++ ) {
		bool first = true;
	    
		double max = 0.0;
		FindMax(dte_Ideal[bs][momtype], &max, Beamsizename(bs));
		for ( int q = q_start; q < q_end; q+=q_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Quadname(q));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Quadname(q));
		  }
		}
	    
	    
		dte_Ideal[bs][momtype]->SetMaximum(1.1*max);
		for ( int q = q_start; q < q_end; q+=q_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	    
	    
	    
		for ( int q = q_start; q < q_end; q+=q_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad(q));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad(q));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad(q));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad(q));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
		if ( beamstart != kCentralOrbit ) { dte_Ideal[bs][momtype]->Draw("LP"); }
	    	    
		TLegend *leg = new TLegend(0.60, 0.50, 0.80, 0.88);
		leg->SetTextFont(42);
		leg->SetTextSize(0.03);
		leg->SetFillColor(0);
	    
		gPad->SetRightMargin(0.15);
		PlotBasic(beamstart, bs, -1, inf, kick, momtype, field, btype, 0.65);

		// Order for Quads is NoQuads -> FullQuads
		vector<int> q_id;
		for ( int q = q_start; q < q_end; q+=q_inc ) {
		  if ( q == kNoQuads ) { q_id.push_back(kNoQuads); }
		}
		for ( int q = q_start; q < q_end; q+=q_inc ) {
		  if ( q == kNoQuadSupports ) { q_id.push_back(kNoQuadSupports); }
		}
		for ( int q = q_start; q < q_end; q+=q_inc ) {
		  if ( q == kNewSupports ) { q_id.push_back(kNewSupports); }
		}
		
		if ( beamstart != kCentralOrbit ) { leg->AddEntry(dte_Ideal[bs][momtype], "Ideal x_{e} = R_{m}", "LP"); }
		for ( int qid = 0; qid < (int)q_id.size(); qid++ ) {
		  int q = q_id[qid];
		  stringstream name;
		  name << NiceQuadname(q);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { 
		    leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP");
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP");
		  }
		}
		leg->Draw("same");
	    
		stringstream c1name;
		c1name << Plotname("AllQuads", "", beamstart, bs, -1, inf, kick, momtype, field, btype);
		cout << "Trying to save file: " << c1name.str() << endl;
		if ( zoom ) { c1->SetLogx(0); }
		else { c1->SetLogx(1); }
		c1->SaveAs(c1name.str().c_str());
		c1->Clear();   
		cout << "Wrote: " << c1name.str() << endl;
		cout << endl;
		delete leg;
	      }
	    }
	  }
	}
      }




  
      //
      // Plotting for all inflector states together 
      //
      cout << endl;
      cout << "Plot inflector states together" << endl;
      cout << endl;

      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      int beamsize = Beamsize(bs);
	    
	      bool found = false;
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	  
	      for ( int plot = plot_start; plot < plot_end; plot++ ) {
		bool first = true;
	    
		double max = 0.0;
		FindMax(dte_Ideal[bs][momtype], &max, Beamsizename(bs));
		for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Infname(inf));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Infname(inf));
		  }
		}
	    
	    
		dte_Ideal[bs][momtype]->SetMaximum(1.1*max);
		for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	    
	    
	    
		for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf(inf));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf(inf));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf(inf));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf(inf));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
		if ( beamstart != kCentralOrbit ) { dte_Ideal[bs][momtype]->Draw("LP"); }
	    
		gPad->SetRightMargin(0.15);
		PlotBasic(beamstart, bs, q, dummy, kick, momtype, field, btype, 0.5);
	    
		TLegend *leg = new TLegend(0.60, 0.50, 0.80, 0.88);
		leg->SetTextFont(42);
		leg->SetTextSize(0.03);
		leg->SetFillColor(0);
	    
		
		// Order for Infs  is Open -> Closed
		vector<int> inf_id;
		for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		  if ( inf == kOpen ) { inf_id.push_back(kOpen); }
		}
		for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		  if ( inf == kPartiallyOpen ) { inf_id.push_back(kPartiallyOpen); }
		}
		for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		  if ( inf == kClosed ) { inf_id.push_back(kClosed); }
		}
		
		if ( beamstart != kCentralOrbit ) { leg->AddEntry(dte_Ideal[bs][momtype], "Ideal x_{e} = R_{m}", "LP"); }
		for ( int infid = 0; infid < (int)inf_id.size(); infid++ ) {
		  int inf = inf_id[infid];
		  stringstream name;
		  name << NiceInfname(inf);
	      
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		}
		leg->Draw("same");
	    
		stringstream c1name;
		c1name << Plotname("AllInflectors", none, beamstart, bs, q, dummy, kick, momtype, field, btype);
		cout << "Trying to save file: " << c1name.str() << endl;
		if ( zoom ) { c1->SetLogx(0); }
		else { c1->SetLogx(1); }
		c1->SaveAs(c1name.str().c_str());
		c1->Clear();   
		cout << "Wrote: " << c1name.str() << endl;
		cout << endl;
		delete leg;
	      }
	    }
	  }
	}
      }


  
      //
      // Plotting for all beamsize states together 
      //
      cout << endl;
      cout << "Plot beamsize and quad states together" << endl;
      cout << endl;
  
      //
      // Plotting for all beamsize states together 
      //
      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	  
	    bool found = false;	   
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
		if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	      }
	      if ( found == false ) { continue; }
	    }
	  
	    for ( int plot = plot_start; plot < plot_end; plot++ ) {
	      bool first = true;
	    
	      double max = 0.0;
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		  }
		}
	      }
	    
	    
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		  }
		}
	      }
	    
	    
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  int beamsize = Beamsize(bs);
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		    dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad_X(q, bs));
		    dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad_X(q, bs));
		    if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad_X(q, bs));
		    dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad_X(q, bs));
		    if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		    else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		  }
		}
	      }
	    
	      gPad->SetRightMargin(0.15);
	      PlotBasic(beamstart, -1, -1, inf, kick, momtype, field, btype, 0.5);
	    
	      TLegend *leg = new TLegend(0.55, 0.55, 0.75, 0.88);
	      leg->SetTextFont(42);
	      leg->SetTextSize(0.03);
	      leg->SetFillColor(0);
	    
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		  stringstream name;
		  int beamsize = Beamsize(bs);
		  name << "#splitline{" << NiceBeamsizename(bs) << "}{" << NiceQuadname(q) << "}";
		
		  if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		  if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		}
	      }
	      leg->Draw("same");
	    
	      stringstream c1name;
	      c1name << Plotname("AllQuadsBeamsizes", "", beamstart, -1, -1, inf, kick, momtype, field, btype);
	      cout << "Trying to save file: " << c1name.str() << endl;
	      if ( zoom ) { c1->SetLogx(0); }
	      else { c1->SetLogx(1); }
	      c1->SaveAs(c1name.str().c_str());
	      c1->Clear();   
	      cout << "Wrote: " << c1name.str() << endl;
	      cout << endl;
	      delete leg;
	    }
	  }
	}
      }


  
    //
    // Plotting for all beamsize states together 
    //
    cout << endl;
    cout << "Plot inflector and quad states together" << endl;
    cout << endl;
  
    //
    // Plotting for all beamsize states together 
    //
    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
	for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	  int beamsize = Beamsize(bs);	   
	  
	  bool found = false;	   
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
	      if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
	      if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	    }
	    if ( found == false ) { continue; }
	  }
	  
	  for ( int plot = plot_start; plot < plot_end; plot++ ) {
	    bool first = true;
	    
	    double max = 0.0;
	    FindMax(dte_Ideal[bs][momtype], &max, Beamsizename(bs));
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		}
	      }
	    }
	    
	    
	    dte_Ideal[bs][momtype]->SetMaximum(1.1*max);
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
	      }
	    }
	    
	    
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, q));
		  dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, q));
		  dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, q));
		  if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, q));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, q));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, q));
		  if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
	      }
	    }
	    if ( beamstart != kCentralOrbit ) { dte_Ideal[bs][momtype]->Draw("LP"); }
	    
	    gPad->SetRightMargin(0.15);
	    PlotBasic(beamstart, bs, -1, -1, kick, momtype, field, btype, 0.5);
	    
	    TLegend *leg = new TLegend(0.55, 0.50, 0.80, 0.88);
	    leg->SetTextFont(42);
	    leg->SetTextSize(0.035);
	    leg->SetFillColor(0);
	    

	    // Order for Quads is NoQuads -> FullQuads
	    // Order for Infs  is Open -> Closed
	    vector<int> q_id, inf_id, kick_id;
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNoQuads ) { q_id.push_back(kNoQuads); }
	    }
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNoQuadSupports ) { q_id.push_back(kNoQuadSupports); }
	    }
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNewSupports ) { q_id.push_back(kNewSupports); }
	    }
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kOpen ) { inf_id.push_back(kOpen); }
	    }
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kPartiallyOpen ) { inf_id.push_back(kPartiallyOpen); }
	    }
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kClosed ) { inf_id.push_back(kClosed); }
	    }
	    
	    if ( beamstart != kCentralOrbit ) { leg->AddEntry(dte_Ideal[bs][momtype], "Ideal x_{e} = R_{m}", "LP"); }
	    for ( int qid = 0; qid < (int)q_id.size(); qid++ ) {
	      for ( int infid = 0; infid < (int)inf_id.size(); infid++ ) {
		int q = q_id[qid];
		int inf = inf_id[infid];
		  
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		stringstream name;
		int beamsize = Beamsize(bs);
		name << "#splitline{" << NiceInfname(inf) << "}{" << NiceQuadname(q) << "}";
		  
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
	      }
	    }
	    leg->Draw("same");

	    stringstream c1name;
	    c1name << Plotname("AllQuadsInflectors", "", beamstart, bs, -1, -1, kick, momtype, field, btype);
	    cout << "Trying to save file: " << c1name.str() << endl;
	    if ( zoom ) { c1->SetLogx(0); }
	    else { c1->SetLogx(1); }
	    c1->SaveAs(c1name.str().c_str());
	    c1->Clear();   
	    cout << "Wrote: " << c1name.str() << endl;
	    cout << endl;
	    delete leg;
	  }
	}
      }
    }

  

  
    //
    // Plotting for all quads and beamsize states together 
    //
    //
    cout << endl;
    cout << "Plot beamsize and quad states together" << endl;
    cout << endl;
    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
     
	  bool found = false;
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    if ( q == kNoQuadSupports ) { continue; }
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      int beamsize = Beamsize(bs);
	      if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
	      if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	    }
	  }
	  if ( found == false ) { continue; }
	
	  for ( int plot = plot_start; plot < plot_end; plot++ ) {
	    bool first = true;
	
	    double max = 0.0;
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNoQuadSupports ) { continue; }
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  FindMax(dte[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
	      }
	    }

	  
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNoQuadSupports ) { continue; }
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
	      }
	    }
	  
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNoQuadSupports ) { continue; }
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetLineWidth(Width_Quad_X(q, bs));
		  dte[bs][inf][q][kick][momtype]->SetLineStyle(Style_Quad_X(q, bs));
		  dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad_X(q, bs));
		  dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad_X(q, bs));
		  if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineWidth(Width_Quad_X(q, bs));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(Style_Quad_X(q, bs));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad_X(q, bs));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad_X(q, bs));
		  if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
	      }
	    }

	
	    gPad->SetRightMargin(0.15);
	    PlotBasic(beamstart, -1, -1, inf, kick, momtype, field, btype, 0.5);
	    
	    TLegend *leg = new TLegend(0.50, 0.45, 0.80, 0.92);
	    leg->SetTextFont(42);
	    leg->SetTextSize(0.028);
	    leg->SetFillColor(0);

	    // Order for Quads is NoQuads -> FullQuads
	    vector<int> q_id;
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNoQuads ) { q_id.push_back(kNoQuads); }
	    }
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNoQuadSupports ) { q_id.push_back(kNoQuadSupports); }
	    }
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( q == kNewSupports ) { q_id.push_back(kNewSupports); }
	    }
	    
	    for ( int qid = 0; qid < (int)q_id.size(); qid++ ) {
	      int q = q_id[qid];
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		if ( q == kNoQuadSupports ) { continue; }
		//if ( q == kNoQuads && inf == kClosed ) { continue; }
		stringstream name;
		int beamsize = Beamsize(bs);
		name << "#splitline{" << NiceBeamsizename(bs) << "}{" << NiceQuadname(q) << "}";
		
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
	      }
	    }
	  }
	  leg->Draw("same");

	  stringstream c1name;
	  c1name << Plotname("AllBeamsizesQuads", "", beamstart, -1, -1, inf, kick, momtype, field, btype);
	  cout << "Trying to save file: " << c1name.str() << endl;
	  if ( zoom ) { c1->SetLogx(0); }
	  else { c1->SetLogx(1); }
	  c1->SaveAs(c1name.str().c_str());
	  c1->Clear();   
	  cout << "Wrote: " << c1name.str() << endl;
	  cout << endl;
	  delete leg;
	}
      }
    }



  

  
    //
    // Plotting for all quads and beamsize states together 
    //
    //
    cout << endl;
    cout << "Plot beamsize and inflector states together" << endl;
    cout << endl;
    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
	for ( int q = q_start; q < q_end; q+=q_inc ) {	   
     
	  bool found = false;
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    if ( inf == kPartiallyOpen ) { continue; }
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      int beamsize = Beamsize(bs);
	      if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
	      if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	    }
	  }
	  if ( found == false ) { continue; }
	
	  for ( int plot = plot_start; plot < plot_end; plot++ ) {
	    bool first = true;
	
	    double max = 0.0;
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kPartiallyOpen ) { continue; }
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  FindMax(dte[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
	      }
	    }

	  
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kPartiallyOpen ) { continue; }
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
	      }
	    }
	  
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kPartiallyOpen ) { continue; }
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetLineWidth(Width_Inf_X(inf, bs));
		  dte[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, bs));
		  dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, bs));
		  dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, bs));
		  if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineWidth(Width_Inf_X(inf, bs));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, bs));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, bs));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, bs));
		  if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
	      }
	    }

	
	    gPad->SetRightMargin(0.15);
	    PlotBasic(beamstart, -1, q, -1, kick, momtype, field, btype, 0.5);
	    
	    TLegend *leg = new TLegend(0.50, 0.45, 0.80, 0.92);
	    leg->SetTextFont(42);
	    leg->SetTextSize(0.028);
	    leg->SetFillColor(0);

	  // Order for Infs  is Open -> Closed
	    vector<int> inf_id;
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kOpen ) { inf_id.push_back(kOpen); }
	    }
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kPartiallyOpen ) { inf_id.push_back(kPartiallyOpen); }
	    }
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      if ( inf == kClosed ) { inf_id.push_back(kClosed); }
	    }
	    
	    for ( int infid = 0; infid < (int)inf_id.size(); infid++ ) {
	      int inf = inf_id[infid];
	      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
		if ( inf == kPartiallyOpen ) { continue; }
		stringstream name;
		int beamsize = Beamsize(bs);
		name << "#splitline{" << NiceBeamsizename(bs) << "}{" << NiceInfname(inf) << "}";
		
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
	      }
	    }
	  }
	  leg->Draw("same");
	  
	  stringstream c1name;
	  c1name << Plotname("AllBeamsizesInflectors", "", beamstart, -1, q, -1, kick, momtype, field, btype);
	  cout << "Trying to save file: " << c1name.str() << endl;
	  if ( zoom ) { c1->SetLogx(0); }
	  else { c1->SetLogx(1); }
	  c1->SaveAs(c1name.str().c_str());
	  c1->Clear();   
	  cout << "Wrote: " << c1name.str() << endl;
	  cout << endl;
	  delete leg;
	}
      }
    }


 

    //
    // Plotting for all beamsize states together 
    //
    cout << endl;
    cout << "Plot inflector, kick, and quad states together" << endl;
    cout << endl;
  
    //
    // Plotting for all beamsize states together 
    //
    for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
      for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	int beamsize = Beamsize(bs);	   
	
	bool found = false;	   
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	      if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
	      //if ( q == kNoQuads && inf == kClosed ) { continue; }
	      if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
	      if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	    }
	    if ( found == false ) { continue; }
	  }
	}
	
	for ( int plot = plot_start; plot < plot_end; plot++ ) {
	  bool first = true;
	  
	  double max = 0.0;
	  FindMax(dte_Ideal[bs][momtype], &max, Beamsizename(bs));
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		//if ( q == kNoQuads && inf == kClosed ) { continue; }
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		}
	      }
	    }
	  }
	    
	    
	  // Override max
	  if ( beamsize == 40 ) {
	    max = 25;
	    if ( momtype == kPerfectmatch ) { max = 32.5; }
	  }

	  dte_Ideal[bs][momtype]->SetMaximum(1.1*max);
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		//if ( q == kNoQuads && inf == kClosed ) { continue; }
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
	      }
	    }
	  }
	      
	    
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		//if ( q == kNoQuads && inf == kClosed ) { continue; }
		  
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetLineWidth(Width_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, q, kick));
		  if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineWidth(Width_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, q, kick));
		  if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
	      }
	    }
	  }
	  if ( beamstart != kCentralOrbit ) { dte_Ideal[bs][momtype]->Draw("LP"); }


	  gPad->SetRightMargin(0.15);
	  PlotBasic(beamstart, bs, -1, -1, -1, momtype, field, btype, 0.5);
	    
	  TLegend *leg = new TLegend(0.50, 0.45, 0.80, 0.92);
	  leg->SetTextFont(42);
	  leg->SetTextSize(0.025);
	  leg->SetFillColor(0);

	  // Order for Quads is NoQuads -> FullQuads
	  // Order for Infs  is Open -> Closed
	  // Order for Kicks is Square -> LCR
	  vector<int> q_id, inf_id, kick_id;
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    if ( q == kNoQuads ) { q_id.push_back(kNoQuads); }
	  }
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    if ( q == kNoQuadSupports ) { q_id.push_back(kNoQuadSupports); }
	  }
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    if ( q == kNewSupports ) { q_id.push_back(kNewSupports); }
	  }
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    if ( inf == kOpen ) { inf_id.push_back(kOpen); }
	  }
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    if ( inf == kPartiallyOpen ) { inf_id.push_back(kPartiallyOpen); }
	  }
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    if ( inf == kClosed ) { inf_id.push_back(kClosed); }
	  }
	  for ( int kick = kick_start; kick < kick_end; kick+=kick_inc ) {
	    if ( kick == kSquareKick ) { kick_id.push_back(kSquareKick); }
	  }
	  for ( int kick = kick_start; kick < kick_end; kick+=kick_inc ) {
	    if ( kick == kLCRKick ) { kick_id.push_back(kLCRKick); }
	  }
	  for ( int kick = kick_start; kick < kick_end; kick+=kick_inc ) {
	    if ( kick == kNoKick ) { kick_id.push_back(kNoKick); }
	  }

	  if ( beamstart != kCentralOrbit ) { leg->AddEntry(dte_Ideal[bs][momtype], "Ideal x_{e} = R_{m}", "LP"); }
	  for ( int qid = 0; qid < (int)q_id.size(); qid++ ) {
	    for ( int infid = 0; infid < (int)inf_id.size(); infid++ ) {
	      for ( int kickid = 0; kickid < (int)kick_id.size(); kickid++ ) {
		int q = q_id[qid];
		int inf = inf_id[infid];
		int kick = kick_id[kickid];
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		//if ( q == kNoQuads && inf == kClosed ) { continue; }
		stringstream name;
		int beamsize = Beamsize(bs);
		name << "#splitline{" << NiceInfname(inf) << "}{" << NiceQuadname(q) << " , " << NiceKickname(kick) << "}";
		  
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
	      }
	    }
	  }
	  leg->Draw("same");

	  stringstream c1name;
	  c1name << Plotname("AllQuadsInflectorsKicks", "", beamstart, bs, -1, -1, -1, momtype, field, btype);
	  cout << "Trying to save file: " << c1name.str() << endl;
	  if ( zoom ) { c1->SetLogx(0); }
	  else { c1->SetLogx(1); }
	  c1->SaveAs(c1name.str().c_str());
	  c1->Clear();   
	  cout << "Wrote: " << c1name.str() << endl;
	  cout << endl;
	  delete leg;
	}
      }
    }
  


    //
    // Plotting for all beamsize states together 
    //
    cout << endl;
    cout << "Plot inflector, kick, momtype, and quad states together" << endl;
    cout << endl;
  
    //
    // Plotting for all beamsize states together 
    //
    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
      int beamsize = Beamsize(bs);	   
      
      bool found = false;	   
      for ( int q = q_start; q < q_end; q+=q_inc ) {
	for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	  for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	    for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
	      if ( q == kNoQuads && inf == kClosed ) { continue; }
	      if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
	      if ( kick == kLCRKick && momtype == kBNLmatch ) { continue; }
	      if ( kick == kSquareKick && momtype == kPerfectmatch ) { continue; }
	      if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
	      if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	    }
	    if ( found == false ) { continue; }
	  }
	}
      }
	
      for ( int plot = plot_start; plot < plot_end; plot++ ) {
	bool first = true;
	
	double max = 0.0;
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		if ( q == kNoQuads && inf == kClosed ) { continue; }
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		if ( kick == kLCRKick && momtype == kBNLmatch ) { continue; }
		if ( kick == kSquareKick && momtype == kPerfectmatch ) { continue; }
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  FindMax(dte[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Beamsizename(bs));
		}
	      }
	    }
	  }
	}
	
	
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		if ( q == kNoQuads && inf == kClosed ) { continue; }
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		if ( kick == kLCRKick && momtype == kBNLmatch ) { continue; }
		if ( kick == kSquareKick && momtype == kPerfectmatch ) { continue; }
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
	      }
	    }
	  }
	}
	      
	
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		if ( q == kNoQuads && inf == kClosed ) { continue; }
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		if ( kick == kLCRKick && momtype == kBNLmatch ) { continue; }
		if ( kick == kSquareKick && momtype == kPerfectmatch ) { continue; }
		
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMarkerSize(MarkerSize_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetMarkerStyle(MarkerStyle_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetLineWidth(Width_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, q, kick));
		  dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, q, kick));
		  if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerSize(MarkerSize_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerStyle(MarkerStyle_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineWidth(Width_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineStyle(Style_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Inf_X(inf, q, kick));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Inf_X(inf, q, kick));
		  if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
	      }
	    }
	  }
	}
	
	gPad->SetRightMargin(0.15);
	PlotBasic(beamstart, bs, -1, -1, -1, momtype, field, btype, 0.5);
	
	TLegend *leg = new TLegend(0.45, 0.50, 0.80, 0.92);
	leg->SetTextFont(42);
	leg->SetTextSize(0.025);
	leg->SetFillColor(0);
	
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
	      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {
		if ( q == kNoQuads && inf == kClosed ) { continue; }
		if ( q == kNoQuadSupports || inf == kPartiallyOpen ) { continue; }
		if ( kick == kLCRKick && momtype == kBNLmatch ) { continue; }
		if ( kick == kSquareKick && momtype == kPerfectmatch ) { continue; }
		stringstream name;
		int beamsize = Beamsize(bs);
		name << "#splitline{" << NiceInfname(inf) << " , " << NiceMomtypename(momtype) << "}{" << NiceQuadname(q) << " , " << NiceKickname(kick) << "}";
		
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) { leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP"); }
	      }
	    }
	  }
	}
	leg->Draw("same");
	
	stringstream c1name;
	c1name << Plotname("AllQuadsInflectorsMomtypesKicks", "", beamstart, bs, -1, -1, -1, -1, field, btype);
	cout << "Trying to save file: " << c1name.str() << endl;
	if ( zoom ) { c1->SetLogx(0); }
	else { c1->SetLogx(1); }
	c1->SaveAs(c1name.str().c_str());
	c1->Clear();   
	cout << "Wrote: " << c1name.str() << endl;
	cout << endl;
	delete leg;
      }
    }

    
    return;
  

  
    //
    // Plotting for all inflector and quad states together 
    //
    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
	for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	  int beamsize = Beamsize(bs);
	  
	  bool found = false;
	  for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	    for ( int q = q_start; q < q_end; q+=q_inc ) {
	      if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
	      if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	    }
	  }
	  if ( found == false ) { continue; }
	  
	  for ( int plot = plot_start; plot < plot_end; plot++ ) {
	    bool first = true;
	    
	    double max = 0.0;
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  FindMax(dte[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
	      }
	    }

	  
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
	      }
	    }
	  
	
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad(q));
		  dte[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad(q));
		  if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_Quad(q));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_Quad(q));
		  if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
	      }
	    }

	    gPad->SetRightMargin(0.15);
	    PlotBasic(beamsize, mom_start, 0.42);
	
	    //TLegend *leg = new TLegend(0.55, 0.45, 0.85, 0.88);
	    TLegend *leg = new TLegend(0.25, 0.20, 0.55, 0.50);
	    leg->SetTextFont(42);
	    leg->SetTextSize(0.025);
	    leg->SetFillColor(0);
	
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      for ( int q = q_start; q < q_end; q+=q_inc ) {
		stringstream name;
		name << NiceInfname(inf) << " - " << NiceQuadname(q);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { 
		  leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP");
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP");
		}
	      }
	    }
	    leg->Draw("same");
		
	    stringstream c1name;
	    c1name << "~/WWW/Inflector/" << Startname(beamstart) << "/AllQuadsInflectors/gr_dP_P";
	    if ( plot == 1 ) { c1name << "_NoLaunch"; }
	    if ( mom_start == 8 ) { c1name << "_IdealmatchBeam"; }
	    if ( mom_start == 20 ) { c1name << "_E821matchBeam"; }
	    c1name << "_" << Beamsizename(bs);
	    c1name << "_" << Beamtypename(btype);
	    c1name << "_AllQuads";
	    c1name << "_AllInflectors";
	    c1name << "_" << Fieldname(field) << ".png";
	    cout << "Trying to save file: " << c1name.str() << endl;
	    c1->SetLogx(1);
	    c1->SetLogy(1);
	    c1->SaveAs(c1name.str().c_str());
	    c1->Clear();   
	    cout << "Wrote: " << c1name.str() << endl;
	    cout << endl;
	    delete leg;
	  }
	}
      }
    }




  
    //
    // Plotting for all quads, inflector, and beamsize states together 
    //
    for ( int kick = kick_start; kick < kick_end; kick += kick_inc ) {
      for ( int momtype = momtype_start; momtype < momtype_end; momtype += momtype_inc ) {	   
    
	bool found = false;
	for ( int q = q_start; q < q_end; q+=q_inc ) {
	  for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	    for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
	      int beamsize = Beamsize(bs);
	      if ( dte[bs][inf][q][kick][momtype] ) { found = true; }
	      if ( dte_NoLaunch[bs][inf][q][kick][momtype] ) { found = true; }
	    }
	  }
	}
	if ( found == false ) { continue; }
    
	for ( int plot = plot_start; plot < plot_end; plot++ ) {
	  bool first = true;
      
	  double max = 0.0;
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  FindMax(dte[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  FindMax(dte_NoLaunch[bs][inf][q][kick][momtype], &max, Quadname(q));
		}
	      }
	    }
	  }

	  
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMaximum(1.1*max);
		}
	      }
	    }
	  }
      
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		int beamsize = Beamsize(bs);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) {
		  dte[bs][inf][q][kick][momtype]->SetMarkerColor(Color_BS_Quad(bs, q));
		  dte[bs][inf][q][kick][momtype]->SetLineColor(Color_BS_Quad(bs, q));
		  if ( first ) { dte[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetMarkerColor(Color_BS_Quad(bs, q));
		  dte_NoLaunch[bs][inf][q][kick][momtype]->SetLineColor(Color_BS_Quad(bs, q));
		  if ( first ) { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("ALP"); first = false;}
		  else { dte_NoLaunch[bs][inf][q][kick][momtype]->Draw("LP"); }
		}
	      }
	    }
	  }
      
      
	  gPad->SetRightMargin(0.15);
	  PlotBasic(beamsize, mom_start, 0.45);
      
	  TLegend *leg = new TLegend(0.25, 0.20, 0.50, 0.50);
	  //TLegend *leg = new TLegend(0.50, 0.50, 0.85, 0.88);
	  leg->SetTextFont(42);
	  leg->SetTextSize(0.02);
	  leg->SetFillColor(0);
      
	  for ( int q = q_start; q < q_end; q+=q_inc ) {
	    for ( int bs = bs_start; bs < bs_end; bs+=bs_inc ) {
	      for ( int inf = inf_start; inf < inf_end; inf+=inf_inc ) {
		int beamsize = Beamsize(bs);
		stringstream name;
		name << NiceInfname(inf) << " - " << NiceBeamsizename(bs) << " - " << NiceQuadname(q);
		if ( plot == 0 && dte[bs][inf][q][kick][momtype] ) { 
		  leg->AddEntry(dte[bs][inf][q][kick][momtype], name.str().c_str(), "LP");
		}
		if ( plot == 1 && dte_NoLaunch[bs][inf][q][kick][momtype] ) {
		  leg->AddEntry(dte_NoLaunch[bs][inf][q][kick][momtype], name.str().c_str(), "LP");
		}
	      }
	    }
	  }
	  leg->Draw("same");
      
	  stringstream c1name;
	  c1name << "~/WWW/Inflector/" << Startname(beamstart) << "/AllBeamsizesQuadsInflectors/gr_dP_P";
	  if ( plot == 1 ) { c1name << "_NoLaunch"; }
	  if ( mom_start == 8 ) { c1name << "_IdealmatchBeam"; }
	  if ( mom_start == 20 ) { c1name << "_E821matchBeam"; }
	  c1name << "_AllBeamsizes";
	  c1name << "_" << Beamtypename(btype);
	  c1name << "_AllQuads";
	  c1name << "_AllInflectors";
	  c1name << "_" << Fieldname(field) << ".png";
	  cout << "Trying to save file: " << c1name.str() << endl;
	  c1->SetLogx(1);
	  c1->SetGrid(0, 0);
	  c1->SaveAs(c1name.str().c_str());
	  c1->Clear();   
	  cout << "Wrote: " << c1name.str() << endl;
	  cout << endl;
	  delete leg;
	}
      }
    }


  } // beamstart

  

}



void FindMax(TGraph *gr, double *max, string name)
{
  bool debug = false;

  double tmp_max = GetMax(gr);

  if ( debug ) { cout << "Max of " << name << " : " << tmp_max << "\t" << *max << endl; }
  if ( tmp_max > *max ) { *max = tmp_max; }
}

void FindMax(TGraph2D *gr, double *max, string name)
{
  bool debug = true;

  int n = gr->GetN();
  double *x = gr->GetX();
  double *y = gr->GetY();
  double *z = gr->GetZ();

  double mx = 0.0;
  for ( int i = 0; i < n; i++ ) {
    if ( z[i] > mx ) { mx = z[i]; }
  }
  *max = mx;

  double minx = 99.9;
  double maxx = -99.9;
  double miny = 99.9;
  double maxy = -99.9;
  double minz = 99.9;
  double maxz = -99.9;
  for ( int i = 0; i < n; i++ ) {
    if ( x[i] < minx ) { minx = x[i]; }
    if ( x[i] > maxx ) { maxx = x[i]; }
    if ( y[i] < miny ) { miny = y[i]; }
    if ( y[i] > maxy ) { maxy = y[i]; }
    if ( z[i] < minz ) { minz = z[i]; }
    if ( z[i] > maxz ) { maxz = z[i]; }
  }

  if ( debug ) { 
    cout << "Max of " << name << " : " << "\t" << *max << endl;
    cout << "  [x] = [" << minx << " , " << maxx << "]" << endl;
    cout << "  [y] = [" << miny << " , " << maxy << "]" << endl;
    cout << "  [z] = [" << minz << " , " << maxz << "]" << endl;
  }
}

double GetMax(TGraph *gr)
{
  int n = gr->GetN();
  double *y = gr->GetY();

  double mx = 0.0;
  for ( int i = 0; i < n; i++ ) {
    if ( y[i] > mx ) { mx = y[i]; }
  }

  return( mx );
}

int Color(int bs)
{
  if ( bs == 0 ) { return( kAzure+6 ); }
  if ( bs == 1 ) { return( kMagenta ); }
  if ( bs == 2 ) { return( kOrange+6 ); }
  if ( bs == 3 ) { return( 30 ); }
  if ( bs == 4 ) { return( 28 ); }
  if ( bs == 5 ) { return( 38 ); }
  if ( bs == 6 ) { return( kSpring ); }
  if ( bs == 7 ) { return( kRed ); }

  return( 1 );
}

int Color_Quad(int quad)
{
  if ( quad == kNewSupports ) { return( kAzure+6 ); }
  if ( quad == kNoQuads ) { return( kMagenta ); }
  if ( quad == kNoQuadSupports ) { return( kOrange+6 ); }

  return( 1 );
}

int Color_Inf(int inf)
{
  if ( inf == kOpen ) { return( kAzure+6 ); }
  if ( inf == kPartiallyOpen ) { return( kMagenta ); }
  if ( inf == kClosed ) { return( kOrange+6 ); }

  return( 1 );
}

int Color_Momtype(int momtype)
{
  if ( momtype == kBNLmatch ) { return( kAzure+6 ); }
  if ( momtype == kPerfectmatch ) { return( kOrange+6 ); }

  return( 1 );
}

int Color_Kick(int momtype)
{
  if ( momtype == kBNLmatch ) { return( kAzure+6 ); }
  if ( momtype == kPerfectmatch ) { return( kOrange+6 ); }

  return( 1 );
}

int Color_Beamsize(int bs)
{
  if ( bs == kBeamsize0pi ) { return( kAzure+6 ); }
  if ( bs == kBeamsize40pi ) { return( kOrange+6 ); }
  if ( bs == kBeamsize1pi ) { return( kMagenta ); }
  if ( bs == kBeamsize20pi ) { return( kSpring ); }
  if ( bs == kBeamsize42pi ) { return( 38 ); }

  return( 1 );
}


int Color_Momtype_X(int momtype, int x)
{
  int color = Color_Momtype(momtype);
  color += x;

  return( color );
}

int Color_Kick_X(int momtype, int x)
{
  int color = Color_Kick(momtype);
  color += x;

  return( color );
}

int Color_Quad_X(int quad, int x)
{
  int color = Color_Quad(quad);
  color -= x;

  return( color );
}

int Color_Inf_X(int inf, int x)
{
  int color = Color_Inf(inf);
  color -= x;

  return( color );
}

int Color_Inf_X(int inf, int x, int y)
{
  int color = Color_Inf(inf);
  color -= x;

  return( color );
}

int Color_Beamsize_X(int bs, int x)
{
  int color = Color_Beamsize(bs);
  color += x;

  return( color );
}
	      
int Color_Ideal()
{
  int color = kSpring;

  return( color );
}
	      
int Style_Ideal()
{
  int style = 1;

  return( style );
}

int Style_Quad_X(int q, int x)
{
  int style = 1;
  if ( x == kLCRKick ) { style = 2; }
  if ( x == kSquareKick ) { style = 1; }
  if ( x == kBeamsize40pi ) { style = 1; }
  if ( x == kBeamsize0pi ) { style = 2; }
  if ( x == kBeamsize42pi ) { style = 3; }
  //style = 1;
  
  return( style );
}

int Style_Inf_X(int inf, int x)
{
  int style = 1;
  if ( x == kLCRKick ) { style = 2; }
  if ( x == kSquareKick ) { style = 1; }
  if ( x == kBeamsize40pi ) { style = 1; }
  if ( x == kBeamsize0pi ) { style = 2; }
  if ( x == kBeamsize42pi ) { style = 3; }
  //style = 1;
  
  return( style );
}

int Style_Inf_X(int inf, int x, int y)
{
  int style = 1;
  if ( y == kLCRKick ) { style = 2; }
  if ( y == kSquareKick ) { style = 1; }
  if ( y == kBeamsize40pi ) { style = 1; }
  if ( y == kBeamsize0pi ) { style = 2; }
  if ( y == kBeamsize42pi ) { style = 3; }
  //style = 1;
  
  return( style );
}

int MarkerStyle_Inf_X(int inf, int x)
{
  int style = 21;
  if ( x > 0 ) { style = 8; }
  
  return( style );
}

int MarkerStyle_Inf_X(int inf, int x, int y)
{
  int style = 21;
  if ( y > 0 ) { style = 8; }
  
  return( style );
}

int MarkerSize_Inf_X(int inf, int x)
{
  int size = 1.90;
  if ( x > 0 ) { size = 1.90; }
  
  return( size );
}

int MarkerSize_Inf_X(int inf, int x, int y)
{
  int size = 1.90;
  if ( y > 0 ) { size = 1.90; }
  
  return( size );
}


int Width_Inf_X(int inf, int x)
{
  int style = 3 - x;
  int width = 3;
  
  return( width );
}

int Width_Inf_X(int inf, int x, int y)
{
  int style = 3 - y;
  int width = 3;
  
  return( width );
}


int Width_Quad_X(int inf, int x)
{
  int style = 3 - x;
  int width = 3;
  
  return( width );
}

int Width_Quad_X(int inf, int x, int y)
{
  int style = 3 - y;
  int width = 3;
  
  return( width );
}
