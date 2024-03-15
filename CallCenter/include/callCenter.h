#pragma once

#include "httpServer.h"
#include "CDRHandler.h"
#include "CDRHandlerCfg.h"
#include "indexGenerator.h"
#include "operatorManager.h"
#include "operatorManagerCfg.h"
#include "operator.h"

class CallCenter {
private:
    static constexpr char m_serverTraceName[] = "serverTrace";
    static constexpr char m_handlerTraceName[] = "handlerTrace";
    static constexpr char m_OMTraceName[] = "omTrace";
    static constexpr char m_cdrTraceName[] = "cdrTrace";

    static constexpr char m_cfgFileName[] = "../cfg/cfg.json";

    static constexpr ms_t rewriteCfgTime = 600000;

    std::shared_ptr<IP7_Trace> m_pTraceCDR;
    std::shared_ptr<IP7_Trace> m_pTraceServer;
    std::shared_ptr<IP7_Trace> m_pTraceHandler;
    std::shared_ptr<IP7_Trace> m_pTraceOM;

 public:
    CallCenter();

    void run();

    ~CallCenter();
private:
    void readCfg(
        const std::shared_ptr<IOperatorManagerCfg>& pOMCfg,
        const std::shared_ptr<ICDRHandlerCfg>& pHandlerCfg
    );
    bool initLogs();
};