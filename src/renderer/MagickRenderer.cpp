/*
 * MagickRenderer.cpp
 *
 *  Created on: May 15, 2021
 *      Author: Sergii Kutnii
 */

#include "MagickRenderer.h"
#include "BlobWrapper.hpp"

namespace ImageServer {

using namespace Magick;
using namespace Utils;
using namespace Couchbase;

//------------------------------------------------------------------------------

DataProviderPtr ImageServer::MagickRenderer::aspectFit(
        const Size& origSize,
        const DataProviderPtr& origData,
        const Size& box,
        const std::string& outFormat) {
    auto transform = [&]() -> BlobWrapperPtr {
        BlobWrapperPtr result = std::make_shared<BlobWrapper>();

        Blob src{ origData->bytes(), origData->size() };
        Magick::Image image{ src };

        double scaleFactor =
                std::min((double)box.width / (double)origSize.width,
                        (double)box.height / (double)origSize.height);
        Geometry destGeo{ (size_t)(origSize.width * scaleFactor),
                          (size_t)(origSize.height * scaleFactor) };
        image.scale(destGeo);
        image.write(&result->blob(), outFormat);

        return result;
    };

    //Execute on the thread pool
    return m_executor.sync<BlobWrapperPtr, decltype(transform)>(transform);
}

//------------------------------------------------------------------------------

DrawableCompositeImage
MagickRenderer::renderTile(const Tile &tile, double scale) {
    Blob tileSrc{ tile.data->bytes(), tile.data->size() };
    Magick::Image tileImage{ tileSrc };

    double boxWidth = scale * tile.bounds.size.width;
    double boxHeight = scale * tile.bounds.size.height;

    double stretchRate = std::min(boxWidth / tile.originalSize.width,
            boxHeight / tile.originalSize.height);

    double width = stretchRate * tile.originalSize.width;
    double height = stretchRate * tile.originalSize.height;

    double x = scale * tile.bounds.origin.x + 0.5 * (boxWidth - width);
    double y = scale * tile.bounds.origin.y + 0.5 * (boxHeight - height);
    return DrawableCompositeImage{ x, y, width, height, tileImage };
}


//------------------------------------------------------------------------------

DataProviderPtr
MagickRenderer::renderSprite(const Sprite &sprite,
        const std::string& background,
        const std::string& outFormat,
        double scale) {
    auto renderOp = [&]() -> DataProviderPtr {
        Color bgColor{ background };
        size_t width = sprite.bounds.size.width * scale;
        size_t height = sprite.bounds.size.height * scale;
        Geometry bounds{ width, height };

        Magick::Image canvas{ bounds, background };

        //Draw tiles
        size_t tileCount = sprite.tiles.size();
        std::vector<Drawable> drawables;
        drawables.reserve(tileCount);
        for (const Tile &tile : sprite.tiles)
            drawables.push_back(renderTile(tile, scale));

        canvas.draw(drawables);

        BlobWrapperPtr result = std::make_shared<BlobWrapper>();
        canvas.write(&result->blob(), outFormat);
        return result;
    };

    return m_executor.sync<DataProviderPtr, decltype(renderOp)>(renderOp);
}

}


