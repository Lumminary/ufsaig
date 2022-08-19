/*
 * Write.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_WRITE_H_
#define COUCHBASE_WRITE_H_

#include "Command.h"
#include <future>

namespace Couchbase {

class Write : public Command {
public:
    Write(const std::string& key,
          lcb_STORE_OPERATION operation = LCB_STORE_UPSERT);

    void setValue(const char *value, size_t length);

    virtual ~Write();

    void schedule(lcb_INSTANCE *connection) override;
    void reject(const std::string& reason) override;

    void complete(const lcb_RESPSTORE *response);

    std::shared_future<void> result() { return m_result; }

private:
    lcb_CMDSTORE *m_command = nullptr;

    std::promise<void> m_promise;
    std::shared_future<void> m_result = m_promise.get_future().share();
};

} /* namespace Couchbase */

#endif /* COUCHBASE_WRITE_H_ */

