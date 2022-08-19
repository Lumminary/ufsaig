/*
 * SpriteCSS.cpp
 *
 *  Created on: Mar 5, 2021
 *      Author: Sergii Kutnii
 */

#include "SpriteCSS.h"

namespace ImageServer {

using namespace httpserver;

const std::shared_ptr<httpserver::http_response>
SpriteCSS::render(const httpserver::http_request &req) {
    try {
        std::string key = req.get_arg("key");
        std::string classPrefix = req.get_arg("classPrefix");
        if (classPrefix.empty())
            classPrefix = "sp1";
        return std::make_shared<string_response>(
                        spriteController->spriteCSS(key, classPrefix),
                        http::http_utils::http_ok,
                        "text/css");
    } catch (std::exception& e) {
        return std::make_shared<string_response>(
                e.what(), 400, "text/plain");
    }
}

} /* namespace ImageServer */
