#!/bin/bash

#Check number of parameters
if [ "$#" -ne 3 ]; then
    echo "Illegal number of parameters."
    exit 1
fi

#Assign parameters
inputdir=$1
outputdir=$2
maxthreads=$3

#Check if input dir exists AND is a directory
if [ ! -d "$inputdir" ]; then
    echo "$inputdir doesn't exist or isn't a directory."
    exit 1
fi

#Check if output dir exists AND is a directory
if [ ! -d "$outputdir" ]; then
    echo "$outputdir doesn't exist or isn't a directory."
    exit 1
fi

#Check if output dir exists AND is a directory
if [ ! $maxthreads -gt 0 ]; then
    echo "number of threads $maxthreads is less than 1."
    exit 1
fi

#Check if tecnicofs exists
if [ ! -f "./tecnicofs" ]; then
    echo "./tecnicofs doesn't exist."
    exit 1
fi

#Loop tecnicofs with different inputs
for input in $inputdir/*.txt
do
    output=$(echo $input | cut -d / -f2 | cut -d . -f1)
    for i in $(seq 1 $maxthreads)
    do
        echo "InputFile=$input NumThreads=$i"
        ./tecnicofs $input $outputdir/$output-$i.txt $i | tail -n 1

    done
done