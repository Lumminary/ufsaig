/*
 * ImageVersion.h
 *
 *  Created on: Jan 28, 2021
 *      Author: Sergii Kutnii
 */

#ifndef MODEL_IMAGEVERSION_H_
#define MODEL_IMAGEVERSION_H_

#include <string>
#include <json/json.h>

namespace ImageServer {

struct ImageVersion {
    std::string key;
    unsigned int width = 0;
    unsigned int height = 0;
    bool original = false;
    std::string mimeType;
    std::string fileName;
    std::string extension;
    unsigned int sizeBytes = 0;
    std::string md5;

    ImageVersion() = default;
    ImageVersion(const Json::Value& json);
    ImageVersion(const ImageVersion& other) = default;

    Json::Value json() const;
};

} /* namespace ImageServer */

#endif /* MODEL_IMAGEVERSION_H_ */
