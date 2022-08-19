/*
 * mime.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#include "mime.h"
#include <algorithm>

namespace ImageServer {

namespace Utils {

std::string mimeType(const std::string &format, bool applyLowercase) {
    if (applyLowercase) {
        std::string fmt{ format };

        std::transform(format.begin(),
                format.end(),
                fmt.begin(),
                [](char c) {
                    return tolower(c);
                });

        return mimeType(fmt, false);
    }

    if (format == "jpg")
        return "image/jpeg";

    return "image/" + format;
}


}


}

