/*
 * ImageGetter.cpp
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageGetter.h"
#include <algorithm>
#include <sstream>
#include "utils/mime.h"
#include "utils/ImageResponse.h"
#include "utils/convert.h"

namespace ImageServer {

namespace Core {

using namespace Couchbase;
using namespace httpserver;
using namespace Utils;

//------------------------------------------------------------------------------

const std::shared_ptr<http_response>
ImageGetter::render(const http_request& req) {
    try {
        std::string key = req.get_arg("key");
        std::string widthStr = req.get_arg("width");
        std::string heightStr = req.get_arg("height");
        std::string format = req.get_arg("format");
        std::transform(format.begin(),
                format.end(),
                format.begin(),
                [](char c) {
                    return tolower(c);
                });

        std::string save = req.get_arg("save");

        ImageDescriptor desc;
        desc.imageKey = key;

        if (widthStr.empty() || heightStr.empty() || format.empty()) {
            desc.original = true;
        } else {
            desc.width = std::stol(widthStr);
            desc.height = std::stol(heightStr);
            desc.type = format;
            desc.save = (save == "save");
        }

        std::string mime = Utils::mimeType(format);

        DataProviderPtr data = imageController->getImage(desc);
        return imageResponse(data, mime);
   } catch(std::exception& e) {
        return std::make_shared<string_response>(
                e.what(), 400, "text/plain");
   }
}

}

}

