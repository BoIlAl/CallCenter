#include "timeoutQueue.h"

TimeoutQueue::TimeoutQueue(std::function<void(uint64_t)> timeOutCallback) 
    : m_timeOutCallback(timeOutCallback) {
}

void TimeoutQueue::push(uint64_t elem, const std::chrono::system_clock::time_point& endPoint) {
    std::lock_guard<std::mutex> lock(m_mutex);

    clearThreads();
    
    auto p_qe = std::make_shared<TimeoutQueueElement>(elem);
    m_list.push_front(p_qe);
    p_qe->m_it = m_list.begin();

    m_threads.insert({ elem, std::make_unique<std::thread>([p_qe, endPoint, this](){ this->timeout(p_qe, endPoint); }) });
    m_contains.insert({ elem, p_qe });
}

bool TimeoutQueue::tryPop(uint64_t& value) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_list.empty()) {
        return false;
    }

    auto back = m_list.back();
    m_list.pop_back();

    m_contains.erase(back->m_value);

    back->m_it = m_list.end();
    back->m_cond.notify_one();

    value = back->m_value;

    clearThreads();

    return true;
}

bool TimeoutQueue::contains(uint64_t value) {
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_contains.find(value) != m_contains.end();
}

void TimeoutQueue::erase(uint64_t value) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_contains.find(value);

    if (it == m_contains.end()) {
        return;
    }

    m_list.erase(it->second->m_it);

    it->second->m_it = m_list.end();
    it->second->m_cond.notify_one();

    m_contains.erase(value);
}

size_t TimeoutQueue::size() {
    return m_list.size();
}


TimeoutQueue::~TimeoutQueue() {
    std::lock_guard<std::mutex> lock(m_mutex);

    clearThreads();

    for (auto& p_qe: m_list) {
        p_qe->m_cond.notify_one();
    }

    for (auto& p_qe: m_list) {
        m_threads[p_qe->m_value]->join();
    }
}

void TimeoutQueue::clearThreads() {
    for (uint64_t val: m_endedThreads) {
        m_threads[val]->join();
        m_threads.erase(val);
    }

    m_endedThreads.clear();
}

void TimeoutQueue::timeout(
    std::shared_ptr<TimeoutQueueElement> p_qe,
    const std::chrono::system_clock::time_point& endPoint
) {
    std::unique_lock<std::mutex> lock(m_mutex);

    p_qe->m_cond.wait_until(lock, endPoint, [=]() { return p_qe->m_it == m_list.end(); });

    if (p_qe->m_it != m_list.end()) {
        m_list.erase(p_qe->m_it);

        m_contains.erase(p_qe->m_value);

        lock.unlock();
        
        m_timeOutCallback(p_qe->m_value);

        lock.lock();
    }

    m_endedThreads.push_back(p_qe->m_value);
}
