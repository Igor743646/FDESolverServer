#pragma once
#include <unordered_map>
#include <limits>
#include <numbers>
#include <numeric>
#include <cmath>
#include <functional>

#include <TLexer.h>
#include <TParser.h>
#include <TParserBaseVisitor.h>

namespace antlrcpptest {
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
            if (std::isnan(res)) {
                res = std::numeric_limits<double>::infinity();
            }
            return res;
        }

        std::unordered_map<std::string, double> Values;
    };


    class MathExpressionCalculator {
    public:

        MathExpressionCalculator(std::string expression, const std::vector<std::string>& variables) 
        : Expression(expression) {
            Expression = expression;
            for (auto& var : variables) {
                Visitor.SetValue(var, 0.0);
            }
        }

        void SetVar(std::string var, double value) {
            Visitor.SetValue(var, value);
        }

        double Calc() {
            antlr4::ANTLRInputStream input(Expression);
            antlrcpptest::TLexer lexer(&input);
            antlr4::CommonTokenStream tokens(&lexer);
            tokens.fill();
            antlrcpptest::TParser parser(&tokens);
            antlrcpptest::TParser::RootContext *tree = parser.root();
            
            if (tree == nullptr) {
                throw "Tree is nullptr";
            }
            try{
                std::any result = tree->accept(dynamic_cast<antlr4::tree::ParseTreeVisitor*>(&Visitor));

                if (result.has_value()) {
                    return std::any_cast<double>(result);
                }
            } catch (...) {
                std::cout << tree->toStringTree() << std::endl;
                std::cout << tree->toString() << std::endl;
            }
            

            throw "Can not calculate value";
        }

    private:

        antlrcpptest::ExpressionVisitor Visitor;
        std::unordered_map<std::string, double> Variables;
        std::string Expression;
    };
}

