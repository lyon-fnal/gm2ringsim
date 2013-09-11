#!/bin/sh


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
	
	if ! [ -z ${gm2size} ]; then
	    extra="${extra}_Gm2_${gm2size}"
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
	elif [ ${offset} -lt 0 ]; then
	    loc_offset=`echo " ${offset} * -1" | bc`
	    extra="${extra}_Offsetm${loc_offset}"
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
    
    if [ ${sigmat} -gt 0 ]; then
	extra="${extra}_tSigma${sigmat}"
    elif [ ${sigmat} -lt 0 ]; then
	loc_sigmat=`echo " ${sigmat} * -1" | bc`
	extra="${extra}_GausstSigma${loc_sigmat}"
    else
	extra="${extra}_FNALt0"
    fi
    
    kickname="NoKick"
    if [ ${lcrkick} == 1 ]; then
	if [ ${kickhv} -gt 0 ]; then
	    kickname="${kickhv}kVLCRKick"
	fi
    fi
    if [ ${squarekick} == 1 ]; then
	if [ ${kicksk} -gt 0 ]; then
	    kickname="${kicksk}GSquareKick"
	fi
    fi
    extra="${extra}_${kickname}"
    
    if [ ${infstart} == 1 ]; then
	extra="${extra}_${inftype}"
	
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
	extra="${extra}_${inftype}"
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
	if [ ${beamtype} == "Gaussian" ]; then
	    extra="${extra}_InflectorGun"
	else
	    extra="${extra}_UniformInflectorGun"
	fi

	if ! [ -z ${mom} ]; then
	    extra="${extra}_${mom}"
	else
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
	    if [ ${match_flat} == 1 ]; then
		extra="${extra}_PerfectMatch_Flat"
	    fi
	    if [ ${match_flatdp2} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP2"
	    fi
	    if [ ${match_flatdp1} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP1"
	    fi
	    if [ ${match_flatdp001} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP001"
	    fi
	    if [ ${match_flatdp0001} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP0001"
	    fi
	    if [ ${match_flatdp005} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP005"
	    fi
	    if [ ${match_flatdp0075} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP0075"
	    fi
	    if [ ${match_flatdp0025} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP0025"
	    fi
	    if [ ${match_flatdp025} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP025"
	    fi
	    if [ ${match_flatdp075} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP075"
	    fi
	    if [ ${match_flatdp05} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP05"
	    fi
	    if [ ${match_flatdp5} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP5"
	    fi
	    if [ ${match_flatdp10} == 1 ]; then
		extra="${extra}_PerfectMatch_FlatdP10"
	    fi
	    if [ ${bnlmatch} == 1 ]; then
		extra="${extra}_E821Match"
	    fi
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

    if [ ${flatdecay} == 1 ] && ! [ ${muondecay} == "none" ]; then
	extra="${extra}_FlatDecayTime"
    fi
	
}

carol=0
match=0
infstart=1
infgun=1
carol=0
match=0
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
match_flat=0
match_flatdp0025=0
match_flatdp0075=0
match_flatdp025=0
match_flatdp075=0
match_flatdp10=0
match_flatdp5=0
match_flatdp2=0
match_flatdp1=0
match_flatdp001=0
match_flatdp0001=0
match_flatdp05=0
match_flatdp005=0
bnlmatch=0
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


# Collimator Status
#
export collimator_status=1

# Muon Decay
#

export flatdecay=1
export muondecay="none"
#export muondecay="iso"
export muondecay="sm"
muondecayname="decay ${muondecay}"
if ! [ ${muondecay} == "none" ] && [ ${flatdecay} == 1 ]; then
    muondecayname="${muondecayname} flatdecay"
fi

#
# Injected Particle
#
particle="proton"
particle="mu+"
charge=1
export particle=${particle}
export charge=${charge}

#
# Spin Tracking
#
export gm2val=-1
export edmval=0
gm2size=""
spintracking="edm"
spintracking="spin"
#spintracking=""
edmsize="0"
edmsizename="edmsize ${edmsize}"
if ! [ -z ${gm2size} ]; then
    edmsizename="${edmsizename} gm2size ${gm2size}"
fi

export spintracking=${spintracking}


bestedms()
{
    thebestedms="0"
}

bestkicks()
{
    squarekick=1
    lcrkick=0

    thebestkicks="95"
    if [ ${infstart} == 1 ]; then
	if [ ${beamstart} == "CentralOrbit" ]; then
	    thebestkicks="0"
	    if [ ${offset} -gt 0 ]; then
		if [ ${lcrkick} == 1 ]; then
		    thebestkicks="80 85 90 95 100 105"
		    thebestkicks="0"
		fi
		if [ ${squarekick} == 1 ]; then
		    thebestkicks="130 160 190 200 210 220 230 240 250 280 310"
		    thebestkicks="0"
		fi
	    fi
		
	    return
	fi
	if [ ${beamstart} == "DownstreamMandrel" ] || [ ${beamstart} == "UpstreamMandrel" ] || [ ${beamstart} == "UpstreamCryo" ]; then
	    if [ ${lcrkick} == 1 ]; then
		thebestkicks="85 90 95 100"
		thebestkicks="90"
	    fi
	    if [ ${squarekick} == 1 ]; then
		thebestkicks="150 160 170 180 190 200 210 220 230 240 250 260"
		thebestkicks="0 10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 160 170 180 190 200 210 230 240 250 260 270 280 290 300"
		thebestkicks="220"
#		thebestkicks="219"
#		thebestkicks="0"
	    fi

	    return;
	fi
    fi

    if [ ${test} == 1 ]; then
	thebestkicks="220"
    fi
}

bestoffsets()
{
    thebestoffsets="0"
    if [ ${beamstart} == "CentralOrbit" ]; then
	thebestoffsets="10 20 30 40"
	thebestoffsets="-10 -20 -30 -40"
#	thebestoffsets="20 40"
	thebestoffsets="0"
	return;
    else
	thebestoffsets="0"
	return;
    fi
    
    if [ ${test} == 1 ]; then
	thebestoffsets="0"
    fi
}

bestdeltas()
{
    thebestdeltas=""
    if [ ${beamstart} == "CentralOrbit" ]; then
	thebestdeltas="0"
	return;
    fi

    if [ ${beamstart} == "DownstreamMandrel" ]; then
#	thebestdeltas="15 10 7 3 0 -3 -7 -10 -15 -20"
#	echo "Core = ${core}"
	if [ ${field} == 1 ]; then
	    if [ ${core} == -1 ]; then
		thebestdeltas="0"
	    fi
	    if [ ${core} == 0 ]; then
		thebestdeltas="0"
	    fi
	    if [ ${core} == 1 ]; then
		thebestdeltas="0 -2"
	    fi
	    if [ ${core} == 2 ]; then
		thebestdeltas="15 10 7 3 0 -3 -7 -10 -15 -20"
	    fi
	    if [ ${core} == 3 ]; then
		thebestdeltas="30 25 20 15 10 7 3 0 -3 -7 -10 -15 -20 -25 -30"
	    fi
	    if [ ${core} == 4 ]; then
		thebestdeltas="-10 -9 -8 -7 -6 -5 -4 -3 -2 -1 0 1 2 3"
	    fi
	fi
	if [ ${field} == 0 ]; then
	    if [ ${core} == -1 ]; then
		thebestdeltas="0"
	    fi
	    if [ ${core} == 0 ]; then
		thebestdeltas="0"
	    fi
	    if [ ${core} == 1 ]; then
		thebestdeltas="0 -2"
	    fi
	    if [ ${core} == 2 ]; then
		thebestdeltas="15 10 7 3 0 -3 -7 -10 -15 -20"
	    fi
	    if [ ${core} == 3 ]; then
		thebestdeltas="30 25 20 15 10 7 3 0 -3 -7 -10 -15 -20 25 30"
	    fi
	    if [ ${core} == 4 ]; then
		thebestdeltas="-10 -9 -8 -7 -6 -5 -4 -3 -2 -1 0 1 2 3"
	    fi
	fi
    fi
    if [ ${beamstart} == "UpstreamMandrel" ]; then
	thebestdeltas="10 5 3 0 -3 -7 -10 -15 -20"
	thebestdeltas="0 -7"
    fi
    if [ ${beamstart} == "UpstreamCryo" ]; then
#	echo "Core = ${core}"
#	thebestdeltas="10 5 3 0 -3 -7 -10 -15 -20"
#	thebestdeltas="3 0 -3 -4 -5 -6 -7 -10"
	if [ ${field} == 0 ]; then
	    if [ ${core} == -1 ]; then
		thebestdeltas="-6"
	    fi
	    if [ ${core} == 0 ]; then
		thebestdeltas="-6"
	    fi
	    if [ ${core} == 1 ]; then
		thebestdeltas="-7 -5"
	    fi
	    if [ ${core} == 2 ]; then
		thebestdeltas="3 2 1 0 -1 -2 -3 -4 -5 -6 -7 -10 -12"
		thebestdeltas="3 2 1 0 -1 -2 -3 -4 -6 -10 -12"
	    fi
	    if [ ${core} == 3 ]; then
		thebestdeltas="50 45 40 35 30 25 20 15 10 7 5 3 0 -3 -4 -5 -6 -7 -10 -12 -15 -20 -25 -30 -35 -40 -45 -50"
	    fi
	    if [ ${core} == 4 ]; then
		thebestdeltas="1 0 -1 -2 -3 -4 -5 -6 -7"
	    fi
	fi
	if [ ${field} == 1 ]; then
	    if [ ${core} == -1 ]; then
		thebestdeltas="-3"
	    fi
	    if [ ${core} == 0 ]; then
		thebestdeltas="-3"
	    fi
	    if [ ${core} == 1 ]; then
		thebestdeltas="-4"
	    fi
	    if [ ${core} == 2 ]; then
		thebestdeltas="10 5 3 2 1 0 -1 -2 -3 -4 -7 -10 -15 -20"
		thebestdeltas="10 5 3 2 1 0 -1 -2 -7 -10 -15 -20"
	    fi
	    if [ ${core} == 3 ]; then
		thebestdeltas="50 45 40 35 30 25 20 15 10 5 3 2 1 0 -1 -2 -3 -4 -7 -10 -15 -20 -25 -30 -35 -40 -45 -50"
	    fi
	    if [ ${core} == 4 ]; then
		thebestdeltas="1 0 -1 -2 -3 -4 -5 -6 -7"
	    fi
	fi
    fi

    if [ ${test} == 1 ]; then
	thebestdeltas="0"
    fi

}

bestlaunches()
{
    thebestlaunches=""
    if [ ${test} == 1 ]; then
	thebestlaunches="-22 -20 -18 -15 -12 -10 -7 -5 -2 0 2 5"
    fi
    if [ ${beamstart} == "CentralOrbit" ]; then
	thebestlaunches="0"
	if [ ${offset} -gt 0 ]; then
	    thebestlaunches="0" # -2 -4 -6 -8 -10"
	fi
	return
    fi

    
    if [ ${beamstart} == "UpstreamMandrel" ]; then
	thebestlaunches="5 0 -7 -15 -20 -25"
	if [ ${delta} -ge 5 ]; then
	    thebestlaunches="-20 -15 -10"
	elif [ ${delta} -ge 0 ]; then
	    thebestlaunches="-20 -15 -10 -7 -3"
	    thebestlaunches="-15 -10"
	elif [ ${delta} -ge -5 ]; then
	    thebestlaunches="-20 -15 -10 -7 -3"
	    thebestlaunches="-15 -10"
	else
	    thebestlaunches="-15 -10 -7 -3 0 5"
	    thebestlaunches="-15 -10"
	fi
	return;
    elif [ ${beamstart} == "UpstreamCryo" ]; then	
	incnum=2
	if [ ${core} == -1 ]; then
	    sigma=0	
	elif [ ${core} == 0 ]; then
	    sigma=2
	elif [ ${core} == 1 ]; then
	    sigma=5
	elif [ ${core} == 2 ]; then
	    sigma=7	    
	elif [ ${core} == 3 ]; then
	    sigma=20
	    incnum=5	    
	elif [ ${core} == 4 ]; then
	    sigma=6
	    incnum=1
	else
	    sigma=7
	fi

	if [ ${field} == "1" ]; then
	    if [ ${delta} -ge -4 ]; then
		let base=-12
	    else
		let base=-10
	    fi
	    thebestlaunches="${base}"
	    let lch=${base}-${sigma}
	    while [ ${lch} -lt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch+=${incnum}))
	    done
	    let lch=${base}+${sigma}
	    while [ ${lch} -gt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch-=${incnum}))
	    done
	fi	    
	if [ ${field} == "0" ]; then
	    let base=0
	    thebestlaunches="${base}"
	    let lch=${base}-${sigma}
	    while [ ${lch} -lt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch+=${incnum}))
	    done
	    let lch=${base}+${sigma}
	    while [ ${lch} -gt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch-=${incnum}))
	    done
	    
	    zeroOK=0
	    for lch in ${thebestlaunches}; do
		if [ ${lch} == 0 ]; then
		    zeroOK=1
		fi
	    done
	fi
	return
    elif [ ${beamstart} == "DownstreamMandrel" ]; then
	incnum=2
	if [ ${core} == -1 ]; then
	    sigma=0
	elif [ ${core} == 0 ]; then
	    sigma=0
	elif [ ${core} == 1 ]; then
	    sigma=2
	elif [ ${core} == 2 ]; then
	    sigma=5
	elif [ ${core} == 3 ]; then
	    sigma=20
	    incnum=20
	elif [ ${core} == 3 ]; then
	    sigma=4
	    incnum=1
	else
	    sigma=5
	fi

	if [ ${field} == "1" ]; then
	    loc_delta=`echo " ${delta} * -1" | bc`
	    let base=${loc_delta}
	    thebestlaunches="${base}"
	    let lch=${base}-${sigma}
	    while [ ${lch} -lt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch+=${incnum}))
	    done
	    let lch=${base}+${sigma}
	    while [ ${lch} -gt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch-=${incnum}))
	    done
	fi	    
	if [ ${field} == "0" ]; then
	    loc_delta=`echo " ${delta} * -1" | bc`
	    let base=${loc_delta}-6
	    thebestlaunches="${base}"
	    let lch=${base}-${sigma}
	    while [ ${lch} -lt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch+=${incnum}))
	    done
	    let lch=${base}+${sigma}
	    while [ ${lch} -gt ${base} ]; do
		thebestlaunches="${thebestlaunches} ${lch}"
		((lch-=${incnum}))
	    done
	fi


#	thebestlaunches="-3141"
	return;
    else
	if [ ${kick} == 220 ]; then
	    thebestlaunches="5 0 -7 -15"
	else
	    thebestlaunches="0 -7"
	fi
    fi
}

#Chih-Hsiang Cheng (CalTech)

nt () {
    local OPTIND
    local fmt="%Y"
    while getopts "amc" opt; do
        case "$opt" in
            a) fmt="%X" ;;
            m) fmt="%Y" ;;
            c) fmt="%Z" ;;
        esac
    done
    shift $(( OPTIND - 1))
    [[ $(stat -c "$fmt" "$1") > $(stat -c "$fmt" "$2") ]]
}



#
#
# EXTRANAME
#
#
extraname="_normal2"


script="./run.sh"

datadir="/gm2/data/users/tgadfort/gm2ringsim/output"

#
# Inflector
#
inftypes="InflectorOpen ClosedInflector PartiallyOpen"
inftypes="InflectorOpen"
#infs="InflectorOpen ClosedInflector"
#infs="InflectorOpen"
#infs="PartiallyOpen"

#
# Beamstarts
#

beamstarts="CentralOrbit DownstreamMandrel UpstreamCryo"
#beamstarts="CentralOrbit"
#beamstarts="UpstreamCryo DownstreamMandrel"
#beamstarts="UpstreamCryo"
beamstarts="CentralOrbit"
#beamstarts="DownstreamMandrel"

fields="0 1"
fields="0"




sigmats="1 50"
sigmats="50"
sigmats="1 25 50"
sigmats="1 25"
sigmats="1 5 25 50"
#sigmats="1 5 50"
sigmats="-1 -5 -25 -50"
sigmats="1 25 50 -1 -25"
sigmats="25 -25"
sigmats="100"
sigmats="100 0"
#sigmats="50"
sigmats="1 -5 -15 25 -25 50 100 0"
sigmats="-15 100 0"
sigmats="1"


#
# QUADS
#
quads="NoQuads NoQuadSupports NewSupports"
#quads="NoQuads NewSupports"
quads="NewSupports"
#quads="NoQuads NoQuadSupports NewSupports FullHighMom FullHigherMom FullHighestMom"
#quads="NoQuadSupports"
#quads="NoQuads"
#quads="NewSupports FullHighestMom FullHigherMom FullHighMom"
#quads="NoQuads NoneHighestMom NoneHigherMom NoneHighMom"

#
# Moms
#
if [ ${beamstarts} == "CentralOrbit" ]; then
    moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP0025 PerfectMatch_dP005 PerfectMatch_dP0075 PerfectMatch PerfectMatch_dP025 PerfectMatch_dP05 PerfectMatch_dP075 PerfectMatch_dP1 PerfectMatch_dP2 PerfectMatch_dP5 PerfectMatch_dP10"
    moms="${moms} E821Match_dP0001 E821Match_dP001 E821Match_dP0025 E821Match_dP005 E821Match_dP0075 E821Match E821Match_dP025 E821Match_dP05 E821Match_dP075 E821Match_dP1 E821Match_dP2 E821Match_dP5 E821Match_dP10"
#    moms="E821Match_dP05 PerfectMatch_dP05 E821Match_dP005 PerfectMatch_dP005 E821Match_dP0001 PerfectMatch_dP0001"
    moms="PerfectMatch_dP0001 PerfectMatch PerfectMatch_dP05 PerfectMatch_dP2"
# PerfectMatch"
    moms="PerfectMatch PerfectMatch_Flat"
#    moms="PerfectMatch_FlatdP05"
#    moms="PerfectMatch_FlatdP0001 PerfectMatch_Flat PerfectMatch_FlatdP05 PerfectMatch_FlatdP2"
    moms="PerfectMatch_dP001 PerfectMatch_dP005 PerfectMatch PerfectMatch_dP05"
    moms="${moms} PerfectMatch_FlatdP001 PerfectMatch_FlatdP005 PerfectMatch_Flat PerfectMatch_FlatdP05"
# PerfectMatch_dP05 PerfectMatch_dP001"
    if ! [ -z ${spintracking} ]; then
	if [ ${sigmats} == "-5" ] || [ ${sigmats} == "-15" ]; then
	    moms="PerfectMatch_dP001 PerfectMatch_dP005 PerfectMatch PerfectMatch_dP05"
	    moms="${moms} PerfectMatch_FlatdP001 PerfectMatch_FlatdP005 PerfectMatch_Flat PerfectMatch_FlatdP05"
	else
	    moms="PerfectMatch"
	fi
    fi
    moms="PerfectMatch_dP05 PerfectMatch_FlatdP05"
    moms="PerfectMatch"
    core=0
#    fields="0 1"
    fields="0"
#    moms="E821Match_dP0001 E821Match_dP10"
else
    fields="0"
    moms="E821Match_dP05 PerfectMatch_dP05"
    core=-1
    fields="0"
 
   moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP0025 PerfectMatch_dP005 PerfectMatch_dP0075 PerfectMatch PerfectMatch_dP025 PerfectMatch_dP05 PerfectMatch_dP075 PerfectMatch_dP1 PerfectMatch_dP2 PerfectMatch_dP5 PerfectMatch_dP10"
    moms="${moms} E821Match_dP0001 E821Match_dP001 E821Match_dP0025 E821Match_dP005 E821Match_dP0075 E821Match E821Match_dP025 E821Match_dP05 E821Match_dP075 E821Match_dP1 E821Match_dP2 E821Match_dP5 E821Match_dP10"


    core=-1
    fields="0 1"
    moms="E821Match_dP0001 E821Match_dP05 PerfectMatch_dP05 PerfectMatch_dP0001"
#    moms="E821Match_dP0001 PerfectMatch_dP0001 E821Match_dP10 PerfectMatch_dP10"
    core=2
    moms="E821Match_dP05"
    moms="PerfectMatch_dP05"
fi
#moms="E821Match_dP05 PerfectMatch_dP05"

#moms="E821Match_dP0001 E821Match_dP001 E821Match_dP0025 E821Match_dP005 E821Match E821Match_dP05 E821Match_dP1 E821Match_dP2 E821Match_dP10"
#moms="E821Match_dP0001 E821Match_dP001 E821Match E821Match_dP05 E821Match_dP2 E821Match_dP10"
#moms="E821Match_dP0001 E821Match_dP05 E821Match_dP10"
#moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP0025 PerfectMatch_dP005 PerfectMatch_dP0075 PerfectMatch PerfectMatch_dP025 PerfectMatch_dP05 PerfectMatch_dP1 PerfectMatch_dP2 PerfectMatch_dP10 E821Match"
#moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP005 PerfectMatch PerfectMatch_dP005 PerfectMatch_dP05 PerfectMatch PerfectMatch_dP2 _dP2 PerfectMatch_dP10"
#moms="PerfectMatch_dP05 E821Match_dP05" # E821Match_dP2"
# PerfectMatch_dP05 E821Match_dP05 E821Match_dP2"

#
# Inflector Ap Study
#
#quads="NoQuads_Ap18mm NoQuads_Ap30mm"
#quads="NoQuads_Ap18mm NoQuads_Ap40mm"
#quads="NewSupports_Ap40mm NewSupports_Ap18mm"
##beta="5.625" ### for Ap=30
#betaX="10.0" ### for Ap=40
#betaX="2.025"
setbetax=0

#extraname="${extraname}"

isNoQuads=0
isNewSupports=1
isNoSupports=0
if [ ${isNoQuads} == "true;" ]; then
    isNoQuads=1
else
    isNoQuads=0
fi
if [ ${isNewSupports} == "true;" ]; then
    isNewSupports=1
else
    isNewSupports=0
fi
if [ ${isNoSupports} == "true;" ]; then
    isNoSupports=1
else
    isNoSupports=0
fi

echo ${isNoQuads} ${isNewSupports} ${isNoSupports}

#
# If we need to scale BetaX
#
#setbetax=1
#betaX="5.625"
betaxname=""
if [ ${setbetax} == 1 ]; then
    betaxname="BetaX ${betaX}"
fi

#
#
# General Run Information
#
#
numevts=25000
#numevts=10000

scrapings="OFF"
beamtypes="Gaussian Uniform"
beamtypes="Uniform"
#beamtypes="Gaussian"
numturns=30
numturns=1
#numturns=150

beamsizes="10 20 30 40 50 60"
#beamsizes="40 1 20"
#beamsizes="1 20"
#beamsizes="0"
beamsizes="40 0 20"
#beamsizes="40"
beamsizes="40 0"
#beamsizes="40"
beamsizes="40"


#####
#
# For x-ray pictures
#
#####
xray=0
if [ ${xray} == 1 ]; then
    beamsizes="600"
    numturns=1
    moms="PerfectMatch_dP0001"
    numevts=25000
fi

#beamsizes="0"
#extraname="_${beamsizes}"
#extraname="tmp"
#extraname="_dPoP"


cleanval="clean"
cleanval=""
submitname="local"
#submitname="submit"
if [ ${xray} == 1 ]; then
    submitname="local"
fi
runall=0
subjob=0
clearnotinqueue=0
suball=0
if [ -z ${1} ]; then
#    extraname="_${beamstarts}"
#    inftypes="InflectorOpen ClosedInflector PartiallyOpen"
    subjob=1
else
    if [ ${1} == "rm" ] || [ ${1} == "del" ]; then
	clearnotinqueue=1
	subjob=1
	suball=1
	cleanval="clean"
    elif [ ${1} == "keep" ]; then
	subjob=1
	suball=1
	clearnotinqueue=0
	cleanval=""
    elif [ ${1} == "all" ]; then
	subjob=0
	suball=0
	runall=1
	clearnotinqueue=0
	cleanval=""
    elif [ ${1} == "local" ]; then
	submitname="local"
    else
	subjob=0
    fi
fi

let nrunjobs=0
let nsubjobs=0
let ndonejobs=0
let nnocatjobs=0
let nnotdonejobs=0

nrun=0
njob=0
test=0

sleepnum=0


if [ -z ${1} ]; then
    infstart=1
else
    infstart=${1}
fi

export submittingjob=1

if [ ${submitname} == "local" ]; then
    cleanval=""
fi


njobsmax=25000


runit=1

if [ ${runit} == 1 ]; then    
       
    nrun=0
    njob=0
    infstart=1
    echo "HI"
    sleep 2
    
    scriptoutname="_inflector${extraname}"
#    if [ ${fields} == 1 ]; then
#	scriptoutname="_mappedfield_inflector"
#	beamsizes="40"
#    fi
    
    outfile="makeoutput${scriptoutname}.sh"
    if [ -a ${outfile} ]; then
	rm ${outfile}
    fi
    echo "#!/bin/sh" >> ${outfile}
    echo "" >> ${outfile}
    echo "doit=0" >> ${outfile}
    echo "" >> ${outfile}
    echo "if [ \${doit} == 0 ]; then" >> ${outfile}
    subfile="makesubmit${scriptoutname}.sh"
    if [ -a ${subfile} ]; then
	rm ${subfile}
    fi
    


			
    for scraping in ${scrapings}; do
	
	if [ ${scraping} == "ON" ]; then
	    continue;
	fi
	
	#echo "          Scraping [${scraping} / (${scrapings})]"
	
	for sigmat in ${sigmats}; do
	    export sigmat=${sigmat}
	    
	    #echo "  sigmaT [${sigmat} / (${sigmats})]"

	    for beamtype in ${beamtypes}; do

		echo "Beamtype [${beamtype} / ${beamtypes}]"
	    
		numevents_og=${numevts}
		for beamsize in ${beamsizes}; do
		    
#		    if [ ${beamsize} -le 1 ]; then
#			numevts=`echo " ${numevents_og} / 5" | bc`
		    echo "  Beamsize [${beamsize} / (${beamsizes})]"		    
#		    else
#			numevts=${numevents_og}
#		    fi
		
		for quad in ${quads}; do
		    quadname="_${quad}_"
		    if [ ${quad} == "FullQuads" ]; then
			quadname="_"
		    fi
		    
		    for inftype in ${inftypes}; do
			
			echo "    Inflector [${inftype} / (${inftypes})]:   [${nrunjobs} , ${nsubjobs}(${nnotdonejobs}) , ${ndonejobs} , (${nnocatjobs})]"
			
			for beamstart in ${beamstarts}; do
			    if [ ${beamstart} == "CentralOrbit" ]; then
				if ! [ ${inftype} == "InflectorOpen" ]; then
				    continue;
				fi
			    fi

			    
			    echo "      Beamstart [${beamstart} / (${beamstarts})]"		    
			    
			    for field in ${fields}; do
				
#				if [ ${field} == "1" ]; then
#				    if [ ${inftype} == "PartiallyOpen" ]; then
#					continue;
#				    fi
#				    if ! [ ${beamsize} == "40" ]; then
#					continue;
#				    fi
#				fi
				echo "        Field [${field} / (${fields})]"
				
				fieldname=""
				resubfieldname="none"
				if [ ${field} == 1 ]; then
				    fieldname="_MappedField"
				    resubfieldname="mapped"
				fi
					
				bestoffsets
				offsets="${thebestoffsets}"					    
				for offset in ${offsets}; do
				    
				    if [ ${offset} == 0 ] && [ ${field} == 1 ]; then
					continue;
				    fi
				    echo "          Offset [${offset} / (${offsets})]"
				    #continue;
				    
				    bestkicks
				    kicks="${thebestkicks}"
				    for kick in ${kicks}; do
					
					if [ ${squarekick} == 1 ]; then
					    kicksk=${kick}
					    kickname="${kick}GSquareKick"
					    kicksubname="sqk"
					elif [ ${lcrkick} == 1 ]; then
					    kickhv=${kick}
					    kickname="${kick}kVLCRKick"
					    kicksubname="lcr"
					else
					    echo "No kick set."
					    sleep 1000000
					fi
					echo "            Kick [${kick} / (${kicks})]"

					
					for mom in ${moms}; do
					    
					    echo "              Momentum [${mom} / (${moms})]"
					#continue
					    
					    bestdeltas
					    deltas="${thebestdeltas}"
					    
					    for delta in ${deltas}; do
						echo "                Delta [${delta} / (${deltas})]"
					    #continue
						
						d=${delta}
						bestlaunches
						launches="${thebestlaunches}"
						
						
						deltaname=""
						if [ ${delta} -gt 0 ]; then
						    deltaname="Deltap${delta}mrad_"
						elif [ ${delta} -lt 0 ]; then
						    loc_delta=`echo " ${delta} * -1" | bc`
						    deltaname="Deltam${loc_delta}mrad_"
						else
						    deltaname="NoDelta_"
						fi
						
						for launch in ${launches}; do
						
						    echo "                  Launch [${launch} / (${launches})]"
						#continue;
						
						launchname=""
						if [ ${launch} -gt 0 ]; then
						    launchname="Launchp${launch}mrad"
						elif [ ${launch} -lt 0 ]; then
						    loc_launch=`echo " ${launch} * -1" | bc`
						    launchname="Launchm${loc_launch}mrad"
						else
						    launchname="NoLaunch"
						fi	    
						
						
						myinf=${inftype}
#						echo ${name}

						if [ ${sigmat} -ge 0 ]; then
						    sigmatname="tSigma${sigmat}"
						else
						    loc_sigmat=`echo " ${sigmat} * -1" | bc`
						    sigmatname="GausstSigma${loc_sigmat}"
						fi

						name
						outname="${extra}"
#						echo "Myname=${myname}"
#						echo " Extra=${extra}"
#						continue;

						printname=0
						if [ ${printname} == 1 ]; then
						# Test things
						    ./submit.sh ${submitname} ${myinf} ${beamstart} ${resubfieldname} d ${delta} l ${launch} ${kicksubname} ${kick} BeamSize ${beamsize} ${mom} ${sigmatname} o ${offset} num ${numevts} quad ${quad} scraping ${scraping} numturns ${numturns} ${beamtype} Particle ${particle} ${charge} ${spintracking} "${edmsizename}" "${muondecayname}" ${betaxname} "print"
						    echo ${outname}
						    echo "Above is local name"
						    echo ""
						    echo ""
						    continue
						fi

						if [ ${subjob} == 1 ]; then
						    if ! [ -d ${datadir}/${outname} ] || [ ${suball} == 1 ]; then
							if [ ${clearnotinqueue} == 1 ]; then
							    if [ -d ${datadir}/${outname} ]; then
								rm -rf ${datadir}/${outname}
								echo "Found and removed [${outname}]"
							    fi
							fi

							((nsubjobs++))
							if ! [ -d ${datadir}/${outname} ]; then
							    echo "No dir [${outname}]"
							else
							    if [ ${suball} == 1 ]; then
								echo "Found dir [${outname}], but rerunning."
								rootfile="${outname}.root"
							    fi
							fi
							echo "export submittingjob=1" >> ${subfile}
							echo "./submit.sh ${submitname} ${myinf} ${beamstart} ${resubfieldname} d ${delta} l ${launch} ${kicksubname} ${kick} BeamSize ${beamsize} ${mom} ${sigmatname} o ${offset} num ${numevts} quad ${quad} scraping ${scraping} numturns ${numturns} ${beamtype} Particle ${particle} ${charge} ${spintracking} ${edmsizename} ${muondecayname} ${betaxname} ${cleanval}" >> ${subfile}
							echo "sleep ${sleepnum}" >> ${subfile}
							continue;
						    else
							if [ -a ${datadir}/${outname}/START ]; then
							    if [ ${clearnotinqueue} == 1 ]; then
								echo "Removed [${outname}] because it was requested."
								rm -fr ${datadir}/${outname}
								continue
							    fi
							    #rm -fr ${datadir}/${outname}
							    echo "job is currently running [${outname}]"
							    ((nnotdonejobs++))
							    continue
							else
							    if ! [ -a ${datadir}/${outname}/DONE ]; then
								if [ ${clearnotinqueue} == 1 ]; then
								    echo "Removed [${outname}] because it was requested."
								    rm -fr ${datadir}/${outname}
								    continue
								fi
								echo "job is not in the queue yet [${outname}]"
								((nnotdonejobs++))
								continue
							    fi
							fi
						    fi
						fi
						
						filecheck="g2MIGTRACE_${outname}.root"
						filecheck="Eff/${beamstart}_${inftype}/${outname}.dat"
						if [ ${beamstart} == "CentralOrbit" ] && [ ${offset} -gt 0 ]; then
						    filecheck="Eff/${beamstart}_Offset${offset}_${inftype}/${outname}.dat"
						fi						    
						
						nfile=0
						if ! [ -a ${filecheck} ]; then

						    nfile=0
						    for file in ${datadir}/${outname}/*.root; do
							if [ -a ${file} ]; then
							    ((nfile++))
							fi
						    done
						    
						    if [ ${nfile} -gt 0 ]; then
							((nrunjobs++))
							echo "### Missing ${filecheck}" >> ${outfile}
#							echo "${script} ${outname}"
							echo "rm tmp; echo \"### [${njob}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							echo "rm tmp; echo \"### [${nrun} / ${njobsmax}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							echo "${script} ${outname}" >> ${outfile}
							echo "rm tmp; echo \"### [${nrun} / ${njobsmax}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							((nrun++))
							if [ ${nrun} -gt ${njobsmax} ]; then
							    ((njob++))
							    echo "elif [ \${doit} == ${njob} ]; then" >> ${outfile}
							    nrun=0
							fi
							continue
						    fi
						    
						    if [ -a ${datadir}/${outname}/DONE ] && [ ${nfile} == 0 ]; then
							if [ ${subjob} == 1 ]; then
							    ((nsubjobs++))
							    echo "No root file in [${outname}]"
							    echo "export submittingjob=1" >> ${subfile}
							    echo "./submit.sh ${submitname} ${myinf} ${beamstart} ${resubfieldname} d ${delta} l ${launch} ${kicksubname} ${kick} BeamSize ${beamsize} ${mom} ${sigmatname} o ${offset} num ${numevts} quad ${quad} scraping ${scraping} numturns ${numturns} ${beamtype} Particle ${particle} ${charge} ${spintracking} ${edmsizename} ${muondecayname} ${betaxname} ${cleanval}" >> ${subfile}
							    echo "sleep ${sleepnum}" >> ${subfile}
							    continue				
							fi
						    fi
						else
						    nfile=1
						fi

						file1="${datadir}/${outname}/DONE"
						file2="g2MIGTRACE_${outname}.root"	    
						file2="Eff/${beamstart}_${inftype}/${outname}.dat"
						if [ ${beamstart} == "CentralOrbit" ] && [ ${offset} -gt 0 ]; then
						    file2="Eff/${beamstart}_Offset${offset}_${inftype}/${outname}.dat"
						fi						    

						if [ ${runall} == 1 ]; then
						    if [ -a ${file1} ] && [ -a ${file2} ]; then
							echo "#More events seem to be available [${outname}]."
							echo "rm tmp; echo \"### [${njob}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							echo "rm tmp; echo \"### [${nrun} / ${njobsmax}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							echo "${script} ${outname}" >> ${outfile}
							echo "rm tmp; echo \"### [${nrun} / ${njobsmax}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							((nrun++))
							((nrunjobs++))
							if [ ${nrun} -gt ${njobsmax} ]; then
							    ((njob++))
							    echo "elif [ \${doit} == ${njob} ]; then" >> ${outfile}
							    nrun=0
							fi
							continue
						    fi
						fi
						
						
						if [ -a ${file1} ] && [ -a ${file2} ]; then
						    if nt -c ${file1} ${file2}; then
							echo "#More events seem to be available [${outname}]."
							echo "rm tmp; echo \"### [${njob}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							echo "rm tmp; echo \"### [${nrun} / ${njobsmax}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							echo "${script} ${outname}" >> ${outfile}
							echo "rm tmp; echo \"### [${nrun} / ${njobsmax}] ###\" > tmp ; chmod 777 tmp; cat tmp" >> ${outfile}
							((nrun++))
							((nrunjobs++))
							if [ ${nrun} -gt ${njobsmax} ]; then
							    ((njob++))
							    echo "elif [ \${doit} == ${njob} ]; then" >> ${outfile}
							    nrun=0
							fi
							continue
						    else
							((ndonejobs++))
							continue
						    fi
						fi
						
						
						if [ ${subjob} == 1 ]; then
						    if [ -a ${file2} ]; then
		                       	    #
                                            # Check for zero gen
                                            #
							THETA=`tail -n -1 ${file2}`
							if [ -z ${THETA} ]; then
							    ((nsubjobs++))
							    echo "${outname}"
							    echo "Weird...."
							    continue;
							    echo "export submittingjob=1"
							    echo "./submit.sh ${submitname} ${myinf} ${beamstart} ${resubfieldname} d ${delta} l ${launch} ${kicksubname} ${kick} BeamSize ${beamsize} ${mom} ${sigmatname} o ${offset} num ${numevts} quad ${quad} scraping ${scraping} numturns ${numturns} ${beamtype} Particle ${particle} ${charge} ${spintracking} ${edmsizename} ${muondecayname} ${betaxname} ${cleanval}"
							    echo "export submittingjob=1" >> ${subfile}
							    echo "./submit.sh ${submitname} ${myinf} ${beamstart} ${resubfieldname} d ${delta} l ${launch} ${kicksubname} ${kick} BeamSize ${beamsize} ${mom} ${sigmatname} o ${offset} num ${numevts} quad ${quad} scraping ${scraping} numturns ${numturns} ${beamtype} Particle ${particle} ${charge} ${spintracking} ${edmsizename} ${muondecayname} ${betaxname} ${cleanval}" >> ${subfile}
							    echo "sleep ${sleepnum}" >> ${subfile}
							    continue;
							else
							    if [ ${THETA} == "nan" ]; then
								((nsubjobs++))
								echo "No events were generated..."
								echo "### No events were generated..." >> ${subfile}
								echo "${outname}"
								echo "export submittingjob=1"
								echo "./submit.sh ${submitname} ${myinf} ${beamstart} ${resubfieldname} d ${delta} l ${launch} ${kicksubname} ${kick} BeamSize ${beamsize} ${mom} ${sigmatname} o ${offset} num ${numevts} quad ${quad} scraping ${scraping} numturns ${numturns} ${beamtype} Particle ${particle} ${charge} ${spintracking} ${edmsizename} ${muondecayname} ${betaxname} ${cleanval}"
								echo "export submittingjob=1" >> ${subfile}
								echo "./submit.sh ${submitname} ${myinf} ${beamstart} ${resubfieldname} d ${delta} l ${launch} ${kicksubname} ${kick} BeamSize ${beamsize} ${mom} ${sigmatname} o ${offset} num ${numevts} quad ${quad} scraping ${scraping} numturns ${numturns} ${beamtype} Particle ${particle} ${charge} ${spintracking} ${edmsizename} ${muondecayname} ${betaxname} ${cleanval}" >> ${subfile}
								echo "sleep ${sleepnum}" >> ${subfile}
								continue;
							    fi
							fi
						    fi
						fi

						echo ${outname}
						echo ${filecheck}
						ls ${filecheck}
						ls ${datadir}/${outname}
						echo ${nfile}
						sleep 5000
						((nnocatjobs++))
						done
					    done
					done
				    done
				done
			    done
			done
		    done
		done
		done
	    done
	done
    done

    echo "fi" >> ${outfile}
    
    
    if [ -a ${outfile} ]; then
	chmod 777 ${outfile}
    fi
    if [ -a ${subfile} ]; then
	chmod 777 ${subfile}
    fi

    if [ -a ${subfile} ]; then
	echo "Number of jobs to submit"
	grep "submit.sh " ${subfile} | wc -l
    fi

    if [ -a ${outfile} ]; then
	echo "Number of finished jobs"
	grep "run.sh " ${outfile} | wc -l
    fi

    if [ -a ${subfile} ]; then
	echo "./${subfile}"
    fi
    echo "./${outfile}"
fi
