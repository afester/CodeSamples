// Note: when creating IDL files from type libraries using MIDL,
// there are some strings which are not enclosed in apostrophes - 
// in order to keep the grammar clean, it was decided to require the generated
// IDL file to be fixed instead of trying to brain-dead parse a broken IDL file!  

grammar Midl;

import MidlLexical;

midl : properties library ;

propparam : UUID
		  | NUMBER		
          | FLOAT
          | HEXNUMBER
		  | ID
          | STR
          | '<' ID ID '*' '>'
          ;

property : ID 
		 | ID '(' propparam (',' propparam)* ')'
		 ;

properties : '[' property (',' property)* ']';


library : 'library' ID '{' def+ '}' ';' ;

interfacedecl : 'interface' ID
			  | 'dispinterface' ID 
			  ;
def : 'importlib' '(' STR ')' ';'							      # IMPORT
    | interfacedecl ';'								              # INTERFACEDEC
    | properties? 'dispinterface' ID '{' dispBody '}' ';'	      # DISPINTERFACE
    | properties  'interface'     ID ':' ID '{' method* '}' ';'	  # INTERFACE
    | properties  'coclass'       ID '{' (properties interfacedecl ';')+ '}' ';'	# COCLASS
    | 'typedef' properties? type  ID ';'						  # TYPEDEF
    ;

dispBody : 'properties' ':' 'methods' ':' method+ 
		 ;

method : properties? type ID? ID '(' parameters? ')' ';'
	   ; 

parameters : parameter (',' parameter)*
		   ;

parameter : properties? type ID
		  ;

simpleType: ID
		  | ID '*'
		  | 'unsigned' ID
		  | 'unsigned' ID '*'
		  ;

// TODO: separate type for typedef and parameters
type : simpleType 
     | simpleType '*'
     | ID '(' simpleType ')'
     | ID '(' simpleType ')' '*'
     | 'enum' '{' enumElement (',' enumElement)* '}'
     ;

enumElement : ID '=' NUMBER
            | ID '=' HEXNUMBER
			;
