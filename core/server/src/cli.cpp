#include <argument_parser.hpp>
#include <google/protobuf/util/json_util.h>
#include <service_impl.hpp>

class TCLISolver : NFDESolverService::TFDESolverService {
public:

    explicit TCLISolver(const NArgumentParser::TArgumentParserResult& arguments) 
        : NFDESolverService::TFDESolverService(), 
          Arguments(arguments) {}

    void RunTask() {
        auto config = GetConfig();

        PFDESolver::TResults results;
        NFDESolverService::TFDESolverService service;

        service.DoRunTask(config.get(), &results);
        SaveResults(results);
    }

private:

    void Check(bool expression, const std::string& message) {
        if (!expression) {
            std::cerr << message << std::endl;
            std::exit(1);
        }
    }

    std::unique_ptr<PFDESolver::TClientConfig> GetConfig() {
        const std::string& fileName = Arguments.Get<const std::string&>("--input-file");

        auto config = std::make_unique<PFDESolver::TClientConfig>();

        std::ifstream inputFile(fileName, std::ios::in | std::ios::binary);

        Check(inputFile.is_open(), "Can not open file: " + fileName);

        std::ostringstream sstr;
        sstr << inputFile.rdbuf();
        std::string jsonConfig = sstr.str();
        
        auto status = google::protobuf::util::JsonStringToMessage(std::string_view(jsonConfig), config.get());

        if (!status.ok()) {
            std::cerr << status.message() << std::endl;
            std::exit(1);
        }

        return config;
    }

    void SaveResults(const PFDESolver::TResults& results) {
        const std::string& fileName = Arguments.Get<const std::string&>("--output-file");
        const bool& isJson = Arguments.Get<const bool&>("--output-json");

        std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);
        Check(outputFile.is_open(), "Can not open file: " + fileName);

        if (isJson) {
            std::string jsonSerializedMessage;
            auto status = google::protobuf::util::MessageToJsonString(results, &jsonSerializedMessage);

            Check(status.ok(), "Can not serialize data as json in file: " + fileName);

            outputFile << jsonSerializedMessage;

            std::cout << "Serialized json success." << std::endl;
        } else {
            Check(results.SerializePartialToOstream(&outputFile), "Can not serialize data as binary in file: " + fileName);
            
            std::cout << "Serialized binary success." << std::endl;
        }
    }

    NArgumentParser::TArgumentParserResult Arguments;
};

NArgumentParser::TArgumentParserResult ParseArgs(int argc, char** argv) {
    NArgumentParser::TArgumentParser parser;
    parser.AddArgument("--input-file", NArgumentParser::TArgumentType::STRING_VALUE, 
                       "input file with json view of protobuf TClientConfig", true);
    parser.AddArgument("--output-file", NArgumentParser::TArgumentType::STRING_VALUE, 
                       "output file with view of protobuf TResults", false, std::string("result.txt"));
    parser.AddArgument("--output-json", NArgumentParser::TArgumentType::FLAG, 
                       "json output file format", false, false);
    return parser.Parse(argc, argv);
}

int main(int argc, char** argv) {
    NLogger::ChangeLogLevel(3);
    
    auto args = ParseArgs(argc, argv);
    TCLISolver solver(args);
    solver.RunTask();
    return 0;
}
