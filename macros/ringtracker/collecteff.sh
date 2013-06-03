#!/bin/sh

outfile="files.dat"
rm ${outfile}
missfile="missing.dat"
rm ${missfile}

start=0

doDM=0
doUM=0
doUC=1
doCO77=0
doCO=0
doALL=0
if [ ${doALL} == 1 ]; then
    doUM=0;
    doUC=1;
    doDM=1;
    doCO=1;
    doCO77=1;
fi

dobss=""
if [ ${doDM} == 1 ]; then
    dobss="${dobss} dm"
fi
if [ ${doUM} == 1 ]; then
    dobss="${dobss} um"
fi
if [ ${doUC} == 1 ]; then
    dobss="${dobss} uc"
fi
if [ ${doCO77} == 1 ]; then
    dobss="${dobss} co77"
fi
if [ ${doCO} == 1 ]; then
    dobss="${dobss} co"
fi

doit=1
if ! [ -z ${1} ]; then
    doit=0
fi



sigmats="25"
fields="0"
fields="1"

beamsizes="1 10 20 30 40 50 60"
#beamsizes="20 40 60"
beamsizes="1 20 40"
beamsizes="1 20 40"
beamsizes="40 1 20"
beamsizes="40 0 20"
beamsizes="40 20 1 0"
beamsizes="0 40"
beamsizes="42"

kicks="90 220"

plottypes="1 2 3 4"
plottypes="1 2 3 4 5"
#plottypes="2 3 4 5"
plottypes="1 2 3 4 5"
plottypes="1"

infs="ClosedInflector PartiallyOpen InflectorOpen"
#infs="ClosedInflector InflectorOpen"
infs="InflectorOpen"
#infs="PartiallyOpen"

#quads="NewSupports NoQuads NoQuadSupports FullHighMom FullHigherMom FullHighestMom"
#quads="FullHighMom FullHigherMom FullHighestMom"
quads="NewSupports NoQuads NoQuadSupports"
#quads="NoQuadSupports NoQuads"
#quads="NewSupports NoQuadSupports NoQuads"
#quads="NoQuadSupports NoQuads"
quads="NoQuads"
#quads="NewSupports"
scrapings="OFF"
beamtypes="Gaussian Uniform"
#beamtypes="Uniform"

#beamtypes="Gaussian"
momtypes="BNLmatch Perfectmatch"
momtypes="BNLmatch"
momtypes="Perfectmatch"
#beamtypes="Gaussian"
beamtypes="Uniform"
infs="InflectorOpen PartiallyOpen ClosedInflector"
#infs="PartiallyOpen"
infs="InflectorOpen"
#infs="ClosedInflector"
#quads="NoQuads NoQuadSupports NewSupports"
#quads="NoQuadSupports"
#quads="NoQuads NewSupports"
quads="NoQuads"
#quads="NewSupports"
beamsizes="42"
fields="0"

if [ ${doit} == 1 ]; then
    for bs in ${dobss}; do
	bsname="${bs}"
	if [ ${bsname} == "um" ]; then
	    bsname="UpstreamMandrel"
	elif [ ${bsname} == "dm" ]; then
	    bsname="DownstreamMandrel"
	elif [ ${bsname} == "uc" ]; then
	    bsname="UpstreamCryo"
	elif [ ${bsname} == "co77" ]; then
	    bsname="CentralOrbit_Offset77"
	elif [ ${bsname} == "co" ]; then
	    bsname="CentralOrbit"
	fi

	for momtype in ${momtypes}; do

	if [ ${bs} == "co" ] || [ ${bs} == "co77" ]; then
	    if [ ${momtype} == "Perfectmatch" ]; then
		moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP0025 PerfectMatch_dP005 PerfectMatch_dP0075 PerfectMatch_dP025 PerfectMatch_dP05 PerfectMatch_dP075 PerfectMatch_dP1 PerfectMatch_dP2 PerfectMatch_dP5 PerfectMatch_dP10"
	    fi
	    if [ ${momtype} == "BNLmatch" ]; then
		moms="E821Match_dP0001 E821Match_dP001 E821Match_dP0025 E821Match_dP005 E821Match_dP0075 E821Match_dP025 E821Match_dP05 E821Match_dP075 E821Match_dP1 E821Match_dP2 E821Match_dP5 E821Match_dP10"
	    fi
#	    moms="E821Match_dP0001 PerfectMatch_dP0001 PerfectMatch_dP10"
	else
	    moms="E821Match_dP05"
	    if [ ${momtype} == "BNLmatch" ]; then
		moms="E821Match_dP0001 E821Match_dP10"
		moms="E821Match_dP0001"
#		moms="E821Match_dP05"
	    fi
	    if [ ${momtype} == "Perfectmatch" ]; then
		moms="PerfectMatch_dP0001 PerfectMatch_dP10"
		moms="PerfectMatch_dP0001"
#		moms="PerfectMatch_dP05"
	    fi
	    if [ ${momtype} == "Perfectmatch" ]; then
		moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP0025 PerfectMatch_dP005 PerfectMatch_dP0075 PerfectMatch_dP025 PerfectMatch_dP05 PerfectMatch_dP075 PerfectMatch_dP1 PerfectMatch_dP2 PerfectMatch_dP5 PerfectMatch_dP10"
	    fi
	    if [ ${momtype} == "BNLmatch" ]; then
		moms="E821Match_dP0001 E821Match_dP001 E821Match_dP0025 E821Match_dP005 E821Match_dP0075 E821Match_dP025 E821Match_dP05 E821Match_dP075 E821Match_dP1 E821Match_dP2 E821Match_dP5 E821Match_dP10"
	    fi
	    if [ ${momtype} == "BNLmatch" ]; then
		moms="E821Match_dP0001 E821Match_dP10"
		moms="E821Match_dP05"
	    fi
	    if [ ${momtype} == "Perfectmatch" ]; then
		moms="PerfectMatch_dP0001 PerfectMatch_dP10"
		moms="PerfectMatch_dP05"
	    fi
	fi

	echo ${bsname}
#	continue;
	
	for beamtype in ${beamtypes}; do
	    for scraping in ${scrapings}; do
	    for quad in ${quads}; do
		for beamsize in ${beamsizes}; do
		    for inf in ${infs}; do

			if [ ${bsname} == "CentralOrbit_Offset77" ] || [ ${bsname} == "CentralOrbit" ]; then
			    if ! [ ${inf} == "InflectorOpen" ]; then
				continue;
			    fi
			fi

			for sigmat in ${sigmats}; do
			    for field in ${fields}; do
				fieldname=${field}
				if [ ${field} == 0 ]; then
				    fieldname="Vanish"
				elif [ ${field} == 1 ]; then
				    fieldname="Mapped"
				fi
				for mom in ${moms}; do
				    if [ ${bsname} == "CentralOrbit" ]; then
					kicks="0"
				    else
					kicks="220 90"
				    fi
				    for kick in ${kicks}; do
					for plottype in ${plottypes}; do
					    
					    rm ${outfile}
					    echo ${plottype} >> ${outfile}
					    
					    if [ ${kick} -gt 0 ]; then
						if [ ${plottype} == 0 ]; then
						    if [ ${kick} -gt 120 ]; then
							kickname="GSquareKick"
						    else
							kickname="kVLCRKick"
						    fi
						fi
						if [ ${plottype} == 1 ] || [ ${plottype} == 2 ] || [ ${plottype} == 3 ] || [ ${plottype} == 4 ] || [ ${plottype} == 5 ]; then
						    if [ ${kick} -gt 120 ]; then
							kickname="${kick}GSquareKick"
						    else
							kickname="${kick}kVLCRKick"
						    fi
						fi
					    else
						kickname="NoKick"
					    fi
					    
					    if [ ${quad} == "FullQuads" ]; then
						prefix="Scraping${scraping}_101Turns"
					    else
						prefix="Scraping${scraping}_${quad}_101Turns"
					    fi
					    
					    beamtypename=""
					    if [ ${beamtype} == "Uniform" ]; then
						beamtypename="Uniform"
					    fi	
					    

#					    if ! [ -a datfiles/storage_inf_${plottype}_${bs}_${kickname}_${beamtype}_${sigmat}_${field}_${inf}_${mom}_${beamsize}pi_${prefix}.dat ]; then
#					    echo "===== ${bsname} ===== ${beamtypename} ===== ${mom} ===== ${beamsize}pi ===== ${inf} ====== ${sigmat} ====== ${fieldname} ====== ${prefix} ====== ${kickname} ====== ${plottype} ======"
#					    fi
#					    continue;
					    

					    if [ ${field} == 0 ]; then
						for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_NoDelta_Launch*mrad_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
						    echo ${file}
						    if [ -a ${file} ]; then
							echo ${file} >> ${outfile}
						    fi
						done
						
						for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_NoDelta_NoLaunch_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
						    if [ -a ${file} ]; then
							echo ${file}  >> ${outfile}
						    fi
						done
						
						if [ ${plottype} == 1 ] || [ ${plottype} == 2 ] || [ ${plottype} == 3 ] || [ ${plottype} == 4 ] || [ ${plottype} == 5 ]; then
						    for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_Delta*_Launch*mrad_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
							if [ -a ${file} ]; then
							    echo ${file}  >> ${outfile}
							fi
						    done
						    
						    for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_Delta*_NoLaunch_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
							if [ -a ${file} ]; then
							    echo ${file}  >> ${outfile}
							fi
						    done
						fi
					    fi
					    
					    
					    if [ ${field} == 1 ]; then
						for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_NoDelta_Launch*mrad_MappedField_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
						    if [ -a ${file} ]; then
							echo ${file} >> ${outfile}
						    fi
						done
						
						for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_NoDelta_NoLaunch_MappedField_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
						    if [ -a ${file} ]; then
							echo ${file}  >> ${outfile}
						    fi
						done
						
						if [ ${plottype} == 1 ] || [ ${plottype} == 2 ] || [ ${plottype} == 3 ] || [ ${plottype} == 4 ] || [ ${plottype} == 5 ]; then
						    for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_Delta*_Launch*mrad_MappedField_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
							if [ -a ${file} ]; then
							    echo ${file}  >> ${outfile}
							fi
						    done
						    
						    for file in Eff/${bsname}_${inf}/${prefix}_${bsname}_tSigma${sigmat}_*${kickname}_*${inf}_Delta*_NoLaunch_MappedField_BeamSize${beamsize}pimmmrad_${beamtypename}InflectorGun_${mom}_CollimatorON.dat; do
							if [ -a ${file} ]; then
							    echo ${file}  >> ${outfile}
							fi
						    done
						fi
					    fi
					    
					    
					    echo "===== ${bsname} ===== ${momtype} ===== ${beamtype} ===== ${quad}  ===== ${beamsize}pi ===== ${inf} ====== ${sigmat} ====== ${fieldname} ===== ${mom} ====== ${kickname} ====== ${plottype} ======"
					    NUM=`wc -l ${outfile} | awk '{print $1}'`
					    if [ ${NUM} -gt 1 ]; then					
						root -l collecteff.C+ -b -q					
						mv -f storage_dm2.dat datfiles/storage_inf_${plottype}_${bs}_${kickname}_${beamtype}_${sigmat}_${field}_${inf}_${mom}_${beamsize}pi_${prefix}.dat
					    else
						echo "${plottype}_${bs}_${kickname}_${beamtype}_${sigmat}_${field}_${inf}_${mom}_${beamsize}pi_${prefix}.dat" >> notthere
					    fi
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
fi


echo ""
echo "======================="
echo "    Making C files"
echo "======================="
echo ""


beamtypes="Gaussian Uniform"
fields="1 0"
beamsizes="40 0"
sigmats="25"
plottypes="0 1 2 3 4 5"
infs="ClosedInflector PartiallyOpen InflectorOpen"
scrapings="OFF"
quads="NewSupports NoQuads NoQuadSupports"
kicks="NoKick 220GSquareKick 90kVLCRKick"
momtypes="BNLmatch Perfectmatch"			

for plottype in ${plottypes}; do
    plotname=${plottype}
    if [ ${plottype} == 0 ]; then
	plotname="KL"
    elif [ ${plottype} == 1 ]; then
	plotname="DL"
    elif [ ${plottype} == 2 ]; then
	plotname="DL_Inf"
    elif [ ${plottype} == 3 ]; then
	plotname="DL_InfLength"
    elif [ ${plottype} == 4 ]; then
	plotname="DL_RingAcc"
    elif [ ${plottype} == 5 ]; then
	plotname="DL_InfAcc"
    fi
    for field in ${fields}; do
	fieldname=${field}
	if [ ${field} == 0 ]; then
	    fieldname="Vanish"
	elif [ ${field} == 1 ]; then
	    fieldname="Mapped"
	fi
	for quad in ${quads}; do
	    for beamsize in ${beamsizes}; do

		starts="DM UC CO CO77"
		for start in ${starts}; do
		    if [ -a Includes/FillEff_${start}_${plotname}_${fieldname}_${quad}_${beamsize}pi.C ]; then
			rm Includes/FillEff_${start}_${plotname}_${fieldname}_${quad}_${beamsize}pi.C
		    fi
		    #touch Includes/FillEff_${start}_${plotname}_${fieldname}_${quad}_${beamsize}pi.C
		done

		for start in ${starts}; do

		    if [ ${start} == "DM" ]; then
			smallstart="dm"
		    elif [ ${start} == "UM" ]; then
			smallstart="um"
		    elif [ ${start} == "UC" ]; then
			smallstart="uc"
		    elif [ ${start} == "CO77" ]; then
			smallstart="co77"
		    elif [ ${start} == "CO" ]; then
			smallstart="co"
		    fi


		    for momtype in ${momtypes}; do
			
			if [ ${start} == "CO" ] || [ ${start} == "CO77" ]; then
			    if [ ${momtype} == "Perfectmatch" ]; then
				moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP0025 PerfectMatch_dP005 PerfectMatch_dP0075 PerfectMatch_dP025 PerfectMatch_dP05 PerfectMatch_dP075 PerfectMatch_dP1 PerfectMatch_dP2 PerfectMatch_dP5 PerfectMatch_dP10"
			    fi
			    if [ ${momtype} == "BNLmatch" ]; then
				moms="E821Match_dP0001 E821Match_dP001 E821Match_dP0025 E821Match_dP005 E821Match_dP0075 E821Match_dP025 E821Match_dP05 E821Match_dP075 E821Match_dP1 E821Match_dP2 E821Match_dP5 E821Match_dP10"
			    fi
			else
			    if [ ${momtype} == "Perfectmatch" ]; then
				moms="PerfectMatch_dP0001 PerfectMatch_dP001 PerfectMatch_dP0025 PerfectMatch_dP005 PerfectMatch_dP0075 PerfectMatch_dP025 PerfectMatch_dP05 PerfectMatch_dP075 PerfectMatch_dP1 PerfectMatch_dP2 PerfectMatch_dP5 PerfectMatch_dP10"
			    fi
			    if [ ${momtype} == "BNLmatch" ]; then
				moms="E821Match_dP0001 E821Match_dP001 E821Match_dP0025 E821Match_dP005 E821Match_dP0075 E821Match_dP025 E821Match_dP05 E821Match_dP075 E821Match_dP1 E821Match_dP2 E821Match_dP5 E821Match_dP10"
			    fi
			fi
			
			for scraping in ${scrapings}; do
			    prefix="Scraping${scraping}_${quad}_101Turns"
			    for inf in ${infs}; do
				for sigmat in ${sigmats}; do
				    for kick in ${kicks}; do
					kickname=${kick}
					for beamtype in ${beamtypes}; do
					    for mom in ${moms}; do
						
						if [ -a datfiles/storage_inf_${plottype}_${smallstart}_${kickname}_${beamtype}_${sigmat}_${field}_${inf}_${mom}_${beamsize}pi_${prefix}.dat ]; then
						    cat datfiles/storage_inf_${plottype}_${smallstart}_${kickname}_${beamtype}_${sigmat}_${field}_${inf}_${mom}_${beamsize}pi_${prefix}.dat >> Includes/FillEff_${start}_${plotname}_${fieldname}_${quad}_${beamsize}pi.C
						fi
						
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
