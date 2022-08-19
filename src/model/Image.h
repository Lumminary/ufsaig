/*
 * Image.h
 *
 *  Created on: Jan 28, 2021
 *      Author: Sergii Kutnii
 */

#ifndef MODEL_IMAGE_H_
#define MODEL_IMAGE_H_

#include <vector>
#include <string>
#include <json/json.h>
#include <memory>
#include "DataProvider.h"

namespace ImageServer {

struct ImageVersion;

struct Image {
    std::string key;
    std::vector<ImageVersion> versions;

    uint64_t createdTimestamp = 0;
    uint64_t updatedTimestamp = 0;

    Image() = default;
    Image(const Json::Value& json);
    Image(const Image& other) = default;

    Json::Value json() const;
    DataProviderPtr data() const;

    ImageVersion& addVersion(const ImageVersion& versionTemplate);
};

using ImagePtr = std::shared_ptr<Image>;

} /* namespace ImageServer */

#endif /* MODEL_IMAGE_H_ */
