/*
 * lcm_decoder_node.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: jelavice
 */


#include <iostream>
#include <memory>
#include "ros_2_lcm/lcm_decoder.h"

int main( int argc, char** argv) {

    std::string lcmTopicName(argv[1]);
    LcmImageDecoder lcmImageDecoder(lcmTopicName);

    lcmImageDecoder.listen();

    return 0;
}
