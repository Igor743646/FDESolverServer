#pragma once

#include <iostream>
#include <utils.hpp>

namespace NStackTracer {

    class TExceptionWithStack : public std::exception {
    public:

        explicit TExceptionWithStack(const std::string& message) : Message(message) {}

        const char* what() const noexcept override { //NOLINT
            return Message.c_str();
        }

    private:

        std::string Message;
    };

    class TStackTracer {
    public:

        static void CurrentStack() {
            std::cout << NUtils::StackTrace() << '\n';
        }

        [[noreturn]] static void ThrowWithMessage(const char* message) {
            std::ostringstream ss;
            ss << message << '\n' << NUtils::StackTrace();
            throw TExceptionWithStack(ss.str());
        }

        [[noreturn]] static void ThrowWithMessage(const std::string& message) {
            std::ostringstream ss;
            ss << message << '\n' << NUtils::StackTrace();
            throw TExceptionWithStack(ss.str());
        }

        static void CatchAndPrintStack(const TExceptionWithStack& exception) {
            std::cerr << exception.what() << '\n';
        }

    };
}  // namespace NStackTracer

#ifdef NDEBUG

    #define STACK_ASSERT(expression, message) ((void)0)

#else

    #define STACK_ASSERT(expression, message) (void)(                    \
            (static_cast<bool>(expression)) ||                           \
            (::NStackTracer::TStackTracer::ThrowWithMessage(message), 0) \
        )

#endif
