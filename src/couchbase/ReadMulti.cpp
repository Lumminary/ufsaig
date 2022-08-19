/*
 * ReadMulti.cpp
 *
 *  Created on: Apr 27, 2021
 *      Author: Sergii Kutnii
 */

#include "ReadMulti.h"

namespace Couchbase {

//------------------------------------------------------------------------------

ReadMulti::ReadMulti(const std::vector<std::string> &keys) {
    for (const std::string& key : keys) {
        m_operations.emplace(key, key);
    }
}

//------------------------------------------------------------------------------

void ReadMulti::schedule(lcb_INSTANCE *connection) {
    if (m_operations.empty())
        return;
    
    lcb_sched_enter(connection);

    for (auto& entry : m_operations) {
        entry.second.schedule(connection);
    }

    lcb_sched_leave(connection);
}

//------------------------------------------------------------------------------

void ReadMulti::reject(const std::string &reason) {
}

//------------------------------------------------------------------------------

GetResults ReadMulti::getResults() {
    GetResults results;
    results.reserve(m_operations.size());

    for (auto& entry : m_operations) {
        try {
            EntryPtr result = entry.second.result().get();
            results.emplace(entry.first, result);
        } catch (...) {
            continue;
        }
    }

    return results;
}

} /* namespace Couchbase */
