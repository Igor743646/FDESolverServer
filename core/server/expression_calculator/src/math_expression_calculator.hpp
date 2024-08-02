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
    class TExpressionVisitor : public TParserVisitor {
    public:

        explicit TExpressionVisitor(std::unordered_map<std::string, double>&& vals) : Values(std::move(vals)) {}
        TExpressionVisitor() = default;

        virtual void SetValue(const std::string&, double);
        virtual std::optional<double> GetValue(const std::string&);

        std::any visitRoot(TParser::RootContext *context) override;
        std::any visitMainExpressionCondExpr(TParser::MainExpressionCondExprContext *context) override;
        std::any visitMainExpressionConstant(TParser::MainExpressionConstantContext *context) override;
        std::any visitMainExpressionIdentifier(TParser::MainExpressionIdentifierContext *context) override;
        std::any visitConditionalExpression(TParser::ConditionalExpressionContext *context) override;
        std::any visitLogicalOrExpression(TParser::LogicalOrExpressionContext *context) override;
        std::any visitLogicalAndExpression(TParser::LogicalAndExpressionContext *context) override;
        std::any visitEqualityExpression(TParser::EqualityExpressionContext *context) override;
        std::any visitRelationalExpression(TParser::RelationalExpressionContext *context) override;
        std::any visitAdditiveExpression(TParser::AdditiveExpressionContext *context) override;
        std::any visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *context) override;
        std::any visitCastExpressionUnary(TParser::CastExpressionUnaryContext *context) override;
        std::any visitCastExpressionParen(TParser::CastExpressionParenContext *context) override;
        std::any visitCastExpressionConstant(TParser::CastExpressionConstantContext *context) override;
        std::any visitCastExpressionIdentifier(TParser::CastExpressionIdentifierContext *context) override;
        std::any visitUnaryExpressionUnCastExpr(TParser::UnaryExpressionUnCastExprContext *context) override;
        std::any visitUnaryExpressionFunc(TParser::UnaryExpressionFuncContext *context) override;
        std::any visitFunctionExpression(TParser::FunctionExpressionContext *context) override;
        std::any visitArgumentExpressionList(TParser::ArgumentExpressionListContext *context) override;

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


    class TMathExpressionCalculator {
    public:

        TMathExpressionCalculator(const std::string&, const std::vector<std::string>&);

        void SetVar(const std::string&, double);
        double Calc();

    private:

        ANTLRMathExpParser::TExpressionVisitor Visitor;
        std::unordered_map<std::string, double> Variables;
        std::unique_ptr<antlr4::ANTLRInputStream> Input;
        std::unique_ptr<ANTLRMathExpParser::TLexer> Lexer;
        std::unique_ptr<antlr4::CommonTokenStream> Tokens;
        std::unique_ptr<ANTLRMathExpParser::TParser> Parser;
        ANTLRMathExpParser::TParser::RootContext* Tree;
    };
}  // namespace ANTLRMathExpParser

