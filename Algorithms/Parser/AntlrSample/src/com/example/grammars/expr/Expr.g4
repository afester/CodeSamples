/** 
 * From The definitive antlr 4 reference, chapter 3.1
 * 
 * Note: the Eclipse plugin automatically uses the package name where
 * the grammar is located for the generated sources!
 */

grammar Expr;

prog: stat+ ;

stat : expr NEWLINE
     | ID '=' NEWLINE
     | NEWLINE
     ;

expr : expr ('*'|'/') expr
	 | expr ('+'|'-') expr
     | INT
     | ID
     | '(' expr ')'
     ;

ID 	: [a-zA-Z]+	;
INT	: [0-9]+    	;
NEWLINE: '\r'? '\n'  ;
WS	: [ \t]+ -> skip ;
