#pragma once

namespace NUtils {
    [[noreturn]] inline void Unreachable() {
    #if defined(_MSC_VER) && !defined(__clang__)
        __assume(false);
    #else
        __builtin_unreachable();
    #endif
    }
}
