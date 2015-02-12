#!/usr/bin/env sh

if [ $# -eq 0 ]
then
	echo
	echo " usage: batch.sh <CPUs> <trials> "
	echo
	echo " Run the EhrenfestModel code for multiple configurations."
	echo
	exit 0
fi

if [ ! $2 ] || [ $# -ge 3 ]
then
	echo
	echo " Expected two arguments. See usage"
	echo
	exit -1
fi

execute="bin/EhrenfestModel"
configuration="--num-threads=$1 --num-trials=$2 --num-particles"

for x in 2 4 6 8 10 12 14 16 18 20 24 26 28 30
do
	echo " -- Running --num-particles=$x ,"
	echo

	${execute} --num-threads=$1 --num-trials=$2 --num-particles=$x \
		--output-file=out/EhrenfestModel_$x.dat
	echo
done
