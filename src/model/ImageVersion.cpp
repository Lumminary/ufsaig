/*
 * ImageVersion.cpp
 *
 *  Created on: Jan 28, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageVersion.h"

namespace ImageServer {

//------------------------------------------------------------------------------

ImageVersion::ImageVersion(const Json::Value &json) {
    key = json["key"].asString();
    width = json["width"].asUInt();
    height = json["height"].asUInt();
    original = json["original"].asBool();
    mimeType = json["mime_type"].asString();
    fileName = json["filename"].asString();
    extension = json["extension"].asString();
    sizeBytes = json["size_bytes"].asUInt();
    md5 = json["md5"].asString();
}

//------------------------------------------------------------------------------

Json::Value ImageVersion::json() const {
    Json::Value json;

    json["key"] = Json::Value{ key };
    json["width"] = Json::Value{ width };
    json["height"] = Json::Value{ height };
    json["mime_type"] = Json::Value{ mimeType };
    json["filename"] = Json::Value{ fileName };
    json["extension"] = Json::Value{ extension };
    json["original"] = Json::Value{ original };
    json["size_bytes"] = Json::Value{ sizeBytes };
    json["md5"] = Json::Value{ md5 };

    return json;
}

} /* namespace ImageServer */
