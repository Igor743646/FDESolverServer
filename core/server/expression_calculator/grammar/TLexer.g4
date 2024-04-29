lexer grammar TLexer;

// Standart operators
Plus: 		'+';
Minus: 		'-';
Mul: 		'*';
Div: 		'/';

// Boolean operators
LT: 	'<';
GT:  	'>';
LEQ: 	'<=';
GEQ: 	'>=';
Equal: 	'==';
And: 	'&&';
Or: 	'||';

// Other
OpenPar: 	'(';
ClosePar: 	')';
Comma: 		',';
Dot:        '.';
Qmark:      '?';
Colon:      ':';

fragment Digit: 	    [0-9];
Int:    Digit+;
Float:  Digit* Dot Digit+;

fragment Letter: [a-zA-Z];
Id: Letter (Letter | '0'..'9')*;



WS : [ \r\t\n]+ -> skip;
