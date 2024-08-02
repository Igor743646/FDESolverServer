#pragma once

#include <iostream>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

namespace NStackTracer {

    class TExceptionWithStack : public std::exception {
    public:

        explicit TExceptionWithStack(const char* message) : Message(message) {}

        const char* what() const noexcept override { //NOLINT
            return Message;
        }

    private:

        const char* Message;
    };

    class TStackTracer {
        using Traced = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

    public:

        static void CurrentStack() {
            std::cout << boost::stacktrace::stacktrace() << '\n';
        }

        [[noreturn]] static void ThrowWithMessage(const char* message) {
            throw boost::enable_error_info(TExceptionWithStack(message))
                << Traced(boost::stacktrace::stacktrace());
        }

        [[noreturn]] static void ThrowWithMessage(const std::string& message) {
            throw boost::enable_error_info(TExceptionWithStack(message.c_str()))
                << Traced(boost::stacktrace::stacktrace());
        }

        static void CatchAndPrintStack(const TExceptionWithStack& exception) {
            std::cout << exception.what() << '\n';
            const boost::stacktrace::stacktrace* trace = boost::get_error_info<Traced>(exception);
            if (trace != nullptr) {
                std::cerr << *trace;
            }
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
