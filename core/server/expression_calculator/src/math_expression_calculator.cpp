#include "math_expression_calculator.hpp"
#include <random>
#include <stacktracer.hpp>
#include <utils.hpp>

namespace {
    constexpr size_t ArgumentListMax = 5;
}

namespace ANTLRMathExpParser {

    void TExpressionVisitor::SetValue(const std::string& name, double val) {
        Values[name] = val;
    }

    std::optional<double> TExpressionVisitor::GetValue(const std::string& name) {
        auto iter = Values.find(name);
        if (iter == Values.end()) {
            return std::nullopt;
        }
        
        return iter->second;
    }

    std::any TExpressionVisitor::visitRoot(TParser::RootContext *context) {
        std::any childResult = context->children[0]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any TExpressionVisitor::visitMainExpressionCondExpr(TParser::MainExpressionCondExprContext *context) {
        std::any childResult = context->children[0]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any TExpressionVisitor::visitMainExpressionConstant(TParser::MainExpressionConstantContext *context) {
        return std::stod(context->getText());
    }

    std::any TExpressionVisitor::visitMainExpressionIdentifier(TParser::MainExpressionIdentifierContext *context) {
        std::string val = context->getText();
        
        if (Values.contains(val)) {
            return Values[val];
        }

        NStackTracer::TStackTracer::ThrowWithMessage("Error: Identifier not found");
        NUtils::Unreachable();
    }

    std::any TExpressionVisitor::visitConditionalExpression(TParser::ConditionalExpressionContext *context) {
        std::any logicalExpr = context->children[0]->accept(this);
        double result = std::any_cast<double>(logicalExpr);

        if (context->children.size() <= 1) {
            return result;
        }

        if (result != 0.0) {
            return std::any_cast<double>(context->children[2]->accept(this));
        }
        
        return std::any_cast<double>(context->children[4]->accept(this));
    }

    std::any TExpressionVisitor::visitLogicalOrExpression(TParser::LogicalOrExpressionContext *context) {
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

    std::any TExpressionVisitor::visitLogicalAndExpression(TParser::LogicalAndExpressionContext *context) {
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

    std::any TExpressionVisitor::visitEqualityExpression(TParser::EqualityExpressionContext *context) {
        std::any relationalExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(relationalExp);

        if (context->children.size() > 1) {
            std::string opr = context->children[1]->getText();
            std::any relationalExp2 = context->children[2]->accept(this);
            
            if (opr == "==") {
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

    std::any TExpressionVisitor::visitRelationalExpression(TParser::RelationalExpressionContext *context) {
        std::any additiveExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(additiveExp);

        if (context->children.size() > 1) {
            std::string opr = context->children[1]->getText();
            std::any additiveExp2 = context->children[2]->accept(this);
            
            if (opr == ">") {
                if (result > std::any_cast<double>(additiveExp2)) {
                    return 1.0;
                } 
            } else if (opr == "<") {
                if (result < std::any_cast<double>(additiveExp2)) {
                    return 1.0;
                } 
            } else if (opr == ">=") {
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

    std::any TExpressionVisitor::visitAdditiveExpression(TParser::AdditiveExpressionContext *context) {
        std::any multiplyExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(multiplyExp);

        if (context->children.size() > 1) {
            for (size_t i = 2; i < context->children.size(); i+=2) {
                std::string opr = context->children[i - 1]->getText();

                if (opr == "+") {
                    result += std::any_cast<double>(context->children[i]->accept(this));
                } else {
                    result -= std::any_cast<double>(context->children[i]->accept(this));
                }
            }
        }

        return result;
    }

    std::any TExpressionVisitor::visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *context) {
        std::any castExp = context->children[0]->accept(this);
        double result = std::any_cast<double>(castExp);

        if (context->children.size() > 1) {
            for (size_t i = 2; i < context->children.size(); i+=2) {
                std::string opr = context->children[i - 1]->getText();

                if (opr == "*") {
                    result *= std::any_cast<double>(context->children[i]->accept(this));
                } else {
                    result /= std::any_cast<double>(context->children[i]->accept(this));
                }
            }
        }

        return result;
    }

    std::any TExpressionVisitor::visitCastExpressionUnary(TParser::CastExpressionUnaryContext *context) {
        std::any childResult = context->children[0]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any TExpressionVisitor::visitCastExpressionParen(TParser::CastExpressionParenContext *context) {
        std::any childResult = context->children[1]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any TExpressionVisitor::visitCastExpressionConstant(TParser::CastExpressionConstantContext *context) {
        return std::stod(context->getText());
    }

    std::any TExpressionVisitor::visitCastExpressionIdentifier(TParser::CastExpressionIdentifierContext *context) {
        std::string val = context->getText();
        
        if (Values.contains(val)) {
            return Values[val];
        }

        NStackTracer::TStackTracer::ThrowWithMessage("Error: Identifier not found");
        NUtils::Unreachable();
    }

    std::any TExpressionVisitor::visitUnaryExpressionUnCastExpr(TParser::UnaryExpressionUnCastExprContext *context) {
        std::any castExp = context->children[1]->accept(this);
        double result = std::any_cast<double>(castExp);

        std::string opr = context->children[0]->getText();
        if (opr == "-") {
            return -result;
        }

        if (opr == "!") {
            return ((result == 0.0) ? 1.0 : 0.0);
        }

        return result;
    }

    std::any TExpressionVisitor::visitUnaryExpressionFunc(TParser::UnaryExpressionFuncContext *context) {
        std::any childResult = context->children[0]->accept(this);
        return std::any_cast<double>(childResult);
    }

    std::any TExpressionVisitor::visitFunctionExpression(TParser::FunctionExpressionContext *context) {
        std::string functionName = std::any_cast<std::string>(context->children[0]->getText());

        if (context->children.size() == 3) {
            if (functionName == "randomf") {
                std::random_device device;
                std::default_random_engine engine(device());
                std::uniform_real_distribution<double> urd;
                return urd(engine);
            }
        } else {
            auto arguments = std::any_cast<std::array<double, ArgumentListMax>>(context->children[2]->accept(this));
            
            if (functionName == "abs") {return std::abs(arguments[0]);}
            if (functionName == "sin") {return std::sin(arguments[0]);}
            if (functionName == "cos") {return std::cos(arguments[0]);}
            if (functionName == "Gamma") {return Gamma(arguments[0]);}
            if (functionName == "Beta") {return Beta(arguments[0], arguments[1]);}
            if (functionName == "lgamma") {return std::lgamma(arguments[0]);}
            if (functionName == "log") {return std::log(arguments[0]);}
            if (functionName == "pow") {return std::pow(arguments[0], arguments[1]);}
        }

        NStackTracer::TStackTracer::ThrowWithMessage("No function: unimpemented");
        NUtils::Unreachable();
    }

    std::any TExpressionVisitor::visitArgumentExpressionList(TParser::ArgumentExpressionListContext *context) {
        std::array<double, ArgumentListMax> result = {0};

        for (size_t i = 0; (i << 1) < context->children.size() && i < ArgumentListMax; i++) {
            std::any mainExp = context->children[i << 1]->accept(this);
            result.at(i) = std::any_cast<double>(mainExp);
        }
        
        return result;
    }
}  // namespace ANTLRMathExpParser

namespace ANTLRMathExpParser {
    TMathExpressionCalculator::TMathExpressionCalculator(const std::string& expression, const std::vector<std::string>& variables) {
        Input = std::make_unique<antlr4::ANTLRInputStream>(expression);
        Lexer = std::make_unique<ANTLRMathExpParser::TLexer>(Input.get());
        Tokens = std::make_unique<antlr4::CommonTokenStream>(Lexer.get());
        Tokens->fill();
        Parser = std::make_unique<ANTLRMathExpParser::TParser>(Tokens.get());
        Tree = Parser->root();
        
        for (const auto& var : variables) {
            Visitor.SetValue(var, 0.0);
        }
    }

    void TMathExpressionCalculator::SetVar(const std::string& var, double value) {
        Visitor.SetValue(var, value);
    }

    double TMathExpressionCalculator::Calc() {
        STACK_ASSERT(Tree != nullptr, "Can not calculate value because parse tree error (Tree = nullptr)");
        
        std::any result = Tree->accept(dynamic_cast<antlr4::tree::ParseTreeVisitor*>(&Visitor));
        return std::any_cast<double>(result);
    }
}  // namespace ANTLRMathExpParser
