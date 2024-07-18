#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstring>

#ifdef __linux__ 
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
    #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

namespace {
    const auto LogFileName = "log.log";
}

namespace NLogger {

    unsigned char ChangeLogLevel(unsigned char logLevel);
    int GetUserLogLevel();

    class TLogHelperBase {
    public:
        TLogHelperBase() {
            static bool started = false;
            if (!started) { // Just for truncating
                std::ofstream file(LogFileName, std::ios_base::out | std::ios_base::trunc);
                started = true;
                file.close();
            }
        }
    };

    template<unsigned char LogLevel = 0>
    class TLogHelper : public TLogHelperBase {
    public:

        static_assert(LogLevel == 1 || LogLevel == 2 || LogLevel == 3);

        TLogHelper(const char* name, const char* file, int line, const char* color) : TLogHelperBase() {
            if (LogLevel <= GetUserLogLevel()) {
                Out << "[ " << std::setw(5) << name << " ] " << file << "(" << line << ")";
                Out << " Thread id: " << std::this_thread::get_id() << " Message: ";
            }
        }

        TLogHelper(const TLogHelper&) = delete;
        TLogHelper(TLogHelper&&) = default;
        TLogHelper& operator=(const TLogHelper&) = delete;
        TLogHelper& operator=(TLogHelper&&) = delete;
        
        ~TLogHelper() {
            if (LogLevel <= GetUserLogLevel()) {
                std::ofstream file(LogFileName, std::ios_base::out | std::ios_base::app);

                if (file.is_open()) {
                    file << Out.str();
                    file.close();
                } else {
                    std::cerr << Out.str();
                }
            }
        }

        template<class T>
        friend TLogHelper&& operator<<(TLogHelper&& out, const T& mes) {
            if (LogLevel <= GetUserLogLevel()) {
                out.Out << mes;
            }
            return std::move(out);
        }

    private:

        std::ostringstream Out;
    };
}


#define BASE_LOG(level, file, line, ...) NLogger::TLogHelper<level>{file, line, __VA_ARGS__}
#define INFO_LOG  BASE_LOG(2, "INFO", (__FILENAME__), (__LINE__), "\033[1;93m")
#define DEBUG_LOG BASE_LOG(3, "DEBUG", (__FILENAME__), (__LINE__), "\033[1;94m")
#define ERROR_LOG BASE_LOG(1, "ERROR", (__FILENAME__), (__LINE__), "\033[1;91m")
#define WARNING_LOG BASE_LOG(1, "WARN", (__FILENAME__), (__LINE__), "\033[38;5;207m")
#define Endl '\n'