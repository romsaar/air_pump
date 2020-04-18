#!/bin/sh

###################################################################
# usage
###################################################################
display_usage() { 

    echo "Usage: ./launch.sh [OPTION ...]" 
    echo "Example: ./launch.sh -m 0" 
	echo "Options:"
    echo "\t-m MODE\tMode of initialization 0,1" 
    echo "\t\t0 - Run roscore" 
    echo "\t\t1 - Don't run roscore" 
}
if [  $# -le 0 ] 
	then 
		display_usage
		exit 1
	fi
while getopts ":hm:" opt; do
  case ${opt} in
    h ) # process option a
		display_usage
      ;;
    m ) # process option m
        mode=${OPTARG}
      ;;
  esac
done 
if [ -z ${mode} ]
	then
		echo "Error!!! mode  was not set"
		display_usage
        exit
	fi
##################
# IP settings
##################
export ROS_MASTER_URI=http://10.0.0.6:11311
export ROS_HOSTNAME=10.0.0.16
export ROS_IP=10.0.0.16
#common practice
#https://sookocheff.com/post/bash/parsing-bash-script-arguments-with-shopts/ 
shift $((OPTIND -1))
if [ "${mode}" = "0" ] ; then
    roscore &  rosrun rosserial_python serial_node.py /dev/ttyUSB0
else
    rosrun rosserial_python serial_node.py /dev/ttyUSB0
fi
