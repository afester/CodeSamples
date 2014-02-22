package com.example;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;

import com.example.expressionparser.ExpressionLexer;
import com.example.expressionparser.ExpressionParser;

/**
 * A simple function definition which can define parameterized functions of the 
 * form
 * 
 * f(x) := f(x, t1, t2, ..., tn)
 * 
 */
public class FunctionDefinition {

   private String expression;


   /**
    * Creates a new function definition.
    * 
    * @param expression The expression of the function. The expression can 
    *                   contain one free variable (x) and any number of additional
    *                   parameters with arbitrary names.
    *                   Example: "sin(x) + offset"
    */
   public FunctionDefinition(String expression) {
      this.expression = expression;
   }
   

   /**
    * @return The expression as it has been passed to the constructor of this 
    *         class.
    */
   public String getExpression() {
      return expression;
   }


   /**
    * Compiles this function definition and returns an Expression which can be
    * used to evaluate the function.
    *
    * @return An Expression object to evaluate the function.
    */
   public Expression compiled() {
      // create all necessary objects
      CharStream input = new ANTLRInputStream(expression);
      ExpressionLexer expressionLexer = new ExpressionLexer(input);
      CommonTokenStream tokens = new CommonTokenStream(expressionLexer);
      ExpressionParser expressionParser = new ExpressionParser(tokens);

      // parse the expression and create the tree visitor  
      ParseTree tree = expressionParser.expr(0);

      ExpressionCompiler compiler = new ExpressionCompiler();
      return compiler.compile(tree);
   }


   /**
    * Interprets this function definition and returns an Expression which can be
    * used to evaluate the function.
    *
    * @return An Expression object to evaluate the function.
    */
   public Expression interpreted() {
      // create all necessary objects
      CharStream input = new ANTLRInputStream(expression);
      ExpressionLexer expressionLexer = new ExpressionLexer(input);
      CommonTokenStream tokens = new CommonTokenStream(expressionLexer);
      ExpressionParser expressionParser = new ExpressionParser(tokens);

      // parse the expression and create the tree visitor  
      final ParseTree tree = expressionParser.expr(0);

      return new Expression() {
         
         private ExpressionInterpreter visitor = new ExpressionInterpreter();

         @Override
         public double evaluate(double x) {
            visitor.setVariable("x", x);
            return visitor.visit(tree);
         }

         @Override
         public void setParameter(String name, double value)
               throws NoSuchFieldException {
            visitor.setVariable(name, value);
         }
         
      };
   }
}
