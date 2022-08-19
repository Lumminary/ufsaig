/*
 * ThreadPool.h
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_THREADPOOL_H_
#define UTILS_THREADPOOL_H_

#include <thread>
#include <functional>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <future>

namespace ImageServer {

namespace Utils {

class ThreadPool {
public:
    ThreadPool(size_t size);

    template<typename Block>
    void async(Block block) {
        if (m_stop)
            return;

        {
            std::unique_lock<std::mutex> queueLock;
            m_queue.emplace_back(block);
        }

        m_semaphore.notify_one();
    }

    template<typename Result, typename Block>
    Result sync(Block block);

    template<typename Block>
    void sync(Block block);

    ~ThreadPool();

private:
    void processOperations();

    std::vector<std::thread> m_threads;

    using Operation = std::function<void ()>;
    std::deque<Operation> m_queue;

    std::mutex m_queueMutex;
    std::condition_variable m_semaphore;

    bool m_stop = false;
};

//------------------------------------------------------------------------------

template<typename Result, typename Block>
Result ThreadPool::sync(Block block) {
    std::promise<Result> promise;

    async([&]() {
        try {
            promise.set_value(block());
        } catch(...) {
            promise.set_exception(std::current_exception());
        }
    });

    return promise.get_future().get();
}

//------------------------------------------------------------------------------

template<typename Block>
void ThreadPool::sync(Block block) {
    std::promise<void> promise;

    async([&]() {
        try {
            promise.set_value();
        } catch(...) {
            promise.set_exception(std::current_exception());
        }
    });

    promise.get_future().get();
}


}

} /* namespace ImageServer */

#endif /* UTILS_THREADPOOL_H_ */
