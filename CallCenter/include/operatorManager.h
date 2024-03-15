#pragma once
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>
#include <thread>
#include <stack>

#include "common.h"
#include "IOperatorManagerCfg.h"
#include "ICDRHandler.h"
#include "IOperator.h"

class OperatorManager {
private:
    std::shared_ptr<IOperatorManagerCfg> m_pCfg;
    std::shared_ptr<ICDRHandler> m_pHandler;

    std::unordered_map<opID_t, std::shared_ptr<IOperator>> m_operators;
    std::unordered_map<opID_t, std::shared_ptr<CDR>> m_busy;
    std::unordered_set<opID_t> m_free;

    std::unordered_map<opID_t, std::shared_ptr<std::thread>> m_waitingThreadPool;

    std::stack<opID_t> m_finishedThreads;

    std::shared_ptr<IP7_Trace> m_pLogTrace;

    std::mutex m_mutex;
    std::condition_variable m_cond;

    bool m_isRunning;
public:
    OperatorManager(
        std::shared_ptr<IOperatorManagerCfg> cfg,
        std::shared_ptr<ICDRHandler> handler,
        std::shared_ptr<IP7_Trace> pLogTrace
    );

    void run();
    
    void stop();

    void addOperator(std::shared_ptr<IOperator> newOp);

    ~OperatorManager();
    
private:
    void sendResult(opID_t id);
    void clearFinishedThreads();
};