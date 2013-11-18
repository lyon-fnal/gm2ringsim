#!/bin/sh

fixfooter() {
    footer_fcl=${1}
    if ! [ -a ${footer_fcl} ]; then
	return;
    fi

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
	if [ ${2} == "truth" ]; then
	    sed "s/SaveTruthHits: false/SaveTruthHits: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi	    
	if [ ${2} == "notruth" ]; then
	    sed "s/SaveTruthHits: true/SaveTruthHits: false/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi	    
	if [ ${2} == "fill" ]; then
	    sed "s/fill: false/fill: true/g" ${footer_fcl} > ${footer_fcl}.tmp
	fi	    
	if [ ${2} == "nofill" ]; then
	    sed "s/fill: true/fill: false/g" ${footer_fcl} > ${footer_fcl}.tmp
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


maxfiles=150


beamstarts="UM UC DM CO"
for beamstart in ${beamstarts}; do
    check_beamstart=${input/${beamstart}/}
    if ! [ ${check_beamstart} == ${input} ]; then
	subdir=${beamstart}
	break;
    fi
done

infs="InfClosed InfOpen InfPartial"
for inf in ${infs}; do
    check_inf=${input/${inf}/}
    if ! [ ${check_inf} == ${input} ]; then
	subdir="${subdir}_${inf}"
    fi
done

#echo "Input=${input}"
#echo "Subdir=${subdir}"

if [ ${1} == "mv" ] || [ ${1} == "move" ]; then
echo "Moving files from processed."
for beamstart in ${beamstarts}; do
    for inf in ${infs}; do
	for file in /gm2/data/users/tgadfort/gm2ringsim/output/processed/gm2ringsim_*${beamstart}_*${inf}_*.root; do
	    if [ -a ${file} ]; then
		echo "  mv ${file}"
		tmp_subdir="${beamstart}_${inf}"
		mv ${file} rootfiles/${tmp_subdir}/.
	    fi
	done
    done
done
echo "Done moving."
exit;
fi

if [ -z ${1} ]; then
    echo "Leaving."
    exit;
fi

output="/gm2/data/users/tgadfort/gm2ringsim/output/"
if [ -a input.dat ]; then
    rm input.dat
fi
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
    if [ ${2} == "log" ]; then
	prefixs="reproc sub2"
	for prefix in ${prefixs}; do
	    for file in /gm2/data/users/condor-tmp/tgadfort/${prefix}*.cmd; do
		if [ -a ${file} ]; then
		    results=`grep "${input}" ${file}`
		    let nres=0
		    for result in ${results}; do
			((nres++))
		    done
		    if [ ${nres} -gt 0 ]; then
			base=${file/.cmd/}		    
			echo ${base}
			logfiles=`ls ${base}*`
			echo ${logfiles}
			for logfile in ${logfiles}; do
			    if [ -a ${logfile} ]; then
				checkout=${logfile/.out/}
				if [ ${checkout} == ${logfile} ]; then
				    rm ${logfile}
				else
				    if [ ${prefix} == "sub2" ]; then
					mv ${logfile} /gm2/data/users/tgadfort/gm2ringsim/output/processed/${input}.proclogfile
					echo "more /gm2/data/users/tgadfort/gm2ringsim/output/processed/${input}.proclogfile"
				    fi
				    if [ ${prefix} == "reproc" ]; then
					mv ${logfile} /gm2/data/users/tgadfort/gm2ringsim/output/processed/${input}.genlogfile
					echo "more /gm2/data/users/tgadfort/gm2ringsim/output/processed/${input}.genlogfile"
				    fi
				fi
			    fi
			done
		    fi
		fi
	    done
	done
	echo "Stopping."
	echo "Hit Control-X"
	sleep 100000
    elif [ ${2} == "sub" ]; then
	local=0
	ls ${output}/*${input}*/*${suffix}*.root > input.dat
    elif [ ${2} == "plot" ]; then
	runplot=1
	if ! [ -z ${3} ]; then
	    echo "${3}" > plotcmd
	fi
    elif [ ${2} == "add" ] || [ ${2} == "merge" ] || [ ${2} == "mergedel" ]; then
	runplot=1
	echo "Merging files into single ${input} file."
	hadd -f rootfiles/${subdir}/gm2ringsim_${input}.root `ls rootfiles/${subdir}/gm2ringsim_${input}_*.root`
	echo "Done..."
	if [ ${2} == "mergedel" ]; then
	    for file in rootfiles/${subdir}/gm2ringsim_${input}_*.root; do
		echo "rm ${file}"
		rm ${file}
	    done
	fi
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
#	    echo "Extra Suffix = ${extra_suffix}"
	    if [ ${extra_suffix} == "test" ]; then
		ls ${output}/*${input}/*${suffix}*_0.root > input.dat
		maxfiles=1
		numevts=100
	    elif [ ${extra_suffix} == "test2" ]; then
		ls ${output}/*${input}/*${suffix}*_0.root > input.dat
		maxfiles=100
		numevts=1500000
	    elif [ ${extra_suffix} == "test3" ]; then
		ls ${output}/*${input}/*${suffix}*_1.root > input.dat
		ls ${output}/*${input}/*${suffix}*_2.root >> input.dat
		ls ${output}/*${input}/*${suffix}*_3.root >> input.dat
		maxfiles=100
		numevts=1500000
	    elif [ ${extra_suffix} == "test4" ]; then
		ls ${output}/*${input}/*${suffix}*_4.root > input.dat
		ls ${output}/*${input}/*${suffix}*_5.root >> input.dat
		ls ${output}/*${input}/*${suffix}*_6.root >> input.dat
		maxfiles=100
		numevts=2500000
	    else
		#
		# 0  = 10 jobs
		# -3 = 20 jobs
		#
		manyfiles=-3
		if [ ${manyfiles} == -4 ]; then
		    if [ ${extra_suffix} -lt 10 ]; then
			ls ${output}/*${input}/*${suffix}*0_${extra_suffix}.root > input.dat
			ls ${output}/*${input}/*${suffix}*1_${extra_suffix}.root >> input.dat
		    elif [ ${extra_suffix} -lt 20 ]; then
			loc_extra_suffix=`echo " ${extra_suffix} - 10" | bc`
			extra_suffix2=${loc_extra_suffix}
			ls ${output}/*${input}/*${suffix}*2_${extra_suffix2}.root > input.dat
			ls ${output}/*${input}/*${suffix}*3_${extra_suffix2}.root >> input.dat
		    elif [ ${extra_suffix} -lt 30 ]; then
			loc_extra_suffix=`echo " ${extra_suffix} - 20" | bc`
			extra_suffix2=${loc_extra_suffix}
			ls ${output}/*${input}/*${suffix}*4_${extra_suffix2}.root > input.dat
			ls ${output}/*${input}/*${suffix}*5_${extra_suffix2}.root >> input.dat
		    elif [ ${extra_suffix} -lt 40 ]; then
			loc_extra_suffix=`echo " ${extra_suffix} - 30" | bc`
			extra_suffix2=${loc_extra_suffix}
			ls ${output}/*${input}/*${suffix}*6_${extra_suffix2}.root > input.dat
			ls ${output}/*${input}/*${suffix}*7_${extra_suffix2}.root >> input.dat
		    elif [ ${extra_suffix} -lt 50 ]; then
			loc_extra_suffix=`echo " ${extra_suffix} - 40" | bc`
			extra_suffix2=${loc_extra_suffix}
			ls ${output}/*${input}/*${suffix}*8_${extra_suffix2}.root > input.dat
			ls ${output}/*${input}/*${suffix}*9_${extra_suffix2}.root >> input.dat
		    fi
		    echo ${extra_suffix}
#		    cp input.dat input_${extra_suffix}.dat
#		    wc -l input.dat
#		    exit;
		elif [ ${manyfiles} == -3 ]; then
		    if [ ${extra_suffix} -lt 10 ]; then
			ls ${output}/*${input}/*${suffix}*0_${extra_suffix}.root > input.dat
			ls ${output}/*${input}/*${suffix}*1_${extra_suffix}.root >> input.dat
			ls ${output}/*${input}/*${suffix}*2_${extra_suffix}.root >> input.dat
			ls ${output}/*${input}/*${suffix}*3_${extra_suffix}.root >> input.dat
			ls ${output}/*${input}/*${suffix}*4_${extra_suffix}.root >> input.dat
		    else
			loc_extra_suffix=`echo " ${extra_suffix} - 10" | bc`
			extra_suffix2=${loc_extra_suffix}
			ls ${output}/*${input}/*${suffix}*5_${extra_suffix2}.root > input.dat
			ls ${output}/*${input}/*${suffix}*6_${extra_suffix2}.root >> input.dat
			ls ${output}/*${input}/*${suffix}*7_${extra_suffix2}.root >> input.dat
			ls ${output}/*${input}/*${suffix}*8_${extra_suffix2}.root >> input.dat
			ls ${output}/*${input}/*${suffix}*9_${extra_suffix2}.root >> input.dat
		    fi
		elif [ ${manyfiles} == -2 ]; then
		    ls ${output}/*${input}/*${suffix}*_*${extra_suffix}.root > input.dat
		elif [ ${manyfiles} == -1 ]; then
		    maxfiles=100
		    if [ ${extra_suffix} == 0 ]; then
			ls ${output}/*${input}/*${suffix}*_*1.root > input.dat
			ls ${output}/*${input}/*${suffix}*_*0.root >> input.dat
		    elif [ ${extra_suffix} == 1 ]; then
			ls ${output}/*${input}/*${suffix}*_*3.root > input.dat
			ls ${output}/*${input}/*${suffix}*_*2.root >> input.dat
		    elif [ ${extra_suffix} == 2 ]; then
			ls ${output}/*${input}/*${suffix}*_*5.root > input.dat
			ls ${output}/*${input}/*${suffix}*_*4.root >> input.dat
		    elif [ ${extra_suffix} == 3 ]; then
			ls ${output}/*${input}/*${suffix}*_*7.root > input.dat
			ls ${output}/*${input}/*${suffix}*_*6.root >> input.dat
		    elif [ ${extra_suffix} == 4 ]; then
			ls ${output}/*${input}/*${suffix}*_*9.root > input.dat
			ls ${output}/*${input}/*${suffix}*_*8.root >> input.dat
		    fi
		elif [ ${manyfiles} == 0 ]; then
		    maxfiles=100
		    ls ${output}/*${input}/*${suffix}*_${extra_suffix}.root > input.dat
		else
		    if [ ${extra_suffix} -ge 0 ]; then
			echo "  >0"
			if [ ${extra_suffix} -gt 19 ]; then
			    echo "  *6, *8"
			    loc_extra_suffix=`echo " ${extra_suffix} - 19" | bc`
			    ls ${output}/*${input}/*${suffix}*6_${loc_extra_suffix}.root > input.dat
			    ls ${output}/*${input}/*${suffix}*8_${loc_extra_suffix}.root >> input.dat
			else
			    echo "  *0, *2, *4"
			    ls ${output}/*${input}/*${suffix}*0_${extra_suffix}.root > input.dat
			    ls ${output}/*${input}/*${suffix}*2_${extra_suffix}.root >> input.dat
			    ls ${output}/*${input}/*${suffix}*4_${extra_suffix}.root >> input.dat
			fi
		    else
			echo "  <0"
			loc_extra_suffix=`echo " ${extra_suffix} * -1" | bc`
			if [ ${loc_extra_suffix} -gt 19 ]; then
			    loc_extra_suffix=`echo " ${loc_extra_suffix} - 19" | bc`
			    echo "  *7, *9"
			    ls ${output}/*${input}/*${suffix}*7_${loc_extra_suffix}.root > input.dat
			    ls ${output}/*${input}/*${suffix}*9_${loc_extra_suffix}.root >> input.dat
			else
			    echo "  *1, *3, *5"
			    ls ${output}/*${input}/*${suffix}*1_${loc_extra_suffix}.root > input.dat
			    ls ${output}/*${input}/*${suffix}*3_${loc_extra_suffix}.root >> input.dat
			    ls ${output}/*${input}/*${suffix}*5_${loc_extra_suffix}.root >> input.dat
			fi
		    fi
		fi
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
    elif [ ${2} == "vp1" ] || [ ${2} == "vp" ] || [ ${2} == "novp1" ] || [ ${2} == "novp" ] || [ ${2} == "debug" ] || [ ${2} == "nodebug" ] || [ ${2} == "truth" ] || [ ${2} == "notruth" ] || [ ${2} == "fill" ] || [ ${2} == "nofill" ] || [ ${2} == "ring" ] || [ ${2} == "noring" ]; then	
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
    echo "======= FOUND ${nfiles} ======="
else
    echo "No input.dat needed."
fi

if [ ${nfiles} == 0 ]; then
    exit;
fi

update=0

#if ! [ -z ${extra_suffix} ]; then
#    if [ ${extra_suffix} == 0 ] || [ ${extra_suffix} == 10 ] || [ ${extra_suffix} == -10 ] || [ ${extra_suffix} == 20 ] || [ ${extra_suffix} == -20 ] || [ ${extra_suffix} == 30 ] || [ ${extra_suffix} == -30 ]; then
#	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
#	fixfooter ${footer_fcl} "novp"
#    else
#	footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
#	fixfooter ${footer_fcl} "novp"
#    fi
#fi

footer_fcl=`ls ${output}/*${input}*/footer_reader.fcl`
fixfooter ${footer_fcl} "novp"

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
#
# If I just want a few files
#
#
if [ -a newinputfile.dat ]; then
    rm newinputfile.dat
fi

#
# Create INPUT_DIR_ARRAY
#
INPUT_DIR_ARRAY=""
for file in ${inputfiles}; do
    if [ ${n} -gt ${maxfiles} ] && [ ${maxfiles} -gt 0 ]; then
	break;
    fi

    if [ -a ${file} ]; then
	echo ${file} >> newinputfile.dat
	INPUT_DIR_ARRAY="-f ${file} ${INPUT_DIR_ARRAY}"
	if [ ${n} == 1 ]; then
	    inputs="\"${file}\""
	else
	    inputs="${inputs}, \"${file}\""
	fi
	((n++))
    fi
done
#INPUT_DIR_ARRAY=""
echo "========= size(INPUT_DIR_ARRAY) = ${n}-1 ========="
cp newinputfile.dat input.dat

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
      fill: true
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
#cat ${files_fcl} >> ${fcl}
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
    echo "Found output file: ${outfile}"
    echo "   w/ size=${fhsize}"
    nfiles=1
else
    fsize=-1
    fhsize=-1
    echo "Output file: ${outfile}"
    echo "No output file exists."
    if [ -a input.dat ]; then
	nfiles=`wc -l input.dat | awk '{print $1}'`
    fi
fi

#echo "Pre:  Runplot = ${runplot}  ; Filesize = ${fhsize}  ; nfiles=${nfiles}"
if [ ${runplot} == 2 ] && [ ${fsize} -lt 1000000 ]; then
    runplot=1
elif [ ${runplot} == 1 ] && [ ${fsize} -gt 1000000 ]; then
    runplot=2
fi
#echo "Post: Runplot = ${runplot}  ; Filesize = ${fhsize}  ; nfiles=${nfiles}"
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


#echo "Post: Runplot = ${runplot}  ; Filesize = ${fhsize}  ; nfiles=${nfiles}"


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
		    gm2 -c ${fcl} -n ${numevts} -T rootfiles/${subdir}/gm2ringsim_${input}${xsuffix}.root --source-list input.dat
		    echo "File Location:"
		    echo "root -l rootfiles/${subdir}/gm2ringsim_${input}${xsuffix}.root"
		    echo ""
		else
		    gm2 -c ${fcl} -T rootfiles/${subdir}/gm2ringsim_${input}${xsuffix}.root --source-list input.dat
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

		# Fix inputdat file
		files=`cat input.dat`
		if [ -a fileinputs.dat ]; then
		    rm fileinputs.dat
		fi
		for file in ${files}; do
		    filename=`basename ${file}`
		    echo ${filename} >> fileinputs.dat
		done

		if ! [ -z ${extra_suffix} ]; then
#		    cp input.dat ${outdir}/${input}_${extra_suffix}.dat
		    cp fileinputs.dat ${outdir}/${input}_${extra_suffix}.dat
		    cp ${fcl} ${outdir}/${input}_${extra_suffix}.fcl
		else
#		    cp input.dat ${outdir}/${input}.dat
		    cp fileinputs.dat ${outdir}/${input}.dat
		    cp ${fcl} ${outdir}/${input}.fcl
		fi
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
