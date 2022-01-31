#pragma once

#include <thread>
#include <vector>
#include <utility>

class thread_pool
{
private:
    std::vector<std::thread> m_threads{};
public:
    thread_pool() = default;
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool &operator=(const thread_pool&) = delete;
    thread_pool &operator=(thread_pool&&) = delete;
    ~thread_pool() {
        release();
    };
    /**
     *  Start a function on another thread
     *
     *  @param fn function to be run on a new thread
     *  @param args arguments that will be forward to run the function
     */
    template<typename Function, typename ...Args>
    void add(Function &&fn, Args &&...args) {
        m_threads.emplace_back(std::forward<Function>(fn), std::forward<Args>(args)...);
    }
    /**
     *  Remove a thread from the thread pool
     *
     *  @param position position of the thread to be remove
     */
    void remove(size_t position) {
        if (position >= m_threads.size())
            throw std::out_of_range("thread_pool::remove");
        m_threads[position].join();
        m_threads.erase(m_threads.begin() + position);
    }
    /**
     *  Release all threads from thread pool
     *  This function will be called by the class destructor
     * 
     */
    void release() {
        for (auto &thread : m_threads)
            thread.join();
        m_threads.clear();
    }
};