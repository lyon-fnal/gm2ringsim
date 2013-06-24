#!/bin/sh


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


#root -l compile.C -b -q >& compile.out

#ERR=`grep "error" compile.out`
#if [ -z ${ERR} ]; then
#    echo "No errors."
#else
#    echo ""
#    echo ""
#    echo ${ERR}
#    echo ""
#    echo ""
#    sleep 100000
#fi

#rm compile.out

input=${1}
input2=${input/gm2ringsim_/}
input3=${input2/.root/}
input=${input3}



output="/gm2/data/users/tgadfort/gm2ringsim/output/"
runplot=0
numevts=-1
suffix=""
if [ -z ${2} ]; then
    ls ${output}/*${input}*/*${suffix}*.root > input.dat
else
    if [ ${2} == "plot" ]; then
	runplot=1
	ls ${output}/*${input}*/*${suffix}*.root > input.dat
    elif [ ${2} == "onlyplot" ]; then
	runplot=2
	ls ${output}/*${input}*/*${suffix}*.root > input.dat
    elif [ ${2} == "html" ]; then
	runplot=3
	ls ${output}/*${input}*/*${suffix}*.root > input.dat
    elif [ ${2} == "n" ]; then
	numevts=${3}
	echo "Running over ${numevts} events."
	ls ${output}/*${input}/*${suffix}*.root > input.dat
    elif [ ${2} == "vp1" ] || [ ${2} == "vp" ] || [ ${2} == "debug" ] || [ ${2} == "nodebug" ]; then
	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
	if [ ${2} == "vp" ]; then
	    sed "s/SaveVRingHits: false/SaveVRingHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	    sed "s/SaveVRing1PlaneHits: false/SaveVRing1PlaneHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "vp1" ]; then
	    sed "s/SaveVRing1PlaneHits: false/SaveVRing1PlaneHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "debug" ]; then
	    sed "s/debug: false/debug: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi	    
	if [ ${2} == "nodebug" ]; then
	    sed "s/debug: true/debug: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi	    
	mv ${footer_fcl}.tmp ${footer_fcl}
	if ! [ -z ${3} ]; then
	    if [ ${3} == "n" ]; then
		numevts=${4}
		echo "Running over ${numevts} events."
	    fi
	fi
	ls ${output}/*${input}/*${suffix}*.root > input.dat
    elif [ ${2} == "novp1" ]; then
	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
	sed "s/SaveVRing1PlaneHits: true/SaveVRing1PlaneHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	mv ${footer_fcl}.tmp ${footer_fcl}
	if ! [ -z ${3} ]; then
	    if [ ${3} == "n" ]; then
		numevts=${4}
		echo "Running over ${numevts} events."
	    fi
	fi
	ls ${output}/*${input}/*${suffix}*.root > input.dat
    else
	ls ${output}/*${input}/*${suffix}*.root > input.dat
    fi
fi
#ls myfiles/*${suffix}*.root > input.dat
echo "${input}" > name.dat
cat input.dat
nfiles=`wc -l input.dat | awk '{print $1}'`
update=0

inputs=""
inputfiles=`cat input.dat`
n=1
for file in ${inputfiles}; do
    if [ -a ${file} ]; then
	if [ ${n} == 1 ]; then
	    inputs="\"${file}\""
	else
	    inputs="${inputs}, \"${file}\""
	fi
	((n++))
    fi
done



files_fcl=`ls ${output}/*${input}*/files_reader.fcl`
header_fcl=`ls ${output}/*${input}*/header_reader.fcl`
footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
fcl="${output}/${input}/reader.fcl"
if [ -a ${fcl} ]; then
    rm ${fcl}
fi
echo "  fileNames: [ ${inputs} ]" > ${files_fcl}
cat ${header_fcl} >> ${fcl}
cat ${files_fcl} >> ${fcl}
cat ${footer_fcl} >> ${fcl}



beamstarts="UpstreamMandrel UpstreamCryo DownstreamMandrel PerfectStorage CentralOrbit_Offset77 CentralOrbit"
for beamstart in ${beamstarts}; do
    check_beamstart=${input/${beamstart}/}
    if ! [ ${check_beamstart} == ${input} ]; then
	subdir=${beamstart}
	break;
    fi
done

infs="ClosedInflector InflectorOpen PartiallyOpen"
for inf in ${infs}; do
    check_inf=${input/${inf}/}
    if ! [ ${check_inf} == ${input} ]; then
	subdir="${subdir}_${inf}"
    fi
done

echo "Subdir=${subdir}"

basedir=""


    #
    # Check for Eff/rootfile
    #
if [ -a rootfiles/${subdir}/g2MIGTRACE_${input}.root ]; then
    fsize=`ls -l rootfiles/${subdir}/g2MIGTRACE_${input}.root | awk '{print $5}'`
    fhsize=`ls -lh rootfiles/${subdir}/g2MIGTRACE_${input}.root | awk '{print $5}'`
else
    fsize=-1
    fsize=-1
fi

echo ${runplot} ${fhsize}
if [ ${runplot} == 2 ] && [ ${fsize} -lt 1000000 ]; then
    runplot=1
elif [ ${runplot} == 1 ] && [ ${fsize} -gt 1000000 ]; then
    runplot=2
fi
echo ${runplot} ${fhsize}
#sleep 10000



#
# Check that file is up-to-date
#
fcl=`ls ${output}/*${input}*/reader.fcl`
file1=`ls ${output}/*${input}*/reader.fcl`
file2=rootfiles/${subdir}/gm2ringsim_${input}.root
if [ -a ${file2} ]; then
    if [ -a ${file1} ]; then
	if nt -c ${file1} ${file2}; then
	    echo "I found newer root files in [${output}/${input}]. I will regenerate the histogram file."
	    if [ ${runplot} == 2 ]; then
		runplot=1
	    fi
	fi
    fi
fi



if [ ${runplot} == 2 ]; then
    ./plotinf.sh ${input} ${update}
elif [ ${runplot} == 3 ]; then
    ./plotinf.sh ${input} html
else

    if [ ${nfiles} -gt 0 ]; then
	if [ ${runplot} == 1 ]; then
	    ./plotinf.sh ${input} ${update}
	    ./copy.sh
	else
	    if [ ${numevts} -gt 0 ]; then
		gm2 -c ${fcl} -n ${numevts}
	    else
		gm2 -c ${fcl}
	    fi
#	    root -l runner.C -b -q
	fi
    fi
fi
rm name.dat
rm input.dat
