/*
 * ImageServerApp.cpp
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#include "App.h"
#include "Config.h"
#include <json/json.h>
#include <httpserver.hpp>
#include <memory>
#include "services/uploads/Uploads.h"
#include "services/core/Core.h"
#include "couchbase/client.h"
#include "repos/ImageStore.h"
#include "renderer/ImageRenderer.h"
#include <iostream>
#include "utils/Socket.h"
#include "sprite_controller/SpriteController.h"
#include "image_controller/ImageController.h"

namespace ImageServer {

//------------------------------------------------------------------------------

App::App(const std::string &configPath) : m_config{ configPath } {
    auto metaProvider =
            std::make_shared<Couchbase::Worker>( m_config.metaDataSource );

    auto dataProvider =
            std::make_shared<Couchbase::Worker>( m_config.binaryDataSource );

    auto imageStore = std::make_shared<ImageStore>();
    imageStore->metaConnector = metaProvider;
    imageStore->dataConnector = dataProvider;

    m_main = Core::Root::create();
    m_main->imageStore = imageStore;

    RendererPtr renderer =
            ImageRenderer::create(m_config.renderingEngine,
                    m_config.transformerThreadCount,
                    imageStore);
    m_main->transformer = renderer;

    SpriteControllerPtr spriteController = std::make_shared<SpriteController>();
    spriteController->renderer = renderer;
    spriteController->imageStore = imageStore;

    m_main->spriteController = spriteController;

    ImageControllerPtr imageController = std::make_shared<ImageController>();
    imageController->imageStore = imageStore;
    imageController->renderer = renderer;
    m_main->imageController = imageController;

    if (m_config.enableUploads) {
        m_uploads = Uploads::Root::create(m_config.staticPath);
        m_uploads->imageStore = imageStore;
    }
}

//------------------------------------------------------------------------------

using namespace httpserver;

void App::run() {
    Utils::Socket sock{ (unsigned int)m_config.port };

    webserver engine = create_webserver()
                         .bind_socket(sock.addr())
                         .content_size_limit(m_config.maxRequestBytes)
                         .start_method(http::http_utils::THREAD_PER_CONNECTION);

    m_main->attach(engine);
    if (m_uploads)
        m_uploads->attach(engine, "/upload");

    engine.start(true);
}

}
