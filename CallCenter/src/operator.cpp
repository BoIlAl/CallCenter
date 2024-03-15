#include "operator.h"

Operator::Operator(opID_t id) : m_id(id) {};

uint16_t Operator::getId() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_id;
}

void Operator::answer(const std::shared_ptr<CDR>& cdr,  
    std::function<void(opID_t)> callback,
    const time_point& endPoint) 
{
    std::unique_lock<std::mutex> lock(m_mutex);

    cdr->dateTimeAnswer = std::chrono::system_clock::now();

    cdr->operatorID = m_id;

    if (endPoint <= cdr->dateTimeAnswer) {
        cdr->duration = duration_t(0);
    }
    cdr->duration = endPoint - cdr->dateTimeAnswer;

    lock.unlock();

    std::this_thread::sleep_until(endPoint);

    lock.lock();

    cdr->rc = ReleaseCause::RC_OK;

    callback(m_id);
}