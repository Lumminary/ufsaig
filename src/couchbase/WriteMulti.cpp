/*
 * WriteMulti.cpp
 *
 *  Created on: Apr 30, 2021
 *      Author: Sergii Kutnii
 */

#include "WriteMulti.h"

namespace Couchbase {

//------------------------------------------------------------------------------

WriteMulti::WriteMulti(const DataMap &dataMap) {
    m_commands.reserve(dataMap.size());
    for (auto& entry: dataMap) {
        m_commands.emplace_back(std::make_shared<Write>(entry.first));
        m_commands[m_commands.size() - 1]->setValue(
                (const char *)entry.second->bytes(), entry.second->size());
    }
}

//------------------------------------------------------------------------------

void WriteMulti::schedule(lcb_INSTANCE *connection) {
    if (m_commands.empty())
        return;
    
    lcb_sched_enter(connection);

    for (WritePtr& entry : m_commands) {
        entry->schedule(connection);
    }

    lcb_sched_leave(connection);
}

//------------------------------------------------------------------------------

void WriteMulti::reject(const std::string &reason) {
}

//------------------------------------------------------------------------------

void WriteMulti::wait() {
    for (WritePtr& command: m_commands) {
        command->result().get();
    }
}

} /* namespace Couchbase */
