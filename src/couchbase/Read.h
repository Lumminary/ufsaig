/*
 * Read.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_READ_H_
#define COUCHBASE_READ_H_

#include "Command.h"
#include <future>
#include <string>
#include <memory>
#include <vector>

namespace Couchbase {

class Read: public Command {
public:
    Read(const std::string &key);

    void schedule(lcb_INSTANCE *connection) override;
    void reject(const std::string& reason) override;
    void complete(const lcb_RESPGET *response);

    std::shared_future<EntryPtr> result() { return m_future; }

    virtual ~Read();
private:
    lcb_CMDGET *m_command = nullptr;
    std::promise<EntryPtr> m_promise;
    std::shared_future<EntryPtr> m_future = m_promise.get_future().share();
};

}

#endif /* COUCHBASE_READ_H_ */
