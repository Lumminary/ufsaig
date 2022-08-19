/*
 * ImageTransfomer.h
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#ifndef TRANSFORMER_IMAGETRANSFOMER_H_
#define TRANSFORMER_IMAGETRANSFOMER_H_

#include <memory>
#include "DataProvider.h"
#include "model/Image.h"
#include "model/ImageVersion.h"
#include "repos/ImageStore.h"
#include "model/Tile.h"
#include "model/Rect.h"
#include "model/Sprite.h"

namespace ImageServer {

class ImageRenderer;
using RendererPtr = std::shared_ptr<ImageRenderer>;

class ImageRenderer {
public:
    static RendererPtr
    create(const std::string& type,
           size_t numThreads,
           ImageStorePtr imageStore);

    DataProviderPtr aspectFit(const ImageVersion& original,
                            const DataProviderPtr& origData,
                            const Size& box,
                            const std::string& outFormat) {
        Size origSize{ original.width, original.height };
        return aspectFit(origSize, origData, box, outFormat);
    };

    virtual DataProviderPtr aspectFit(const Size& origSize,
                            const DataProviderPtr& origData,
                            const Size& box,
                            const std::string& outFormat) = 0;

    virtual DataProviderPtr renderSprite(const Sprite& sprite,
            const std::string& background,
            const std::string& outFormat,
            double scale) = 0;

    virtual ~ImageRenderer() = default;
};

} /* namespace ImageServer */

#endif /* TRANSFORMER_IMAGETRANSFOMER_H_ */
