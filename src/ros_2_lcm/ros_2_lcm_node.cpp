/*
 * toLCM_node.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: jelavice
 */

#include <bitset>
#include <iostream>
#include <memory>
#include <lcm/lcm-cpp.hpp>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
//#include <sensor_msgs/CameraInfo.h>
//#include <sensor_msgs/image_encodings.h>

#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//TODO add types and run some inf of message generation
#include "BinaryBlob.hpp"

void callback(const sensor_msgs::Image::ConstPtr& msg) {
    std::cout << "shit received." << std::endl;
}

static const std::string OPENCV_WINDOW = "Image window";


void int2charArray(int num, char* array){

    array[0] = num & 0xff;
    array[1] = (num>>8)  & 0xff;
    array[2] = (num>>16) & 0xff;
    array[3] = (num>>24) & 0xff;
}

void addIntToBlob(int num, idsc::BinaryBlob& blob){

    char a[4];
    int2charArray(num, a);
    for (int i=0; i < 4; i++)
        blob.data.push_back(a[i]);

}

class ImageConverter {
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;
    std::unique_ptr<lcm::LCM> m_lcmPtr;

public:
    ImageConverter() :
            it_(nh_), m_lcmPtr(new lcm::LCM()) {
        // Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("/color_camera_node/image_raw", 1, &ImageConverter::imageCb, this);
        image_pub_ = it_.advertise("/image_converter/output_video", 1);

        cv::namedWindow(OPENCV_WINDOW);
    }

    ~ImageConverter() {
        cv::destroyWindow(OPENCV_WINDOW);
    }

    void imageCb(const sensor_msgs::ImageConstPtr& msg) {

        cv_bridge::CvImagePtr cv_ptr;
        try {
            //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8); //photos are still blueish

        } catch (cv_bridge::Exception& e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }


        //TODO distinguidh different cases depending on the image format
//        std::cout << "received image, size is: " << cv_ptr->image.rows << " x " << cv_ptr->image.cols << std::endl;
//        if (cv_ptr->image.depth() == CV_8U)
//            std::cout << "Unsigned char image" << std::endl;
//        std::cout << "Its got: " << cv_ptr->image.channels() << " channels." << std::endl;
//        std::cout << "Step: " << step << std::endl;
//        std::cout << "input: " << (int) input[0] << std::endl;
//        std::cout << "MEmory is : " << cv_ptr->image.isContinuous() << std::endl;

        if (m_lcmPtr->good() == false)
            throw std::runtime_error("runtime_error");

        //convert image and send it

        int sizeInBytes;
        if (cv_ptr->image.isContinuous())
            sizeInBytes = cv_ptr->image.total() * cv_ptr->image.elemSize();
        else
            sizeInBytes = cv_ptr->image.step[0] * cv_ptr->image.rows;

        //std::cout << "Size in bytes: " << sizeInBytes << std::endl;

        idsc::BinaryBlob image;

        int headerSizeInBytes = 16;

        int rows = cv_ptr->image.rows;
        int cols = cv_ptr->image.cols;
        int channels = 3;
        int type = 0; //TODO make this better
        image.data_length = sizeInBytes + headerSizeInBytes; //segfaults if you don't set the size to 0

        addIntToBlob(rows, image);
        addIntToBlob(cols, image);
        addIntToBlob(channels, image);
        addIntToBlob(type, image);



        cv::MatIterator_<cv::Vec3b> it, end;
        for (it = cv_ptr->image.begin<cv::Vec3b>(), end = cv_ptr->image.end<cv::Vec3b>(); it != end; ++it) {
            unsigned char g = (*it)[0];
            unsigned char r = (*it)[1];
            unsigned char b = (*it)[2];
            image.data.push_back(b);
            image.data.push_back(g);
            image.data.push_back(r);
        }
        //std::cout << "Vector size: " << image.data.size() << std::endl;
        m_lcmPtr->publish("pylon_camera_lcm", &image);

        // Update GUI Window
        cv::imshow(OPENCV_WINDOW, cv_ptr->image);
        cv::waitKey(3);

        // Output modified video stream
        //image_pub_.publish(cv_ptr->toImageMsg());
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "image_converter");
    ImageConverter ic;
    ros::spin();
    return 0;
}

//int main(int argc, char** argv){
//
//    ros::init(argc, argv, "ros_2_lcm");
//
//    //TODO move this to the class
//    ros::NodeHandle n;
//    ros::Subscriber sub = n.subscribe("/color_camera_node/image_raw", 1000, callback);
//
//    //TODO add LCM types and stuff
//    ros::spin();
//
//    return 0;
//}

