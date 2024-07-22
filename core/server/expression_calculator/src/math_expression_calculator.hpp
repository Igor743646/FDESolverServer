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

        explicit ExpressionVisitor(std::unordered_map<std::string, double>&& vals) : Values(std::move(vals)) {}
        ExpressionVisitor() : Values() {}

        virtual void SetValue(std::string, double);
        virtual std::optional<double> GetValue(std::string);

        virtual std::any visitRoot(TParser::RootContext *context) override;
        virtual std::any visitMainExpressionCondExpr(TParser::MainExpressionCondExprContext *context) override;
        virtual std::any visitMainExpressionConstant(TParser::MainExpressionConstantContext *context) override;
        virtual std::any visitMainExpressionIdentifier(TParser::MainExpressionIdentifierContext *context) override;
        virtual std::any visitConditionalExpression(TParser::ConditionalExpressionContext *context) override;
        virtual std::any visitLogicalOrExpression(TParser::LogicalOrExpressionContext *context) override;
        virtual std::any visitLogicalAndExpression(TParser::LogicalAndExpressionContext *context) override;
        virtual std::any visitEqualityExpression(TParser::EqualityExpressionContext *context) override;
        virtual std::any visitRelationalExpression(TParser::RelationalExpressionContext *context) override;
        virtual std::any visitAdditiveExpression(TParser::AdditiveExpressionContext *context) override;
        virtual std::any visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *context) override;
        virtual std::any visitCastExpressionUnary(TParser::CastExpressionUnaryContext *context) override;
        virtual std::any visitCastExpressionParen(TParser::CastExpressionParenContext *context) override;
        virtual std::any visitCastExpressionConstant(TParser::CastExpressionConstantContext *context) override;
        virtual std::any visitCastExpressionIdentifier(TParser::CastExpressionIdentifierContext *context) override;
        virtual std::any visitUnaryExpressionUnCastExpr(TParser::UnaryExpressionUnCastExprContext *context) override;
        virtual std::any visitUnaryExpressionFunc(TParser::UnaryExpressionFuncContext *context) override;
        virtual std::any visitFunctionExpression(TParser::FunctionExpressionContext *context) override;
        virtual std::any visitArgumentExpressionList(TParser::ArgumentExpressionListContext *context) override;

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

        MathExpressionCalculator(const std::string&, const std::vector<std::string>&);

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

