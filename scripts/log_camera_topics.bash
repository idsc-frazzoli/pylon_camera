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
}




topics=$(rostopic list)
colorCameraFound=false
BWcameraFound=false

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
	rosbag record color_camera_node/image_raw &
	colorCameraPID=$!
	echo "Started recording"
	echo "Color camera PID is: $colorCameraPID"
fi

if [ "$BWcameraFound" = true ]; then
	echo "Found BW camera"
	rosbag record BW_camera_node/image_raw &
	BWcameraPID=$!
	echo "Started recording"
	echo "BW camera PID is: $BWcameraPID"
fi


trap 'ctrl_c $colorCameraPID $BWcameraPID' SIGINT



while true; do
	sleep 1
	echo "recording"	
done



