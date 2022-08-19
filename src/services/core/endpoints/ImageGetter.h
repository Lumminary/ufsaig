/*
 * ImageGetter.h
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_CORE_IMAGEGETTER_H_
#define SERVICES_CORE_IMAGEGETTER_H_

#include "utils/Injected.hpp"
#include <httpserver.hpp>
#include "couchbase/client.h"
#include "repos/ImageStore.h"
#include "DataProvider.h"
#include "renderer/ImageRenderer.h"
#include "image_controller/ImageController.h"

namespace ImageServer {

namespace Core {

//------------------------------------------------------------------------------

class ImageGetter : public httpserver::http_resource {
public:
    Injected<ImageController> imageController;

    ImageGetter() {
        disallow_all();
        set_allowing("GET", true);
    }

    const std::shared_ptr<httpserver::http_response>
    render(const httpserver::http_request& req) override;
};


}

}



#endif /* SERVICES_CORE_IMAGEGETTER_H_ */
