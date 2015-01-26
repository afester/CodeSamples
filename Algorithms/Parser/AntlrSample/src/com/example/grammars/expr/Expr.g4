/** 
 * From The definitive antlr 4 reference, chapter 3.1
 * 
 * Note: the Eclipse plugin automatically uses the package name where
 * the grammar is located for the generated sources!
 */

grammar Expr;

import Lexical;

prog: stat+ ;

stat : expr NEWLINE
     | ID '=' expr NEWLINE
     | NEWLINE
     ;

expr : expr ('*'|'/') expr
	 | expr ('+'|'-') expr
     | INT
     | ID
     | '(' expr ')'
     ;
