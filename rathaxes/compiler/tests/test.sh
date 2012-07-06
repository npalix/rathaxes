#!/bin/sh
cd `dirname $0`
export HERE=`pwd`
echo $HERE
codeworker --quiet -I $CNORM_PATH/src -I $HERE/../ -I $HERE/../../samples/rs232_mockup -script $1 -args $HERE/../..
