/*
 * hex.cpp
 *
 *  Created on: Jan 27, 2021
 *      Author: Sergii Kutnii
 */


#include "hex.h"
#include <cstdio>
#include <iomanip>
#include <sstream>

namespace ImageServer {

namespace Utils {

//------------------------------------------------------------------------------

std::string hexify(const uuid_t uuid) {
    return hexify(uuid, 16);
}

//------------------------------------------------------------------------------

std::string hexify(const unsigned char *bytes, size_t length) {
    std::stringstream converter;
    converter << std::noshowbase
              << std::setw(1)
              << std::hex;


    for (size_t i = 0; i < length; ++i) {
        unsigned int big = bytes[i] / 16;
        unsigned int little = bytes[i] % 16;
        converter << big << little;
    }

    return converter.str();
}

}


}

