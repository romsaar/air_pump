#!/bin/sh

###################################################################
# usage
###################################################################
display_usage() { 

    echo "Usage: ./build.sh [OPTION ...]" 
    echo "Example: ./build.sh -c 1 -u 1" 
	echo "Options:"
    echo "\t-c FLAG\tCompile [0,1]" 
    echo "\t\t0 - Compile" 
    echo "\t\t1 - Don't compile" 
    echo "\t-u FLAG\tUpload [0,1]" 
    echo "\t\t0 - Upload" 
    echo "\t\t1 - Don't upload" 
}
if [  $# -le 0 ] 
	then 
		display_usage
		exit 1
	fi
while getopts ":hc:u:" opt; do
  case ${opt} in
    h ) # process option a
		display_usage
      ;;
    c ) # process option m
        compileflag=${OPTARG}
      ;;
    u ) # process option m
        uploadflag=${OPTARG}
      ;;
  esac
done 
if [ -z ${compileflag} ]
	then
		echo "Error!!! compile flag was not set"
		display_usage
        exit
	fi
if [ -z ${uploadflag} ]
	then
		echo "Error!!! upload flag was not set"
		display_usage
        exit
	fi
echo "compileflag:" ${compileflag}
echo "uploadlag:" ${uploadflag}
#common practice
#https://sookocheff.com/post/bash/parsing-bash-script-arguments-with-shopts/ 
shift $((OPTIND -1))
if [ "${compileflag}" = "1" ] && [ "${uploadflag}" = "1" ] ; then
        echo "compiling and uploading..."
        arduino-cli compile -b arduino:avr:mega -u -p /dev/ttyUSB0 ../air_pump -v
else
    if [ "${compileflag}" = "1" ] ; then
        echo "compiling..."
        arduino-cli compile -b arduino:avr:mega ../air_pump -v
    else
        if [ "${uploadflag}" = "1" ] ; then
            echo "uploading..."
            arduino-cli upload -b arduino:avr:mega ../air_pump -p /dev/ttyUSB0 -v
        fi
    fi
fi
