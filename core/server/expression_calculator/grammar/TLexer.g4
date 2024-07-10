lexer grammar TLexer;

PLUS: '+';
MINUS: '-';
TIMES: '*';
DIV: '/';

LPAREN: '(';
RPAREN: ')';

LT: '<';
GT: '>';
LEQ: '<=';
GEQ: '>=';
EQ: '==';
NEQ: '!=';
NOT: '!';
AND: '&&';
OR: '||';

COMMA: ',';
DOT: '.';
QUESTION: '?';
COLON: ':';

Identifier
    : IdentifierNondigit (IdentifierNondigit | Digit)*
    ;

Constant
    : IntegerConstant
    | FloatingConstant
    ;

DigitSequence
    : Digit+
    ;

fragment IdentifierNondigit
    : [a-zA-Z_]
    ;

fragment Digit
    : [0-9]
    ;

fragment NonzeroDigit
    : [1-9]
    ;

fragment IntegerConstant
    : DecimalConstant
    ;

fragment DecimalConstant
    : NonzeroDigit Digit*
    | Digit
    ;

fragment FloatingConstant
    : DecimalFloatingConstant
    ;

fragment DecimalFloatingConstant
    : FractionalConstant ExponentPart? 
    | DigitSequence ExponentPart 
    ;

fragment FractionalConstant
    : DigitSequence? '.' DigitSequence
    | DigitSequence '.'
    ;

fragment ExponentPart
    : [eE] Sign? DigitSequence
    ;

fragment Sign
    : [+-]
    ;

WS
    : [ \r\n\t]+ -> skip
    ;
