/*
 * Connection.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#include "Connection.h"
#include <stdexcept>
#include "CreateOpts.h"
#include "Command.h"

namespace Couchbase {

//------------------------------------------------------------------------------

Connection::Connection(const CreateOpts &options) {
    lcb_STATUS res = lcb_create(&m_instance, options.rawOptions());
    if (LCB_SUCCESS != res)
        throw std::runtime_error(
                "Unable to instantiate a couchbase connection.");

    res = lcb_connect(m_instance);
    if (LCB_SUCCESS != res)
        throw std::runtime_error("Unable to connect to couchbase.");

    lcb_wait(m_instance, LCB_WAIT_DEFAULT);
    res = lcb_get_bootstrap_status(m_instance);
    if (LCB_SUCCESS != res)
        throw std::runtime_error(
                "Couchbase connection error after default timeout.");

    lcb_install_callback(m_instance, LCB_CALLBACK_GET,
            (lcb_RESPCALLBACK) Command::onRead);

    lcb_install_callback(m_instance, LCB_CALLBACK_STORE,
            (lcb_RESPCALLBACK) Command::onWrite);
}

//------------------------------------------------------------------------------

Connection::Connection(Connection &&other) {
    std::swap(m_instance, other.m_instance);
}

//------------------------------------------------------------------------------

Connection::~Connection() {
    lcb_destroy(m_instance);
}

//------------------------------------------------------------------------------

Connection Connection::create(lcb_INSTANCE_TYPE type,
        const std::string &connStr,
        const std::string &user,
        const std::string &password,
        const std::string &bucket) {
    CreateOpts options{ type };
    options.connectionString(connStr).credentials(user, password);
    if (LCB_TYPE_BUCKET == type)
        options.bucket(bucket);

    return Connection{ options };
}

} /* namespace couchbase */
