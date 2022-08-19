/*
 * Worker.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_WORKER_H_
#define COUCHBASE_WORKER_H_

#include "Decl.h"
#include <string>
#include <mutex>
#include <deque>
#include <thread>
#include "Connection.h"
#include <condition_variable>
#include "DataProvider.h"

namespace Couchbase {

class Worker {
public:

    Worker(lcb_INSTANCE_TYPE type,
           const std::string &connStr,
           const std::string &user,
           const std::string &password,
           const std::string &bucket = "");

    Worker(const Config& config);

    virtual ~Worker();

    void schedule(const CommandPtr &command);

    EntryPtr findEntry(const std::string& key);
    GetResults findEntries(const std::vector<std::string>& keys);
    void saveEntries(const DataMap& entries);
    void saveData(const std::string& key, const char *data, size_t length);

    bool hasEntry(const std::string& key);

private:

    bool m_close = false;
    std::condition_variable m_semaphore;

    /*
     * A more robust implementation based on lock-free queue
     * may be needed for better performance
     */
    std::mutex m_queueMutex;
    std::deque<CommandPtr> m_commandQueue;

    std::thread m_thread;

    Connection m_connection;

    void execute();
};

} /* namespace couchbase */

#endif /* COUCHBASE_WORKER_H_ */
