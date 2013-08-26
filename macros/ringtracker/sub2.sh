#!/bin/bash
# Running gm2ringsim on Fermigrid
# Submit with
# jobsub -g --opportunistic -dMYDIR <outputdir>

echo "gm2ringsim_job" 
date
hostname
echo Job ID $CLUSTER
nano=`date +%N | sed -e 's/000$//' -e 's/^0//'`

suffix=`hostname`
suffix=${suffix}_${nano}

echo "=============================="
echo "FullDir = ${fullDir}"
echo "=============================="

# Restore environment
echo "Restoring environment" 
date
#cd /gm2/app/users/lyon/work-201303ws-a/stuff
cd /gm2/app/users/tgadfort/gm2ART7/srcs/gm2ringsim/fcl
source ../../../localProducts_v201211_e2-prof/setup
export MYAPPDIR=$PWD
cd /gm2/app/users/tgadfort/gm2ART7/build/
source gm2d setup_for_development
setup g4emlow v6_23
cd /gm2/app/users/tgadfort/gm2ART7/srcs/gm2ringsim/fcl

export LD_LIBRARY_PATH=`dropit -p $LD_LIBRARY_PATH 'gm2ringsim/v0_0_1'`
export LD_LIBRARY_PATH=`dropit -p $LD_LIBRARY_PATH 'artg4/v0_0_1'`

# Run gm2
cd $CONDOR_DIR_MYDIR
echo "======================================"
echo "CONDOR_DIR_MYDIR = ${CONDOR_DIR_MYDIR}"
echo "======================================"
date

ls ${fullDir}/runner.fcl
#cp ${fullDir}/runner.fcl ${fullDir}/runner_${CLUSTER}.fcl 

#sed -i 's/.root/${suffix}.root/g' $MYAPPDIR/runner.fcl > $MYAPPDIR/runner_${suffix}.fcl
#sed s/.root/_${suffix}.root/g ${fullDir}/runner.fcl > ${fullDir}/runner_${suffix}.fcl
#sed "s/seed: */seed: ${nano}/g" ${fullDir}/runner_${suffix}.fcl ${fullDir}/runner2_${suffix}.fcl 
#mv ${fullDir}/runner2_${suffix}.fcl ${fullDir}/runner_${suffix}.fcl 

#ls ${fullDir}/runner_${suffix}.fcl

echo Running gm2 from $CONDOR_DIR_MYDIR
touch ${fullDir}/START
#gm2 -c ${fullDir}/runner_${suffix}.fcl
#gm2 -c ${fullDir}/runner.fcl -o output_${CLUSTER}_${PROCESS}.root -T histograms_${CLUSTER}_${PROCESS}.root
gm2 -c ${fullDir}/runner.fcl -o output_${CLUSTER}_${PROCESS}.root
touch ${fullDir}/DONE
rm ${fullDir}/START
#rm ${fullDir}/runner_${suffix}.fcl
#gm2 -c $MYAPPDIR/mygm2ringsim.fcl -n 10
#rm $MYAPPDIR/infgun_${suffix}.fcl

date
echo 'DONE'
