#include <argument_parser.hpp>
#include <google/protobuf/util/json_util.h>
#include <service_impl.hpp>
#include <utility>
#include <utils.hpp>
#include <signal.h>

using NArgumentParser::TArgumentParserResult;
using NArgumentParser::TArgumentParser;
using NArgumentParser::TArgumentType;
using NArgumentParser::TArgumentTypeAliasType;

class TCLISolver : NFDESolverService::TFDESolverService {
public:

    explicit TCLISolver(TArgumentParserResult&& arguments) 
        : Arguments(std::move(arguments)) {
        signal(SIGINT, OnInterrupt);
        signal(SIGFPE, NUtils::TerminateWithStack);
        signal(SIGSEGV, NUtils::TerminateWithStack);
        signal(SIGABRT, NUtils::TerminateWithStack);
        signal(SIGTERM, NUtils::TerminateWithStack);
        i64 logLevel = Arguments.Get<i64>("--log-level");
        STACK_ASSERT(1 <= logLevel && logLevel <= 5, "Log level must be from 1 to 5");
        NLogger::TLogHelper::SetLogLevel(NLogger::TLogHelperBase::TLevel(logLevel));
    }

    void RunTask() {
        auto config = GetConfig();

        PFDESolver::TResults results;
        
        Service.DoRunTask(config.get(), &results);
        SaveResults(results);
    }

private:

    static void OnInterrupt(int sig) {
        WARNING_LOG << "Interupting..." << Endl;
        Service.Interrupt();
        WARNING_LOG << "Interuptted service" << Endl;
        std::exit(1);
    }

    static void Check(bool expression, const std::string& message) {
        if (!expression) {
            std::cerr << message << '\n';
            std::exit(1);
        }
    }

    std::unique_ptr<PFDESolver::TClientConfig> GetConfig() {
        const auto& fileName = Arguments.Get<const std::string&>("--input-file");

        auto config = std::make_unique<PFDESolver::TClientConfig>();

        std::ifstream inputFile(fileName, std::ios::in | std::ios::binary);

        Check(inputFile.is_open(), "Can not open file: " + fileName);

        std::ostringstream sstr;
        sstr << inputFile.rdbuf();
        std::string jsonConfig = sstr.str();
        
        auto status = google::protobuf::util::JsonStringToMessage(std::string_view(jsonConfig), config.get());

        if (!status.ok()) {
            std::cerr << status.message() << '\n';
            std::exit(1);
        }

        return config;
    }

    void SaveResults(const PFDESolver::TResults& results) {
        const auto& fileName = Arguments.Get<const std::string&>("--output-file");
        const bool& isJson = Arguments.Get<const bool&>("--output-json");

        std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);
        Check(outputFile.is_open(), "Can not open file: " + fileName);

        if (isJson) {
            std::string jsonSerializedMessage;
            auto status = google::protobuf::util::MessageToJsonString(results, &jsonSerializedMessage);

            Check(status.ok(), "Can not serialize data as json in file: " + fileName);

            outputFile << jsonSerializedMessage;

            std::cout << "Serialized json success." << '\n';
        } else {
            Check(results.SerializePartialToOstream(&outputFile), "Can not serialize data as binary in file: " + fileName);
            
            std::cout << "Serialized binary success." << '\n';
        }
    }

    TArgumentParserResult Arguments;
    static NFDESolverService::TFDESolverService Service;
};

NFDESolverService::TFDESolverService TCLISolver::Service;

TArgumentParserResult ParseArgs(int argc, char** argv) {
    const TArgumentTypeAliasType<TArgumentType::STRING_VALUE> defaultOutFile = "result.txt";
    const TArgumentTypeAliasType<TArgumentType::FLAG> defaultJsonMode = false;
    const TArgumentTypeAliasType<TArgumentType::INT_VALUE> defaultLogLevel = 5;

    TArgumentParser parser;
    parser.AddArgument("--input-file", TArgumentType::STRING_VALUE, 
                       "input file with json view of protobuf TClientConfig", true);
    parser.AddArgument("--output-file", TArgumentType::STRING_VALUE, 
                       "output file with view of protobuf TResults", false, defaultOutFile);
    parser.AddArgument("--output-json", TArgumentType::FLAG, 
                       "json output file format", false, defaultJsonMode);
    parser.AddArgument("--log-level", TArgumentType::INT_VALUE, 
                       "log level: 1 (CRITICAL), 2 (ERROR), 3 (WARN), 4 (INFO), 5 (DEBUG)", 
                       false, defaultLogLevel);
    return parser.Parse(argc, argv);
}

int main(int argc, char** argv) { //NOLINT
    auto args = ParseArgs(argc, argv);
    TCLISolver solver(std::move(args));
    solver.RunTask();
    return 0;
}
