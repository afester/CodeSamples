lexer grammar ExpressionLexerRules;

// match paranthesis
OPAR : '(' ; 
CPAR : ')' ;

// match a number
NUMBER: [0-9]+('.'[0-9]+)? ;

// match an operator
// OPER : [\+\-\*/^] ;

// match lower-case identifiers
ID : [A-Za-z]+ ;

// skip spaces, tabs, newlines
WS : [ \t\r\n]+ -> skip ;
