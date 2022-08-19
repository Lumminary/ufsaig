/*
 * Image.cpp
 *
 *  Created on: Jan 28, 2021
 *      Author: Sergii Kutnii
 */

#include "Image.h"
#include "ImageVersion.h"
#include "couchbase/client.h"
#include "utils/ArrayWrapper.h"
#include <filesystem>

namespace ImageServer {

//------------------------------------------------------------------------------

Image::Image(const Json::Value &json) {
    key = json["key"].asString();
    createdTimestamp = json["created_timestamp"].asUInt64();
    updatedTimestamp = json["updated_timestamp"].asUInt64();

    Json::Value copies = json["versions"];
    if (!copies.isArray())
        return;

    Json::ArrayIndex copyCount = copies.size();
    for (Json::ArrayIndex i = 0; i < copyCount; ++i)
        versions.emplace_back(copies[i]);
}

//------------------------------------------------------------------------------

Json::Value Image::json() const {
    Json::Value json;

    json["key"] = Json::Value{ key };
    json["type"] = "image";
    json["created_timestamp"] =
            Json::Value{ (Json::Value::UInt64)createdTimestamp };
    json["updated_timestamp"] =
            Json::Value{ (Json::Value::UInt64)updatedTimestamp };

    json["versions"] = Json::Value{ Json::arrayValue };

    for (const ImageVersion& copy : versions) {
        json["versions"].append(copy.json());
    }

    return json;
}

//------------------------------------------------------------------------------

DataProviderPtr Image::data() const {
    Json::StreamWriterBuilder builder;
    std::string meta = Json::writeString(builder, json());
    std::shared_ptr<Utils::ArrayWrapper<char> > dataWrapper =
            std::make_shared<Utils::ArrayWrapper<char> >();
    dataWrapper->data.assign(meta.begin(), meta.end());
    return dataWrapper;
}

//------------------------------------------------------------------------------

ImageVersion& Image::addVersion(const ImageVersion &versionTemplate) {
    versions.push_back(versionTemplate);
    ImageVersion& version = versions[versions.size() - 1];

    std::stringstream keyStream;
    keyStream << key
              << "_"
              << versionTemplate.width
              << "x"
              << versionTemplate.height;

    if (!version.extension.empty()) {
        keyStream << "_" << version.extension;
        std::filesystem::path name =
                std::filesystem::path{ version.fileName }.stem();
        name.append("." + version.extension);
        version.fileName = name.string();
    }

    version.key = keyStream.str();

    return version;
}

} /* namespace ImageServer */
