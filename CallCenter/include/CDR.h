#pragma once
#include <iostream>

#include "types.h"
#include "common.h"

enum class ReleaseCause {
    RC_OK,
    RC_TIMEOUT,
    RC_ALREADY_IN_QUEUE,
    RC_ALREADY_ANSWERING,
    RC_CALL_DUPLICATION,
    RC_OVERLOAD,
    RC_SYSTEM_ERROR,
    RC_DEFAULT
};

struct CDR {
    uint64_t callID;
    uint64_t number;

    time_point dateTimeIn;
    time_point dateTimeOut;

    time_point dateTimeAnswer;
    duration_t duration;

    ReleaseCause rc;
    opID_t operatorID;

    CDR();
};

std::string toString(const ReleaseCause& rc);
std::ostream& operator<<(std::ostream & os, const ReleaseCause& rc);
std::ostream& operator<<(std::ostream & os, const CDR& cdr);