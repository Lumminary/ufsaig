/*
 * Main.h
 *
 *  Created on: Jan 29, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_MAIN_H_
#define SERVICES_MAIN_H_

#include "repos/ImageStore.h"
#include "services/HttpService.h"
#include "renderer/ImageRenderer.h"
#include "sprite_controller/SpriteController.h"
#include "image_controller/ImageController.h"

namespace ImageServer {

namespace Core {

class Root;
using RootPtr = std::shared_ptr<Root>;

class Root : public HttpService {
public:
    //Dependencies
    ImageStorePtr imageStore;
    RendererPtr transformer;
    SpriteControllerPtr spriteController;
    ImageControllerPtr imageController;

    static RootPtr create();
};

}

} /* namespace ImageServer */

#endif /* SERVICES_MAIN_H_ */
