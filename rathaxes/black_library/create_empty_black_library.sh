#!/bin/sh

OS=`ls`;
for i in $OS
do
  if [ "./$i" != $0 ]
  then
      mkdir $i/RKM
      touch $i/_RKM.blt
      mkdir $i/RKM/entry_point
      touch $i/RKM/_entry_point.blt
      touch $i/RKM/entry_point/load.blt
      touch $i/RKM/entry_point/unload.blt
      touch $i/RKM/kernel_registration.blt
  fi
done
