// Define a grammar for simple expressions

grammar Expression;

options {
  language=Cpp;
}

@parser::includes {
   #include "ExpressionLexer.hpp"
   #include <iostream>
}

@lexer::traits {
  class ExpressionLexer;
  class ExpressionParser;
  typedef antlr3::Traits< ExpressionLexer, ExpressionParser > ExpressionLexerTraits;
  typedef ExpressionLexerTraits ExpressionParserTraits;

  typedef double NumberType;
}


start : expr { std::cerr << "RESULT:" << $expr.value << std::endl; }
      ;

expr returns [NumberType value]
      : t1=term { $value = $t1.value; }
             ('+' t2=term { $value = $value + $t2.value; }
             |'-' t2=term { $value = $value - $t2.value; }
             )*
      ;

term returns [NumberType value]
      : f1=factor { $value = $f1.value; } 
             ('*' f2=factor { $value = $value * $f2.value; }
             |'/' f2=factor { $value = $value / $f2.value; }
             )*
      ;

factor returns [NumberType value]
    : NUMBER	        { $value = atol($NUMBER.text.c_str()); }
    | '(' expr ')'	{ $value = $expr.value; }
    ;

NUMBER: '0'..'9' + ;
ID:     'a'..'z' + ;
WS:     ( ' ' |'\n' |'\r')+ { $channel=HIDDEN; } ; // ignore whitespace
