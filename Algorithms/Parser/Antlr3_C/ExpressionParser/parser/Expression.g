// Define a grammar for simple expressions

grammar Expression;

options {
  language=Cpp;
}

@parser::includes {
   #include "ExpressionLexer.hpp"
   #include "SymbolTable.h"
   #include <iostream>
}

// transparently inserted into the public section of the parser class (in parser.hpp)
@parser::context {

   SymbolTable* st;

   void setSymbolTable(SymbolTable* theTable);

}

// transparently inserted into the parser.cpp file
@parser::members {

   void ExpressionParser::setSymbolTable(SymbolTable* theTable) {
      st = theTable;
   }

}

@lexer::traits {
  class ExpressionLexer;
  class ExpressionParser;
  typedef antlr3::Traits< ExpressionLexer, ExpressionParser > ExpressionLexerTraits;
  typedef ExpressionLexerTraits ExpressionParserTraits;

  typedef double NumberType;
}


/* Dummy rule to avoid "no start rule" warning */
start : expr
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
    : NUMBER	      { $value = atol($NUMBER.text.c_str());                 }
    | ID              { $value = st->getValue($ID.text);                     }
    | '(' expr ')'    { $value = $expr.value;                                }
    | ID '(' expr ')' { $value = st->executeFunction($ID.text, $expr.value); }
    ;

NUMBER: '0'..'9' + ;
ID:     'a'..'z' + ;
WS:     ( ' ' |'\n' |'\r')+ { $channel=HIDDEN; } ; // ignore whitespace
