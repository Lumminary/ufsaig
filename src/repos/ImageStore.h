/*
 * ImageStore.h
 *
 *  Created on: Jan 28, 2021
 *      Author: Sergii Kutnii
 */

#ifndef STORAGE_IMAGESTORE_H_
#define STORAGE_IMAGESTORE_H_

#include "couchbase/client.h"
#include <memory>
#include "model/Image.h"
#include "model/ImageVersion.h"
#include <map>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace ImageServer {

class ImageStore {
public:
    Couchbase::WorkerPtr metaConnector = nullptr;
    Couchbase::WorkerPtr dataConnector = nullptr;

    using Params = std::map<std::string, std::string>;

    ImagePtr newImage(const std::string& name,
                   const Params& params,
                   const char *data,
                   size_t dataLength);

    ImagePtr findImage(const std::string& key);

    using Images = std::unordered_map<std::string, ImagePtr>;
    Images findImages(const std::vector<std::string>& keys);

    void addVersion(const Image& image,
                    const ImageVersion& version,
                    const char *data);

    class ValidationError : public std::runtime_error {
    public:
        ValidationError(const char *what)
                : std::runtime_error{ what } {}
    };

private:
    void saveImageInstance(const Image& image,
            const ImageVersion& instance,
            const char *data);
};

using ImageStorePtr = std::shared_ptr<ImageStore>;

} /* namespace ImageServer */

#endif /* STORAGE_IMAGESTORE_H_ */
