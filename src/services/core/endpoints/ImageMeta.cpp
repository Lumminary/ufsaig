/*
 * ImageMeta.cpp
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageMeta.h"
#include "couchbase/client.h"

namespace ImageServer {

namespace Core {

using namespace httpserver;
using namespace Couchbase;

const std::shared_ptr<http_response>
ImageMeta::render(const http_request& req) {
    try {
        std::string key = req.get_arg("key");
        std::string subkey = req.get_arg("subkey");
        EntryPtr entry = imageStore->metaConnector->findEntry(key);

        if (subkey.empty()) {
            //Don't parse, just return document content
            char *data = entry->content.data();
            std::string body{ data, data + entry->content.size() };
            return std::make_shared<string_response>(body,
                            http::http_utils::http_ok,
                            "application/json");
        } else {
            Json::Value partial = entry->json()[subkey];
            Json::StreamWriterBuilder writerBuilder;
            std::string body = Json::writeString(writerBuilder, partial);
            return std::make_shared<string_response>(body,
                            http::http_utils::http_ok,
                            "application/json");
        }
    } catch(std::exception& e) {
        return std::make_shared<string_response>(
                e.what(), 400, "text/plain");
    }
}


}

}


