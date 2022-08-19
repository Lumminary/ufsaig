/*
 * ImageMeta.h
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_CORE_ENDPOINTS_IMAGEMETA_H_
#define SERVICES_CORE_ENDPOINTS_IMAGEMETA_H_

#include "utils/Injected.hpp"
#include <httpserver.hpp>
#include "repos/ImageStore.h"

namespace ImageServer {

namespace Core {

class ImageMeta : public httpserver::http_resource {
public:
    Injected<ImageStore> imageStore;

    ImageMeta() {
        disallow_all();
        set_allowing("GET", true);
    }

    const std::shared_ptr<httpserver::http_response>
    render(const httpserver::http_request& req) override;
};


}

}


#endif /* SERVICES_CORE_ENDPOINTS_IMAGEMETA_H_ */
