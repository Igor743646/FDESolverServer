#include "server_impl.hpp"

#include <timer.hpp>


TFDESolverServerImpl::TStatus TFDESolverServerImpl::RunTask(TServerContext* context, const TClientConfig* request, TResults* response) {
    gpr_log(GPR_INFO, "Start task");
    
    NTimer::TTimer timer;

    try {
        DoRunTask(request, response);
        gpr_log(GPR_INFO, "End task");
    } catch (const NStackTracer::TExceptionWithStack& e) {
        gpr_log(GPR_ERROR, "Error:");
        NStackTracer::TStackTracer::CatchAndPrintStack(e);
        return TStatus(::grpc::StatusCode::ABORTED, e.what());
    } catch (...) {
        gpr_log(GPR_ERROR, "Error: unknown error");
        return TStatus(::grpc::StatusCode::UNKNOWN, "Unexpected exception");
    }

    auto workTime = timer.MilliSeconds().count();

    context->AddTrailingMetadata("work-time", std::to_string(workTime));
    context->AddTrailingMetadata("version", "0.1");
    
    return TStatus::OK;
}
