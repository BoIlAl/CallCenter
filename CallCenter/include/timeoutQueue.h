#pragma once
#include <memory>
#include <list>
#include <vector>
#include <thread>
#include <condition_variable>
#include <functional>
#include <unordered_map>
#include <unordered_set>

class TimeoutQueue {
private:
    struct TimeoutQueueElement {
    public:
        uint64_t m_value;
        std::condition_variable m_cond;
        std::list<std::shared_ptr<TimeoutQueueElement>>::iterator m_it;
        std::thread m_thread;
    public:
        TimeoutQueueElement(uint64_t val) : m_value(val) {};
    };
private:
    std::function<void(uint64_t)> m_timeOutCallback;
    std::function<void(uint64_t)> m_popCallback;

    std::mutex m_mutex;
    std::unordered_map<uint64_t, std::unique_ptr<std::thread>> m_threads;
    std::vector<uint64_t> m_endedThreads;

    std::list<std::shared_ptr<TimeoutQueueElement>> m_list;
    std::unordered_map<uint64_t, std::shared_ptr<TimeoutQueueElement>> m_contains;
public:
    TimeoutQueue(std::function<void(uint64_t)> timeOutCallback);

    void push(uint64_t elem, const std::chrono::system_clock::time_point& endPoint);

    bool tryPop(uint64_t& value);

    bool contains(uint64_t value);
    void erase(uint64_t value);

    size_t size();

    ~TimeoutQueue();

private:
    void clearThreads();

    void timeout(
        std::shared_ptr<TimeoutQueueElement> p_qe,
        const std::chrono::system_clock::time_point& endPoint
    );
};

