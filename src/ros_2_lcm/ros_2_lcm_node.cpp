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

    std::string rosNodeName = std::string(argv[1]);
    std::string rosTopicName = std::string(argv[2]);
    std::string lcmTopicName = std::string(argv[3]);

    ros::init(argc, argv, rosNodeName);
    ros2lcm::RosToLcm rosToLcm(rosTopicName, lcmTopicName);
    ros::spin();
    return 0;
}

