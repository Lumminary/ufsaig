/*
 * Main.cpp
 *
 *  Created on: Jan 29, 2021
 *      Author: Sergii Kutnii
 */

#include "Core.h"
#include "couchbase/client.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include "endpoints/ImageGetter.h"
#include "endpoints/ImageMeta.h"
#include "endpoints/SpriteMaker.h"
#include "endpoints/SpriteSheet.h"
#include "endpoints/SpriteCSS.h"
#include "endpoints/Base64Images.h"

namespace ImageServer {

namespace Core {

//------------------------------------------------------------------------------

using namespace httpserver;
using namespace Couchbase;

//------------------------------------------------------------------------------

class RootImpl : public Root {
public:
    RootImpl();
    void attach(webserver& engine, const std::string& prefix = "") override;

private:
    ImageMeta m_meta;
    ImageGetter m_img;
    SpriteMaker m_spriteMaker;
    SpriteSheet m_spriteSheet;
    SpriteCSS m_CSS;
    Base64Images m_base64;
};

//------------------------------------------------------------------------------

RootImpl::RootImpl() {
    m_meta.imageStore.inject([&]() -> ImageStore* {
        return imageStore.get();
    });

    m_img.imageController.inject([&]() -> ImageController* {
        return imageController.get();
    });

    m_spriteMaker.spriteController.inject([&]() -> SpriteController* {
        return spriteController.get();
    });

    m_spriteMaker.imageController.inject([&]() -> ImageController* {
        return imageController.get();
    });

    m_spriteSheet.imageStore.inject([&]() -> ImageStore* {
        return imageStore.get();
    });

    m_CSS.spriteController.inject([&]() -> SpriteController* {
        return spriteController.get();
    });

    m_base64.imageController.inject([&]() -> ImageController* {
        return imageController.get();
    });
}

//------------------------------------------------------------------------------

void RootImpl::attach(webserver &engine,
        const std::string &prefix) {
    engine.register_resource(prefix + "/meta/{key}", &m_meta);
    engine.register_resource(prefix + "/meta/{key}/{subkey}", &m_meta);

    engine.register_resource(prefix + "/img/{key}", &m_img);
    engine.register_resource(
            prefix + "/img/{key}/{width}/{height}/{format}/{save}", &m_img);
    engine.register_resource(
            prefix + "/img/{key}/{width}/{height}/{format}", &m_img);

    engine.register_resource(
            prefix + "/sprite", &m_spriteMaker);
    m_spriteMaker.cssEndpoint = prefix + "/sprite_css";

    engine.register_resource(
            prefix + "/sprite_sheet/{metaKey}/{binKey}", &m_spriteSheet);
    engine.register_resource(
            prefix + "/sprite_css/{key}/{classPrefix}", &m_CSS);

    engine.register_resource(prefix + "/base64_images", &m_base64);
}

//------------------------------------------------------------------------------

RootPtr Root::create() {
    return std::make_shared<RootImpl>();
}

}

} /* namespace ImageServer */
