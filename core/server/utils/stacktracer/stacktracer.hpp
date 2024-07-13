#pragma once

#include <iostream>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

namespace NStackTracer {
    class TStackTracer {
        using traced = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

    public:

        static void CurrentStack() {
            std::cout << boost::stacktrace::stacktrace() << std::endl;
        }

        template <class Exception, class... Args>
        static void Throw(Args&&... args) {
            throw boost::enable_error_info(Exception(std::forward<Args>(args)...))
                << traced(boost::stacktrace::stacktrace());
        }

        template <class Exception>
        static void ThrowWithMessage(const std::string& message) {
            throw boost::enable_error_info(Exception(message.c_str()))
                << traced(boost::stacktrace::stacktrace());
        }

        template <class Exception>
        static void CatchAndPrintStack(const Exception& e) {
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

    #define STACK_ASSERT(expression, message) (void)(                                \
            (static_cast<bool>(expression)) ||                                          \
            (::NStackTracer::TStackTracer::ThrowWithMessage<std::exception>(message), 0) \
        )

#endif
