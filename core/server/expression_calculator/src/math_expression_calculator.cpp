#include "math_expression_calculator.h"

namespace ANTLRMathExpParser {

    void ExpressionVisitor::SetValue(std::string name, double val) {
        Values[name] = val;
    }

    std::optional<double> ExpressionVisitor::GetValue(std::string name) {
        if (!Values.contains(name)) {
            return std::nullopt;
        }
        
        return Values[name];
    }

    std::any ExpressionVisitor::visitRoot(TParser::RootContext *context) {
        std::any childResult = context->children[0]->accept(this);
        double result = std::any_cast<double>(childResult);

        return result;
    }

    std::any ExpressionVisitor::visitFunction(TParser::FunctionContext *context) {
        return context->children[0]->accept(this);
    }

    std::any ExpressionVisitor::visitBrackets_operation(TParser::Brackets_operationContext *context) {
        return context->children[1]->accept(this);
    }

    std::any ExpressionVisitor::visitIdential(TParser::IdentialContext *context) {

        std::string val = context->getText();
        if (Values.contains(val)) {
            return Values[val];
        }

        throw "visitIdential";
    }

    std::any ExpressionVisitor::visitValue(TParser::ValueContext *context) {
        return std::stod(context->getText());
    }

    std::any ExpressionVisitor::visitUnary_operation(TParser::Unary_operationContext *context) {
        std::string op = std::any_cast<std::string>(context->children[0]->getText());
        double a = std::any_cast<double>(context->children[1]->accept(this));

        if (op == "-") return -a;
        if (op == "+") return a;

        throw "visitUnary_operation";
    }

    std::any ExpressionVisitor::visitBinary_operation(TParser::Binary_operationContext *context) {
        double a = std::any_cast<double>(context->children[0]->accept(this));
        double b = std::any_cast<double>(context->children[2]->accept(this));
        std::string op = std::any_cast<std::string>(context->children[1]->getText());

        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") return a / b;
        if (op == "<") return static_cast<double>(a < b);
        if (op == ">") return static_cast<double>(a > b);
        if (op == "<=") return static_cast<double>(a <= b);
        if (op == ">=") return static_cast<double>(a >= b);
        if (op == "==") return static_cast<double>(a == b);
        if (op == "&&") return static_cast<double>(a && b);
        if (op == "||") return static_cast<double>(a || b);

        throw "visitBinary_operation";
    }

    std::any ExpressionVisitor::visitTernary_operation(TParser::Ternary_operationContext *context) {
        double a = std::any_cast<double>(context->children[0]->accept(this));
        double b = std::any_cast<double>(context->children[2]->accept(this));
        double c = std::any_cast<double>(context->children[4]->accept(this));
        return (a ? b : c);
    }

    std::any ExpressionVisitor::visitFunc(TParser::FuncContext *context) {
        std::string functionName = std::any_cast<std::string>(context->children[0]->getText());
        std::vector<double> arguments;

        for (int i = 2; i < context->children.size(); i += 2) {
            double temp = std::any_cast<double>(context->children[i]->accept(this));
            arguments.push_back(temp);
        }

        if (functionName == "sin") return std::sin(arguments[0]);
        if (functionName == "cos") return std::cos(arguments[0]);
        if (functionName == "Gamma") return Gamma(arguments[0]);
        if (functionName == "Beta") return Beta(arguments[0], arguments[1]);
        if (functionName == "lgamma") return std::lgamma(arguments[0]);
        if (functionName == "log") return std::log(arguments[0]);
        if (functionName == "pow") return std::pow(arguments[0], arguments[1]);
        
        throw "visitFunc";
    }
}

namespace ANTLRMathExpParser {
    MathExpressionCalculator::MathExpressionCalculator(std::string expression, const std::vector<std::string>& variables) {
        Input = std::make_unique<antlr4::ANTLRInputStream>(expression);
        Lexer = std::make_unique<ANTLRMathExpParser::TLexer>(Input.get());
        Tokens = std::make_unique<antlr4::CommonTokenStream>(Lexer.get());
        Tokens->fill();
        Parser = std::make_unique<ANTLRMathExpParser::TParser>(Tokens.get());
        Tree = Parser->root();
        
        for (auto& var : variables) {
            Visitor.SetValue(var, 0.0);
        }
    }

    void MathExpressionCalculator::SetVar(std::string var, double value) {
        Visitor.SetValue(var, value);
    }

    double MathExpressionCalculator::Calc() {
        if (Tree) {
            std::any result = Tree->accept(dynamic_cast<antlr4::tree::ParseTreeVisitor*>(&Visitor));
            return std::any_cast<double>(result);
        }

        throw "Can not calculate value because parse tree error (Tree = nullptr)";
    }
}
