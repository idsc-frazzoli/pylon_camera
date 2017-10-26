/*
 * toLCM_node.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: jelavice
 */




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

void callback(const sensor_msgs::Image::ConstPtr& msg)
{
    std::cout << "shit received." << std::endl;
}



static const std::string OPENCV_WINDOW = "Image window";

 class ImageConverter
 {
   ros::NodeHandle nh_;
   image_transport::ImageTransport it_;
   image_transport::Subscriber image_sub_;
   image_transport::Publisher image_pub_;
   std::unique_ptr<lcm::LCM> m_lcmPtr;

 public:
   ImageConverter()
     : it_(nh_), m_lcmPtr(new lcm::LCM())
   {
     // Subscrive to input video feed and publish output video feed
     image_sub_ = it_.subscribe("/color_camera_node/image_raw", 1,
       &ImageConverter::imageCb, this);
     image_pub_ = it_.advertise("/image_converter/output_video", 1);





     cv::namedWindow(OPENCV_WINDOW);
   }

   ~ImageConverter()
   {
     cv::destroyWindow(OPENCV_WINDOW);
   }

   void imageCb(const sensor_msgs::ImageConstPtr& msg)
   {

       cv_bridge::CvImagePtr cv_ptr;
     try
     {
       //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);

     }
     catch (cv_bridge::Exception& e)
     {
       ROS_ERROR("cv_bridge exception: %s", e.what());
       return;
     }


     if(m_lcmPtr->good() == false)
         throw std::runtime_error("runtime_error");


     //convert image and send it
     int8_t ranges[15];
     idsc::BinaryBlob image;

     //segfaults if you don't set the size to 0
     image.data_length = 0;


     m_lcmPtr->publish("pylon_camera_lcm", &image);



     // Update GUI Window
     cv::imshow(OPENCV_WINDOW, cv_ptr->image);
     cv::waitKey(3);

     // Output modified video stream
     //image_pub_.publish(cv_ptr->toImageMsg());
   }
 };

 int main(int argc, char** argv)
 {
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

