#!/bin/bash

runmacro=$1
filename=$2
process=$3
xsec=$4
lumi=$5
totposevents=$6
totnegevents=$7
treename=$8
suffix=$9
xsecfile=${10}
outputname=${11}
outputdir=${12}
scramdir=${13}

workdir=`pwd`

echo `hostname`
echo "scramdir: $scramdir"
echo "workdir: $workdir"
echo "args: $*"

cd $scramdir/src/
SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`
cd $workdir

cp ${scramdir}/${runmacro} .
cp ${scramdir}/rootlogon.C .

root -l -b -q $runmacro+\(\"${filename}\",\"${process}\",$xsec,$lumi,$totposevents,$totnegevents,\"${treename}\",\"${suffix}\",\"${xsecfile}\"\)

status=`echo $?`
echo "Status = $status"

cmsStage -f ${outputname} ${outputdir}

exit $status
