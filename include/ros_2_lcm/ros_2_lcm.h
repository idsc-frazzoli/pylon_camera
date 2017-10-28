/*
 * ToLCM.h
 *
 *  Created on: Oct 25, 2017
 *      Author: jelavice
 */

#ifndef ROS_2_LCM_H_
#define ROS_2_LCM_H_




#include <bitset>
#include <iostream>
#include <memory>
#include <lcm/lcm-cpp.hpp>
#include <ros/ros.h>
#include <image_transport/image_transport.h>


#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include <image_view/ImageViewConfig.h>

#include <dynamic_reconfigure/server.h>


//TODO add types and run some inf of message generation
#include "ros_2_lcm/BinaryBlob.hpp"

namespace ros2lcm {


//#define DEBUG
#define VISUALIZATION
static const std::string OPENCV_WINDOW = "Image window";

class RosToLcm {

public:

    RosToLcm(const std::string& topic_name, const std::string& lcm_topic_name) :
            m_it(m_nh), m_lcm_topic(lcm_topic_name), m_lcmPtr(new lcm::LCM()) {

        // Subscribe to input video feed and publish output video feed
        m_image_sub = m_it.subscribe(topic_name, 1, &RosToLcm::imageCallBack, this);

#ifdef VISUALIZATION
        cv::namedWindow(OPENCV_WINDOW);
#endif
        std::cout << "Ros2lcm node initialized." << std::endl;
    }

    ~RosToLcm() {
#ifdef VISUALIZATION
        cv::destroyWindow(OPENCV_WINDOW);
#endif
    }

    void imageCallBack(const sensor_msgs::ImageConstPtr& msg);

private:

    void int2charArray(int num, char* array);

    void addIntToBlob(int num, idsc::BinaryBlob& blob);

    ros::NodeHandle m_nh;
    image_transport::ImageTransport m_it;
    image_transport::Subscriber m_image_sub;
    std::unique_ptr<lcm::LCM> m_lcmPtr;
    std::string m_lcm_topic;

};


} /* ros2lcm */
#endif /* ROS_2_LCM_H_ */
