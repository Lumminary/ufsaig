/*
 * ImageUpload.cpp
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageUpload.h"
#include <filesystem>
#include "utils/MultiPartForm.h"
#include <iostream>
#include "utils/md5.h"

namespace ImageServer {
namespace Uploads {

using namespace httpserver;
using namespace Utils;

//------------------------------------------------------------------------------

ImageUpload::ImageUpload(const std::string &staticPath) {
    disallow_all();
    set_allowing("GET", true);
    set_allowing("POST", true);

    std::filesystem::path formPath =
            std::filesystem::path{ staticPath } / "image_upload.html";
    m_form = std::make_shared<file_response>(
            formPath.string(), http::http_utils::http_ok, "text/html");

    std::filesystem::path successPath =
            std::filesystem::path{ staticPath } / "upload_success.html";
    m_success = std::make_shared<file_response>(
            successPath.string(), http::http_utils::http_ok, "text/html");
}

//------------------------------------------------------------------------------

const ResponsePtr
ImageUpload::render_GET(const http_request &req) {
    return m_form;
}

//------------------------------------------------------------------------------

const ResponsePtr
ImageUpload::render_POST(const http_request &req) {
    try {
        MultiPartForm form;
        form.parse(req);

        if (form.args.empty())
            return badRequest("Empty form");

        auto iImage = form.args.find("image");
        if (iImage == form.args.end())
            return badRequest("Image not found");

        MultiPartForm::File& image =
                std::get<MultiPartForm::File>(iImage->second);

        //TODO: value escaping
        std::map<std::string, std::string> params;
        params["width"] = std::get<std::string>(form.args["width"]);
        params["height"] = std::get<std::string>(form.args["height"]);
        params["mime_type"] = std::get<std::string>(form.args["mime_type"]);
        params["md5"] = md5String((unsigned char*)image.data.data(),
                                                  image.data.size());

        imageStore->newImage(image.name,
                params, image.data.data(), image.data.size());

        return m_success;
    } catch(ImageStore::ValidationError& e) {
        return badRequest(e.what());
    } catch(std::exception& e) {
        return serverError(e.what());
    }
}

//------------------------------------------------------------------------------

const ResponsePtr ImageUpload::badRequest(const std::string &message) {
    return std::make_shared<string_response>(message, 400, "text/plain");
}

//------------------------------------------------------------------------------

const ResponsePtr ImageUpload::serverError(const std::string &message) {
    return std::make_shared<string_response>(message, 500, "text/plain");
}

} /* namespace Uploads */
} /* namespace ImageServer */
