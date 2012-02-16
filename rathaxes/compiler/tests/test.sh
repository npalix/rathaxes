#!/bin/sh
codeworker --quiet -I $CNORM_PATH/src -I `pwd`/../ -script $1 -args $PWD/../../
