// Define a grammar for simple expressions

grammar ExpressionAST;

options {
  language=Cpp;
}

@parser::includes {
   #include "ExpressionASTLexer.hpp"
   #include "SymbolTable.h"
   #include "ExpressionAST.h"
   #include <iostream>
}

// transparently inserted into the public section of the parser class (in parser.hpp)
@parser::context {

   SymbolTable* st;

   void setSymbolTable(SymbolTable* theTable);

}

// transparently inserted into the parser.cpp file
@parser::members {

   void ExpressionASTParser::setSymbolTable(SymbolTable* theTable) {
      st = theTable;
   }

}

@lexer::traits {
  class ExpressionASTLexer;
  class ExpressionASTParser;
  typedef antlr3::Traits< ExpressionASTLexer, ExpressionASTParser > ExpressionASTLexerTraits;
  typedef ExpressionASTLexerTraits ExpressionASTParserTraits;
}

/* Rules *********************************************************************/

/* Dummy rule to avoid "no start rule" warning */
start : expr
      ;

expr returns [Node* result]
      : t1=term { $result = $t1.result; }
             ('+' t2=term { $result = new BinaryOperationNode(OP_ADD, $t1.result, $t2.result); }
             |'-' t2=term { $result = new BinaryOperationNode(OP_SUB, $t1.result, $t2.result); }
             )*
      ;

term returns [Node* result]
      : f1=factor { $result = $f1.result;	} 
             ('*' f2=factor { $result = new BinaryOperationNode(OP_MUL, $f1.result, $f2.result); }
             |'/' f2=factor { $result = new BinaryOperationNode(OP_DIV, $f1.result, $f2.result); }
             )*
      ;

factor returns [Node* result]
    : NUMBER {
    	double value = atol($NUMBER.text.c_str());
        $result = new NumberNode(value);    
      }

    | FLOATNUMBER {
    	// TODO: check number format!
    	double value = atof($FLOATNUMBER.text.c_str());
    	$result = new NumberNode(value);
      }

    | ID {
    	const SymbolTableEntry& entry = st->getSymbol($ID.text);
    	if (entry.getKind() == SYM_FUN) {
    		// TODO: Error handling
    	}

        $result = new IdNode(entry); 
      }

    | '(' expr ')' {
        $result = $expr.result;
      }

    | ID '(' expr ')' {
    	const SymbolTableEntry& entry = st->getSymbol($ID.text);
    	if (entry.getKind() != SYM_FUN) {
    		// TODO: Error handling
    	}

        $result = new FunctionNode(entry.getFun(), $expr.result);
      }

    ;


/* Tokens ********************************************************************/

NUMBER      : '0'..'9' + ;
FLOATNUMBER : NUMBER ('.' '0'..'9'+)?;
ID          :     'a'..'z' + ;
WS          :     ( ' ' |'\n' |'\r')+ { $channel=HIDDEN; } ; // ignore whitespace
