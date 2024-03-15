#include "CDR.h"

CDR::CDR()
    : callID(0)
    , number(0)
    , dateTimeIn(time_point::min())
    , dateTimeOut(time_point::min())
    , dateTimeAnswer(time_point::min())
    , duration(0)
    , rc(ReleaseCause::RC_DEFAULT)
    , operatorID(0)
{}

std::string toString(const ReleaseCause& rc) {
    switch (rc) {
        case ReleaseCause::RC_OK: {
            return "OK";
        }
        case ReleaseCause::RC_TIMEOUT: {
            return "Timeout";
        }
        case ReleaseCause::RC_ALREADY_IN_QUEUE: {
            return "Already in queue";
        }
        case ReleaseCause::RC_ALREADY_ANSWERING: {
            return "Already answering";
        }
        case ReleaseCause::RC_CALL_DUPLICATION: {
            return "Call dublication";
        }
        case ReleaseCause::RC_OVERLOAD: {
            return "Overload";
        }
        case ReleaseCause::RC_SYSTEM_ERROR: {
            return "System error";
        }
        case ReleaseCause::RC_DEFAULT: {    
        }
        default: {
            return "Unknown cause";
        }
    }
}

std::ostream& operator<<(std::ostream & os, const ReleaseCause& rc) {
    return os << toString(rc);
}

std::ostream& operator<<(std::ostream & os, const CDR& cdr) {
    os << tpToString(cdr.dateTimeIn) << ";" << cdr.callID << ";" << cdr.number << ";" <<
        tpToString(cdr.dateTimeOut) << ";" << cdr.rc << ";";

    if (cdr.rc == ReleaseCause::RC_OK) {
        return os << tpToString(cdr.dateTimeAnswer) << ";" << cdr.operatorID << ";" << 
            std::chrono::duration_cast<std::chrono::seconds>(cdr.duration).count() << "s" << ";";
    }
    else {
        return os << ";" << ";" << ";";
    }  
}

