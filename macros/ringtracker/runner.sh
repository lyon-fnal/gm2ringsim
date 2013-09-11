#!/bin/sh

allmoms="PerfectMatch_dP001 PerfectMatch PerfectMatch_dP005 PerfectMatch_dP05 PerfectMatch_FlatdP001 PerfectMatch_FlatdP005 PerfectMatch_Flat PerfectMatch_FlatdP05"
moms="PerfectMatch_dP001 PerfectMatch_dP005 PerfectMatch PerfectMatch_dP05"
moms="PerfectMatch_dP001 PerfectMatch PerfectMatch_dP005 PerfectMatch_dP05"

#moms="PerfectMatch_dP001"
#moms="PerfectMatch_dP005"
#moms="PerfectMatch"
moms="PerfectMatch PerfectMatch_Flat"

#moms="PerfectMatch_FlatdP001 PerfectMatch_FlatdP005 PerfectMatch_Flat PerfectMatch_FlatdP05"
#moms="PerfectMatch_Flat PerfectMatch_FlatdP05"
#moms="PerfectMatch_FlatdP001 PerfectMatch_FlatdP005"
#moms="PerfectMatch_FlatdP05"

sigmats="1 25 50 -1 -25"
#sigmats="25 -25"
#sigmats="1 50 -1"
sigmats="25 -25"
#sigmats="50 -1 -25"
#sigmats="1"

bss="0 40"
#bss="40"
bss="0"

PulseWidth=150
Tbin=2
Nturns=150

fracs="xx"

spin="NoSpinTracking"
spin="SpinTracking"
if [ ${spin} == "SpinTracking" ]; then
    hist="Positron"
    type=1
    if [ ${type} == 1 ]; then
	moms="PerfectMatch"
	bss="0 40"
	bss="0"
	sigmats="100 0 25 -25 50 1 -5 -15"
	sigmats="0"
    else
	moms=""
	moms="${moms} PerfectMatch"
	moms="${moms} PerfectMatch_dP05 PerfectMatch_FlatdP05"
	bss="0 40"
	bss="0"
	sigmats="0"
    fi
fi

if [ ${spin} == "NoSpinTracking" ]; then
    hist="Muon"
    type=1
    if [ ${type} == 1 ]; then
	moms="PerfectMatch"
	bss="0 40"
	bss="0"
	sigmats="100 0 -5 -15 25 50 1"
    else
	moms=""
#	moms="${moms} PerfectMatch_Flat"
	moms="${moms} PerfectMatch_dP05 PerfectMatch_FlatdP05"
	bss="0 40"
	bss="0"
	sigmats="0"
	sigmats="100"
    fi
fi



ringvp=0
merge=0
mergeall=0
run=0
check=0
missing=0
missingdir=0
plot=0
fr=0
plotfr=0

if [ -z ${1} ]; then
    echo "Need input."
    exit;
else
    nums="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10 -11 -12 -13 -14 -15 -16 -17 -18 -19"
    for num in ${nums}; do
	if [ ${1} == ${num} ]; then
	    run=1
	    fracs=${num}
	    if [ ${fracs} == 0 ]; then
		ringvp=1
	    else
		ringvp=0
	    fi
	fi
    done

    for n1 in ${nums}; do
	for n2 in ${nums}; do
	    if [ ${n2} -le ${n1} ]; then
		continue;
	    fi
	    if [ ${1} == "${n1}-${n2}" ]; then
		echo "HI"
		let n=${n1}
		fracs=""
		while [ ${n} -le ${n2} ]; do
		    fracs="${fracs} ${n}"
		    ((n++))
		done
		run=1
		break;
	    fi    
	done
    done


    if [ ${1} == "run" ]; then
	frac=${2}
	run=1
	if [ ${frac} == 0 ]; then
	    ringvp=0
	else
	    ringvp=0
	fi
    fi

    if [ ${1} == "check" ]; then
	check=1
    fi

    if [ ${1} == "missingdir" ]; then
	missingdir=1
    fi

    if [ ${1} == "missing" ]; then
	missing=1
	if ! [ -z ${2} ]; then
	    for num in ${nums}; do
		if [ ${2} == ${num} ]; then
		    frac=${2}
		fi
	    done
	    if [ ${2} == "x" ]; then
		frac="x"
	    fi
	fi
    fi

    if [ ${1} == "merge" ]; then
	merge=1
    fi

    if [ ${1} == "mergeall" ]; then
	mergeall=1
    fi

    if [ ${1} == "plot" ]; then
	plot=1
    fi

    if [ ${1} == "fr" ] || [ ${1} == "plotfr" ] || [ ${1} == "checkfr" ]; then
	fr=1
	if [ ${1} == "plotfr" ]; then
	    fr=0
	fi

	if [ ${1} == "checkfr" ]; then
	    moms=""
	    moms="${moms} PerfectMatch PerfectMatch_Flat"
	    moms="${moms} PerfectMatch_dP05 PerfectMatch_FlatdP05"
	    moms="${moms} PerfectMatch_dP005 PerfectMatch_FlatdP005"
	    moms="${moms} PerfectMatch_dP001 PerfectMatch_FlatdP001"
	    moms="PerfectMatch_dP05"
	    sigmats="1 25 -25 50 100 0"
	    sigmats="100"
	    nrads="15 51 101 201"
	    bss="0"
	    lambs="0"
	    for mom in ${moms}; do
		for sigmat in ${sigmats}; do
		    if [ ${sigmat} -gt 0 ]; then
			tname="tSigma${sigmat}"
		    elif [ ${sigmat} -lt 0 ]; then
			loc_sigmat=`echo " ${sigmat} * -1" | bc`
			tname="GausstSigma${loc_sigmat}"
		    else
			tname="FNALt0"
		    fi
		    for bs in ${bss}; do
			tmpcheck=0
			for nrad in ${nrads}; do
			    for lamb in ${lambs}; do				
				if [ ${tmpcheck} == 1 ]; then
				    continue;
				fi
				base="ScrapingOFF_NewSupports_${Nturns}Turns_muplus_${spin}_CentralOrbit_${tname}_NoKick_InflectorOpen_NoDelta_NoLaunch_BeamSize${bs}pimmmrad_UniformInflectorGun_${mom}_CollimatorON_StandardModelMuonDecay_FlatDecayTime"
				newname=${base/"NewSupports_${Nturns}Turns_muplus_"/}
				newname2=${newname/"CentralOrbit_"/}
				newname3=${newname2/"NoKick_InflectorOpen_NoDelta_NoLaunch_"/}
				newname4=${newname3/"UniformInflectorGun_"/}
				newname5=${newname4/"CollimatorON_"/}
				newbase=${newname5}
				rootfile="rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root"
				if ! [ -a ${rootfile} ]; then
				    echo "./runner.sh fr ${newbase}"
				    tmpcheck=1
				    break;
				fi
			    done
			done
		    done
		done
	    done
	    echo "Hit Control-X"
	    sleep 1000000000
	fi

	nrads="15 51 101 201"
	nrads="201"
	nrads="7 15 51 101 201"
	nrads="15 51"
	nrads="101 201"
	lambs="0"
	checkfr=0
	stop=0
    
	if ! [ -z ${2} ]; then
	    if [ ${1} == "plotfr" ]; then
		nrads="15 51 101 201"
		nrads="15"
		for nrad in ${nrads}; do
		    for lamb in ${lambs}; do
			./plotfast.sh rootfiles/fastrotation/${2}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root
		    done
		done
		echo "Hit Control-X"
		sleep 1000000000
	    fi
	    
	    if [ ${1} == "fr" ]; then
		nrads="15 51 101 201"
		nrads="15"
		for nrad in ${nrads}; do
		    for lamb in ${lambs}; do
			echo "./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${2} ${hist}"
			./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${2} ${hist}
			./plotfast.sh rootfiles/fastrotation/${2}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root
		    done
		done
		echo "Hit Control-X"
		sleep 1000000000
	    fi
	fi

	for mom in ${moms}; do
	    for sigmat in ${sigmats}; do
	if [ ${sigmat} -gt 0 ]; then
	    tname="tSigma${sigmat}"
	elif [ ${sigmat} -lt 0 ]; then
	    loc_sigmat=`echo " ${sigmat} * -1" | bc`
	    tname="GausstSigma${loc_sigmat}"
	else
	    tname="FNALt0"
	fi
		for bs in ${bss}; do
		    for nrad in ${nrads}; do
			for lamb in ${lambs}; do
			    if ! [ -z ${2} ]; then
				stop=1
				if [ ${fr} == 1 ]; then
				    echo "./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${2} ${hist}"
				    ./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${2} ${hist}
				fi
				./plotfast.sh rootfiles/fastrotation/${2}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root
			    else
				base="ScrapingOFF_NewSupports_${Nturns}Turns_muplus_${spin}_CentralOrbit_${tname}_NoKick_InflectorOpen_NoDelta_NoLaunch_BeamSize${bs}pimmmrad_UniformInflectorGun_${mom}_CollimatorON_StandardModelMuonDecay_FlatDecayTime"
				echo ${base}
				newname=${base/"NewSupports_${Nturns}Turns_muplus_"/}
				newname2=${newname/"CentralOrbit_"/}
				newname3=${newname2/"NoKick_InflectorOpen_NoDelta_NoLaunch_"/}
				newname4=${newname3/"UniformInflectorGun_"/}
				newname5=${newname4/"CollimatorON_"/}
				newbase=${newname5}
				if [ ${fr} == 1 ]; then
				    echo "./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${newbase} ${hist}"
				    ./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${newbase} ${hist}
				fi
				./plotfast.sh rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root		      	
				checkfr=1
			    fi
			done
		    done
		done
	    done
	done
	if [ ${checkfr} == 0 ] || [ ${stop} == 1 ]; then
	    echo "Hit Control-X"
	    sleep 1000000000
	fi

	redo=${2}
	redo="0"
#	moms=""
#	moms="${moms} PerfectMatch PerfectMatch_Flat"
#	moms="${moms} PerfectMatch_dP05 PerfectMatch_FlatdP05"
#	moms="${moms} PerfectMatch_dP005 PerfectMatch_FlatdP005"
#	moms="${moms} PerfectMatch_dP001 PerfectMatch_FlatdP001"
#	bss="0"
#	sigmats="50 1 25 -25"
#	sigmats="100 50 1 25 -25"
#	sigamts="100 0"
#	sigmats="100"
    fi
fi

if [ ${fracs} == "x" ]; then
    fracs="0 1 2 3 4 5 6 7 8 9"
    fracss="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10 -11 -12 -13 -14 -15 -16 -17 -18 -19"
fi
   

dir="/gm2/data/users/tgadfort/gm2ringsim/output"
for mom in ${moms}; do
    for sigmat in ${sigmats}; do
	for bs in ${bss}; do
	if [ ${sigmat} -gt 0 ]; then
	    tname="tSigma${sigmat}"
	elif [ ${sigmat} -lt 0 ]; then
	    loc_sigmat=`echo " ${sigmat} * -1" | bc`
	    tname="GausstSigma${loc_sigmat}"
	else
	    tname="FNALt0"
	fi

	base="ScrapingOFF_NewSupports_${Nturns}Turns_muplus_${spin}_CentralOrbit_${tname}_NoKick_InflectorOpen_NoDelta_NoLaunch_BeamSize${bs}pimmmrad_UniformInflectorGun_${mom}_CollimatorON_StandardModelMuonDecay_FlatDecayTime"
	prefixnospin="ScrapingOFF_NewSupports_${Nturns}Turns_muplus"
	basenospin="CentralOrbit_${tname}_NoKick_InflectorOpen_NoDelta_NoLaunch_BeamSize${bs}pimmmrad_UniformInflectorGun_${mom}_CollimatorON_StandardModelMuonDecay_FlatDecayTime"

	if [ ${missingdir} == 1 ]; then
	    echo "Checking ${base}"
	    if [ -d /gm2/data/users/tgadfort/gm2ringsim/output/${base} ]; then
		let nf=0
		for file in rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${base}_*.root; do
		    if [ -a ${file} ]; then
			((nf++))
		    fi
		done
		if [ ${nf} == 0 ]; then
		    echo "./run.sh ${base} frac 9"
		else
		    mergedfile="rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${base}.root"
		    if ! [ -a ${mergedfile} ]; then
			echo "./run.sh ${base} merge"
		    else
			newname=${base/"NewSupports_${Nturns}Turns_muplus_"/}
			newname2=${newname/"CentralOrbit_"/}
			newname3=${newname2/"NoKick_InflectorOpen_NoDelta_NoLaunch_"/}
			newname4=${newname3/"UniformInflectorGun_"/}
			newname5=${newname4/"CollimatorON_"/}
			newbase=${newname5}
			formatfile="rootfiles/fastrotation/${newbase}.root"
			if ! [ -a ${formatfile} ]; then
			    echo "You need to format ${formatfile}"
			fi
		    fi
		fi
	    fi	  
	fi
  
#	echo ${mom} ${sigmat} ${bs}
	if [ ${missing} == 1 ]; then
	    if [ ${frac} == "x" ]; then
		fracs="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10 -11 -12 -13 -14 -15 -16 -17 -18 -19"
#		fracs="0 1 2 3 4 5 6 7 8 9"
	    else
		fracs="${frac}"
	    fi
	    if [ -d /gm2/data/users/tgadfort/gm2ringsim/output/${base} ]; then
		for myfrac in ${fracs}; do
		    rootfile="rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${base}_${myfrac}.root"
		    if [ -a ${rootfile} ]; then
			fsize=`ls -l ${rootfile} | awk '{print $5}'`
			if [ ${fsize} -lt 1000000 ] && [ ${fsize} -gt 1000 ]; then
			    echo -n "#"
			    ls -lh ${rootfile}
			    echo "./run.sh ${base} frac ${myfrac}"
			fi
		    else
			echo "./run.sh ${base} frac ${myfrac}"
#		    echo "./runner.sh spin=${spin} bs=${bs} frac=${frac} sigmat=${sigmat} mom=${mom}"
		    fi
		done
	    fi
	    continue;
	fi


	if [ ${run} == 1 ]; then
	    if ! [ -d ${dir}/${base} ]; then
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
	fi

	if [ ${check} == 1 ]; then
	    ./run.sh ${base} check
	fi

	if [ ${run} == 1 ]; then
	    ./run.sh ${base} check
	    for myfrac in ${fracs}; do 
		./run.sh ${base} frac ${myfrac}
	    done
	fi

	if [ ${merge} == 1 ]; then
	    if [ -d /gm2/data/users/tgadfort/gm2ringsim/output/${base} ]; then
		mergedfile="rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${base}.root"
		if [ -a ${mergedfile} ]; then
		    echo "Merged file already exists."
		    ./run.sh ${base} merge
		else
		    let nf=0
		    for file in rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${base}_*.root; do
			if [ -a ${file} ]; then
			    ((nf++))
			fi
		    done
		    if [ ${nf} -gt 0 ]; then
#			hadd -f rootfiles/${subdir}/gm2ringsim_${input}.root `ls rootfiles/${subdir}/gm2ringsim_${input}_*.root`
			./run.sh ${base} merge
		    fi
		fi
	    fi
	fi

	if [ ${mergeall} == 1 ]; then
	    if [ -d /gm2/data/users/tgadfort/gm2ringsim/output/${base} ]; then
		hadd -f rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${base}.root `ls rootfiles/CentralOrbit_InflectorOpen/gm2ringsim_${prefixnospin}*${basenospin}_*.root`
	    fi
	fi

	if [ ${plot} == 1 ]; then
	    ./run.sh ${base} plot
	fi

	if [ ${fr} == 1 ]; then
	    newname=${base/"NewSupports_${Nturns}Turns_muplus_"/}
	    newname2=${newname/"CentralOrbit_"/}
	    newname3=${newname2/"NoKick_InflectorOpen_NoDelta_NoLaunch_"/}
	    newname4=${newname3/"UniformInflectorGun_"/}
	    newname5=${newname4/"CollimatorON_"/}
	    newbase=${newname5}
	    nrads="7 15 51 101"
	    nrads="15 101 201"
	    nrads="15 51 101 201"
#	    nrads="201"
	    lambs="0"
	    for nrad in ${nrads}; do
		for lamb in ${lambs}; do
		    if ! [ -a rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root ]; then
			echo ${newbase}
		    fi

		    if [ ${checkfr} == 1 ]; then
			continue
		    fi
		    
		    if [ -a rootfiles/fastrotation/${newbase}.root ]; then
			ok=1;
			if [ ${redo} == 0 ] && [ -a rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root ]; then
			    if ! [ -d ~/WWW/plots/FastRotation/rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb} ]; then
				./plotfast.sh rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root
			    fi
			    continue;
			fi

			if [ ${redo} == 1 ] || ! [ -a rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root ]; then
			    echo "./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${newbase} ${hist}"
			    ./gen.sh run ${PulseWidth} ${Tbin} ${nrad} ${lamb} ${newbase} ${hist}
			fi

#			sleep 10000
			if [ -a rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root ]; then
			    ok=0
			    ./plotfast.sh rootfiles/fastrotation/${newbase}_Delta${PulseWidth}ns_t0${Tbin}ns_NRad${nrad}_Lamb${lamb}.root
			fi
		    else
			echo "Missing ${newbase}"
		    fi
#		    sleep 1000000
		done
	    done
	fi
    done
done
done