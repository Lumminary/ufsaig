/*
 * CreateOpts.h
 *
 *  Created on: Jan 22, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_CREATEOPTS_H_
#define COUCHBASE_CREATEOPTS_H_

#include <libcouchbase/couchbase.h>
#include <string>

namespace Couchbase {

class CreateOpts {
public:
    CreateOpts(lcb_INSTANCE_TYPE type = LCB_TYPE_BUCKET);
    virtual ~CreateOpts();

    CreateOpts(const CreateOpts& other) = delete;

    CreateOpts& connectionString(const std::string &str);
    CreateOpts& credentials(const std::string &username,
            const std::string &password);
    CreateOpts& bucket(const std::string &bucketName);

    lcb_CREATEOPTS* rawOptions() const {
        return m_options;
    }
private:
    lcb_CREATEOPTS *m_options = nullptr;
};

} /* namespace couchbase */

#endif /* COUCHBASE_CREATEOPTS_H_ */
