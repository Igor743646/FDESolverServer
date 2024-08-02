#pragma once

#include <boost/stacktrace.hpp>
#include <logger.hpp>

namespace NUtils {
    [[noreturn]] inline void Unreachable() {
    #if defined(_MSC_VER) && !defined(__clang__)
        __assume(false);
    #else
        __builtin_unreachable();
    #endif
    }

    [[noreturn]] inline void TerminateWithStack() {

        auto trace = boost::stacktrace::stacktrace();

        CRITICAL_LOG << trace << Endl;
        std::cerr << std::format("Stacktrace:\n{}", boost::stacktrace::to_string(trace));

        std::abort();
    }
}  // namespace NUtils
