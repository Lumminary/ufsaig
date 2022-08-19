/*
 * CreateOpts.cpp
 *
 *  Created on: Jan 22, 2021
 *      Author: Sergii Kutnii
 */

#include "CreateOpts.h"

namespace Couchbase {

//------------------------------------------------------------------------------

CreateOpts::CreateOpts(lcb_INSTANCE_TYPE type) {
    lcb_createopts_create(&m_options, type);
}

//------------------------------------------------------------------------------

CreateOpts::~CreateOpts() {
    lcb_createopts_destroy(m_options);
}

//------------------------------------------------------------------------------

CreateOpts& CreateOpts::connectionString(const std::string &str) {
    if (nullptr != m_options)
        lcb_createopts_connstr(m_options, str.c_str(), str.size());

    return *this;
}

//------------------------------------------------------------------------------

CreateOpts& CreateOpts::credentials(const std::string &username,
        const std::string &password) {
    if (nullptr != m_options)
        lcb_createopts_credentials(m_options, username.c_str(), username.size(),
                password.c_str(), password.size());

    return *this;
}

//------------------------------------------------------------------------------

CreateOpts& CreateOpts::bucket(const std::string &bucketName) {
    if (nullptr != m_options)
        lcb_createopts_bucket(m_options, bucketName.c_str(), bucketName.size());

    return *this;
}

} /* namespace couchbase */
