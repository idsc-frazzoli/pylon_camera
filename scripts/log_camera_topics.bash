#!/bin/bash

colorCameraPID=-1
BWcameraPID=-1

function ctrl_c() {
echo ""
echo "SIGINT received, killing rosbags"
if [ "$1" -gt -1 ]
then
	echo "killing process: $1"
	kill $1	
fi

if [ "$2" -gt -1 ]
 then
	echo "killing process: $2"
	kill $2	
fi

echo "Bags saved in $(pwd)"
trap - SIGINT #clear trap
}




topics=$(rostopic list)
colorCameraFound=false
BWcameraFound=false
currTime=$(date +%Y-%m-%d-%H-%M-%S)
for i in $topics
do
	if [[ $i == *"BW_camera_node"* ]]; then
  		BWcameraFound=true
	fi

	if [[ $i == *"color_camera_node"* ]]; then
		colorCameraFound=true
	fi
	
done




if [ "$colorCameraFound" = true ]; then
	echo "Found color camera"
	name="color_camera-"$currTime
	rosbag record -O $name -b 1024 --duration=5m color_camera_node/image_raw &
	colorCameraPID=$!
	echo "Started recording"
	echo "Color camera PID is: $colorCameraPID"
fi

if [ "$BWcameraFound" = true ]; then
	echo "Found BW camera"
	name="BW_camera-"$currTime
	rosbag record -O $name -b 1024 --duration=5m BW_camera_node/image_raw &
	BWcameraPID=$!
	echo "Started recording"
	echo "BW camera PID is: $BWcameraPID"
fi


trap 'ctrl_c $colorCameraPID $BWcameraPID' SIGINT



while true; do
	sleep 1
	echo "recording"	
done



