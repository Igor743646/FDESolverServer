#include "server_impl.hpp"

#include <grpcpp/server_builder.h>
#include <utils.hpp>
#include <signal.h>
#include <chrono>

using namespace std::chrono_literals;

class MainContext {
public:
    
    MainContext(const std::string& address) 
    : Address(address) {
        signal(SIGINT, OnExit);
        signal(SIGFPE, NUtils::TerminateWithStack);
        signal(SIGSEGV, NUtils::TerminateWithStack);
        signal(SIGABRT, NUtils::TerminateWithStack);
        signal(SIGTERM, NUtils::TerminateWithStack);
    #ifndef NDEBUG
        NLogger::TLogHelper::SetLogLevel(NLogger::TLogHelper::TLevel::lvDEBUG);
    #endif
    }

    void Run() {
        TFDESolverServerImpl service;
        grpc::ServerBuilder builder;
        builder.AddListeningPort(Address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        Server = std::move(builder.BuildAndStart());

        if (Server) {
            INFO_LOG << std::format("Server listening on address: {}\n", Address);
            Server->Wait();
        } else {
            INFO_LOG << std::format("Can not run server on address: {}\n", Address);
        }
    }

    static void OnExit(int sig) {
        if (MainContext::Server == nullptr) {
            return;
        }

        INFO_LOG << "Shutdown server" << Endl;
        MainContext::Server->Shutdown(std::chrono::system_clock::time_point(1s));
        INFO_LOG << "Server stoped" << Endl;

        exit(0);
    }

private:
    std::string Address;
    static std::unique_ptr<grpc::Server> Server;
};

std::unique_ptr<grpc::Server> MainContext::Server = nullptr;

int main(int argc, char** argv) {
    std::string serverAddress = "[::]:50001";
    
    if (argc > 2) {
        ERROR_LOG << "Wrong arguments count!" << Endl;
    }

    if (argc == 2) {
        serverAddress = argv[1];
    }

    MainContext ctx(serverAddress);
    ctx.Run();
    NUtils::Unreachable();

    return 0;
}
