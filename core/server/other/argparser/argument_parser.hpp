#pragma once

#include <iostream>
#include <unordered_map>
#include <any>

namespace NArgumentParser {

    enum class TArgumentType : int8_t {
        FLAG,
        INT_VALUE,
        STRING_VALUE,
    };

    class TArgumentParserResult;
    struct TArgumentInfo {
        TArgumentType   Type;
        std::string     Description;
        bool            Require;
        std::any        Default;

        const std::string& Help() const {
            return Description;
        }

        bool IsRequire() const {
            return Require;
        }

        bool HasDefault() const {
            return Default.has_value();
        }
    };
}

namespace NArgumentParser {
    class TArgumentParser {
        friend TArgumentParserResult;

    public:
        TArgumentParser();
        void AddArgument(const std::string&, 
                         TArgumentType, 
                         const std::string& = "", 
                         bool = false,
                         std::any = std::any{});
        TArgumentParserResult Parse(int, char**);
    private:
        void Info();
        std::unordered_map<std::string, TArgumentInfo> Arguments;
    };

    class TArgumentParserResult {
    public:

        TArgumentParserResult() = default;

        template<class T, class... Args>
        void Set(const TArgumentInfo& info, const std::string& key, Args&&... args) {
            Results[key].first.emplace<T>(std::forward<Args>(args)...);
            Results[key].second = info;
        }

        template<>
        void Set<std::any, std::any&>(const TArgumentInfo& info, const std::string& key, std::any& defaultValue) {
            Results[key].first.swap(defaultValue);
            Results[key].second = info;
        }

        bool Has(const std::string& key) const {
            if (Results.contains(key)) {
                return true;
            }

            return false;
        }

        const std::string& Help(const std::string& key) const {
            return Results.at(key).second.Help();
        }

        template<class T>
        T& Get(const std::string& key) {
            return std::any_cast<T>(Results.at(key).first);
        }

        template<class T>
        const T& Get(const std::string& key) const {
            return std::any_cast<T>(Results.at(key).first);
        }

    private:

        std::unordered_map<std::string, std::pair<std::any, TArgumentInfo>> Results;
    };
}


