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
//#include <sensor_msgs/CameraInfo.h>
//#include <sensor_msgs/image_encodings.h>

#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//TODO add new lcm type
//#include "exlcm/example_t.hpp"


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

 public:
   ImageConverter()
     : it_(nh_)
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
       std::cout << "shit received." << std::endl;
     cv_bridge::CvImagePtr cv_ptr;
     try
     {
       //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       cv_ptr = cv_bridge::toCvCopy(msg, "");

     }
     catch (cv_bridge::Exception& e)
     {
       ROS_ERROR("cv_bridge exception: %s", e.what());
       return;
     }

     // Draw an example circle on the video stream
//     if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
//       cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

     // Update GUI Window
     cv::imshow(OPENCV_WINDOW, cv_ptr->image);
     cv::waitKey(3);

     // Output modified video stream
     image_pub_.publish(cv_ptr->toImageMsg());
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

