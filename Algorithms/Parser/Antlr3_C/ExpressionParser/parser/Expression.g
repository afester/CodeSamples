// Define a grammar for simple expressions

grammar Expression;

options {
  language=Cpp;
  backtrack=true;
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
}


start : expr+
      ;

expr : term
     | term '+' expr	{ std::cerr << "term: " << $term.text << std::endl;  }
     ;

term : factor 
     | factor '*' term
     ;

factor : NUMBER	        { std::cerr << "NUMBER: " << $NUMBER.text << std::endl;  }
       | ID				{ std::cerr << "ID: " << $ID.text << std::endl;  }
       | '(' expr ')'
       ;

NUMBER: '0'..'9' + ;

ID: 'a'..'z' + ;

WS: ( ' ' |'\n' |'\r')+ {$channel=HIDDEN;} ; // ignore whitespace
