#pragma once
#include <memory>
#include <mutex>
#include <condition_variable>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <thread>

#include "ICDRHandler.h"
#include "ICDRHandlerCfg.h"
#include "IIndexGenerator.h"
#include "common.h"
#include "timeoutQueue.h"

class CDRHandler : public ICDRHandler {
private:
    static constexpr ms_t m_waitTimeNext = 5000;

    std::shared_ptr<ICDRHandlerCfg> m_pCfg;
    std::shared_ptr<IIndexGenerator> m_pGen;

    TimeoutQueue m_tq;
    
    std::unordered_map<uint64_t, std::shared_ptr<CDR>> m_handling;
    std::unordered_set<uint64_t> m_answering;

    mutable std::mutex m_mutex;
    mutable std::condition_variable m_condProcess;
    mutable std::condition_variable m_condQueue;

    std::shared_ptr<IP7_Trace> m_pLogTrace;
    std::shared_ptr<IP7_Trace> m_pCDRTrace;
public:
    CDRHandler(
        std::shared_ptr<ICDRHandlerCfg> cfg,
        std::shared_ptr<IIndexGenerator> gen,
        std::shared_ptr<IP7_Trace> pLogTrace,
        std::shared_ptr<IP7_Trace> pCdrLogTrace
    );

    virtual ReleaseCause handleNumber(uint64_t number) override;
    virtual std::shared_ptr<CDR> next() override;
    virtual void handleFilled(const std::shared_ptr<CDR>& cdr) override;

    virtual ~CDRHandler();
private:
    void handleTimeout(uint64_t number);

    bool tryInitCDR(const std::shared_ptr<CDR>& cdr, uint64_t number);
    bool isAlreadyAnswering(const std::shared_ptr<CDR>& cdr);
    bool isAlreadyInQueue(const std::shared_ptr<CDR>& cdr);
    bool isOverload(const std::shared_ptr<CDR>& cdr);
    void addToPools(const std::shared_ptr<CDR>& cdr);
    
    ReleaseCause releaseCDR(CDR& cdr);

    bool isFilled(const CDR& cdr) const;

    void tryLogCDR(const CDR& cdr);
};