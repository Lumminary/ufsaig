/*
 * SpriteCSS.h
 *
 *  Created on: Mar 5, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_CORE_ENDPOINTS_SPRITECSS_H_
#define SERVICES_CORE_ENDPOINTS_SPRITECSS_H_

#include "utils/Injected.hpp"
#include <httpserver.hpp>
#include "sprite_controller/SpriteController.h"

namespace ImageServer {

class SpriteCSS : public httpserver::http_resource {
public:
    Injected<SpriteController> spriteController;

    SpriteCSS() {
        disallow_all();
        set_allowing("GET", true);
    }

    const std::shared_ptr<httpserver::http_response>
    render(const httpserver::http_request& req) override;
};

} /* namespace ImageServer */

#endif /* SERVICES_CORE_ENDPOINTS_SPRITECSS_H_ */
