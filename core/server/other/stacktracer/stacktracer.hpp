#pragma once

#include <iostream>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

namespace NStackTracer {

    class TExceptionWithStack : std::exception {
    public:

        TExceptionWithStack(const char* message) : std::exception() {
            Message = message;
        }

        const char* what() const noexcept override {
            return Message;
        }

    private:

        const char* Message;
    };

    class TStackTracer {
        using traced = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

    public:

        static void CurrentStack() {
            std::cout << boost::stacktrace::stacktrace() << std::endl;
        }

        static void ThrowWithMessage(const char* message) {
            throw boost::enable_error_info(TExceptionWithStack(message))
                << traced(boost::stacktrace::stacktrace());
        }

        static void ThrowWithMessage(const std::string& message) {
            throw boost::enable_error_info(TExceptionWithStack(message.c_str()))
                << traced(boost::stacktrace::stacktrace());
        }

        static void CatchAndPrintStack(const TExceptionWithStack& e) {
            std::cout << e.what() << std::endl;
            const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
            if (st) {
                std::cerr << *st;
            }
        }

    };
}

#ifdef NDEBUG

    #define STACK_ASSERT(expression, message) ((void)0)

#else

    #define STACK_ASSERT(expression, message) (void)(                    \
            (static_cast<bool>(expression)) ||                           \
            (::NStackTracer::TStackTracer::ThrowWithMessage(message), 0) \
        )

#endif