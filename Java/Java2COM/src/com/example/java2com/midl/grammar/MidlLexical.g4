lexer grammar MidlLexical;

STR        : '"' .*? '"'    	 ;
ID         : [a-zA-Z_][a-zA-Z0-9_]* ;
NUMBER     : '-'?[0-9]+	      		 ;
FLOAT      : NUMBER+ '.' NUMBER+ ;
HEXNUMBER  : '0x' HEXDIGIT+ 	 ;

HEXDIGIT : [a-fA-F0-9];
HEXNUM4  : HEXDIGIT HEXDIGIT HEXDIGIT HEXDIGIT;  
HEXNUM8  : HEXNUM4 HEXNUM4;
HEXNUM12 : HEXNUM4 HEXNUM4 HEXNUM4;

UUID     : HEXNUM8'-'HEXNUM4'-'HEXNUM4'-'HEXNUM4'-'HEXNUM12;

LINE_COMMENT : '//' .*? '\r'? '\n' -> skip ;
COMMENT 	 : '/*' .*? '*/' 	   -> skip ;
WS 			 : [ \t\r\n]+          -> skip ;
