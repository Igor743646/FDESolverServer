#include "argument_parser.hpp"
#include <string>
#include <cstring>
#include <iomanip>

namespace NArgumentParser {

    TArgumentParser::TArgumentParser() {
        AddArgument("--help", TArgumentType::FLAG, "show this help message and exit");
    }

    void TArgumentParser::AddArgument(const std::string& argName, 
                                      TArgumentType type, 
                                      const std::string& description, 
                                      bool require,
                                      std::any defaultValue) 
    {
        Arguments[argName] = TArgumentInfo{
            .Type = type,
            .Require = require,
            .Description = description,
            .Default = std::move(defaultValue),
        };
    }

    void TArgumentParser::Info() {
        std::cout << "usage: FDESolverCLI_<build_type>.exe [--help] [--file <file_name>]" << std::endl;
        std::cout << std::endl;
        std::cout << "options:" << std::endl;
        for (auto& [k, v] : Arguments) {
            std::cout << "\t" << std::setw(15) << k << "\t\t\t" << v.Help() << std::endl;
        }
        std::exit(0);
    }

    TArgumentParserResult TArgumentParser::Parse(int argc, char** argv) {
        TArgumentParserResult result;

        for (int i = 1; i < argc; i++) {
            if (!Arguments.contains(std::string(argv[i]))) {
                std::cerr << "Wrong argument: " << argv[i] << std::endl;
                std::exit(1);
            }
            if (std::strcmp("--help", argv[i]) == 0) {
                Info();
            }
            TArgumentInfo argInfo = Arguments[std::string(argv[i])];

            switch (argInfo.Type) {
                case TArgumentType::FLAG: {
                    result.Set<TArgumentTypeAliasType<TArgumentType::FLAG>>(argInfo, argv[i], true);
                    break;
                }
                case TArgumentType::INT_VALUE: {
                    i++;
                    if (i >= argc) {
                        std::cerr << "Wrong argument count!" << std::endl;
                        std::cerr << "After " << argv[i - 1] << " must be int value" << std::endl;
                        std::exit(1);
                    }
                    result.Set<TArgumentTypeAliasType<TArgumentType::INT_VALUE>>(argInfo, argv[i - 1], std::stoi(argv[i]));
                    break;
                }
                case TArgumentType::STRING_VALUE: {
                    i++;
                    if (i >= argc) {
                        std::cerr << "Wrong argument count!" << std::endl;
                        std::cerr << "After " << argv[i - 1] << " must be string value" << std::endl;
                        std::exit(1);
                    }
                    result.Set<TArgumentTypeAliasType<TArgumentType::STRING_VALUE>>(argInfo, argv[i - 1], argv[i]);
                    break;
                }
                default: {
                    std::cerr << "Unimplimented type: " << (int8_t)argInfo.Type << std::endl;
                }
            }
        }

        for (auto& [k, v] : Arguments) {
            if (v.HasDefault() && !result.Has(k)) {
                result.SetDefault(v, k, v.Default);
                continue;
            }
            if (v.IsRequire() && !result.Has(k)) {   
                std::cerr << "Argument " << k << " is require" << std::endl;
                std::exit(1);
            }
        }

        return result;
    }

}
