#pragma once
#include <memory>

#include "ICDRHandler.h"
#include "httplib.h"
#include "common.h"

class HttpServer {
private:
    constexpr static char m_host[] = "localhost";
    constexpr static uint16_t m_port = 8080;

    static bool m_isRunning;

    std::shared_ptr<ICDRHandler> m_handler;
    std::shared_ptr<IP7_Trace> m_pLogTrace;

    httplib::Server m_server;
public:
    HttpServer(
        std::shared_ptr<ICDRHandler> handler, 
        std::shared_ptr<IP7_Trace> pLogTraceName
    );

    void run();
    void stop();
};