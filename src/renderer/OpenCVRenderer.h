/*
 * OpenCVRenderer.h
 *
 *  Created on: May 14, 2021
 *      Author: Sergii Kutnii
 */

#ifndef RENDERER_OPENCVRENDERER_H_
#define RENDERER_OPENCVRENDERER_H_

#include "ImageRenderer.h"
#include "utils/ThreadPool.h"

namespace ImageServer {

class OpenCVRenderer : public ImageRenderer {
public:
    OpenCVRenderer(size_t nThreads) : m_executor{ nThreads } {}

    DataProviderPtr aspectFit(const Size& origSize,
                                const DataProviderPtr& origData,
                                const Size& box,
                                const std::string& outFormat) override;

    DataProviderPtr renderSprite(const Sprite& sprite,
            const std::string& background,
            const std::string& outFormat,
            double scale) override;

    ImageStorePtr imageStore = nullptr;

private:
    std::string encoderExtension(const std::string& format);
    Utils::ThreadPool m_executor;
};

} /* namespace ImageServer */

#endif /* RENDERER_OPENCVRENDERER_H_ */
