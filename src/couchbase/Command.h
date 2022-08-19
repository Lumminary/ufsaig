/*
 * Command.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_COMMAND_H_
#define COUCHBASE_COMMAND_H_

#include "Decl.h"
#include <string>
#include <atomic>

namespace Couchbase {

class Command {
public:
    void cancel() {
        m_cancelled.store(true);
    }

    bool cancelled() {
        return m_cancelled.load();
    }

    virtual void schedule(lcb_INSTANCE *connection) = 0;
    virtual void reject(const std::string& reason) = 0;

    virtual ~Command() = default;

    static void onRead(lcb_INSTANCE *instance, int type,
            const lcb_RESPGET *response);

    static void onWrite(lcb_INSTANCE *instance, int type,
            const lcb_RESPSTORE *response);

private:
    std::atomic_bool m_cancelled = false;
};

} /* namespace couchbase */

#endif /* COUCHBASE_COMMAND_H_ */
