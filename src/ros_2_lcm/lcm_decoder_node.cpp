/*
 * lcm_decoder_node.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: jelavice
 */


#include <iostream>
#include <memory>
#include <lcm/lcm-cpp.hpp>
#include "BinaryBlob.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "BinaryBlob.hpp"

//TODO move this as a separate node


static const std::string OPENCV_WINDOW = "Image window";


class ImageDecoder {

    int popIntFromBlob(const idsc::BinaryBlob& blob){

        char num[4];
        for (int i=m_currentIndex; i < m_currentIndex + 4; i++)
            num[i - m_currentIndex] = blob.data.at(i);

        m_currentIndex += 4;
        int retVal = 0;
        retVal = num[0];
        retVal &= 0xFF;

        retVal |= (num[1] << 8);
        retVal &= 0xFFFF;

        retVal |= (num[2] << 16);
        retVal &= 0xFFFFFF;

        retVal |= (num[3] << 24);
        retVal &= 0xFFFFFFFF;

        return retVal;
    }

    void resetCurrentIndex() {m_currentIndex = 0;}

    int m_currentIndex = 0;

public:
    ImageDecoder() {


        cv::namedWindow(OPENCV_WINDOW);
        std::cout << "Initialized." << std::endl;
    }

    ~ImageDecoder() {
        cv::destroyWindow(OPENCV_WINDOW);
    }

    void decodeImage(const idsc::BinaryBlob& msg) {



        //std::cout << "Size in bytes: " << sizeInBytes << std::endl;


        int headerSizeInBytes = 16;

        int rows = popIntFromBlob(msg);
        int cols = popIntFromBlob(msg);
        int channels = popIntFromBlob(msg);;
        int type = popIntFromBlob(msg);; //TODO make this better


        std::cout << "Num rows: " << rows << std::endl;
        std::cout << "Num cols: " << cols << std::endl;
        std::cout << "Num channels: " << channels << std::endl;
        std::cout << "Num type: " << type << std::endl;


        cv::Mat image(rows,cols, CV_8UC3); //TODO hella magic number



        cv::MatIterator_<cv::Vec3b> it, end;
        for (it = image.begin<cv::Vec3b>(), end = image.end<cv::Vec3b>(); it != end; ++it) {
            unsigned char g =  msg.data.at(m_currentIndex++);
            unsigned char r =  msg.data.at(m_currentIndex++);
            unsigned char b =  msg.data.at(m_currentIndex++);
            (*it)[0] = g;
            (*it)[1] = r;
            (*it)[2] = b;
        }







        // Update GUI Window
        cv::imshow(OPENCV_WINDOW, image);
        cv::waitKey(3);

        // Output modified video stream
        //image_pub_.publish(cv_ptr->toImageMsg());
        std::cout << "Decoded!" <<std::endl;
        resetCurrentIndex();
    }
};

class Handler
{
    ImageDecoder m_imgDecoder;
    public:
        ~Handler() {}

        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan,
                const idsc::BinaryBlob* msg)
        {

            m_imgDecoder.decodeImage(*msg);
            std::cout << "Binary blob received! " << std::endl;
            std::cout << "Binary blob size: " << msg->data.size() << std::endl;
        }
};

int main( int argc, char** argv) {


    lcm::LCM lcm;

    if(!lcm.good())
        return 1;

    Handler handlerObject;
    lcm.subscribe("pylon_camera_lcm", &Handler::handleMessage, &handlerObject);

    std::cout << "Registered." << std::endl;

    while(0 == lcm.handle());

    return 0;
}
