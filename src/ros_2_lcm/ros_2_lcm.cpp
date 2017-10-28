/*
 * ros_2_lcm.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: jelavice
 */

#include "ros_2_lcm/ros_2_lcm.h"

namespace ros2lcm {

void RosToLcm::imageCallBack(const sensor_msgs::ImageConstPtr& msg) {

#ifndef VISUALIZATION
    cv_bridge::CvImagePtr cv_ptr;
#else
    cv_bridge::CvImageConstPtr cv_ptr;
#endif

    try {

#ifndef VISUALIZATION
        cv_ptr = cv_bridge::toCvCopy(msg,"");
#else
        cv_ptr = cv_bridge::cvtColorForDisplay(cv_bridge::toCvShare(msg), "");
#endif

    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    if (m_lcmPtr->good() == false)
        throw std::runtime_error("runtime_error");

    int sizeInBytes;
    if (cv_ptr->image.isContinuous())
        sizeInBytes = cv_ptr->image.total() * cv_ptr->image.elemSize();
    else
        sizeInBytes = cv_ptr->image.step[0] * cv_ptr->image.rows;

    idsc::BinaryBlob image;

    //get image info
    int headerSizeInBytes = 16; //current_convention
    int rows = cv_ptr->image.rows;
    int cols = cv_ptr->image.cols;
    int channels = cv_ptr->image.channels();
    int encoding = cv_ptr->image.depth();
    image.data_length = sizeInBytes + headerSizeInBytes; //segfaults if you don't set the size to 0

#ifdef DEBUG
    std::cout << "Number of rows: " << rows << std::endl;
    std::cout << "NUmber of cols: " << cols << std::endl;
    std::cout << "Type of matrix element: " << encoding << std::endl;
    std::cout << "Number of  channels: " << channels << std::endl;
#endif

    // make the header so that we can decode it
    addIntToBlob(rows, image);
    addIntToBlob(cols, image);
    addIntToBlob(channels, image);
    addIntToBlob(encoding, image);

    cv::MatConstIterator_<cv::Vec3b> it, end;
    //TODO make this work for any data type
    for (it = cv_ptr->image.begin<cv::Vec3b>(), end = cv_ptr->image.end<cv::Vec3b>(); it != end; ++it) {
        unsigned char g = (*it)[0];
        unsigned char r = (*it)[1];
        unsigned char b = (*it)[2];
        image.data.push_back(g);
        image.data.push_back(r);
        image.data.push_back(b);
    }

    m_lcmPtr->publish(m_lcm_topic, &image);

    // Update GUI Window
#ifdef VISUALIZATION
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);
#endif
}

void RosToLcm::int2charArray(int num, char* array) {

    array[0] = num & 0xff;
    array[1] = (num >> 8) & 0xff;
    array[2] = (num >> 16) & 0xff;
    array[3] = (num >> 24) & 0xff;

#ifdef DEBUG
    std::cout << "int value in bits: " << std::bitset<32>(num) <<"   vs: chars stacked up: " << std::bitset<8>(array[3])
    << std::bitset<8>(array[2]) << std::bitset<8>(array[1]) << std::bitset<8>(array[0]) << std::endl;
#endif

}

void RosToLcm::addIntToBlob(int num, idsc::BinaryBlob& blob) {

    char a[4];
    int2charArray(num, a);
    for (int i = 0; i < 4; i++)
        blob.data.push_back(a[i]);

}

} /* ros2lcm */

