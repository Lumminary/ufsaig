/*
 * Entry.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_ENTRY_H_
#define COUCHBASE_ENTRY_H_

#include "Decl.h"
#include <vector>
#include <json/json.h>
#include "DataProvider.h"

namespace Couchbase {

struct Entry : public DataProvider {
    Buffer content;
    uint8_t datatype;
    uint32_t flags;

    const void* bytes() const override { return content.data(); }
    size_t size() const override { return content.size(); }

    Json::Value json();
};

} /* namespace Couchbase */

#endif /* COUCHBASE_ENTRY_H_ */
