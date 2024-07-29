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

    template<TArgumentType T>
    struct TArgumentTypeAlias : std::type_identity<void> {};

    template<>
    struct TArgumentTypeAlias<TArgumentType::FLAG> : std::type_identity<bool> {};

    template<>
    struct TArgumentTypeAlias<TArgumentType::INT_VALUE> : std::type_identity<int64_t> {};

    template<>
    struct TArgumentTypeAlias<TArgumentType::STRING_VALUE> : std::type_identity<std::string> {};

    template<TArgumentType T>
    using TArgumentTypeAliasType = typename TArgumentTypeAlias<T>::type;

    class TArgumentParserResult;
    struct TArgumentInfo {
        TArgumentType   Type;
        bool            Require;
        std::string     Description;
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
        void Set(const TArgumentInfo& info, const std::string& key, Args... args) {
            Results[key].first.emplace<T>(T(std::forward<Args>(args))...);
            Results[key].second = info;
        }

        void SetDefault(const TArgumentInfo& info, const std::string& key, std::any& defaultValue) {
            Results[key].first.swap(defaultValue);
            Results[key].second = info;
        }

        bool Has(const std::string& key) const {
            return Results.contains(key);
        }

        const std::string& Help(const std::string& key) const {
            return Results.at(key).second.Help();
        }

        template<class T>
        T Get(const std::string& key) {
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
