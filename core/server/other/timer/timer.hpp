#pragma once

#include <chrono>

namespace NTimer {
    class TTimer {
    public:

        using TMilli = std::chrono::duration<double, std::milli>;
        using TSeconds = std::chrono::duration<double>;

        TTimer() {
            Start = std::chrono::system_clock::now();
        }

        void Reset() {
            Start = std::chrono::system_clock::now();
        }

        TMilli MilliSeconds() {
            auto point = std::chrono::system_clock::now();
            return std::chrono::duration_cast<TMilli>(point - Start);
        }

        TSeconds Seconds() {
            auto point = std::chrono::system_clock::now();
            return std::chrono::duration_cast<TSeconds>(point - Start);
        }

    private:

        std::chrono::system_clock::time_point Start;
    };
}  // namespace NTimer
