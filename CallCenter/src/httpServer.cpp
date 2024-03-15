#include "httpServer.h"

bool HttpServer::m_isRunning(false);
constexpr char HttpServer::m_host[];

HttpServer::HttpServer(
        std::shared_ptr<ICDRHandler> handler, 
        std::shared_ptr<IP7_Trace> pLogTraceName
    )
    : m_handler(handler)
    , m_pLogTrace(pLogTraceName)
{}

void HttpServer::run() {
    if (m_isRunning) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("Already running"));
        }
        return;
    }

    if (!m_server.is_valid()) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("Can't initialize server"));
        }
        return;
    }

    if (!m_handler) {
        if (m_pLogTrace) {
            m_pLogTrace->P7_ERROR(nullptr, TM("Handler is nullptr"));
        }
        return;
    }

    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Server started successfully"));
    }
    
    m_server.Get("/call", [this](const httplib::Request& req, httplib::Response& res) {
        std::string strNumber = req.get_param_value("number");

        if (strNumber.empty()) {
            if (m_pLogTrace) {
                m_pLogTrace->P7_WARNING(nullptr, TM("Empty request"));
            }
            res.set_content("invalid request", "text/plain");
            return;
        }

        uint64_t number;

        if (!tryParseUInt(strNumber, number)) {
            if (m_pLogTrace) {
                m_pLogTrace->P7_WARNING(nullptr, tm("Incorrect request: " + strNumber));
            }
            res.set_content("invalid request", "text/plain");
            return;
        }

        if (m_pLogTrace) {
            m_pLogTrace->P7_INFO(nullptr, tm("Received request-call from a number " + strNumber));
        }

        res.set_content(toString(this->m_handler->handleNumber(number)), "text/plain");
    });

    m_isRunning = true;
    m_server.listen(m_host, m_port);
    m_isRunning = false;
}

void HttpServer::stop() {
    if (m_pLogTrace) {
        m_pLogTrace->P7_INFO(nullptr, TM("Stop command received"));
    }
    m_server.stop();
}
