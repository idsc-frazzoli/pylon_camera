/*
 * toLCM_node.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: jelavice
 */

#include <string>
#include <iostream>
#include <ros/ros.h>


#include "ros_2_lcm/ros_2_lcm.h"


int main(int argc, char** argv) {
    ros::init(argc, argv, "ros_2_lcm");
    std::string rosTopicName = std::string(argv[1]);
    std::string lcmTopicName = std::string(argv[2]);
    ros2lcm::RosToLcm rosToLcm(rosTopicName, lcmTopicName);
    ros::spin();
    return 0;
}


