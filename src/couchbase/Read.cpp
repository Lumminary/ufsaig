/*
 * Read.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#include "Read.h"
#include "Entry.h"

namespace Couchbase {

//------------------------------------------------------------------------------

Read::Read(const std::string &key) {
    lcb_cmdget_create(&m_command);
    lcb_cmdget_key(m_command, key.c_str(), key.size());
}

//------------------------------------------------------------------------------

void Read::schedule(lcb_INSTANCE *connection) {
    if (cancelled())
        return;

    lcb_get(connection, (void*) this, m_command);
}

//------------------------------------------------------------------------------

void Read::complete(const lcb_RESPGET *response) {
    lcb_STATUS status = lcb_respget_status(response);
    if (LCB_SUCCESS != status) {
        std::runtime_error error(lcb_strerror_short(status));
        m_promise.set_exception(std::make_exception_ptr(error));
    } else {
        try {
            const char *value = nullptr;
            size_t valueSize = 0;

            lcb_respget_value(response, &value, &valueSize);
            EntryPtr result = std::make_shared<Entry>();

            result->content.assign(value, value + valueSize);

            lcb_respget_datatype(response, &result->datatype);
            lcb_respget_flags(response, &result->flags);

            m_promise.set_value(result);
        } catch (...) {
            m_promise.set_exception(std::current_exception());
        }
    }
}

//------------------------------------------------------------------------------

Read::~Read() {
    lcb_cmdget_destroy(m_command);
}

//------------------------------------------------------------------------------

void Read::reject(const std::string &reason) {
    std::runtime_error err(reason);
    m_promise.set_exception(std::make_exception_ptr(err));
}

}
