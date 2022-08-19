/*
 * Worker.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#include "Worker.h"
#include "Connection.h"
#include "CreateOpts.h"
#include <vector>
#include "Command.h"
#include <iostream>
#include "Config.h"
#include "Read.h"
#include "Write.h"
#include "ReadMulti.h"
#include "WriteMulti.h"

namespace Couchbase {

//------------------------------------------------------------------------------

Worker::~Worker() {
    {
        std::unique_lock<std::mutex> lock{ m_queueMutex };
        m_close = true;;
    }

    m_semaphore.notify_one();
    m_thread.join();
}

//------------------------------------------------------------------------------

void Worker::schedule(const CommandPtr &command) {
    {
        std::unique_lock<std::mutex> lock{ m_queueMutex };

        if (m_close) {
            command->reject("Couchbase connection is being closed.");
        } else {
            m_commandQueue.push_back(command);
        }
    }

    m_semaphore.notify_one();
}

//------------------------------------------------------------------------------

Worker::Worker(lcb_INSTANCE_TYPE type,
        const std::string &connStr,
        const std::string &user,
        const std::string &password,
        const std::string &bucket) :
                m_connection {
                    Connection::create(type, connStr, user, password, bucket)
                }
{
}

//------------------------------------------------------------------------------

Worker::Worker(const Config &config) :
    Worker{
        config.cluster ? LCB_TYPE_CLUSTER : LCB_TYPE_BUCKET,
        config.connStr,
        config.user,
        config.password,
        config.bucket
    }
{
    if (config.timeout > 0) {
        lcb_U32 timeout = config.timeout;
        lcb_cntl(m_connection.m_instance,
                LCB_CNTL_SET, LCB_CNTL_OP_TIMEOUT, &timeout);
    }

    m_thread = std::thread{ [&]() {
        execute();
    } };
}

//------------------------------------------------------------------------------

bool Worker::hasEntry(const std::string &key) {
    try {
        return (findEntry(key) != nullptr);
    } catch(...) {
        return false;
    }
}

//------------------------------------------------------------------------------

GetResults Worker::findEntries(const std::vector<std::string> &keys) {
    std::shared_ptr<ReadMulti> batch = std::make_shared<ReadMulti>(keys);
    schedule(batch);
    return batch->getResults();
}

//------------------------------------------------------------------------------

void Worker::saveEntries(const DataMap &entries) {
    std::shared_ptr<WriteMulti> batch = std::make_shared<WriteMulti>(entries);
    schedule(batch);
    batch->wait();
}

//------------------------------------------------------------------------------

void Worker::execute() {
    while (true) {
        std::vector<CommandPtr> batch;

        //Synchronized section
        {
            std::unique_lock<std::mutex> lock{ m_queueMutex };

            m_semaphore.wait(lock, [&]() -> bool {
                return (!m_commandQueue.empty() || m_close);
            });

            batch.assign(m_commandQueue.begin(), m_commandQueue.end());
            m_commandQueue.clear();

            if (m_close) {
                for (CommandPtr& cmd : batch) {
                    if (!cmd->cancelled()) {
                        cmd->reject(
                             "Couchbase connection is being closed.");
                    }
                }

                return;
            }
        }

        for (CommandPtr& command: batch) {
            if (!command->cancelled()) {
                command->schedule(m_connection.m_instance);
            }
        }

        lcb_wait(m_connection.m_instance, LCB_WAIT_DEFAULT);
    }
}

//------------------------------------------------------------------------------

Couchbase::EntryPtr Worker::findEntry(const std::string &key) {
    std::shared_ptr<Read> findCmd = std::make_shared<Read>(key);
    schedule(findCmd);

    return findCmd->result().get();
}

//------------------------------------------------------------------------------

void Worker::saveData(const std::string &key,
        const char *data, size_t length) {
    std::shared_ptr<Write> storeCmd = std::make_shared<Write>(key);
    storeCmd->setValue(data, length);
    schedule(storeCmd);

    storeCmd->result().get();
}

} /* namespace couchbase */
