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
if [ -z ${2} ]; then
    ls ${output}/*${input}*/*.root > input.dat
else
    if [ ${2} == "plot" ]; then
	runplot=1
	ls ${output}/*${input}*/*.root > input.dat
    elif [ ${2} == "onlyplot" ]; then
	runplot=2
	ls ${output}/*${input}*/*.root > input.dat
    elif [ ${2} == "html" ]; then
	runplot=3
	ls ${output}/*${input}*/*.root > input.dat
    else
	ls ${output}/*${input}/*.root > input.dat
    fi
fi
#ls myfiles/*.root > input.dat
echo "${input}" > name.dat
cat input.dat
nfiles=`wc -l input.dat | awk '{print $1}'`
update=0

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
	    if [ ${runplot} == 2 ] || [ ${runplot} == 3 ]; then
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
	    gm2 -c ${fcl}
#	    root -l runner.C -b -q
	fi
    fi
fi
rm name.dat
rm input.dat
