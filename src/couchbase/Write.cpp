/*
 * Write.cpp
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#include "Write.h"

namespace Couchbase {

//------------------------------------------------------------------------------

Write::Write(const std::string& key,
      lcb_STORE_OPERATION operation) {
    lcb_cmdstore_create(&m_command, operation);
    lcb_cmdstore_key(m_command, key.c_str(), key.size());
}

//------------------------------------------------------------------------------

Write::~Write() {
    lcb_cmdstore_destroy(m_command);
}

//------------------------------------------------------------------------------

void Write::schedule(lcb_INSTANCE *connection) {
    lcb_store(connection, this, m_command);
}

//------------------------------------------------------------------------------

void Write::reject(const std::string &reason) {
    std::runtime_error err(reason);
    m_promise.set_exception(std::make_exception_ptr(err));
}

//------------------------------------------------------------------------------

void Write::setValue(const char *value, size_t length) {
    lcb_cmdstore_value(m_command, value, length);
}

//------------------------------------------------------------------------------

void Write::complete(const lcb_RESPSTORE *response) {
    lcb_STATUS status = lcb_respstore_status(response);
    if (LCB_SUCCESS != status) {
        std::runtime_error error(lcb_strerror_short(status));
        m_promise.set_exception(std::make_exception_ptr(error));
    } else {
        m_promise.set_value();
    }
}

} /* namespace Couchbase */
