/*
 * ImageUpload.h
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_UPLOADS_ENDPOINTS_IMAGEUPLOAD_H_
#define SERVICES_UPLOADS_ENDPOINTS_IMAGEUPLOAD_H_

#include "utils/Injected.hpp"
#include <httpserver.hpp>
#include "repos/ImageStore.h"
#include <string>
#include "services/ServiceDecl.h"

namespace ImageServer {
namespace Uploads {

class ImageUpload : public httpserver::http_resource {
public:
    Injected<ImageStore> imageStore;

    ImageUpload(const std::string& staticPath);

    const ResponsePtr
    render_GET(const httpserver::http_request& req) override;

    const ResponsePtr
    render_POST(const httpserver::http_request& req) override;

    const ResponsePtr badRequest(const std::string& message);
    const ResponsePtr serverError(const std::string& message);
private:
    ResponsePtr m_success = nullptr;
    ResponsePtr m_form = nullptr;
};

} /* namespace Uploads */
} /* namespace ImageServer */

#endif /* SERVICES_UPLOADS_ENDPOINTS_IMAGEUPLOAD_H_ */
