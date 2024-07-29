#include "logger.hpp"

namespace NLogger {
    TLogHelper::TLevel TLogHelperBase::stLogLevel = TLogHelper::TLevel::lvERROR;
    std::mutex TLogHelperBase::stMutex;
}

