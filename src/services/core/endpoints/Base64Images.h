/*
 * Base64Images.h
 *
 *  Created on: Apr 23, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_CORE_ENDPOINTS_BASE64IMAGES_H_
#define SERVICES_CORE_ENDPOINTS_BASE64IMAGES_H_

#include "utils/Injected.hpp"
#include <httpserver.hpp>
#include "couchbase/client.h"
#include "repos/ImageStore.h"
#include "DataProvider.h"
#include "renderer/ImageRenderer.h"
#include "utils/base64.h"
#include "image_controller/ImageController.h"

namespace ImageServer {

class Base64Images : public httpserver::http_resource {
public:
    Injected<ImageController> imageController;

    Base64Images() {
        disallow_all();
        set_allowing("POST", true);
    }

    const std::shared_ptr<httpserver::http_response>
    render(const httpserver::http_request& req) override;
};

} /* namespace ImageServer */

#endif /* SERVICES_CORE_ENDPOINTS_BASE64IMAGES_H_ */
