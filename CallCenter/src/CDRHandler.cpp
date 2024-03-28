#include "CDRHandler.h"

constexpr ms_t CDRHandler::m_waitTimeNext;

CDRHandler::CDRHandler(
    std::shared_ptr<ICDRHandlerCfg> cfg,
    std::shared_ptr<IIndexGenerator> gen,
    std::shared_ptr<IP7_Trace> pLogTrace,
    std::shared_ptr<IP7_Trace> pCdrLogTrace)
    : m_pCfg(cfg)
    , m_pGen(gen)
    , m_pLogTrace(pLogTrace)
    , m_pCDRTrace(pCdrLogTrace)
    , m_tq([this](uint64_t value){ handleTimeout(value); })
{};

ReleaseCause CDRHandler::handleNumber(uint64_t number) {
    std::unique_lock<std::mutex> lock(m_mutex);

    if (!m_pCfg) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("cfg is nullptr"));
        }
        return ReleaseCause::RC_SYSTEM_ERROR;
    }

    if (!m_pGen) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("generator is nullptr"));
        }
        return ReleaseCause::RC_SYSTEM_ERROR;
    }

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("Start handling call with number " + std::to_string(number)));
    }
    
    auto cdr = std::make_shared<CDR>();

    if (!tryInitCDR(cdr, number)) {
        return cdr->rc;
    }

    if (isAlreadyAnswering(cdr)) {
        return releaseCDR(*cdr);
    }

    if (isAlreadyInQueue(cdr)) {
        return releaseCDR(*cdr);
    }

    if (isOverload(cdr)) {
        return releaseCDR(*cdr);
    }

    addToPools(cdr);

    m_condProcess.wait(lock, [this, cdr]() {
        return cdr->rc != ReleaseCause::RC_DEFAULT;
    });

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("CDR with id " + std::to_string(cdr->callID) + " got release cause"));
    }
    
    if (cdr->rc != ReleaseCause::RC_TIMEOUT && 
        cdr->rc != ReleaseCause::RC_CALL_DUPLICATION &&
        cdr->rc != ReleaseCause::RC_OK ) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_WARNING(nullptr, tm("CDR with id " + std::to_string(cdr->callID) + " got wrong release cause"));
        }
    }
    
    m_answering.erase(cdr->number);
    m_handling.erase(cdr->number);

    return releaseCDR(*cdr);
}

std::shared_ptr<CDR> CDRHandler::next() {
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Trying to find next element in queue"));
    }

    uint64_t number;

    if (!m_tq.tryPop(number)) {
        m_condQueue.wait_for(lock, std::chrono::milliseconds(m_waitTimeNext), [this](){
            return m_tq.size() != 0;
        });
        
        if (!m_tq.tryPop(number)) {
            if (m_pLogTrace) {
                m_pLogTrace->P7_WARNING(nullptr, TM("Queue empty for to long, timeout"));
            }       
            return nullptr;
        }
    }

    auto cdr = m_handling[number];

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("Giving away cdr with id " + std::to_string(cdr->callID)));
    }

    m_answering.insert(cdr->number);

    return cdr;
}

void CDRHandler::handleFilled(const std::shared_ptr<CDR>& cdr) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("Handle filled CDR with id " + std::to_string(cdr->callID)));
    }

    if (!isFilled(*cdr)) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_WARNING(nullptr, TM("Not filled completely"));
        }
    }

    if (cdr->rc != ReleaseCause::RC_OK) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_WARNING(nullptr, TM("Release cause is not \"OK\""));
        }
    }

    if (m_answering.find(cdr->number) == m_answering.end()) {
        m_pLogTrace->P7_ERROR(nullptr, tm("CDR with id " + std::to_string(cdr->callID) + " has non-existent number"));
        cdr->rc = ReleaseCause::RC_SYSTEM_ERROR;
    }

    m_condProcess.notify_all();
}

void CDRHandler::handleTimeout(uint64_t number) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto cdr = m_handling.find(number)->second;

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("CDR with id " + std::to_string(cdr->callID) + " timeout"));
    }

    cdr->rc = ReleaseCause::RC_TIMEOUT;

    m_condProcess.notify_all();
}

bool CDRHandler::tryInitCDR(const std::shared_ptr<CDR>& cdr, uint64_t number) {
    cdr->callID = m_pGen->next();

    if (cdr->callID == IIndexGenerator::errorValue()) {
        cdr->rc = ReleaseCause::RC_SYSTEM_ERROR;
        if (m_pLogTrace) {
            m_pLogTrace->P7_INFO(nullptr, TM("Index generator overload"));
        }
        return false;
    }

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("Assigned id " + std::to_string(cdr->callID)));
    }

    cdr->number = number;
    cdr->dateTimeIn = std::chrono::system_clock::now();

    return true;
}

bool CDRHandler::isAlreadyAnswering(const std::shared_ptr<CDR>& cdr) {
    if (m_answering.find(cdr->number) == m_answering.end()) {
        return false;
    }

    cdr->rc = ReleaseCause::RC_ALREADY_ANSWERING;
    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Call with this number are already answered"));
    }
    return true;
}

bool CDRHandler::isAlreadyInQueue(const std::shared_ptr<CDR>& cdr) {
    if (m_tq.contains(cdr->number)) {
        if (m_pCfg->isUpdateDublicates()) {
            if (m_pLogTrace) {
                m_pLogTrace->P7_INFO(nullptr, TM("CDR already in queue, notifying about need to release previous"));
            }
            m_handling[cdr->number]->rc = ReleaseCause::RC_CALL_DUPLICATION;
            m_condProcess.notify_all();
        }
        else {
            if (m_pLogTrace) {
                m_pLogTrace->P7_INFO(nullptr, TM("CDR already in queue call release this call"));
            }
            cdr->rc = ReleaseCause::RC_ALREADY_IN_QUEUE;
            return true;
        }
    }

    return false;
}

bool CDRHandler::isOverload(const std::shared_ptr<CDR>& cdr) {
    if (m_tq.size() < m_pCfg->queueLength()) {
        return false;
    }
    
    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Queue overload"));
    }
    cdr->rc = ReleaseCause::RC_OVERLOAD;
    return true;
}

void CDRHandler::addToPools(const std::shared_ptr<CDR>& cdr) {
    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Adding call to queue"));
    }

    m_handling.insert({cdr->number, cdr});
    
    auto endPoint = getPointAfterRndInterval(m_pCfg->rQueueMin(), m_pCfg->rQueueMax());
    
    if (endPoint == time_point::min() && m_pLogTrace) {
        m_pLogTrace->P7_WARNING(nullptr, TM("Cfg data corrupted"));
    }

    m_tq.push(cdr->number, endPoint);

    m_condQueue.notify_one();
}

ReleaseCause CDRHandler::releaseCDR(CDR& cdr) {
    cdr.dateTimeOut = std::chrono::system_clock::now();
    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, tm("End of handling CDR with id " + std::to_string(cdr.callID)));
    }
    tryLogCDR(cdr);

    m_pGen->remove(cdr.callID);
    return cdr.rc;
}

bool CDRHandler::isFilled(const CDR& cdr) const {
    auto minTimePoint = time_point::min();

    if (cdr.dateTimeAnswer == minTimePoint ||
        cdr.dateTimeIn == minTimePoint ||
        cdr.rc != ReleaseCause::RC_OK
    ) {
        return false;
    }
    return true;
}

void CDRHandler::tryLogCDR(const CDR& cdr) {
    if (!m_pCDRTrace) {
        return;
    }

    std::stringstream ss;

    ss << cdr; 

    m_pCDRTrace->P7_INFO(nullptr, TM(ss.str().c_str()));
}

CDRHandler::~CDRHandler() {}