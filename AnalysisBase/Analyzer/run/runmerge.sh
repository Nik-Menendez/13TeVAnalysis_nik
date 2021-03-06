#!/bin/bash

eos='/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select'
filedir=/eos/cms/store/user/vdutta/13TeV/060515
conf=datasets.conf
prefix=""

if [[ "$filedir" =~ ^/eos/cms/.* && "$HOSTNAME" =~ .*cern.ch ]]; then
  $eos mkdir -p ${filedir}/merged
  prefix="root://eoscms/"
else
  mkdir -p ${filedir}/merged
  if [[ "$filedir" =~ ^/eos/uscms/.* && "$HOSTNAME" =~ .*fnal.gov ]]; then
    prefix="root://cmseos:1094/"
  fi
fi

for sname in `cat ${conf} | grep -v ^# | awk '{print $1}'` ; do
  echo ${prefix}${filedir}/merged/${sname}_ntuple.root > merge.txt
  file=""
  if [[ "$filedir" =~ ^/eos/cms/.* && "$HOSTNAME" =~ .*cern.ch ]]; then
    filelist=`$eos find -f ${filedir} | grep output | grep ${sname}`
  else
    filelist=`find ${filedir} | grep output | grep ${sname}`
  fi
  for file in ${filelist} ; do 
    echo ${prefix}${file} >> merge.txt
  done
  root -l -q -b MergeNtuples.C+\(\"merge.txt\"\)
done

rm -f merge.txt
