#include "operatorManager.h"

OperatorManager::OperatorManager(
        std::shared_ptr<IOperatorManagerCfg> cfg,
        std::shared_ptr<ICDRHandler> handler,
        std::shared_ptr<IP7_Trace> pLogTrace
    )
    : m_pCfg(cfg)
    , m_pHandler(handler)
    , m_pLogTrace(pLogTrace)
    , m_isRunning(false)
{}

void OperatorManager::run() {
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_isRunning) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("Already running"));
        }
        return;
    }

    if (!m_pHandler) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("Handler is nullptr"));
        }
        return;
    }

    if (!m_pCfg) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("Cfg is nullptr"));
        }
        return;
    }

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("OM started successfully"));
    }

    m_isRunning = true;

    do {
        if (m_free.empty()) {
            if (m_pLogTrace) {
                m_pLogTrace->P7_INFO(nullptr, TM("No free operators, waiting..."));
            }

            m_cond.wait(lock, [=] {
                return !m_free.empty() || !m_isRunning;
            });
        }

        clearFinishedThreads();
        
        if (!m_isRunning) {
            if (m_pLogTrace) {
                m_pLogTrace->P7_INFO(nullptr, TM("Stopping OM"));
            }
            return;
        }
        
        if (m_pLogTrace) {
            m_pLogTrace->P7_INFO(nullptr, TM("Trying to take next call"));
        }
        
        lock.unlock();

        auto cdr = m_pHandler->next();

        if (!cdr) {
            if (m_pLogTrace) {
                m_pLogTrace->P7_INFO(nullptr, TM("Timeout..."));
            }
            lock.lock();
            continue;
        }

        lock.lock();

        opID_t freeOpId = *m_free.begin();
        m_free.erase(freeOpId);
        auto freeOp = m_operators[freeOpId];

        m_busy.insert({freeOpId, cdr});

        if (m_pLogTrace) {
            m_pLogTrace->P7_INFO(nullptr, tm(
                "Assigning call with id " + std::to_string(cdr->callID) +
                " to operator with id " + std::to_string(freeOpId)
            ));
        }

        auto talkEndPoint = getPointAfterRndInterval(m_pCfg->rOperatorMin(), m_pCfg->rOperatorMax());

        auto opThread = std::make_shared<std::thread>(
            &IOperator::answer, 
            m_operators[freeOpId], 
            cdr,
            [this](opID_t id){
                this->sendResult(id);
            },
            talkEndPoint
        );

        m_waitingThreadPool.insert({freeOpId, opThread});

    } while (m_isRunning);
}

void OperatorManager::stop() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Stop command received"));
    } 

    m_isRunning = false;

    m_cond.notify_one();
}

void OperatorManager::addOperator(std::shared_ptr<IOperator> newOp) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("Adding new operator with id " + std::to_string(newOp->getId())));
    }

    if (m_operators.find(newOp->getId()) != m_operators.end()) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_WARNING(nullptr, tm("Already exist operator with id " + std::to_string(newOp->getId())));
        }
        return;
    }

    m_operators.insert({newOp->getId(), newOp});
    m_free.insert(newOp->getId());
    
    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Successfully added"));
    }

    m_cond.notify_one();
}

void OperatorManager::sendResult(opID_t id) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto cdr = m_busy.at(id);

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm(
            "Got result about call with id " + std::to_string(cdr->callID) +
            " from operator with id " + std::to_string(id)
        ));
    }

    m_busy.erase(id);
    m_free.insert(id);
    m_finishedThreads.push(id);
    
    m_pLogTrace->P7_INFO(nullptr, tm("Returning call with id " + std::to_string(cdr->callID) + " to handler"));

    m_pHandler->handleFilled(cdr);

    m_cond.notify_one();
}

void OperatorManager::clearFinishedThreads() {
    while (!m_finishedThreads.empty()) {
        opID_t id = m_finishedThreads.top();
        m_waitingThreadPool.at(id)->join();
        m_waitingThreadPool.erase(id);
        m_finishedThreads.pop();
    }
}

OperatorManager::~OperatorManager() {
    for (auto& thread : m_waitingThreadPool) {
        if (thread.second->joinable()) {
            thread.second->join();
        }
    }
}