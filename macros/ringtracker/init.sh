#!/bin/sh

targetdir="../../fcl/"
filestomove="allrun.sh   makefcl.sh   move.sh  plotinflector.C  plotinf.sh  run.sh  submit.sh"
for file in ${filestomove}; do
    echo "cp ${file} ${targetdir}"
    cp ${file} ${targetdir}
done

echo "cp -r atlasstyle-00-03-04 ${targetdir}"
cp -r atlasstyle-00-03-04 ${targetdir}
