#include "callCenter.h"

constexpr char CallCenter::m_serverTraceName[];
constexpr char CallCenter::m_handlerTraceName[];
constexpr char CallCenter::m_OMTraceName[];
constexpr char CallCenter::m_cdrTraceName[];
constexpr char CallCenter::m_cfgFileName[];

CallCenter::CallCenter()
    : m_pTraceCDR(nullptr)
    , m_pTraceServer(nullptr)
    , m_pTraceHandler(nullptr)
{}

void CallCenter::run() {
    if (!initLogs()) {
        std::cout << "Cant initialize logs" << std::endl;
    }

    std::shared_ptr<ICDRHandlerCfg> pHandlerCfg = std::make_shared<CDRHandlerCfg>();
    std::shared_ptr<IIndexGenerator> indexGenerator = std::make_shared<IndexGenerator>();

    std::shared_ptr<ICDRHandler> handler = std::make_shared<CDRHandler>(
        std::shared_ptr<ICDRHandlerCfg>(pHandlerCfg),
        std::shared_ptr<IIndexGenerator>(indexGenerator),
        m_pTraceHandler,
        m_pTraceCDR
    );

    HttpServer server(handler, m_pTraceServer);

    std::shared_ptr<IOperatorManagerCfg> pOMCfg = std::make_shared<OperatorManagerCfg>();

    OperatorManager om(pOMCfg, handler, m_pTraceOM);
    
    for (int i = 0; i < 10; ++i) {
        om.addOperator(std::make_shared<Operator>(i));
    }

    readCfg(pOMCfg, pHandlerCfg);

    std::thread serverThread([&server]{
        server.run();
    });

    std::thread omThread([&om]{
        om.run();
    });

    std::string line;

    while(std::getline(std::cin, line)) {
        if (line == "stop") {
            server.stop();
            om.stop();

            serverThread.join();
            omThread.join();

            std::cout << "Successfully stopped" << std::endl;
            return;
        }
        if (line == "cfg") {
            readCfg(pOMCfg, pHandlerCfg);
        }
        if (line == "settings") {
            std::cout << pOMCfg->toStr() << pHandlerCfg->toStr();
        }
    }
}

void CallCenter::readCfg(
    const std::shared_ptr<IOperatorManagerCfg>& pOMCfg, 
    const std::shared_ptr<ICDRHandlerCfg>& pHandlerCfg
) {
    std::ifstream f(m_cfgFileName);
    if (!f) {
        std::cout << "Can`t read from cfg file" << std::endl;
        return;
    }
            
    json js;

    try {
        js = json::parse(f);
    }
    catch (json::parse_error ex) {
        std::cout << "File structure is incorrect" << std::endl;
        return;
    }

    if (!pOMCfg->set(js) || !pHandlerCfg->set(js)) {
        std::cout << "Cfg data partially updated" << std::endl;
    }
    else {
        std::cout << "Cfg data updated" << std::endl;
    }
}


bool CallCenter::initLogs() {
    std::string fileTxtType = "/P7.Sink=FileTxt";

    std::string verb2 = "/P7.Verb=2";
    std::string verb0 = "/P7.Verb=0";

    std::string formatCDR = "/P7.Format=\"%ms\"";
    std::string formatLog = "/P7.Format=\"%lv in %fs, %fn , %fl : %ms -- [%ts]\"";

    std::string dirCDR = "/P7.Dir=../log/cdr/";
    std::string dirServer = "/P7.Dir=../log/svrLog/";
    std::string dirHandler = "/P7.Dir=../log/handlerLog/";
    std::string dirOM = "/P7.Dir=../log/operatorManagerLog/";

    std::string paramsCDR = fileTxtType + " "  + verb0 + " " + formatCDR + " " + dirCDR;
    std::string paramsServer = fileTxtType + " "  + verb2 + " " + formatLog + " " + dirServer;
    std::string paramsHandler = fileTxtType + " "  + verb2 + " " + formatLog + " " + dirHandler;
    std::string paramsOM = fileTxtType + " "  + verb2 + " " + formatLog + " " + dirOM;


    IP7_Client *clientCDR = P7_Create_Client(TM(paramsCDR.c_str()));
    IP7_Client *clientServer = P7_Create_Client(TM(paramsServer.c_str()));
    IP7_Client *clientHandler = P7_Create_Client(TM(paramsHandler.c_str()));
    IP7_Client *clientOM = P7_Create_Client(TM(paramsOM.c_str()));


    m_pTraceCDR = std::shared_ptr<IP7_Trace>(P7_Create_Trace(clientCDR, m_cdrTraceName));
    m_pTraceServer = std::shared_ptr<IP7_Trace>(P7_Create_Trace(clientServer, m_serverTraceName));
    m_pTraceHandler = std::shared_ptr<IP7_Trace>(P7_Create_Trace(clientHandler, m_handlerTraceName));
    m_pTraceOM = std::shared_ptr<IP7_Trace>(P7_Create_Trace(clientOM, m_OMTraceName));


    if (m_pTraceCDR && m_pTraceServer && m_pTraceHandler && m_pTraceOM) {
        return true;
    }
    return false;
}

CallCenter::~CallCenter() {
    if (m_pTraceCDR) {
        m_pTraceCDR->Release();
    }
    if (m_pTraceServer) {
        m_pTraceServer->Release();
    }
    if (m_pTraceHandler) {
        m_pTraceHandler->Release();
    }
    if (m_pTraceOM) {
        m_pTraceOM->Release();
    }
}