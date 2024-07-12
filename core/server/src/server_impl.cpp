#include "server_impl.hpp"


TFDESolverServerImpl::TStatus TFDESolverServerImpl::RunTask(TServerContext* context, const TClientConfig* request, TResults* response) {
    gpr_log(GPR_INFO, "Start task");
    auto start = std::chrono::system_clock::now();

    try {
        DoRunTask(request, response);
        gpr_log(GPR_INFO, "End task");
    } catch (const std::exception& e) {
        gpr_log(GPR_ERROR, "Error:");
        NStackTracer::TStackTracer::CatchAndPrintStack(e);
        return TStatus(::grpc::StatusCode::ABORTED, e.what());
    } catch (...) {
        gpr_log(GPR_ERROR, "Error: unknown error");
        return TStatus(::grpc::StatusCode::UNKNOWN, "Unexpected exception");
    }

    auto stop = std::chrono::system_clock::now();
    f64 workTime = std::chrono::duration<f64, std::milli>(stop - start).count();

    context->AddTrailingMetadata("work-time", std::to_string(workTime));
    context->AddTrailingMetadata("version", "0.1");
    
    return TStatus::OK;
}
