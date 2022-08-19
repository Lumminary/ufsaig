/*
 * SpriteController.h
 *
 *  Created on: Feb 20, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SPRITE_CONTROLLER_SPRITECONTROLLER_H_
#define SPRITE_CONTROLLER_SPRITECONTROLLER_H_

#include <memory>
#include "repos/ImageStore.h"
#include "renderer/ImageRenderer.h"
#include "model/Sprite.h"
#include <vector>
#include <utility>

namespace ImageServer {

class SpriteController {
public:
    ImageStorePtr imageStore;
    RendererPtr renderer;

    void saveSprite(
            const std::string& key,
            const Sprite& sprite,
            const std::string& background,
            const Json::Value& originals);

    std::string
    spriteCSS(const std::string& key, const std::string& classPrefix);

    size_t dpiScaling = 3; //Default value
private:
    void
    saveResolution(const std::string& spriteKey,
            const DataProviderPtr& spriteData,
            const Sprite& sprite, Json::Value& images,
            const std::string background,
            const std::pair<size_t, size_t>& scale);
};

using SpriteControllerPtr = std::shared_ptr<SpriteController>;

} /* namespace ImageServer */

#endif /* SPRITE_CONTROLLER_SPRITECONTROLLER_H_ */
