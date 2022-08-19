/*
 * Root.cpp
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#include "Uploads.h"
#include "endpoints/ImageUpload.h"

namespace ImageServer {

namespace Uploads {

using namespace httpserver;

class RootImpl : public Root {
public:

    RootImpl(const std::string& resPath) : m_upload{ resPath } {
        m_upload.imageStore.inject([&]() -> ImageStore* {
            return imageStore.get();
        });
    }

    void attach(webserver& engine,
            const std::string& prefix = "") override;

private:
    ImageUpload m_upload;
};

//------------------------------------------------------------------------------

void RootImpl::attach(webserver &engine, const std::string &prefix) {
    engine.register_resource(prefix + "/image", &m_upload);
}

//------------------------------------------------------------------------------

RootPtr Root::create(const std::string &resourcePath) {
    return std::make_shared<RootImpl>(resourcePath);
}

} /* namespace Uploads */

} /* namespace ImageServer */
