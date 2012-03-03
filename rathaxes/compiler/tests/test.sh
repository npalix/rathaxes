#!/bin/sh
cd `dirname $0`
export HERE=`pwd`
echo $HERE
codeworker --quiet -I $CNORM_PATH/src -I $HERE/../ -I $HERE/../../samples -script $1 -args $HERE/../..
