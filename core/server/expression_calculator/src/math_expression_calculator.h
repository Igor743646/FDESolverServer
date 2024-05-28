#pragma once
#include <unordered_map>
#include <limits>
#include <numbers>
#include <numeric>
#include <cmath>
#include <functional>
#include <optional>

#include <TLexer.h>
#include <TParser.h>
#include <TParserBaseVisitor.h>

namespace ANTLRMathExpParser {
    class ExpressionVisitor : public TParserVisitor {
    public:

        ExpressionVisitor(std::unordered_map<std::string, double>&& vals) : Values(std::move(vals)) {}
        ExpressionVisitor() : Values() {}

        virtual void SetValue(std::string, double);
        virtual std::optional<double> GetValue(std::string);

        virtual std::any visitRoot(TParser::RootContext *) override;
        virtual std::any visitFunction(TParser::FunctionContext *) override;
        virtual std::any visitBrackets_operation(TParser::Brackets_operationContext *) override;
        virtual std::any visitIdential(TParser::IdentialContext *) override;
        virtual std::any visitValue(TParser::ValueContext *) override;
        virtual std::any visitUnary_operation(TParser::Unary_operationContext *) override;
        virtual std::any visitBinary_operation(TParser::Binary_operationContext *) override;
        virtual std::any visitTernary_operation(TParser::Ternary_operationContext *) override;
        virtual std::any visitFunc(TParser::FuncContext *) override;

    private:

        static constexpr double Gamma(double x) {
            return x > 0 
                ? std::tgamma(x) 
                : -std::numbers::pi_v<double> / (x * std::sin(std::numbers::pi_v<double> * x) * std::tgamma(-x));
        }

        static double Beta(double x, double y) {
            double res = Gamma(x) * Gamma(y) / Gamma(x + y);
            return std::isnan(res) 
                ? std::numeric_limits<double>::infinity() 
                : res;
        }

        std::unordered_map<std::string, double> Values;
    };


    class MathExpressionCalculator {
    public:

        MathExpressionCalculator(std::string, const std::vector<std::string>&);

        void SetVar(std::string, double);
        double Calc();

    private:

        ANTLRMathExpParser::ExpressionVisitor Visitor;
        std::unordered_map<std::string, double> Variables;
        std::unique_ptr<antlr4::ANTLRInputStream> Input;
        std::unique_ptr<ANTLRMathExpParser::TLexer> Lexer;
        std::unique_ptr<antlr4::CommonTokenStream> Tokens;
        std::unique_ptr<ANTLRMathExpParser::TParser> Parser;
        ANTLRMathExpParser::TParser::RootContext* Tree;
    };
}

