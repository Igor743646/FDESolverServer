#include "server_impl.hpp"

#include <timer.hpp>


TFDESolverServerImpl::TStatus TFDESolverServerImpl::RunTask(TServerContext* context, const TClientConfig* request, TResults* response) {
    INFO_LOG << "Start task" << Endl;
    
    NTimer::TTimer timer;

    try {
        DoRunTask(request, response);
        INFO_LOG << "End task" << Endl;
    } catch (const NStackTracer::TExceptionWithStack& e) {
        ERROR_LOG << "Error:" << Endl;
        NStackTracer::TStackTracer::CatchAndPrintStack(e);
        return {::grpc::StatusCode::ABORTED, e.what()};
    } catch (...) {
        ERROR_LOG << "Error: unknown error" << Endl;
        return {::grpc::StatusCode::UNKNOWN, "Unexpected exception"};
    }

    auto workTime = timer.MilliSeconds().count();

    context->AddTrailingMetadata("work-time", std::to_string(workTime));
    context->AddTrailingMetadata("version", "0.1");
    
    return TStatus::OK;
}
