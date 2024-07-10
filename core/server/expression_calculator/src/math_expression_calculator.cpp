#include "math_expression_calculator.hpp"
#include <random>

namespace {
    constexpr size_t ArgumentListMax = 5;
}

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
        return std::any_cast<double>(childResult);
    }

    std::any ExpressionVisitor::visitMainExpressionCondExpr(TParser::MainExpressionCondExprContext *context) {
        std::any childResult = context->children[0]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any ExpressionVisitor::visitMainExpressionConstant(TParser::MainExpressionConstantContext *context) {
        return std::stod(context->getText());
    }

    std::any ExpressionVisitor::visitMainExpressionIdentifier(TParser::MainExpressionIdentifierContext *context) {
        std::string val = context->getText();
        
        if (Values.contains(val)) {
            return Values[val];
        }

        throw std::exception("Error: Identifier not found");
    }

    std::any ExpressionVisitor::visitConditionalExpression(TParser::ConditionalExpressionContext *context) {
        std::any logicalExpr = context->children[0]->accept(this);
        double result = std::any_cast<double>(logicalExpr);

        if (context->children.size() > 1) {
            if (result != 0.0) {
                return std::any_cast<double>(context->children[2]->accept(this));
            } else {
                return std::any_cast<double>(context->children[4]->accept(this));
            }
        }

        return result;
    }

    std::any ExpressionVisitor::visitLogicalOrExpression(TParser::LogicalOrExpressionContext *context) {
        std::any logicalAnd = context->children[0]->accept(this);
        double result = std::any_cast<double>(logicalAnd);

        if (context->children.size() > 1) {
            if (result != 0.0) {
                return 1.0;
            } 
            
            for (size_t i = 2; i < context->children.size(); i+=2) {
                result = std::any_cast<double>(context->children[i]->accept(this));
                if (result != 0.0) {
                    return 1.0;
                }
            }
            
            return 0.0;
        }

        return result;
    }

    std::any ExpressionVisitor::visitLogicalAndExpression(TParser::LogicalAndExpressionContext *context) {
        std::any equalityExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(equalityExp);

        if (context->children.size() > 1) {
            if (result == 0.0) {
                return 0.0;
            } 
            
            for (size_t i = 2; i < context->children.size(); i+=2) {
                result = std::any_cast<double>(context->children[i]->accept(this));
                if (result == 0.0) {
                    return 0.0;
                }
            }
            
            return 1.0;
        }

        return result;
    }

    std::any ExpressionVisitor::visitEqualityExpression(TParser::EqualityExpressionContext *context) {
        std::any relationalExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(relationalExp);

        if (context->children.size() > 1) {
            std::string op = context->children[1]->getText();
            std::any relationalExp2 = context->children[2]->accept(this);
            
            if (op == "==") {
                if (result == std::any_cast<double>(relationalExp2)) {
                    return 1.0;
                } 
            } else {
                if (result != std::any_cast<double>(relationalExp2)) {
                    return 1.0;
                }
            }

            return 0.0;
        }

        return result;
    }

    std::any ExpressionVisitor::visitRelationalExpression(TParser::RelationalExpressionContext *context) {
        std::any additiveExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(additiveExp);

        if (context->children.size() > 1) {
            std::string op = context->children[1]->getText();
            std::any additiveExp2 = context->children[2]->accept(this);
            
            if (op == ">") {
                if (result > std::any_cast<double>(additiveExp2)) {
                    return 1.0;
                } 
            } else if (op == "<") {
                if (result < std::any_cast<double>(additiveExp2)) {
                    return 1.0;
                } 
            } else if (op == ">=") {
                if (result >= std::any_cast<double>(additiveExp2)) {
                    return 1.0;
                } 
            } else {
                if (result <= std::any_cast<double>(additiveExp2)) {
                    return 1.0;
                }
            }
            
            return 0.0;
        }

        return result;
    }

    std::any ExpressionVisitor::visitAdditiveExpression(TParser::AdditiveExpressionContext *context) {
        std::any multiplyExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(multiplyExp);

        if (context->children.size() > 1) {
            for (size_t i = 2; i < context->children.size(); i+=2) {
                std::string op = context->children[i - 1]->getText();

                if (op == "+") {
                    result += std::any_cast<double>(context->children[i]->accept(this));
                } else {
                    result -= std::any_cast<double>(context->children[i]->accept(this));
                }
            }
        }

        return result;
    }

    std::any ExpressionVisitor::visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *context) {
        std::any castExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(castExp);

        if (context->children.size() > 1) {
            for (size_t i = 2; i < context->children.size(); i+=2) {
                std::string op = context->children[i - 1]->getText();

                if (op == "*") {
                    result *= std::any_cast<double>(context->children[i]->accept(this));
                } else {
                    result /= std::any_cast<double>(context->children[i]->accept(this));
                }
            }
        }

        return result;
    }

    std::any ExpressionVisitor::visitCastExpressionUnary(TParser::CastExpressionUnaryContext *context) {
        std::any childResult = context->children[0]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any ExpressionVisitor::visitCastExpressionParen(TParser::CastExpressionParenContext *context) {
        std::any childResult = context->children[1]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any ExpressionVisitor::visitCastExpressionConstant(TParser::CastExpressionConstantContext *context) {
        return std::stod(context->getText());
    }

    std::any ExpressionVisitor::visitCastExpressionIdentifier(TParser::CastExpressionIdentifierContext *context) {
        std::string val = context->getText();
        
        if (Values.contains(val)) {
            return Values[val];
        }

        throw std::exception("Error: Identifier not found");
    }

    std::any ExpressionVisitor::visitUnaryExpressionUnCastExpr(TParser::UnaryExpressionUnCastExprContext *context) {
        std::any castExp = context->children[1]->accept(this);
        double result = std::any_cast<double>(castExp);

        std::string op = context->children[0]->getText();
        if (op == "-") {
            return -result;
        } else if (op == "!") {
            return ((result == 0.0) ? 1.0 : 0.0);
        }

        return result;
    }

    std::any ExpressionVisitor::visitUnaryExpressionFunc(TParser::UnaryExpressionFuncContext *context) {
        std::any childResult = context->children[0]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any ExpressionVisitor::visitFunctionExpression(TParser::FunctionExpressionContext *context) {
        std::string functionName = std::any_cast<std::string>(context->children[0]->getText());

        if (context->children.size() == 3) {
            if (functionName == "randomf") {
                std::random_device r;
                std::default_random_engine e1(r());
                std::uniform_real_distribution<double> urd;
                return urd(e1);
            }
        } else {
            auto arguments = std::any_cast<std::array<double, ArgumentListMax>>(context->children[2]->accept(this));
            
            if (functionName == "sin") {return std::sin(arguments[0]);}
            if (functionName == "cos") {return std::cos(arguments[0]);}
            if (functionName == "Gamma") {return Gamma(arguments[0]);}
            if (functionName == "Beta") {return Beta(arguments[0], arguments[1]);}
            if (functionName == "lgamma") {return std::lgamma(arguments[0]);}
            if (functionName == "log") {return std::log(arguments[0]);}
            if (functionName == "pow") {return std::pow(arguments[0], arguments[1]);}
        }

        throw "visitFunc";
    }

    std::any ExpressionVisitor::visitArgumentExpressionList(TParser::ArgumentExpressionListContext *context) {
        std::array<double, ArgumentListMax> result = {0};

        for (size_t i = 0; i < context->children.size() && i < 10; i+=2) {
            std::any mainExp = context->children[i]->accept(this);
            result[i >> 1] = std::any_cast<double>(mainExp);
        }
        
        return result;
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

        for (auto& var : variables) {
            Visitor.SetValue(var, 0.0);
        }
    }

    void MathExpressionCalculator::SetVar(std::string var, double value) {
        Visitor.SetValue(var, value);
    }

    double MathExpressionCalculator::Calc() {
        if (Tree != nullptr) {
            std::any result = Tree->accept(dynamic_cast<antlr4::tree::ParseTreeVisitor*>(&Visitor));
            return std::any_cast<double>(result);
        }

        throw "Can not calculate value because parse tree error (Tree = nullptr)";
    }
}
