package com.example;

import com.example.expressionparser.ExpressionBaseVisitor;
import com.example.expressionparser.ExpressionParser;
import com.example.expressionparser.ExpressionParser.AddSubContext;
import com.example.expressionparser.ExpressionParser.FunctionContext;
import com.example.expressionparser.ExpressionParser.IdContext;
import com.example.expressionparser.ExpressionParser.MulDivContext;
import com.example.expressionparser.ExpressionParser.NumberContext;
import com.example.expressionparser.ExpressionParser.ParContext;
import com.example.expressionparser.ExpressionParser.PowerContext;
import com.example.symboltable.NoFunctionException;
import com.example.symboltable.Symbol;
import com.example.symboltable.UndefinedSymbolException;
import com.example.symboltable.Variable;

import com.example.symboltable.java8.Function;
import com.example.symboltable.java8.SymbolTable;


public class ExpressionInterpreter extends ExpressionBaseVisitor<Double> {

   private SymbolTable symbolTable = new SymbolTable();

   public ExpressionInterpreter() {
      symbolTable.putSymbol("PI", Math.PI);
      symbolTable.putSymbol("e",  Math.E);

      symbolTable.putSymbol("sin",  x -> Math.sin(x));
      symbolTable.putSymbol("cos",  x -> Math.cos(x));
      symbolTable.putSymbol("tan",  x -> Math.tan(x));
      symbolTable.putSymbol("log",  x -> Math.log(x));
      symbolTable.putSymbol("sqrt", x -> Math.sqrt(x));

      // symbolTable.dump();
   }


   @Override
   public Double visitFunction(FunctionContext ctx)  {

      // get the function and check if it is valid
      String functionName = ctx.ID().getText();

      Symbol fn = symbolTable.getSymbol(functionName);
      if (fn == null) {
         throw new UndefinedSymbolException(functionName);
      }
      if ( !(fn instanceof Function) ) {
         throw new NoFunctionException(functionName);
      }

      // evaluate the function argument
      Double arg = visit(ctx.expr());

      return fn.getValue(arg);
   }


   @Override
   public Double visitPar(ParContext ctx) {
      return visit(ctx.expr());
   }


   @Override
   public Double visitPower(PowerContext ctx) {
      Double left = visit(ctx.expr(0));    // left tree
      Double right = visit(ctx.expr(1));   // right tree

      // System.err.println("visitPower: " + left + "^" + right);

      return Math.pow(left,  right);
   }

   
   @Override
   public Double visitMulDiv(MulDivContext ctx) {
      Double left = visit(ctx.expr(0));    // left tree
      Double right = visit(ctx.expr(1));   // right tree

      // System.err.println("visitMulDiv: " + left + " \"" + ctx.op.getText() + "(" + + ctx.op.getType() + ")"  + "\" " + right);

      if (ctx.op.getType() == ExpressionParser.MUL) {
         return left * right;
      } 

      return left / right;
   }

   @Override
   public Double visitAddSub(AddSubContext ctx) {
      Double left = visit(ctx.expr(0));    // left tree
      Double right = visit(ctx.expr(1));   // right tree

      // System.err.println("visitAddSub: " + left + " \"" + ctx.op.getText() + "(" + + ctx.op.getType() + ")"  + "\" " + right);

      if (ctx.op.getType() == ExpressionParser.ADD) {
         return left + right;
      }

      return left - right;
   }

   @Override
   public Double visitId(IdContext ctx) {
      // get the ID and check if it is valid
      String idName = ctx.ID().getText();

      Symbol id = symbolTable.getSymbol(idName);
      if (id == null) {
         throw new UndefinedSymbolException(idName);
      }
      if ( id instanceof Function) {
         throw new RuntimeException("Not a variable or constant: " + idName);
      }

      return id.getValue();
   }

   @Override
   public Double visitNumber(NumberContext ctx) {
      // convert token into Double
      double result = Double.parseDouble(ctx.getText());

      // System.err.println("visitNumber: " + result);

      return result;
   }
/*
   @Override
   public Double visitInt(IntContext ctx) {
      // convert token into Integer
      double result = Double.parseDouble(ctx.getText());

      // System.err.println("visitInt: " + result);
      
      return result;
   }
 */ 
   
   public void setVariable(String name, Double value) {
      Variable xVar = (Variable) symbolTable.getSymbol(name);
      if (xVar == null) {
         xVar = new Variable(name);
         symbolTable.putSymbol(xVar);
      }
      xVar.setValue(value);
   }
}
