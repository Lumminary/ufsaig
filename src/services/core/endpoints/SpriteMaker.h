/*
 * SpriteMaker.h
 *
 *  Created on: Feb 16, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_CORE_ENDPOINTS_SPRITEMAKER_H_
#define SERVICES_CORE_ENDPOINTS_SPRITEMAKER_H_

#include "utils/Injected.hpp"
#include <httpserver.hpp>
#include "repos/ImageStore.h"
#include "sprite_controller/SpriteController.h"
#include "image_controller/ImageController.h"

namespace ImageServer {

namespace Core {

class SpriteMaker : public httpserver::http_resource {
public:
    Injected<SpriteController> spriteController;
    Injected<ImageController> imageController;
    std::string cssEndpoint;

    SpriteMaker() {
        disallow_all();
        set_allowing("POST", true);
    }

    const std::shared_ptr<httpserver::http_response>
    render(const httpserver::http_request& req) override;
private:
    const std::shared_ptr<httpserver::http_response>
    makeResponse(const std:: string& spriteKey,
            const std::string& cssPrefix = "");
};

}

} /* namespace ImageServer */

#endif /* SERVICES_CORE_ENDPOINTS_SPRITEMAKER_H_ */
