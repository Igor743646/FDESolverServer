#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstring>
#include <mutex>

#ifdef __linux__ 
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
    #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

namespace NLogger {

    namespace Globals {
        constexpr std::string_view GLogFileName = "log.log";
    }

    class TLogHelperBase {
    public:

        enum class TLevel : char {
            lvDEBUG = 5, lvINFO = 4, lvWARN = 3, lvERROR = 2, lvCRITICAL = 1,
        };

        static void SetLogLevel(TLevel level) {
            stLogLevel = level;
        }

        static TLevel GetLogLevel() {
            return stLogLevel;
        }

        TLogHelperBase() {
            static bool started = false;
            if (!started) { // Just for truncating
                std::ofstream file(Globals::GLogFileName.data(), std::ios_base::out | std::ios_base::trunc);
                started = true;
            }
        }

    protected:
        static TLevel stLogLevel;
        static std::mutex stMutex;
    };

    class TLogHelper final : public TLogHelperBase {
    public:

        TLogHelper(const char* name, const char* file, int line) {
            const std::chrono::zoned_time zonedTime{std::chrono::current_zone(), std::chrono::system_clock::now()};
            const std::thread::id threadId = std::this_thread::get_id();

            Out << std::format("[ {: >5} ] {} {}({}) Thread id: {} Message: ", name, zonedTime, file, line, threadId);
        }

        TLogHelper(const TLogHelper&) = delete;
        TLogHelper(TLogHelper&&) noexcept = default;
        TLogHelper& operator=(const TLogHelper&) = delete;
        TLogHelper& operator=(TLogHelper&&) = delete;
        
        ~TLogHelper() {
            std::unique_lock lock{stMutex};
            std::ofstream file(Globals::GLogFileName.data(), std::ios_base::out | std::ios_base::app);
            
            if (file.is_open()) {
                file << Out.str();
            } else {
                std::cerr << Out.str();
            }
        }

        template<class T>
        TLogHelper&& operator<<(T&& mes) && {
            Out << std::forward<T>(mes);
            return std::move(*this);
        }

        TLogHelper&& operator<<(const char* mes) && {
            Out << mes;
            return std::move(*this);
        }

    private:

        std::ostringstream Out;
    };
}  // namespace NLogger

#define BASE_LOG(level, name, file, line) if (level <= ::NLogger::TLogHelper::GetLogLevel()) ::NLogger::TLogHelper{name, file, line}
#define INFO_LOG  BASE_LOG(::NLogger::TLogHelper::TLevel::lvINFO, "INFO", (__FILENAME__), (__LINE__))
#define DEBUG_LOG BASE_LOG(::NLogger::TLogHelper::TLevel::lvDEBUG, "DEBUG", (__FILENAME__), (__LINE__))
#define ERROR_LOG BASE_LOG(::NLogger::TLogHelper::TLevel::lvERROR, "ERROR", (__FILENAME__), (__LINE__))
#define WARNING_LOG BASE_LOG(::NLogger::TLogHelper::TLevel::lvWARN, "WARN", (__FILENAME__), (__LINE__))
#define CRITICAL_LOG BASE_LOG(::NLogger::TLogHelper::TLevel::lvCRITICAL, "CRIT", (__FILENAME__), (__LINE__))
#define Endl '\n' // NOLINT(modernize-macro-to-enum)
