/*
 * lcm_decoder.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: jelavice
 */

#include "ros_2_lcm/lcm_decoder.h"

void LcmImageDecoder::decodeImage(const idsc::BinaryBlob& msg) {

    //std::cout << "Size in bytes: " << sizeInBytes << std::endl;

    int headerSizeInBytes = 16;

    int rows = popIntFromBlob(msg);
    int cols = popIntFromBlob(msg);
    int channels = popIntFromBlob(msg);
    int encoding = popIntFromBlob(msg);

#ifdef DEBUG
    std::cout << "Num rows: " << rows << std::endl;
    std::cout << "Num cols: " << cols << std::endl;
    std::cout << "Num channels: " << channels << std::endl;
    std::cout << "Num type: " << encoding << std::endl;
#endif

    cv::Mat image(rows, cols, encoding + (channels - 1) * 8); //reference for this: http://ninghang.blogspot.ch/2012/11/list-of-mat-type-in-opencv.html

    cv::MatIterator_<cv::Vec3b> it, end;
    //TODO Make this work for any data type
    for (it = image.begin<cv::Vec3b>(), end = image.end<cv::Vec3b>(); it != end; ++it) {
        unsigned char g = msg.data.at(m_currentIndex++);
        unsigned char r = msg.data.at(m_currentIndex++);
        unsigned char b = msg.data.at(m_currentIndex++);
        (*it)[0] = g;
        (*it)[1] = r;
        (*it)[2] = b;
    }

    // Update GUI Window
#ifdef VISUALIZATION
    cv::imshow(OPENCV_WINDOW, image);
    cv::waitKey(3);
#endif

    resetCurrentIndex();
}

int LcmImageDecoder::popIntFromBlob(const idsc::BinaryBlob& blob) {

    char num[4];
    for (int i = m_currentIndex; i < m_currentIndex + 4; i++)
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

void LcmImageDecoder::handleMessage(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const idsc::BinaryBlob* msg) {

        decodeImage(*msg);
#ifdef DEBUG
        std::cout << "Binary blob received! " << std::endl;
        std::cout << "Binary blob size: " << msg->data.size() << std::endl;
#endif
    }
