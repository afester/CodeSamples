package com.example;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.ErrorNode;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeListener;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.antlr.v4.runtime.tree.TerminalNode;

import com.example.expressionparser.ExpressionLexer;
import com.example.expressionparser.ExpressionParser;
import com.example.helloparser.HelloLexer;

public class AntlrSample {

   public static void main(String[] args) {
      new AntlrSample().run();
   }

   
   public void run() {
      System.err.println("Lexer Simple sample: ");
      helloSample();

      System.err.println("\nLexer Expression sample: ");
      expressionSample();

      System.err.println("\nParser Expression sample: ");
      expressionParser();
   }


   private void helloSample() {

      CharStream input = new ANTLRInputStream("hello parrt");
      Lexer helloLexer = new HelloLexer(input);

      while(true) {
         Token t = helloLexer.nextToken();
         if (t.getType() == Token.EOF) {
            break;
         }

         System.err.println("  Token: " + t);
      }
   }

   
   private void expressionSample() {

      // test the expression lexer - returns a stream of tokens
      CharStream input = new ANTLRInputStream("42 + 11 - 44 * 99 / 3 ^ 2 + sin(x)");
      Lexer expressionLexer = new ExpressionLexer(input);

      while(true) {
         Token t = expressionLexer.nextToken();
         if (t.getType() == Token.EOF) {
            break;
         }

         System.err.println("  Token: " + t);
      }
   }

   private void expressionParser() {
      //CharStream input = new ANTLRInputStream("42 + 11 - 44 * 99 / 3 ^ 2 + sin(x)");
      CharStream input = new ANTLRInputStream("11 + 22 * 33\n");
      Lexer expressionLexer = new ExpressionLexer(input);

      CommonTokenStream tokens = new CommonTokenStream(expressionLexer);
      ExpressionParser expressionParser = new ExpressionParser(tokens);
      ParseTree tree = expressionParser.expr(0); // .prog();
      //ExprContext ctx = expressionParser.expr(0);

      ParseTreeListener ptl = new ParseTreeListener() {

         @Override
         public void enterEveryRule(@NotNull ParserRuleContext arg0) {
            System.err.println("enterEveryRule");
         }

         @Override
         public void exitEveryRule(@NotNull ParserRuleContext arg0) {
            System.err.println("exitEveryRule");
         }

         @Override
         public void visitErrorNode(@NotNull ErrorNode arg0) {
            System.err.println("visitErrorNode");
         }

         @Override
         public void visitTerminal(@NotNull TerminalNode arg0) {
            System.err.println("visitTerminal: " + arg0);
         }
         
      };

      System.err.println("Depth first parse tree traversal: ");
      ParseTreeWalker ptw = new ParseTreeWalker();
      ptw.walk(ptl,  tree);

      System.err.println(tree.toStringTree(expressionParser));

      System.err.println("Executing expression visitor: ");
      ExpressionInterpreter visitor = new ExpressionInterpreter();
      Double result = visitor.visit(tree);
      System.err.println("Result: " + result);
   }
}
