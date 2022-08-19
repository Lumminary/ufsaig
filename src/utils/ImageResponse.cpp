/*
 * ImageResponse.cpp
 *
 *  Created on: Feb 18, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageResponse.h"

namespace ImageServer {

namespace Utils {

using namespace httpserver;

//------------------------------------------------------------------------------

struct Payload;
using PayloadPtr = std::shared_ptr<Payload>;

struct Payload {
    DataProviderPtr entry = nullptr;
    char *position;
    char *end;

    static ssize_t
    sendNextPart(PayloadPtr res, char *buf, size_t max);
};

//------------------------------------------------------------------------------

ssize_t
Payload::sendNextPart(PayloadPtr res, char *buf, size_t max) {
    ptrdiff_t rest = res->end - res->position;
    if (rest <= 0) {
        return -1;
    }

    ptrdiff_t count = std::min(rest, (ptrdiff_t)max);

    memset(buf, 0, max);

    memcpy(buf, res->position, count);
    res->position += count;
    return count;
}

//------------------------------------------------------------------------------

std::shared_ptr<http_response>
imageResponse(DataProviderPtr dataSource, const std::string& contentType) {
    PayloadPtr content = std::make_shared<Payload>();
    content->entry = dataSource;
    content->position = (char*)dataSource->bytes();
    content->end = content->position + dataSource->size();

    return std::make_shared<deferred_response<Payload> >(
            Payload::sendNextPart,
            content,
            "",
            http::http_utils::http_ok,
            contentType);
}


}

}


