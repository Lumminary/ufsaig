/*
 * ImageTransfomer.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageRenderer.h"
#include "BlobWrapper.hpp"
#include <mutex>
#include <future>
#include "OpenCVRenderer.h"
#include "MagickRenderer.h"
#include "utils/convert.h"
#include <iostream>

namespace ImageServer {

using namespace Magick;

//------------------------------------------------------------------------------

RendererPtr ImageRenderer::create(const std::string& type,
        size_t numThreads,
        ImageStorePtr imageStore) {
    std::string lowercasedType = Utils::toLower(type);
    if (lowercasedType == "magick") {
        static std::once_flag initFlag;
        std::call_once(initFlag, []() {
            InitializeMagick(nullptr);
        });

        auto impl = std::make_shared<MagickRenderer>(numThreads);
        impl->imageStore = imageStore;

        std::cout << "Using ImageMagick renderer." << std::endl;

        return impl;
    }

    std::cout << "Using OpenCV renderer." << std::endl;
    return std::make_shared<OpenCVRenderer>(numThreads);
}

} /* namespace ImageServer */
