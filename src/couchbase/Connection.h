/*
 * Connection.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_CONNECTION_H_
#define COUCHBASE_CONNECTION_H_

#include "Decl.h"
#include <libcouchbase/couchbase.h>
#include <string>

namespace Couchbase {

class Connection {
public:
    Connection(const CreateOpts &options);
    Connection(const Connection& other) = delete;
    Connection(Connection&& other);

    virtual ~Connection();

    static Connection create(lcb_INSTANCE_TYPE type,
            const std::string &connStr,
            const std::string &user,
            const std::string &password,
            const std::string &bucket = "");

private:
    friend class Worker;
    lcb_INSTANCE *m_instance = nullptr;
};

} /* namespace couchbase */

#endif /* COUCHBASE_CONNECTION_H_ */
