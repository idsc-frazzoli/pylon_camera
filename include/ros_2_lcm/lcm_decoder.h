/*
 * lcm_decoder.h
 *
 *  Created on: Oct 28, 2017
 *      Author: jelavice
 */

#ifndef LCM_DECODER_H_
#define LCM_DECODER_H_

#include <lcm/lcm-cpp.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "ros_2_lcm/BinaryBlob.hpp"

//TODO move this as a separate node

#define VISUALIZATION
static const std::string OPENCV_WINDOW = "Image window";


class LcmImageDecoder {

    int popIntFromBlob(const idsc::BinaryBlob& blob);

    void resetCurrentIndex() {m_currentIndex = 0;}

    int m_currentIndex = 0;

public:
    LcmImageDecoder() {

#ifdef VISUALIZATION
        cv::namedWindow(OPENCV_WINDOW);
#endif
        std::cout << "Initialized." << std::endl;
    }

    ~LcmImageDecoder() {

#ifdef VISUALIZATION
        cv::destroyWindow(OPENCV_WINDOW);
#endif
    }

    void decodeImage(const idsc::BinaryBlob& msg);
};

class Handler
{
    ImageDecoder m_imgDecoder;
    public:

        Handler() {
            lcm::LCM lcm;

               if(!lcm.good())
                   return 1;

               Handler handlerObject;
               lcm.subscribe("pylon_camera_lcm", &Handler::handleMessage, &handlerObject);

               std::cout << "Registered." << std::endl;

               while(0 == lcm.handle());
        }
        ~Handler() {}

        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan,
                const idsc::BinaryBlob* msg)
        {

            m_imgDecoder.decodeImage(*msg);
            std::cout << "Binary blob received! " << std::endl;
            std::cout << "Binary blob size: " << msg->data.size() << std::endl;
        }

    private:


};





#endif /* LCM_DECODER_H_ */
