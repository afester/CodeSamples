// Define a grammar for simple expressions

grammar Expression;

import ExpressionLexer;

expr  : expr '^' <assoc=right> expr # Power
      | expr op=('*'|'/') expr      # MulDiv    // higher precedence than Add
      | expr op=('+'|'-') expr      # AddSub
      | NUMBER                      # Number
      | ID                          # Id
      | ID OPAR expr CPAR           # Function
      | OPAR expr CPAR              # Par
      ;

// define constants to use for the operator type (op.getType())
MUL : '*' ;
ADD : '+' ;

// From http://www.amazon.de/The-Definitive-ANTLR-4-Reference/dp/1934356999
//"One of ANTLR v4’s most significant new features is its ability to handle (most
// kinds of) left-recursive rules. A left-recursive rule is one that invokes itself at
// the start of an alternative. For example, in this grammar, rule expr has alter-
// natives recursively  invoke  expr  on  the  left  edge. Specifying arithmetic 
// expression notation this way is dramatically easier than what we’d need for the 
// typical top-down parser strategy. In that strategy, we’d need multiple rules, 
// one for each operator precedence level." 

// from http://en.wikipedia.org/wiki/Syntax_diagram
//expression : term 
//           | expression '+' term
//           ;
//
//term       : factor 
//           | term '*' factor
//           ;
//
//factor     : constant
//           | variable
//           | '(' expression ')'
//           ;
//
//variable   : 'x' | 'y' | 'z'
//           ;
//
//constant   : digit
//           | digit constant
//           ;
//
//digit      : '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
//           ;
