/*
 * MultiPartForm.cpp
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#include "MultiPartForm.h"
#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

namespace ImageServer {

namespace Utils {

void MultiPartForm::parse(const httpserver::http_request &request) {
    const std::string &contentType = request.get_header("content-type");

    if (contentType.empty())
    {
        return;
    }

    std::string::size_type pos = contentType.find(';');
    if (pos == std::string::npos)
        return;

    std::string type = contentType.substr(0, pos);
    std::transform(type.begin(), type.end(), type.begin(), tolower);
    if (type != "multipart/form-data")
        return;

    static const char *boundaryName = "boundary=";
    static const char *boundaryNameEnd = boundaryName + 9;
    std::string::const_iterator bStart = std::search(contentType.begin(), contentType.end(),
            boundaryName, boundaryNameEnd);

    if (bStart == contentType.end())
        return;

    const std::vector<char>& contentBuffer = request.get_content();

    bStart = bStart + 9;
    std::string::const_iterator bEnd = contentType.end();
    if (((bEnd - bStart) > 2) && (*bStart == '"')) {
        ++bStart;
        --bEnd;
    }

    parse(contentBuffer.begin(), contentBuffer.end(), bStart, bEnd);
}

}

} /* namespace ImageServer */
