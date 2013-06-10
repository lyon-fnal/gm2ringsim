#!/bin/sh

decompose()
{
    fname=${1}
}

checktest()
{
    if [ ${test} == 1 ]; then
	launches="10"
	offsets="0"
	kicks="95"	
	export sigmat=50
	export evts=10000
	export numturns=500
    fi
}

checkgun()
{
    if [ ${infgun} == 1 ]; then
	sigmat="25"
	export sigmat=${sigmat}
    fi
}

bestkicks()
{
    if [ ${sub} == 1 ]; then
	thebestkicks="${kicks}"
	return;
    fi
    
    thebestkicks="95"
    if [ ${kicks} -gt 0 ]; then
	thebestkicks="${kicks}"
	return;
    fi
    
    if [ ${infstart} == 1 ]; then
	thebestkicks="225"
	return;
    else
	if [ ${offset} == 0 ]; then
	    thebestkicks="0 5"
	elif [ ${offset} == 10 ]; then
	    thebestkicks="0 10 20"
	    thebestkicks="5 15"
	elif [ ${offset} == 20 ]; then
	    thebestkicks="10 15 20 25 30"	    
	elif [ ${offset} == 30 ]; then
	    thebestkicks="20 25 30 35 40"
	elif [ ${offset} == 40 ]; then
	    thebestkicks="30 35 40 45 50"
	elif [ ${offset} == 50 ]; then
	    thebestkicks="40 45 50 55 60"
	elif [ ${offset} == 60 ]; then
	    thebestkicks="50 55 60 65 70 75 80"
	elif [ ${offset} == 70 ]; then
	    thebestkicks="60 65 70 75 80 85 90"
	elif [ ${offset} == 75 ]; then
	    thebestkicks="70 75 80 85 90 95 100"
	elif [ ${offset} == 77 ]; then
	    thebestkicks="75 80 85 90 95 100"
	elif [ ${offset} == 80 ]; then
	    thebestkicks="80 85 90 95 100"
	fi
    fi
    
    if [ ${test} == 1 ]; then
	thebestkicks="95"
    fi
}

bestoffsets()
{
    if [ ${sub} == 1 ]; then
	thebestoffsets="${offsets}"
	return;
    fi
    
    if [ ${offsets} -gt 0 ]; then
	thebestoffsets="${offsets}"
	return;
    fi
    
    if [ ${infgun} == 1 ]; then
	thebestoffsets="-1"
	return;
    fi

    if [ ${infgun} == 1 ]; then
	thebestoffsets="-1"
	return;
    fi

    thebestoffsets="-1"
    if [ ${infstart} == 1 ]; then
	thebestoffsets="-1"
	return;
    else
	thebestoffsets="0 10 20 30 40 50 60 70 75 80"
	thebestoffsets="0 10 20 30 40 50 60 70 75 80"
#	thebestoffsets="77"
    fi

    if [ ${test} == 1 ]; then
	thebestoffsets="-1"
    fi
}



bestdeltas()
{
    if [ ${sub} == 1 ]; then
	thebestdeltas="${deltas}"
	return;
    fi
    
    if ! [ -z ${deltas} ]; then
	thebestdeltas="${deltas}"
	return;
    fi
    
    if [ ${infgun} == 1 ]; then
	thebestdeltas="0"
	return;
    fi
    
    if [ ${infgun} == 1 ]; then
	thebestdeltas="0"
	return;
    fi
    
    thebestdeltas=""
    if [ ${infstart} == 1 ]; then
	if [ ${field} == 1 ]; then
	    thebestdeltas="-10 -10 -8 -7 -6 -5 -2 0 2 5"
	    if [ ${beam} == "UpstreamCryo" ] || [ ${beam} == "UpstreamMandrel" ]; then
		thebestdeltas="-8 -7 -6 -5 -4 -3 -2 0 2"
		thebestdeltas="-4 -3"
	    fi
	else
	    thebestdeltas="-15 -12 -10 -7 -5 -2 0 2 5"
	fi
    else
	thebestdeltas="0 3 6"
	thebestdeltas="0"
    fi
    
    if [ ${test} == 1 ]; then
	thebestdeltas="-15 -12 -10 -8 -7 -6 -5 -2 0 2 5"
	thebestdeltas="-15 -12 -10 -7 -5 -2 0 2 5"
	thebestdeltas="0"
    fi   
}

bestlaunches()
{
    if [ ${sub} == 1 ]; then
	thebestlaunches="${launches}"
	return;
    fi

    if ! [ -z ${launches} ]; then
	thebestlaunches="${launches}"
	return;
    fi

	

    if [ ${infgun} == 1 ]; then
	thebestlaunches="0"
	return;
    fi
    
    if [ ${test} == 1 ]; then
	thebestlaunches="-22 -20 -19 -18 -17 -15 -14 -13 -12 -11 -10 -8 -7 -6 -5 -4 -3 -2 0 2 5"
	thebestlaunches="0"
    fi
    
}

name()
{
    extra="Scraping${scraping}${quadname}${numturns}Turns"    
 
    if [ ${particle} == "e+" ]; then
	particlename="eplus"
    elif [ ${particle} == "e-" ]; then
	particlename="eminus"
    elif [ ${particle} == "mu+" ]; then
	particlename="muplus"
    elif [ ${particle} == "mu-" ]; then
	particlename="muminus"
    elif [ ${particle} == "pi+" ]; then
	particlename="piplus"
    elif [ ${particle} == "pi-" ]; then
	particlename="piminus"
    else
	particlename="${particle}"
    fi
    extra="${extra}_${particlename}"

    if [ -z ${spintracking} ]; then
	extra="${extra}_NoSpinTracking"
    else
	if [ ${spintracking} == "spin" ]; then
	    extra="${extra}_SpinTracking"
	elif [ ${spintracking} == "edm" ]; then
	    extra="${extra}_EDMTracking_Eta_${edmsize}"
	fi
    fi

    if [ ${beamstart} == "uc" ] || [ ${beamstart} == "UpstreamCryo" ]; then
	extra="${extra}_UpstreamCryo"
    elif [ ${beamstart} == "dc" ] || [ ${beamstart} == "DownstreamCryo" ]; then
	extra="${extra}_DownstreamCryo"
    elif [ ${beamstart} == "um" ] || [ ${beamstart} == "UpstreamMandrel" ]; then
	extra="${extra}_UpstreamMandrel"
    elif [ ${beamstart} == "dm" ] || [ ${beamstart} == "DownstreamMandrel" ]; then
	extra="${extra}_DownstreamMandrel"
    elif [ ${beamstart} == "co" ] || [ ${beamstart} == "CentralOrbit" ]; then
	extra="${extra}_CentralOrbit"
	if [ ${offset} -gt 0 ]; then
	    extra="${extra}_Offset${offset}"
	fi
    else
	extra="${extra}_PerfectStorage"
	extra="${extra}_Offset${offset}mm"
	
	if [ ${delta} -gt 0 ]; then
	    extra="${extra}_Deltap${delta}deg"
	elif [ ${delta} -lt 0 ]; then
	    loc_delta=`echo " ${delta} * -1" | bc`
	    extra="${extra}_Deltam${loc_delta}deg"
	else
	    extra="${extra}_Delta0deg"
	fi
	
	if [ ${launch} -gt 0 ]; then
	    extra="${extra}_Launchp${launch}mrad"
	elif [ ${launch} -lt 0 ]; then
	    loc_launch=`echo " ${launch} * -1" | bc`
	    extra="${extra}_Launchm${loc_launch}mrad"
	else
	    extra="${extra}_NoLaunch"
	fi
	
	infstart=0
    fi
    
    if [ ${sigmat} -ge 1 ]; then
	extra="${extra}_tSigma${sigmat}"
    fi
    
    if [ ${kickhv} -gt 0 ]; then
	extra="${extra}_${kickhv}kVLCRKick"
    elif [ ${kicksk} -gt 0 ]; then
	extra="${extra}_${kicksk}GSquareKick"
    else
	extra="${extra}_NoKick"
    fi
    
    if [ ${infstart} == 1 ]; then
	if [ ${inftype} == 1 ]; then
	    extra="${extra}_DownstreamInflectorOpen"
	elif [ ${inftype} == 2 ]; then
	    extra="${extra}_InflectorOpen"
	elif [ ${inftype} == 3 ]; then
	    extra="${extra}_PartiallyOpen"
	else
	    extra="${extra}_ClosedInflector"
	fi
	
	if [ ${infgun} == 0 ]; then
	    if [ ${delta} -gt 0 ]; then
		extra="${extra}_Deltap${delta}mrad"
	    elif [ ${delta} -lt 0 ]; then
		loc_delta=`echo " ${delta} * -1" | bc`
		extra="${extra}_Deltam${loc_delta}mrad"
	    else
		extra="${extra}_NoDelta"
	    fi
	    
	    if [ ${launch} -gt 0 ]; then
		extra="${extra}_Launchp${launch}mrad"
	    elif [ ${launch} -lt 0 ]; then
		loc_launch=`echo " ${launch} * -1" | bc`
		extra="${extra}_Launchm${loc_launch}mrad"
	    else
		extra="${extra}_NoLaunch"
	    fi
	else
	    
	    if [ ${delta} -gt 0 ]; then
		extra="${extra}_Deltap${delta}mrad"
	    elif [ ${delta} -lt 0 ]; then
		loc_delta=`echo " ${delta} * -1" | bc`
		extra="${extra}_Deltam${loc_delta}mrad"
	    else
		extra="${extra}_NoDelta"
	    fi
	    
	    if [ ${launch} -gt 0 ]; then
		extra="${extra}_Launchp${launch}mrad"
	    elif [ ${launch} -lt 0 ]; then
		loc_launch=`echo " ${launch} * -1" | bc`
		extra="${extra}_Launchm${loc_launch}mrad"
	    else
		extra="${extra}_NoLaunch"
	    fi
	fi
	
	
	if [ ${field} == 1 ]; then
	    extra="${extra}_MappedField"
	fi
    else
	if [ ${inftype} == 1 ]; then
	    extra="${extra}_DownstreamInflectorOpen"
	elif [ ${inftype} == 2 ]; then
	    extra="${extra}_InflectorOpen"
	elif [ ${inftype} == 3 ]; then
	    extra="${extra}_PartiallyOpen"
	else
	    extra="${extra}_ClosedInflector"
	fi
    fi
    
    if [ ${infstart} == 0 ]; then
	if [ ${beamsize} -gt 0 ]; then
#	extra="${extra}_BeamSize${beamsize}mm"
	    extra="${extra}_BeamSize${beamsize}pimmmrad"
	elif [ ${beamsize} == 0 ]; then
	    extra="${extra}_PencilBeam"
	else
	    extra="${extra}_TrapazoidBeam"
	fi
    else
	if [ ${infgun} == 1 ]; then
	    extra="${extra}_BeamSize${beamsize}pimmmrad"
	else
	    if [ ${beamsize} -gt 0 ]; then
		extra="${extra}_Beam${beamsize}mm${beamsize}mm"
	    elif [ ${beamsize} == 0 ]; then
		extra="${extra}_PencilBeam"
	    else
		extra="${extra}_TrapazoidBeam"
	    fi
	fi
    fi

    if [ ${setbetax} == 1 ]; then
	extra="${extra}_ScaledBetaX"
    fi
    
    
    if [ ${infgun} == 1 ]; then
	if [ ${gengauss} == 1 ]; then
	    extra="${extra}_InflectorGun"
	else
	    extra="${extra}_UniformInflectorGun"
	fi

	if [ ${carol} == 1 ]; then
	    extra="${extra}_CarolMatch"
	fi
	if [ ${match} == 1 ]; then
	    extra="${extra}_PerfectMatch"
	fi
	if [ ${match_dp2} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP2"
	fi
	if [ ${match_dp1} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP1"
	fi
	if [ ${match_dp001} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP001"
	fi
	if [ ${match_dp0001} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP0001"
	fi
	if [ ${match_dp005} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP005"
	fi
	if [ ${match_dp0075} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP0075"
	fi
	if [ ${match_dp0025} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP0025"
	fi
	if [ ${match_dp025} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP025"
	fi
	if [ ${match_dp075} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP075"
	fi
	if [ ${match_dp05} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP05"
	fi
	if [ ${match_dp5} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP5"
	fi
	if [ ${match_dp10} == 1 ]; then
	    extra="${extra}_PerfectMatch_dP10"
	fi
	if [ ${bnlmatch} == 1 ]; then
	    extra="${extra}_E821Match"
	fi
	if [ ${bnlmatch_dp2} == 1 ]; then
	    extra="${extra}_E821Match_dP2"
	fi
	if [ ${bnlmatch_dp1} == 1 ]; then
	    extra="${extra}_E821Match_dP1"
	fi
	if [ ${bnlmatch_dp001} == 1 ]; then
	    extra="${extra}_E821Match_dP001"
	fi
	if [ ${bnlmatch_dp0001} == 1 ]; then
	    extra="${extra}_E821Match_dP0001"
	fi
	if [ ${bnlmatch_dp005} == 1 ]; then
	    extra="${extra}_E821Match_dP005"
	fi
	if [ ${bnlmatch_dp0075} == 1 ]; then
	    extra="${extra}_E821Match_dP0075"
	fi
	if [ ${bnlmatch_dp0025} == 1 ]; then
	    extra="${extra}_E821Match_dP0025"
	fi
	if [ ${bnlmatch_dp025} == 1 ]; then
	    extra="${extra}_E821Match_dP025"
	fi
	if [ ${bnlmatch_dp075} == 1 ]; then
	    extra="${extra}_E821Match_dP075"
	fi
	if [ ${bnlmatch_dp05} == 1 ]; then
	    extra="${extra}_E821Match_dP05"
	fi
	if [ ${bnlmatch_dp5} == 1 ]; then
	    extra="${extra}_E821Match_dP5"
	fi
	if [ ${bnlmatch_dp10} == 1 ]; then
	    extra="${extra}_E821Match_dP10"
	fi
    fi
    
    if [ ${collimator_status} == 0 ]; then
	extra="${extra}_CollimatorOFF"
    elif [ ${collimator_status} == 1 ]; then
	extra="${extra}_CollimatorON"
    else
	extra="${extra}"
    fi
    
    if [ ${muondecay} == "standard" ]; then
	extra="${extra}_StandardMuonDecay"
    elif [ ${muondecay} == "sm" ]; then
	extra="${extra}_StandardModelMuonDecay"
    elif [ ${muondecay} == "iso" ]; then
	extra="${extra}_IsotropicMuonDecay"
    elif [ ${muondecay} == "none" ]; then
	extra="${extra}_NoMuonDecay"
    fi
}

script="submit.sh"
isSOURCE=0

carol=0
match=0

infstart=1
infgun=1
gengauss=0
carol=0
match=0
bnlmatch=0
match_dp0025=0
match_dp0075=0
match_dp025=0
match_dp075=0
match_dp10=0
match_dp5=0
match_dp2=0
match_dp1=0
match_dp001=0
match_dp0001=0
match_dp05=0
match_dp005=0

bnlmatch_dp0025=0
bnlmatch_dp0075=0
bnlmatch_dp025=0
bnlmatch_dp075=0
bnlmatch_dp10=0
bnlmatch_dp5=0
bnlmatch_dp2=0
bnlmatch_dp1=0
bnlmatch_dp001=0
bnlmatch_dp0001=0
bnlmatch_dp05=0
bnlmatch_dp005=0
submit=1
runplot=0
export test=0
export sub=0
printname=0
pmean=-1
export pmean=-1

# KICK
#
export kickhv=95
export kicksk=-1

# BEAM
#
# Off=0  / Kick=0
# Off=10 / Kick=10
# Off=20 / Kick=20
# Off=25
# Off=30 / Kick=30
# Off=35 / Kick=25-35
# Off=40 / Kick=30-40
# Off=45 / Kick=45-60
# Off=50 / Kick=50
# Off=55 / Kick=50
# Off=60 / Kick=60-70
# Off=65 / Kick=70
# Off=70 / Kick=80
# Off=75 / Kick=85
# Off=80 / Kick=90
# Off=85 / Kick=100
# Off=90 / Kick=100
# Off=95 / Kick=105
#
# INFLECTOR
#
# 0 (Closed,Closed), 1 (DS is open), 2 (DS and US are open), 3 (Window/Flange are closed, but Coil is open)
export xc=77
export field=0
if [ ${test} == 1 ]; then
    submittingjob=0
    export submittingjob=${submittingjob}
fi

#echo "submittingjob=${submittingjob}"
#if [ ${submittingjob} == 0 ]; then
#    export infstart=0
#    ps="0 1 2"
#    ps="0 1 2 3"
#    ps="1 3 4"
#    ps="3 4"
#    sub=0
#else
#    sub=1
#fi
    
#sleep 

# Events, nevts, evts, numevt
#
#export evts=1
export evts=10000

# Number of turns
#
#
export numturns=101
#export muondecay="none"
#export muondecay="standard"

#
# Ring Hits
#
export keepringhits="on"

# Collimator Status
#
export collimator_status=1

#
# Spin Tracking
#
export spintrackingname=false
export edmtrackingname=false

if [ ${sub} == 0 ]; then
    if [ ${infstart} == 1 ]; then
	export inftype=0
	export beamstart=dm
	export kickhv=95
	export beamsize=-1
	export offset=-1
	export xc=-1
	export field=0
	export sigmat=50
	export test=0
    else
	export beamstart=0
	export beamsize=-1
	export offset=0
	export xc=-1    
	export kickhv=0
	export sigmat=50
	export test=0
	export inftype=2
	export field=-1
    fi
else
    export evts=5000
    export numturns=101
    export beamsize=-1
fi


submit=0
clean=0
setbetax=0


until [ -z ${1} ]; do
    echo "Arg = ${1}"
    if [ ${1} == "local" ]; then
	submit=0
	shift 1
	continue
    elif [ ${1} == "submit" ]; then
	submit=1
	shift 1
	continue
    elif [ ${1} == "open" ] || [ ${1} == "Open" ] || [ ${1} == "OpenInflector" ] || [ ${1} == "InflectorOpen" ]; then
	export inftype=2
	infs="OpenInflector"
	shift 1
	continue
    elif [ ${1} == "closed" ] || [ ${1} == "Closed" ] || [ ${1} == "ClosedInflector" ] || [ ${1} == "InflectorClosed" ]; then
	export inftype=0
	infs="ClosedInflector"
	shift 1
	continue
    elif [ ${1} == "partial" ] || [ ${1} == "PartiallyOpen" ] || [ ${1} == "PartiallyOpenInflector" ]; then
	export inftype=3
	infs="PartiallyOpen"
	shift 1
	continue
    elif [ ${1} == "uc" ] || [ ${1} == "UpstreamCryo" ] || [ ${1} == "upstreamcryo" ]; then
	export beamstart=uc
	export infstart=1
	beams="UpstreamCryo"
	shift 1
	continue
    elif [ ${1} == "um" ] || [ ${1} == "UpstreamMandrel" ] || [ ${1} == "upstreammandrel" ]; then
	export beamstart=um
	export infstart=1
	beams="UpstreamMandrel"
	offsets="-1"
	shift 1
	continue
    elif [ ${1} == "dc" ] || [ ${1} == "DownstreamCryo" ] || [ ${1} == "downstreamcryo" ]; then
	export beamstart=dc
	export infstart=1
	beams="DownstreamCryo"
	offsets="-1"
	shift 1
	continue
    elif [ ${1} == "dm" ] || [ ${1} == "DownstreamMandrel" ] || [ ${1} == "downstreammandrel" ]; then
	export beamstart=dm
	export infstart=1
	beams="DownstreamMandrel"
	offsets="-1"
	shift 1
	continue
    elif [ ${1} == "co" ] || [ ${1} == "CentralOrbit" ] || [ ${1} == "centralorbit" ]; then
	export beamstart=co	
	export infstart=1
	beams="CentralOrbit"
	offsets="-1"
	shift 1
	continue
    elif [ ${1} == "Perfect" ]; then
	export beamstart=0
	export infstart=0
	beams="DownstreamMandrel"
	shift 1
	continue
    elif [ ${1} == "print" ]; then	
	printname=1
	shift 1
	continue
    elif [ ${1} == "mapped" ]; then
	export field=1
	fields="mapped"
	shift 1
	continue
    elif [ ${1} == "clean" ]; then
	export clean=1
	clean=1
	shift 1
	continue
    elif [ ${1} == "decay" ]; then
	shift 1
	muondecay=${1}
	export muondecay=${1}
	shift 1
	continue
    elif [ ${1} == "spin" ]; then
	spintracking="spin"
	export spintrackingname=true	
	shift 1
	continue
    elif [ ${1} == "edm" ]; then
	spintracking="edm"
	export edmtrackingname=true
	shift 1
	continue
    elif [ ${1} == "edmsize" ]; then
	shift 1
	edmsize=${1}
	edmval=0.0
	mags="3 4 5 6 7 8 9"
	nums="1 2 3 4 5 6 7 8 9"
	for mag in ${mags}; do
	    if ! [ ${edmsize/"m${mag}"/} == ${edmsize} ]; then
		for num in ${nums}; do
		    if ! [ ${edmsize/"${num}em"/} == ${edmsize} ]; then
			edmval="${num}e-${mag}"
		    fi
		done
	    fi
	done
	export edmval="${edmval}"
	echo "EDM = ${edmval}"
	shift 1
	continue
    elif [ ${1} == "numturns" ]; then
	shift 1
	numturns=${1}
	export numturns=${1}
	continue
    elif [ ${1} == "Particle" ]; then
	shift 1
	particle=${1}
	export particle=${1}
	shift 1
	charge=${1}
	export charge=${1}
	continue
    elif [ ${1} == "scraping" ]; then
	shift 1
	scraping=${1}
	export scraping=${1}
	continue
    elif [ ${1} == "quad" ]; then
	shift 1
	quad=${1}
	export quad=${1}
	quadname=_${quad}_
	if [ ${quad} == "FullQuads" ]; then
	    quadname="_"
	fi
	if [ ${quad} == "FullHighMom" ]; then
	    export pmean=3098
	    pmean=3098
	fi
	if [ ${quad} == "FullHigherMom" ]; then
	    export pmean=3102
	    pmean=3102
	fi
	if [ ${quad} == "FullHighestMom" ]; then
	    export pmean=3106
	    pmean=3106
	fi
	continue
    elif [ ${1} == "GenGauss" ] || [ ${1} == "Gaussian" ]; then
	gengauss=1
	export gengauss=1
	shift 1
	continue
    elif [ ${1} == "Uniform" ]; then
	gengauss=0
	export gengauss=0
	shift 1
	continue
    elif [ ${1} == "carol" ] || [ ${1} == "Carol" ] || [ ${1} == "CarolMatch" ]; then
	export infgun=1
	infgun=1
	export carol=1
	carol=1
#	beamsize="40"
	export alphaX="-8.5"
	export betaX="7.9"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.0015"
	shift 1
	continue
    elif [ ${1} == "BetaX" ]; then
	shift 1
	betaX=${1}
	if [ ${betaX} == "2.025" ]; then
	    setbetax=0
	else
	    export betaX="${1}"
	    echo "Setting BetaX to [${betaX}] from 2.025"
	    setbetax=1
	fi
	continue
    elif [ ${1} == "match" ] || [ ${1} == "Match" ] || [ ${1} == "PerfectMatch" ]; then
	export infgun=1
	infgun=1
	export match=1	
	match=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.0015"
	shift 1
	continue
    elif [ ${1} == "match_dP2" ] || [ ${1} == "Match_dP2" ] || [ ${1} == "PerfectMatch_dP2" ]; then
	export infgun=1
	infgun=1
	export match_dp2=1	
	match_dp2=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.02"
	shift 1
	continue
    elif [ ${1} == "match_dP5" ] || [ ${1} == "Match_dP5" ] || [ ${1} == "PerfectMatch_dP5" ]; then
	export infgun=1
	infgun=1
	export match_dp5=1	
	match_dp5=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.05"
	shift 1
	continue
    elif [ ${1} == "match_dP10" ] || [ ${1} == "Match_dP10" ] || [ ${1} == "PerfectMatch_dP10" ]; then
	export infgun=1
	infgun=1
	export match_dp10=1	
	match_dp10=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.10"
	shift 1
	continue
    elif [ ${1} == "match_dP1" ] || [ ${1} == "Match_dP1" ] || [ ${1} == "PerfectMatch_dP1" ]; then
	export infgun=1
	infgun=1
	export match_dp1=1	
	match_dp1=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.01"
	shift 1
	continue
    elif [ ${1} == "match_dP001" ] || [ ${1} == "Match_dP001" ] || [ ${1} == "PerfectMatch_dP001" ]; then
	export infgun=1
	infgun=1
	export match_dp001=1	
	match_dp001=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.0001"
	shift 1
	continue
    elif [ ${1} == "match_dP0001" ] || [ ${1} == "Match_dP0001" ] || [ ${1} == "PerfectMatch_dP0001" ]; then
	export infgun=1
	infgun=1
	export match_dp0001=1	
	match_dp0001=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.0001"
	shift 1
	continue
    elif [ ${1} == "match_dP005" ] || [ ${1} == "Match_dP005" ] || [ ${1} == "PerfectMatch_dP005" ]; then
	export infgun=1
	infgun=1
	export match_dp005=1	
	match_dp005=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.0005"
	shift 1
	continue
    elif [ ${1} == "match_dP05" ] || [ ${1} == "Match_dP05" ] || [ ${1} == "PerfectMatch_dP05" ]; then
	export infgun=1
	infgun=1
	export match_dp05=1	
	match_dp05=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.005"
	shift 1
	continue
    elif [ ${1} == "match_dP0075" ] || [ ${1} == "Match_dP0075" ] || [ ${1} == "PerfectMatch_dP0075" ]; then
	export infgun=1
	infgun=1
	export match_dp0075=1	
	match_dp0075=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.0075"
	shift 1
	continue
    elif [ ${1} == "match_dP0025" ] || [ ${1} == "Match_dP0025" ] || [ ${1} == "PerfectMatch_dP0025" ]; then
	export infgun=1
	infgun=1
	export match_dp0025=1	
	match_dp0025=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.0025"
	shift 1
	continue
    elif [ ${1} == "match_dP025" ] || [ ${1} == "Match_dP025" ] || [ ${1} == "PerfectMatch_dP025" ]; then
	export infgun=1
	infgun=1
	export match_dp025=1	
	match_dp025=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.025"
	shift 1
	continue
    elif [ ${1} == "match_dP075" ] || [ ${1} == "Match_dP075" ] || [ ${1} == "PerfectMatch_dP075" ]; then
	export infgun=1
	infgun=1
	export match_dp075=1	
	match_dp075=1
#	beamsize="40"
	export alphaX="0.0"
	export betaX="7.66477"
	export alphaY="0.0"
	export betaY="19.0763"
	export dPoverP="0.075"
	shift 1
	continue
    elif [ ${1} == "bnlmatch" ] || [ ${1} == "BNLMatch" ] || [ ${1} == "E821Match" ]; then
	export infgun=1
	infgun=1
	export bnlmatch=1	
	bnlmatch=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.0015"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP2" ] || [ ${1} == "BNLMatch_dP2" ] || [ ${1} == "E821Match_dP2" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp2=1	
	bnlmatch_dp2=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.02"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP5" ] || [ ${1} == "BNLMatch_dP5" ] || [ ${1} == "E821Match_dP5" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp5=1	
	bnlmatch_dp5=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.05"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP10" ] || [ ${1} == "BNLMatch_dP10" ] || [ ${1} == "E821Match_dP10" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp10=1	
	bnlmatch_dp10=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.10"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP1" ] || [ ${1} == "BNLMatch_dP1" ] || [ ${1} == "E821Match_dP1" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp1=1	
	bnlmatch_dp1=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.01"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP001" ] || [ ${1} == "BNLMatch_dP001" ] || [ ${1} == "E821Match_dP001" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp001=1	
	bnlmatch_dp001=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.0001"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP0001" ] || [ ${1} == "BNLMatch_dP0001" ] || [ ${1} == "E821Match_dP0001" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp0001=1	
	bnlmatch_dp0001=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.0001"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP005" ] || [ ${1} == "BNLMatch_dP005" ] || [ ${1} == "E821Match_dP005" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp005=1	
	bnlmatch_dp005=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.0005"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP05" ] || [ ${1} == "BNLMatch_dP05" ] || [ ${1} == "E821Match_dP05" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp05=1	
	bnlmatch_dp05=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.005"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP0075" ] || [ ${1} == "BNLMatch_dP0075" ] || [ ${1} == "E821Match_dP0075" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp0075=1	
	bnlmatch_dp0075=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.0075"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP0025" ] || [ ${1} == "BNLMatch_dP0025" ] || [ ${1} == "E821Match_dP0025" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp0025=1	
	bnlmatch_dp0025=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.0025"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP025" ] || [ ${1} == "BNLMatch_dP025" ] || [ ${1} == "E821Match_dP025" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp025=1	
	bnlmatch_dp025=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.025"
	shift 1
	continue
    elif [ ${1} == "bnlmatch_dP075" ] || [ ${1} == "BNLMatch_dP075" ] || [ ${1} == "E821Match_dP075" ]; then
	export infgun=1
	infgun=1
	export bnlmatch_dp075=1	
	bnlmatch_dp075=1
#	beamsize="40"
	export alphaX="-0.543951"
	export betaX="2.025"
	export alphaY="-0.0434492"
	export betaY="19.600"
	export dPoverP="0.075"
	shift 1
	continue
    elif [ ${1} == "none" ]; then
	export field=0
	fields="none"
	shift 1
	continue
    elif [ ${1} == "tSigma50" ]; then
	export sigmat=50
	shift 1
	continue
    elif [ ${1} == "tSigma1" ]; then
	export sigmat=1
	shift 1
	continue
    elif [ ${1} == "tSigma25" ]; then
	export sigmat=25
	shift 1
	continue
    elif [ ${1} == "NoSigmaP_NoPrhat" ] || [ ${1} == "NoPrhat_NoSigmaP" ]; then
	export sigmap=0
	export prhat=0
	ps="0"
	shift 1
	continue
    elif [ ${1} == "SigmaP02_Prhat1mrad" ] || [ ${1} == "Prhat1mrad_SigmaP02" ]; then
	export sigmap=02
	export prhat=1
	ps="1"
	shift 1
	continue
    elif [ ${1} == "SigmaP05_Prhat1mrad" ] || [ ${1} == "Prhat1mrad_SigmaP05" ]; then
	export sigmap=05
	export prhat=1
	ps="2"
	shift 1
	continue
    elif [ ${1} == "SigmaP05_Prhat2mrad" ] || [ ${1} == "Prhat2mrad_SigmaP05" ]; then
	export sigmap=05
	export prhat=2
	ps="3"
	shift 1
	continue
    elif [ ${1} == "SigmaP05_Prhat5mrad" ] || [ ${1} == "Prhat5mrad_SigmaP05" ]; then
	export sigmap=05
	export prhat=5
	ps="4"
	shift 1
	continue
    elif [ ${1} == "BeamSize" ] || [ ${1} == "beamsize" ] || [ ${1} == "beam" ]; then
	shift 1
	export beamsize=${1}
	echo "Arg BeamSize=${1} mm*mrad"
	beamsize="${1}"
	shift 1
	continue
    elif [ ${1} == "kick" ] || [ ${1} == "k" ] || [ ${1} == "lcr" ]; then
	shift 1
	export kickhv=${1}
	echo "Arg kick=${kickhv} kV"
	kicks="${1}"
	shift 1
	continue
    elif [ ${1} == "sqkick" ] || [ ${1} == "sqk" ] || [ ${1} == "sq" ]; then
	shift 1
	export kicksk=${1}
	export kickhv=0
	kicksk=${1}
	kickhv=0
	echo "Arg kick=${kicksk} Sq."
	kicks="${1}"
	shift 1
	continue
    elif [ ${1} == "offset" ] || [ ${1} == "o" ]; then
	shift 1
	export offset=${1}
	offsets="${offset}"
	echo "Arg offset=${offset}"
	shift 1
	continue
    elif [ ${1} == "delta" ] || [ ${1} == "d" ]; then
	shift 1
	dname=${1}
	checkm=${dname/m}
	checkp=${dname/p}
	if ! [ ${checkm} == ${dname} ]; then
	    export delta=-${checkm}
	elif ! [ ${checkp} == ${dname} ]; then
	    export delta=${checkp}
	else
	    export delta=${dname}
	fi
	echo "Arg delta=${delta}"
	deltas="${delta}"
	shift 1
	continue
    elif [ ${1} == "prefix" ]; then
	shift 1
	prefix="${1}"
	prefix=""
#	echo "Prefix is ${prefix}"
	continue;
    elif [ ${1} == "launch" ] || [ ${1} == "l" ]; then
	shift 1
	lname=${1}
#	echo "lname=${lname}"
	checkm=${lname/m}
	checkp=${lname/p}
#	echo "checkm=${checkm}"
#	echo "checkp=${checkp}"
#	echo "launch=${launch}"
	if ! [ ${checkm} == ${lname} ]; then
	    export launch=-${checkm}
	elif ! [ ${checkp} == ${lname} ]; then
	    export launch=${checkp}
	else
#	    echo "Setting launch to [${lname}]"
	    export launch=${lname}
#	    echo "Setting launch to [${launch}]"
	fi
	echo "Arg launch=${launch}"
	launches="${launch}"
	shift 1
	continue
    elif [ ${1} == "num" ] || [ ${1} == "n" ]; then
	shift 1
	export evts=${1}
	shift 1
	continue
    elif [ ${1} == "test" ]; then
	extra=${1}
	export evts=100
#	export delta=0
#	export beamstart=um
#	export beamsize=-2
#	export offset=0
#	export kickhv=95
#	export prhat=0
#	export sigmap=0
	export submit=0
	export test=1
	export field=0
	export numturns=1
	export field=0
	export evts=100
	shift 1
	
	continue
    fi
    
    shift 1
done

#sleep 1000000

checktest
checkgun

bestoffsets
offsets="${thebestoffsets}"
for o in ${offsets}; do
    export offset=${o}
    bestkicks
    kicks="${thebestkicks}"
    
    for k in ${kicks}; do
	if [ ${kickhv} -gt 0 ]; then
	    export kickhv=${k}
	elif [ ${kicksk} -gt 0 ]; then
	    export kicksk=${k}
	fi
	bestdeltas
	deltas="${thebestdeltas}"
	
	for d in ${deltas}; do
	    export delta=${d}
	    bestlaunches
	    launches="${thebestlaunches}"
	    
	    for l in ${launches}; do	    
		export launch=${l}
		
		export timeoffset=-1
		    
		
		echo "===== Checking name ======"
		name
		echo "===== EXTRA=[${extra}] ======="
		
		if [ ${printname} == 1 ]; then
		    echo "${extra}"
		    echo "Above is submit.sh name"
		    continue;
		fi
		
		if [ ${test} == 1 ]; then
		    echo "Real: ${dir}"
		    extra="test"
		fi
		
		
		echo ""
		echo ${extra}; 
		echo "Evts: ${evts}"
		
		echo ""
		echo ""
		
		if [ -z ${2} ] && [ ${submit} == 1 ]; then
		    echo "source ${script}"
		    isSOURCE=1 #0
		else
		    if [ ${submit} == 0 ]; then
			isSOURCE=1
		    else
			if [ ${2} == "source" ]; then
			    echo "${script} is sourced. good to go..."
			    isSOURCE=1
			else
			    echo "not sure..."
			    isSOURCE=1 # 0
			fi
		    fi
		fi
		
		if [ ${isSOURCE} == 1 ]; then
		    
		    base="${extra}"
		    mydate=`date "+%y.%h.%d__%H.%M"`
		    
		    export basename=${extra}
		    
		    export outDir=/gm2/data/users/tgadfort/gm2ringsim/output
		    if [ ${submit} == 1 ]; then
			subDir="${extra}_${mydate}"
			subDir="${extra}"
		    else
			subDir="${extra}"
		    fi
		    export fullDir=${outDir}/${subDir}
		    if [ ${clean} == 1 ]; then
			rm -fr ${fullDir}
		    fi
		    mkdir -p ${fullDir}
		    chmod g+rw ${fullDir}
		    
		    njobs="1"
		    if [ ${submit} == 1 ]; then
			echo "jobsub -g --opportunistic -dMYDIR ${fullDir} sub2.sh"
#			echo "jobsub -e fullDir -e basename -e MYREL -N ${njobs} -g condor2.sh"
		    fi
		    
		    ./makefcl.sh "inflector" 
		    if [ ${submit} == 1 ]; then
			if [ -a cmd.sh ]; then
			    rm cmd.sh
			fi
			echo "Output Directory [${fullDir}]"
			MYDIR=${fullDir}
#			echo "jobsub -g --opportunistic -d ${MYDIR} -e MYREL -e fullDir -N ${njobs} -g condor2.sh" > cmd.sh
#			echo "jobsub -g --opportunistic -e fullDir -e basename -e MYREL -N ${njobs} -g condor2.sh" > cmd.sh
#			echo "jobsub -e fullDir -e basename -e MYREL -N ${njobs} -g condor2.sh" > cmd.sh
			
			jobsub -g --opportunistic -dMYDIR ${fullDir} sub2.sh
#			chmod 777 cmd.sh
#			sleep 1
#			source cmd.sh
#			rm cmd.sh
#			sleep 1
		    else			
			if [ -a ${fullDir}/runner.fcl ]; then
			    echo ${fullDir}/runner.fcl
			    if [ ${test} == 1 ]; then
				rm dump
				gm2 -c ${fullDir}/runner.fcl | tee dump
			    else
				touch ${fullDir}/START
				gm2 -c ${fullDir}/runner.fcl | tee ${fullDir}/output.dat
				rm ${fullDir}/START
				touch ${fullDir}/DONE
#				mv ${extra}*.root ${fullDir}/
				echo ""
				echo "./run.sh ${subDir}"
				echo ""
				ls ${fullDir}/*.dat
				ls ${fullDir}/*.root
			    fi
			fi
		    fi   
		fi
	    done
	done
    done
done
