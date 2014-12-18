/** 
 * From The definitive antlr 4 reference, chapter 3.1
 */

grammar ArrayInit;

/** A rule called init that matches comma-separated values between {...}. */
init : '{' value (',' value)* '}' ;         // must match at least one value

/** A value can be either a nested array/struct or a simple integer (INT) */
value : init
      | INT
      ;

// parser rules start with lowercase letters, lexer rules with uppercase
INT : [0-9]+ ;              // Define token INT as one or more digits
WS  : [ \t\r\n]+ -> skip ;  // Define whitespace rule, toss it out
