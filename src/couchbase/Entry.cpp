/*
 * Entry.cpp
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#include "Entry.h"
#include <memory>

namespace Couchbase {

Json::Value Entry::json() {
    const char *data = content.data();
    Json::CharReaderBuilder readerBuilder;
    Json::Value json;
    std::string errs;
    std::unique_ptr<Json::CharReader> reader{ readerBuilder.newCharReader() };
    reader->parse(data,
            data + content.size(), &json, &errs);

    return json;
}

} /* namespace Couchbase */
