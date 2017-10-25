/*
 * toLCM_node.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: jelavice
 */




#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/image_encodings.h>


//#include "exlcm/example_t.hpp"


void callback(const sensor_msgs::Image::ConstPtr& msg)
{
    std::cout << "shit received." << std::endl;
}


int main(int argc, char** argv){

    ros::init(argc, argv, "ros_2_lcm");

    //TODO move this to the class
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/color_camera_node/image_raw", 1000, callback);

    //TODO add LCM types and stuff
    ros::spin();

    return 0;
}

