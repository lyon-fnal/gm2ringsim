#!/bin/sh

fixfooter() {
    footer_fcl=${1}

    if [ -z ${2} ]; then
	return;
    fi
    if [ ${2} == "vp1" ] || [ ${2} == "vp" ] || [ ${2} == "novp1" ] || [ ${2} == "novp" ] || [ ${2} == "debug" ] || [ ${2} == "nodebug" ] || [ ${2} == "ring" ] || [ ${2} == "noring" ]; then	
	if [ ${2} == "ring" ]; then
	    sed "s/SaveRingHits: false/SaveRingHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "noring" ]; then
	    sed "s/SaveRingHits: true/SaveRingHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "vp" ]; then
	    sed "s/SaveVRingHits: false/SaveVRingHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	    sed "s/SaveVRing1PlaneHits: false/SaveVRing1PlaneHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "novp" ]; then
	    sed "s/SaveVRingHits: true/SaveVRingHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	    mv ${footer_fcl}.tmp ${footer_fcl}
	    sed "s/SaveVRing1PlaneHits: true/SaveVRing1PlaneHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "vp1" ]; then
	    sed "s/SaveVRing1PlaneHits: false/SaveVRing1PlaneHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "novp1" ]; then
	    sed "s/SaveVRing1PlaneHits: true/SaveVRing1PlaneHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi
	if [ ${2} == "debug" ]; then
	    sed "s/debug: false/debug: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi	    
	if [ ${2} == "nodebug" ]; then
	    sed "s/debug: true/debug: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi	    
	mv ${footer_fcl}.tmp ${footer_fcl}
    fi
}

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

echo "Input=${input}"
echo "Subdir=${subdir}"



output="/gm2/data/users/tgadfort/gm2ringsim/output/"
#ls ${output}/*${input}*/output*.root > input.dat
#echo "==============================================="
#wc -l input.dat
#echo "Found this many output files in the output directory."
echo "==============================================="
echo""



runplot=0
numevts=-1
suffix=""
local=1
#rm input.dat
if [ -z ${2} ]; then
    ls ${output}/*${input}*/*${suffix}*.root > input.dat
else
    if [ ${2} == "sub" ]; then
	local=0
    elif [ ${2} == "plot" ]; then
	runplot=1
	if ! [ -z ${3} ]; then
	    echo "${3}" > plotcmd
	fi
    elif [ ${2} == "add" ] || [ ${2} == "merge" ]; then
	runplot=1
	echo "Merging files into single ${input} file."
	hadd -f rootfiles/${subdir}/gm2ringsim_${input}.root `ls rootfiles/${subdir}/gm2ringsim_${input}_*.root`
	echo "Done..."
	exit;
	echo "Hit Control-X"
#	sleep 10000000
    elif [ ${2} == "onlyplot" ]; then
	runplot=2
	ls ${output}/*${input}*/*${suffix}*.root > input.dat
    elif [ ${2} == "html" ]; then
	runplot=3
    elif [ ${2} == "frac" ]; then
	extra_suffix=${3}
	if [ ${extra_suffix} == "0-1" ]; then
	    extra_suffix="_v0_1"
	    echo ${extra_suffix}
	    ls ${output}/*${input}/*${suffix}_*0.root > input.dat
	    ls ${output}/*${input}/*${suffix}_*1.root >> input.dat
	elif [ ${extra_suffix} == "0-2" ]; then
	    extra_suffix="_v0_2"
	    echo ${extra_suffix}
	    ls ${output}/*${input}/*${suffix}_*0.root > input.dat
	    ls ${output}/*${input}/*${suffix}_*1.root >> input.dat
	    ls ${output}/*${input}/*${suffix}_*2.root >> input.dat
	elif [ ${extra_suffix} == "0-3" ]; then
	    extra_suffix="_v0_3"
	    echo ${extra_suffix}
	    ls ${output}/*${input}/*${suffix}_*0.root > input.dat
	    ls ${output}/*${input}/*${suffix}_*1.root >> input.dat
	    ls ${output}/*${input}/*${suffix}_*2.root >> input.dat
	    ls ${output}/*${input}/*${suffix}_*3.root >> input.dat
	elif [ ${extra_suffix} == "0-4" ]; then
	    extra_suffix="_v0_4"
	    echo ${extra_suffix}
	    ls ${output}/*${input}/*${suffix}_*0.root > input.dat
	    ls ${output}/*${input}/*${suffix}_*1.root >> input.dat
	    ls ${output}/*${input}/*${suffix}_*2.root >> input.dat
	    ls ${output}/*${input}/*${suffix}_*3.root >> input.dat
	    ls ${output}/*${input}/*${suffix}_*4.root >> input.dat
	else
	    echo "Extra Suffix = ${extra_suffix}"
	    if [ ${extra_suffix} -ge 0 ]; then
		echo "  <0"
		ls ${output}/*${input}/*${suffix}*0_${extra_suffix}.root > input.dat
		ls ${output}/*${input}/*${suffix}*2_${extra_suffix}.root >> input.dat
		ls ${output}/*${input}/*${suffix}*4_${extra_suffix}.root >> input.dat
		ls ${output}/*${input}/*${suffix}*6_${extra_suffix}.root >> input.dat
		ls ${output}/*${input}/*${suffix}*8_${extra_suffix}.root >> input.dat
	    else
		echo "  >0"
		loc_xsuffix=`echo " ${extra_suffix} * -1" | bc`
		ls ${output}/*${input}/*${suffix}*1_${loc_xsuffix}.root > input.dat
		ls ${output}/*${input}/*${suffix}*3_${loc_xsuffix}.root >> input.dat
		ls ${output}/*${input}/*${suffix}*5_${loc_xsuffix}.root >> input.dat
		ls ${output}/*${input}/*${suffix}*7_${loc_xsuffix}.root >> input.dat
		ls ${output}/*${input}/*${suffix}*9_${loc_xsuffix}.root >> input.dat
	    fi
	fi

	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
	fixfooter ${footer_fcl} ${4}
	if ! [ -z ${4} ]; then
	    if [ ${4} == "sub" ]; then
		local=0
	    elif [ ${4} == "n" ]; then
		numevts=${5}
	    fi
	fi
	echo "Found the following:"
	wc -l input.dat
	echo ""
#	sleep 100000
	echo "checking..."
	root -l checkfile.C -b -q
	echo "Hit Control-X"
	chmod 777 rmfiles.sh
	wc -l rmfiles.sh
	./rmfiles.sh
    elif [ ${2} == "n" ]; then
	numevts=${3}
	echo "Running over ${numevts} events."
	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
	fixfooter ${footer_fcl} ${3}	
	ls ${output}/*${input}/*${suffix}*.root > input.dat
    elif [ ${2} == "vp1" ] || [ ${2} == "vp" ] || [ ${2} == "novp1" ] || [ ${2} == "novp" ] || [ ${2} == "debug" ] || [ ${2} == "nodebug" ] || [ ${2} == "ring" ] || [ ${2} == "noring" ]; then	
	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
	fixfooter ${footer_fcl} ${2}
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
#cat input.dat
if [ -a input.dat ]; then
    nfiles=`wc -l input.dat | awk '{print $1}'`
fi
update=0

if ! [ -z ${extra_suffix} ]; then
    if [ ${extra_suffix} == 0 ]; then
	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
	fixfooter ${footer_fcl} "vp"
    fi
fi

if ! [ -z ${2} ]; then
    
    if [ ${2} == "split" ]; then
	files=`cat input.dat`
    fi

    if [ ${2} == "check" ]; then
	echo "checking..."
	root -l checkfile.C -b -q
	echo "Hit Control-X"
	chmod 777 rmfiles.sh
	wc -l rmfiles.sh
	./rmfiles.sh
	exit;
	sleep 1000000
    fi
fi



inputs=""
if [ -a input.dat ]; then
    inputfiles=`cat input.dat`
else
    inputfiles=""
fi
#cat ${inputfiles}
n=1

#
# Create INPUT_DIR_ARRAY
#
INPUT_DIR_ARRAY=""
for file in ${inputfiles}; do
    if [ -a ${file} ]; then
	INPUT_DIR_ARRAY="-f ${file} ${INPUT_DIR_ARRAY}"
	if [ ${n} == 1 ]; then
	    inputs="\"${file}\""
	else
	    inputs="${inputs}, \"${file}\""
	fi
	((n++))
    fi
done

#
# Make Footer
#
#


footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
size=`wc -l ${footer_fcl} | awk '{print $1}'`
if [ ${size} == -1 ]; then
cat >> ${footer_fcl} <<EOF
}

physics: {
  analyzers: {
    readRingTrackers: {
      module_type: ringTrackerAnalyzer
      hist_dir: ""
      tree_dir: ""
      beamstart: "CentralOrbit"
      basename: "${input}"
      maxturns: 200
      LaunchAngle: 0
      InflectorAngle: 0
      StorageOffset: 0
      SaveInfHits: false
      SaveTruthHits: true
      SaveRingHits: false
      SaveVRingHits: false
      SaveVRing1PlaneHits: false
      debug: false
    }
  }
  path1: [ readRingTrackers ]
  end_paths: [ path1 ]
}
EOF
fi

files_fcl=`ls ${output}/*${input}*/files_reader.fcl`
header_fcl=`ls ${output}/*${input}*/header_reader.fcl`
footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`

#echo "Footer"
#cat ${footer_fcl}
#echo "Files"
#cat ${files_fcl}
#sleep 10000
#header_fcl=`ls ${output}/*${input}*/header_truthreader.fcl`
#footer_fcl=`ls ${output}/*${input}*/footer_truthreader.fcl`
fcl="${output}/${input}/reader.fcl"
if [ -a ${fcl} ]; then
    rm ${fcl}
fi
echo "  fileNames: [ ${inputs} ]" > ${files_fcl}
cat ${header_fcl} >> ${fcl}
cat ${files_fcl} >> ${fcl}
cat ${footer_fcl} >> ${fcl}

basedir=""


#
# Check for Eff/rootfile
#
if [ -z ${extra_suffix} ]; then
    outfile="rootfiles/${subdir}/gm2ringsim_${input}.root"
else
    outfile="rootfiles/${subdir}/gm2ringsim_${input}_${extra_suffix}.root"
fi
  
if [ -a ${outfile} ]; then
    fsize=`ls -l ${outfile} | awk '{print $5}'`
    fhsize=`ls -lh ${outfile} | awk '{print $5}'`
    echo "Found output file w/ size=${fhsize}"
    nfiles=1
else
    fsize=-1
    fhsize=-1
    echo "No output file exists."
    if [ -a input.dat ]; then
	nfiles=`wc -l input.dat | awk '{print $1}'`
    fi
fi

echo "Pre:  Runplot = ${runplot}  ; Filesize = ${fhsize}  ; nfiles=${nfiles}"
if [ ${runplot} == 2 ] && [ ${fsize} -lt 1000000 ]; then
    runplot=1
elif [ ${runplot} == 1 ] && [ ${fsize} -gt 1000000 ]; then
    runplot=2
fi
echo "Post: Runplot = ${runplot}  ; Filesize = ${fhsize}  ; nfiles=${nfiles}"
#sleep 10000



#
# Check that file is up-to-date
#
fcl=`ls ${output}/*${input}*/reader.fcl`
file1=`ls ${output}/*${input}*/reader.fcl`
if ! [ -z ${extra_suffix} ]; then
    file2=rootfiles/${subdir}/gm2ringsim_${input}_${extra_suffix}.root
else
    file2=rootfiles/${subdir}/gm2ringsim_${input}.root
fi
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


echo "Post: Runplot = ${runplot}  ; Filesize = ${fhsize}  ; nfiles=${nfiles}"


#numevts=10000
if [ ${runplot} == 2 ]; then
    ./plotinf.sh ${input} ${update}
elif [ ${runplot} == 3 ]; then
    ./plotinf.sh ${input} html
else
    if [ ${nfiles} -gt 0 ]; then
	if [ ${runplot} == 1 ]; then
	    ./plotinf.sh ${input} ${update}
#	    ./copy.sh
	else
	    if [ ${local} == 1 ]; then
		if ! [ -z ${extra_suffix} ]; then
		    xsuffix="_${extra_suffix}"
		else
		    xsuffix=""
		fi

		if [ ${numevts} -gt 0 ]; then
		    gm2 -c ${fcl} -n ${numevts} -T rootfiles/${subdir}/gm2ringsim_${input}${xsuffix}.root
		    echo "File Location:"
		    echo "root -l rootfiles/${subdir}/gm2ringsim_${input}${xsuffix}.root"
		    echo ""
		else
		    gm2 -c ${fcl} -T rootfiles/${subdir}/gm2ringsim_${input}${xsuffix}.root
		    echo "File Location:"
		    echo "root -l rootfiles/${subdir}/gm2ringsim_${input}${xsuffix}.root"
		    echo ""
		fi
	    else	       
		outdir="/gm2/data/users/tgadfort/gm2ringsim/output/processed"
		export fullDir=${input}
		export outdir=${outdir}
		export extra_suffix=${extra_suffix}
		export fcl=${fcl}
		cp input.dat ${outdir}/${input}_${extra_suffix}.dat
		cp ${fcl} ${outdir}/${input}_${extra_suffix}.fcl
#		jobsub -g --opportunistic -e fcl -e fullDir -e extra_suffix -e outdir -dMYDIR ${outdir} proc.sh
		
		jobsub -g --opportunistic -e fcl -e fullDir -e extra_suffix -e outdir -dMYDIR ${outdir} ${INPUT_DIR_ARRAY} reproc.sh
	    
#		gm2 -c ${fcl} -n ${numevts} -T rootfiles/${subdir}/gm2ringsim_${input}_${extra_suffix}.root
	    fi
	fi
    fi
fi

if [ -a name.dat ]; then
    rm name.dat
fi

if [ -a input.dat ]; then
    rm input.dat
fi
