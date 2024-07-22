#include <argument_parser.hpp>
#include <google/protobuf/util/json_util.h>
#include "service_impl.hpp"


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

std::unique_ptr<PFDESolver::TClientConfig> GetConfig(const NArgumentParser::TArgumentParserResult& args) {
    const std::string& fileName = args.Get<const std::string&>("--input-file");

    auto config = std::make_unique<PFDESolver::TClientConfig>();

    std::ifstream inputFile(fileName, std::ios::in | std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        std::exit(1);
    }

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

void SaveResults(const NArgumentParser::TArgumentParserResult& args, const PFDESolver::TResults& results) {
    const std::string& fileName = args.Get<const std::string&>("--output-file");
    const bool& isJson = args.Get<const bool&>("--output-json");

    if (isJson) {
        std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);
        
        if (!outputFile.is_open()) {
            std::cerr << "Can not open file: " << fileName << std::endl;
            std::exit(1);
        }

        std::string jsonSerializedMessage;
        google::protobuf::util::MessageToJsonString(results, &jsonSerializedMessage);

        outputFile << jsonSerializedMessage;

        std::cout << "Serialized json success." << std::endl;
    } else {
        std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);
        
        if (!outputFile.is_open()) {
            std::cerr << "Can not open file: " << fileName << std::endl;
            std::exit(1);
        }

        if (!results.SerializePartialToOstream(&outputFile)) {
            std::cerr << "Can not serialize data in file: " << fileName << std::endl;
            std::exit(1);
        }

        std::cout << "Serialized binary success." << std::endl;
    }
}

void SolveTask(const NArgumentParser::TArgumentParserResult& args) {
    auto config = GetConfig(args);

    PFDESolver::TResults results;
    NFDESolverService::TFDESolverService service;

    service.DoRunTask(config.get(), &results);
    SaveResults(args, results);
}

int main(int argc, char** argv) {
    auto args = ParseArgs(argc, argv);
    SolveTask(args);
    return 0;
}
