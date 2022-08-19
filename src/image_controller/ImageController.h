/*
 * ImageController.h
 *
 *  Created on: Apr 23, 2021
 *      Author: Sergii Kutnii
 */

#ifndef IMAGE_CONTROILLER_IMAGECONTROLLER_H_
#define IMAGE_CONTROILLER_IMAGECONTROLLER_H_

#include "renderer/ImageRenderer.h"
#include <unordered_map>
#include "DataProvider.h"
#include <vector>
#include "repos/ImageStore.h"
#include <memory>
#include "ImageDescriptor.h"

namespace ImageServer {

class ImageController {
public:
    RendererPtr renderer;
    ImageStorePtr imageStore;

    DataMap
    getImagesData(const std::vector<ImageDescriptor>& descriptors);

    DataProviderPtr
    getImage(const ImageDescriptor& desc);

private:

    struct VersionRequest {
        ImageVersion src;
        std::string type;
        bool isExact = false;

        VersionRequest() = default;
        VersionRequest(const VersionRequest& other) = default;
        VersionRequest(const Image& image, const ImageDescriptor& desc);
    };
};

using ImageControllerPtr = std::shared_ptr<ImageController>;

} /* namespace ImageServer */

#endif /* IMAGE_CONTROILLER_IMAGECONTROLLER_H_ */
