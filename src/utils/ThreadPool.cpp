/*
 * ThreadPool.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#include "ThreadPool.h"

namespace ImageServer {

namespace Utils {

//------------------------------------------------------------------------------

ThreadPool::ThreadPool(size_t size) {
    m_threads.reserve(size);
    for (size_t i = 0; i < size; ++i)
        m_threads.emplace_back([&]() {
            processOperations();
        });
}

//------------------------------------------------------------------------------

ThreadPool::~ThreadPool() {
    m_stop = true;

    m_semaphore.notify_all();

    for (std::thread& thread : m_threads)
        thread.join();
}

//------------------------------------------------------------------------------

void ThreadPool::processOperations() {
    while (true) {
        Operation op;

        {
            std::unique_lock<std::mutex> queueLock{ m_queueMutex };

            m_semaphore.wait(queueLock,
                    [&]() -> bool {
                        return !m_queue.empty() || m_stop;
                    });

            if (m_stop)
                return;

            if (!m_queue.empty()) {
                op = m_queue.front();
                m_queue.pop_front();
            }
        }

        if (op)
            op();
    }
}

}

} /* namespace ImageServer */
