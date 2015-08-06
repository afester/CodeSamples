grammar Simple;

options {
  language=Cpp;
}

@parser::includes {
   #include "SimpleLexer.hpp"
   #include <iostream>
}

@lexer::traits {
  class SimpleLexer;
  class SimpleParser;
  typedef antlr3::Traits< SimpleLexer, SimpleParser > SimpleLexerTraits;
  typedef SimpleLexerTraits SimpleParserTraits;
}


/** Match things like "call foo;" */
r : 'call' ID ';' { std::cout << "Test" << $ID.text ; } ;

ID: 'a' ..  'z' + ;

WS: ( ' ' |'\n' |'\r')+ {$channel=HIDDEN;} ; // ignore whitespace
