// Define a grammar called Hello

// combined grammar - allow both lexer and parser rules.
// to allow lexer rules only, use "lexer grammar ..." ; to allow parser rules
// only, user "parser grammar ..." 
grammar Hello;
r  : 'hello' ID ;         // match keyword hello followed by an identifier
ID : [a-z]+ ;             // match lower-case identifiers
WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines
