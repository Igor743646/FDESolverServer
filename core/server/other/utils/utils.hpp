#pragma once

#ifdef STDSTACKTRACE
    #include <stacktrace>
#else
    #include <boost/stacktrace.hpp>
#endif
#include <logger.hpp>

namespace NUtils {
    [[noreturn]] inline void Unreachable() {
    #if defined(_MSC_VER) && !defined(__clang__)
        __assume(false);
    #else
        __builtin_unreachable();
    #endif
    }

    inline std::string StackTrace() {
        #ifdef STDSTACKTRACE
            return std::to_string(std::stacktrace::current());
        #else
            return boost::stacktrace::to_string(boost::stacktrace::stacktrace());
        #endif
    }

    [[noreturn]] inline void TerminateWithStack() throw() {

        auto trace = StackTrace();

        CRITICAL_LOG << trace << Endl;
        std::cerr << std::format("Stacktrace:\n{}", trace);

        std::exit(1);
    }

    [[noreturn]] inline void TerminateWithStack(int sig) throw() {

        auto trace = StackTrace();

        CRITICAL_LOG << "Sig: " << sig << " " << trace << Endl;
        std::cerr << std::format("Signal: {} Stacktrace:\n{}", sig, trace);

        std::exit(sig);
    }
}  // namespace NUtils
