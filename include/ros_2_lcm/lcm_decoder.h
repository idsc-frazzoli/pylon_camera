/*
 * lcm_decoder.h
 *
 *  Created on: Oct 28, 2017
 *      Author: jelavice
 */

#ifndef LCM_DECODER_H_
#define LCM_DECODER_H_

#include <lcm/lcm-cpp.hpp>
#include <memory>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ros_2_lcm/BinaryBlob.hpp"

//TODO move this in a separate package

//#define DEBUG
#define VISUALIZATION
static const std::string OPENCV_WINDOW = "Image window";

class LcmImageDecoder {

public:
    LcmImageDecoder(const std::string& lcmTopicName) :
        m_currentIndex(0), m_lcmPtr(new lcm::LCM()) {

        if (m_lcmPtr == NULL || m_lcmPtr->good() == false)
            throw std::runtime_error("Lcm creation failed");

        m_lcmPtr->subscribe(lcmTopicName, &LcmImageDecoder::handleMessage, this);


#ifdef VISUALIZATION
        cv::namedWindow(OPENCV_WINDOW);
#endif
        std::cout << "LCM decoder node nitialized." << std::endl;
    }

    ~LcmImageDecoder() {

#ifdef VISUALIZATION
        cv::destroyWindow(OPENCV_WINDOW);
#endif
    }

    void handleMessage(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const idsc::BinaryBlob* msg);

    void decodeImage(const idsc::BinaryBlob& msg);

    void listen() { while (0 == m_lcmPtr->handle());}

private:
    int popIntFromBlob(const idsc::BinaryBlob& blob);

    void resetCurrentIndex() { m_currentIndex = 0; }

    int m_currentIndex;
    std::unique_ptr<lcm::LCM> m_lcmPtr;
};

#endif /* LCM_DECODER_H_ */
