/*
 * MagickRenderer.h
 *
 *  Created on: May 15, 2021
 *      Author: Sergii Kutnii
 */

#ifndef RENDERER_MAGICKRENDERER_H_
#define RENDERER_MAGICKRENDERER_H_

#include "ImageRenderer.h"
#include <Magick++.h>
#include "utils/ThreadPool.h"

namespace ImageServer {

class MagickRenderer : public ImageRenderer {
public:
    MagickRenderer(size_t nThreads) : m_executor{ nThreads } {}

    DataProviderPtr aspectFit(const Size& origSize,
                                const DataProviderPtr& origData,
                                const Size& box,
                                const std::string& outFormat) override;

    DataProviderPtr renderSprite(const Sprite& sprite,
            const std::string& background,
            const std::string& outFormat,
            double scale) override;

    Magick::DrawableCompositeImage
    renderTile(const Tile& tile, double scale);

    ImageStorePtr imageStore = nullptr;

private:
    Utils::ThreadPool m_executor;
};


}

#endif /* RENDERER_MAGICKRENDERER_H_ */
