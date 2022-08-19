/*
 * SpriteSheet.cpp
 *
 *  Created on: Mar 2, 2021
 *      Author: Sergii Kutnii
 */

#include "SpriteSheet.h"
#include "utils/mime.h"
#include "utils/ImageResponse.h"

namespace ImageServer {

using namespace httpserver;
using namespace Couchbase;

const std::shared_ptr<http_response>
SpriteSheet::render(const http_request &req) {
    try {
        std::string metaKey = req.get_arg("metaKey");
        std::string binKey = req.get_arg("binKey");

        EntryPtr metaData = imageStore->metaConnector->findEntry(metaKey);
        Json::Value meta = metaData->json();
        std::string mimeType = meta["mime_type"].asString();

        EntryPtr binData = imageStore->dataConnector->findEntry(binKey);
        return Utils::imageResponse(binData, mimeType);
    } catch(std::exception& e) {
        return std::make_shared<string_response>(
                e.what(), 400, "text/plain");
    }
}

} /* namespace ImageServer */
