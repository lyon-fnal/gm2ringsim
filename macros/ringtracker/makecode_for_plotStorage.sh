#!/bin/sh

beamtypes="Uniform Gaussian"
#beamtypes="Uniform"
fields="Mapped Vanish"
fields="Vanish Mapped"
beamsizes="1 20 40 0"
beamsizes="0 40 42"
sigmats="25"
plottypes="0 1 2"
infs="Closed PartiallyOpen Open"
scrapings="OFF"
#quads="NewSupports NoQuads NoQuadSupports FullHighMom FullHigherMom FullHighestMom"
quads="NewSupports NoQuadSupports NoQuads FullHighMom FullHigherMom FullHighestMom"
quads="NoQuads NoQuadSupports NewSupports"
#quads="NoQuads NewSupports"
#beamstarts="DownstreamMandrel UpstreamMandrel UpstreamCryo CentralOrbit CentralOrbit_Offset77"
beamstarts="CentralOrbit CentralOrbit_Offset77 DownstreamMandrel UpstreamCryo" # UpstreamCryo CentralOrbit_Offset77"
beamstarts="DownstreamMandrel UpstreamCryo"
kicks="LCR Square No"
kicks="Square LCR No"
momtypes="BNLmatch Perfectmatch"


for beamstart in ${beamstarts}; do

    shortbeamstart=""
    if [ ${beamstart} == "DownstreamMandrel" ]; then
	shortbeamstart="DM"
    elif [ ${beamstart} == "UpstreamMandrel" ]; then
	shortbeamstart="UM"
    elif [ ${beamstart} == "UpstreamCryo" ]; then
	shortbeamstart="UC"
    elif [ ${beamstart} == "CentralOrbit" ]; then
	shortbeamstart="CO"
    elif [ ${beamstart} == "CentralOrbit_Offset77" ]; then
	shortbeamstart="CO77"
    fi

    for field in ${fields}; do
	for beamsize in ${beamsizes}; do
	    for quad in ${quads}; do
		if [ -a Includes/FillEff_${shortbeamstart}_DL_${field}_${quad}_${beamsize}pi.C ]; then
		    echo "#include \"Includes/FillEff_${shortbeamstart}_DL_${field}_${quad}_${beamsize}pi.C\""
		fi
		if [ -a Includes/FillEff_${shortbeamstart}_DL_Inf_${field}_${quad}_${beamsize}pi.C ]; then
		    echo "#include \"Includes/FillEff_${shortbeamstart}_DL_Inf_${field}_${quad}_${beamsize}pi.C\""
		fi
		if [ -a Includes/FillEff_${shortbeamstart}_DL_InfLength_${field}_${quad}_${beamsize}pi.C ]; then
		    echo "#include \"Includes/FillEff_${shortbeamstart}_DL_InfLength_${field}_${quad}_${beamsize}pi.C\""
		fi
		if [ -a Includes/FillEff_${shortbeamstart}_DL_RingAcc_${field}_${quad}_${beamsize}pi.C ]; then
		    echo "#include \"Includes/FillEff_${shortbeamstart}_DL_RingAcc_${field}_${quad}_${beamsize}pi.C\""
		fi
		if [ -a Includes/FillEff_${shortbeamstart}_DL_InfAcc_${field}_${quad}_${beamsize}pi.C ]; then
		    echo "#include \"Includes/FillEff_${shortbeamstart}_DL_InfAcc_${field}_${quad}_${beamsize}pi.C\""
		fi
#		if [ -a Includes/FillReqKick_${shortbeamstart}_${field}_${quad}_${beamsize}pi.C ]; then
#		    echo "#include \"Includes/FillReqKick_${shortbeamstart}_${field}_${quad}_${beamsize}pi.C\""
#		fi
	    done
	done
    done
done

echo ""

plottypes="KL DL DL_Inf DL_InfLength DL_RingAcc DL_InfAcc"

for plottype in ${plottypes}; do
    echo ""
    if [ ${plottype} == "KL" ]; then
	echo "void GetEff_${plottype}(double *eff, double *err, int *launch, int *kick, int scraping, int beamstart, int beamtype, int sigmap, int sigmat, int field, int inf, int bs, int q, int momtype, int N2d)"
    fi
    if [ ${plottype} == "DL" ] || [ ${plottype} == "DL_Inf" ] || [ ${plottype} == "DL_InfLength" ] || [ ${plottype} == "DL_RingAcc" ] || [ ${plottype} == "DL_InfAcc" ]; then
	echo "void GetEff_${plottype}(double *eff, double *err, int *launch, int *delta, int kick, int scraping, int beamstart, int beamtype, int sigmap, int sigmat, int field, int inf, int bs, int q, int momtype, int N2d)"
    fi
    if [ ${plottype} == "ReqKick" ]; then
	echo "void Get${plottype}(double *kick, double *kickerr, int scraping, int beamstart, int beamtype, int sigmap, int sigmat, int field, int inf, int bs, int q, int momtype, int N2d)"
    fi
    
    echo "{"
    echo "  bool debug = false;"
    echo "  if ( debug ) { cout << \"\\tScraping = \" << scraping << \"\\tBeamStart = \" <<beamstart << \"\\tSigmat = \" << sigmat << \"\\tSigmaP = \" << sigmap << \"\\tField = \" << field << \"\\tInflector = \" << inf << \"\\tBeamsize = \" << bs << \"\\tQuad = \" << q << \"\\tMomtype = \" << momtype << endl; }"



  echo "  int kON = 0;"
  echo "  int kOFF = 1;"
  echo "  int kGaussian = 0;"
  echo "  int kUniform = 1;"
  echo "  int kDownstreamMandrel = 0;"
  echo "  int kUpstreamMandrel = 1;"
  echo "  int kUpstreamCryo = 2;"
  echo "  int kCentralOrbit_Offset77 = 3;"
  echo "  int kCentralOrbit = 4;"
  echo "  int kVanish = 0;"
  echo "  int kMapped = 1;"
  echo "  int kOpen = 0;"
  echo "  int kPartiallyOpen = 1;"
  echo "  int kClosed = 2;"
  echo "  int kBeamsize0pi = 0;"
  echo "  int kBeamsize1pi = 1;"
  echo "  int kBeamsize20pi = 2;"
  echo "  int kBeamsize40pi = 3;"
  echo "  int kBeamsize42pi = 4;"
  echo "  int kScrapingON = 0;"
  echo "  int kScrapingOFF = 1;"
  echo "  int kNewSupports = 0;"
  echo "  int kNoQuads = 1;"
  echo "  int kNoQuadSupports = 2;"
  echo "  int kFullHighMom = 3;"
  echo "  int kFullHigherMom = 4;"
  echo "  int kFullHighestMom = 5;"
  echo "  int kNoKick = 0;"
  echo "  int kLCRKick = 90;"
  echo "  int kSquareKick = 220;"
  echo "  int kBNLmatch = 0;"
  echo "  int kPerfectmatch = 1;"

for scraping in ${scrapings}; do
    echo "  if ( scraping == k${scraping} ) {"
    for beamstart in ${beamstarts}; do
	
	shortbeamstart=""
	if [ ${beamstart} == "DownstreamMandrel" ]; then
	    shortbeamstart="DM"
	elif [ ${beamstart} == "UpstreamMandrel" ]; then
	    shortbeamstart="UM"
	elif [ ${beamstart} == "UpstreamCryo" ]; then
	    shortbeamstart="UC"
	elif [ ${beamstart} == "CentralOrbit" ]; then
	    shortbeamstart="CO"
	elif [ ${beamstart} == "CentralOrbit_Offset77" ]; then
	    shortbeamstart="CO77"
	fi
	echo "    if ( beamstart == k${beamstart} ) {"
	for sigmat in ${sigmats}; do
	    echo "      if ( sigmat == ${sigmat} ) {"
	    for field in ${fields}; do
		echo "        if ( field == k${field} ) {"
		for inf in ${infs}; do
		    echo "          if ( inf == k${inf} ) {"
		    for beamtype in ${beamtypes}; do
			echo "            if ( beamtype == k${beamtype} ) {"		    
			for beamsize in ${beamsizes}; do
			    echo "              if ( bs == kBeamsize${beamsize}pi ) {"
			    for quad in ${quads}; do
				echo "                if ( q == k${quad} ) {;"
				for momtype in ${momtypes}; do
				    echo "                if ( momtype == k${momtype} ) {;"
				
				    if [ -a Includes/FillReqKick_${shortbeamstart}_${field}_${quad}_${beamsize}pi.C ]; then
					if [ ${plottype} == "ReqKick" ]; then
					    echo "                  GetReqKick_${shortbeamstart}_sigmat${sigmat}_${beamtype}Beam_${momtype}_Beamsize${beamsize}pi_Scraping${scraping}_${quad}_${field}_${inf}(kick, err, beamstart, inf, field, sigmap, sigmat, ${beamsize}, q, beamtype, N2d);"
					fi
				    fi
				    
				    for kick in ${kicks}; do
					kickname="${kick}Kick"
					echo "                  if ( kick == k${kickname} ) {;"
					
					
					if [ -a Includes/FillEff_${shortbeamstart}_${plottype}_${field}_${quad}_${beamsize}pi.C ]; then
					    if [ ${plottype} == "KL" ]; then
						echo "                    GetEffErr_KickLaunch_${shortbeamstart}_sigmat${sigmat}_${kickname}_${beamtype}Beam_${momtype}_Beamsize${beamsize}pi_Scraping${scraping}_${quad}_${field}_${inf}(eff, err, launch, kick, beamstart, inf, field, prhat, sigmap, sigmat, ${beamsize}, q, beamtype, N2d);"
					    fi
					    if [ ${plottype} == "DL" ]; then
						echo "                    GetEffErr_DeltaLaunch_${shortbeamstart}_sigmat${sigmat}_${kickname}_${beamtype}Beam_${momtype}_Beamsize${beamsize}pi_Scraping${scraping}_${quad}_${field}_${inf}(eff, err, launch, delta, kick, beamstart, inf, field, prhat, sigmap, sigmat, ${beamsize}, q, beamtype, N2d);"
					    fi
					    if [ ${plottype} == "DL_Inf" ]; then
						echo "                    GetEffErr_Inflector_DeltaLaunch_${shortbeamstart}_sigmat${sigmat}_${kickname}_${beamtype}Beam_${momtype}_Beamsize${beamsize}pi_Scraping${scraping}_${quad}_${field}_${inf}(eff, err, launch, delta, kick, beamstart, inf, field, prhat, sigmap, sigmat, ${beamsize}, q, beamtype, N2d);"
					    fi
					    if [ ${plottype} == "DL_InfLength" ]; then
						echo "                    GetEffErr_InflectorInternal_DeltaLaunch_${shortbeamstart}_sigmat${sigmat}_${kickname}_${beamtype}Beam_${momtype}_Beamsize${beamsize}pi_Scraping${scraping}_${quad}_${field}_${inf}(eff, err, launch, delta, kick, beamstart, inf, field, prhat, sigmap, sigmat, ${beamsize}, q, beamtype, N2d);"
					    fi
					    if [ ${plottype} == "DL_RingAcc" ]; then
						echo "                    GetEffErr_RingAcceptance_DeltaLaunch_${shortbeamstart}_sigmat${sigmat}_${kickname}_${beamtype}Beam_${momtype}_Beamsize${beamsize}pi_Scraping${scraping}_${quad}_${field}_${inf}(eff, err, launch, delta, kick, beamstart, inf, field, prhat, sigmap, sigmat, ${beamsize}, q, beamtype, N2d);"
					    fi
					    if [ ${plottype} == "DL_InfAcc" ]; then
						echo "                    GetEffErr_InflectorAcceptance_DeltaLaunch_${shortbeamstart}_sigmat${sigmat}_${kickname}_${beamtype}Beam_${momtype}_Beamsize${beamsize}pi_Scraping${scraping}_${quad}_${field}_${inf}(eff, err, launch, delta, kick, beamstart, inf, field, prhat, sigmap, sigmat, ${beamsize}, q, beamtype, N2d);"
					    fi
					fi
					echo "                    }"
				    done
				    echo "                  }"
				done
				echo "                }"
			    done
			    echo "              }"
			done
			echo "            }"
		    done
		    echo "          }"
		done
		echo "        }"
	    done
	    echo "      }"
	done
	echo "    }"
    done
    echo "  }"
done

echo "}"
done