#!/bin/sh


moms="PerfectMatch_dP001 PerfectMatch_dP005 PerfectMatch PerfectMatch_dP05"
moms="PerfectMatch_dP001 PerfectMatch PerfectMatch_dP005 PerfectMatch_dP05"

#moms="PerfectMatch_dP001"
#moms="PerfectMatch_dP005"
#moms="PerfectMatch_dP05"
#moms="PerfectMatch PerfectMatch_Flat"

#moms="PerfectMatch_FlatdP001 PerfectMatch_FlatdP005 PerfectMatch_Flat PerfectMatch_FlatdP05"
#moms="PerfectMatch_Flat PerfectMatch_FlatdP05"
#moms="PerfectMatch_FlatdP001 PerfectMatch_FlatdP005"
#moms="PerfectMatch_FlatdP05"

sigmats="1 25 50 -1 -25"
#sigmats="25 -25"
#sigmats="1 50 -1"
#sigmats="25 50"
#sigmats="50 -1 -25"
#sigmats="1"

bss="0 40"
#bss="40"
#bss="0"

spin="NoSpinTracking"
#spin="SpinTracking"
if [ ${spin} == "SpinTracking" ]; then
    moms="PerfectMatch PerfectMatch_Flat"
    bss="0 40"
    bss="0"
    sigmats="1 25 50 -1 -25"
    sigmats="25 -25"
fi

ringvp=0
merge=0
run=0
check=0
missing=0
plot=0
fr=0

if [ -z ${1} ]; then
    echo "Need input."
    exit;
else
    nums="0 1 2 3 4 5 6 7 8 9"
    for num in ${nums}; do
	if [ ${1} == ${num} ]; then
	    run=1
	    frac=${num}
	    if [ ${frac} == 0 ]; then
		ringvp=1
	    else
		ringvp=0
	    fi
	fi
    done

    if [ ${1} == "run" ]; then
	frac=${2}
	run=1
	if [ ${frac} == 0 ]; then
	    ringvp=1
	else
	    ringvp=0
	fi
    fi

    if [ ${1} == "check" ]; then
	check=1
    fi

    if [ ${1} == "missing" ]; then
	missing=1
	frac=0
	if ! [ -z ${2} ]; then
	    for num in ${nums}; do
		if [ ${2} == ${num} ]; then
		    frac=${2}
		fi
	    done
	fi
    fi

    if [ ${1} == "merge" ]; then
	merge=1
	moms="PerfectMatch_FlatdP001 PerfectMatch_FlatdP005 PerfectMatch_Flat PerfectMatch_FlatdP05"
	sigmats="1 25 50 -1 -25"
	bss="0 40"
    fi

    if [ ${1} == "plot" ]; then
	plot=1
    fi

    if [ ${1} == "fr" ]; then
	fr=1
	moms="PerfectMatch_dP001 PerfectMatch PerfectMatch_dP005 PerfectMatch_dP05"
	sigmats="1 25 50 -1 -25"
	bss="0 40"
    fi
fi

dir="/gm2/data/users/tgadfort/gm2ringsim/output"
for mom in ${moms}; do
    for sigmat in ${sigmats}; do
	for bs in ${bss}; do
	if [ ${sigmat} -ge 0 ]; then
	    tname="tSigma${sigmat}"
	else
	    loc_sigmat=`echo " ${sigmat} * -1" | bc`
	    tname="GausstSigma${loc_sigmat}"
	fi

	base="ScrapingOFF_NewSupports_200Turns_muplus_${spin}_CentralOrbit_${tname}_NoKick_InflectorOpen_NoDelta_NoLaunch_BeamSize${bs}pimmmrad_UniformInflectorGun_${mom}_CollimatorON_StandardModelMuonDecay_FlatDecayTime"

	if [ ${missing} == 1 ]; then
	    rootfile="rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${base}_${frac}.root"
	    if [ -a ${rootfile} ]; then
		ok=1
		#ls -lh ${rootfile}
	    else
		echo "./runner.sh bs=${bs} frac=${frac} sigmat=${sigmat} mom=${mom}"
	    fi
	    continue;
	fi


	if [ ${ringvp} == 0 ]; then
	    footer_fcl=`ls ${dir}/${base}/footer_reader.fcl`
	    sed "s/SaveRingHits: true/SaveRingHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	    
	    sed "s/SaveVRingHits: true/SaveVRingHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	    sed "s/SaveVRing1PlaneHits: true/SaveVRing1PlaneHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	else
	    footer_fcl=`ls ${dir}/${base}/footer_reader.fcl`
	    sed "s/SaveRingHits: true/SaveRingHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	    
	    sed "s/SaveVRingHits: false/SaveVRingHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	    sed "s/SaveVRing1PlaneHits: true/SaveVRing1PlaneHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	fi

	if [ ${check} == 1 ]; then
	    ./run.sh ${base} check
	fi

	if [ ${run} == 1 ]; then
	    ./run.sh ${base} check
	    ./run.sh ${base} frac ${frac}
	fi

	if [ ${merge} == 1 ]; then
	    ./run.sh ${base} merge
	fi

	if [ ${plot} == 1 ]; then
	    ./run.sh ${base} plot
	fi

	if [ ${fr} == 1 ]; then
	    newname=${base/"NewSupports_200Turns_muplus_NoSpinTracking_CentralOrbit_"/}
	    newname2=${newname/"NoKick_InflectorOpen_NoDelta_NoLaunch_"/}
	    newname3=${newname2/"UniformInflectorGun_"/}
	    newname4=${newname3/"CollimatorON_"/}
	    newbase=${newname4}
	    nrads="5 15 25 51"
	    lambs="0 0.01 0.1 1 10"
	    for nrad in ${nrads}; do
		for lamb in ${lambs}; do
		    echo "./gen.sh run 150 1 ${nrad} ${lamb} ${base}"
		    if [ -a rootfiles/fastrotation/${newbase}.root ]; then
			ok=1;
#			./gen.sh run 150 1 ${nrad} ${lamb} ${newbase}
#			if [ -a rootfiles/fastrotation/${newbase}_Delta150ns_t01ns_NRad${nrad}_Lamb${lamb}.root ]; then
#			    ./plotfast.sh rootfiles/fastrotation/${newbase}_Delta150ns_t01ns_NRad${nrad}_Lamb${lamb}.root
#			fi
		    else
			echo "Missing ${newbase}"
		    fi
		done
	    done
	fi
    done
done
done