parser grammar TParser;

options {
	tokenVocab = TLexer;
}

root
    : mainExpression EOF
    ;

mainExpression
    : conditionalExpression     # mainExpressionCondExpr
    | Constant                  # mainExpressionConstant
    | Identifier                # mainExpressionIdentifier
    ;

conditionalExpression
    : logicalOrExpression (QUESTION mainExpression COLON conditionalExpression)?
    ;

logicalOrExpression
    : logicalAndExpression (OR logicalAndExpression)*
    ;

logicalAndExpression
    : equalityExpression (AND equalityExpression)*
    ;

equalityExpression
    : relationalExpression ((EQ | NEQ) relationalExpression)?
    ;

relationalExpression
    : additiveExpression ((LT | GT | LEQ | GEQ) additiveExpression)?
    ;

additiveExpression
    : multiplicativeExpression ((PLUS | MINUS) multiplicativeExpression)*
    ;

multiplicativeExpression
    : castExpression ((TIMES | DIV) castExpression)*
    ;

castExpression
    : unaryExpression                       # castExpressionUnary
    | LPAREN conditionalExpression RPAREN   # castExpressionParen
    | Constant                              # castExpressionConstant
    | Identifier                            # castExpressionIdentifier
    ;

unaryExpression
    : (PLUS | MINUS | NOT) castExpression   # unaryExpressionUnCastExpr
    | functionExpression                    # unaryExpressionFunc
    ;

functionExpression
    : Identifier LPAREN argumentExpressionList? RPAREN
    ;

argumentExpressionList
    : mainExpression (COMMA mainExpression)*
    ;
