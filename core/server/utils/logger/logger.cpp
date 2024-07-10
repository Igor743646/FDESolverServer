#include "logger.hpp"

namespace NLogger { 

    unsigned char ChangeLogLevel(unsigned char logLevel) {
        static unsigned char stUserLogLevel = 1;
        if (logLevel != 0) {
            stUserLogLevel = logLevel;
        }
        return stUserLogLevel;
    }

    int GetUserLogLevel() {
        return ChangeLogLevel(0);
    }

}
