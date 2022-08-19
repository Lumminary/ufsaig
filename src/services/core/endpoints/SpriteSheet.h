/*
 * SpriteSheet.h
 *
 *  Created on: Mar 2, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_CORE_ENDPOINTS_SPRITESHEET_H_
#define SERVICES_CORE_ENDPOINTS_SPRITESHEET_H_

#include "utils/Injected.hpp"
#include <httpserver.hpp>
#include "repos/ImageStore.h"

namespace ImageServer {

class SpriteSheet : public httpserver::http_resource {
public:
    Injected<ImageStore> imageStore;

    SpriteSheet() {
        disallow_all();
        set_allowing("GET", true);
    }

    const std::shared_ptr<httpserver::http_response>
    render(const httpserver::http_request& req) override;
};

} /* namespace ImageServer */

#endif /* SERVICES_CORE_ENDPOINTS_SPRITESHEET_H_ */
