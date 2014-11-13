package com.example.graph;

import com.example.Expression;
import com.example.FunctionDefinition;

public class FunctionGraph implements Graph {

   private FunctionDefinition fn;
   private Expression expr;

   private int index;
   private boolean isError;

   public FunctionGraph(String formula) {
      fn = new FunctionDefinition(formula);
      if (!formula.isEmpty()) {
          expr = fn.compiled();
      }
      this.isError = false;
   }


   void setT(double t) {
      try {
         expr.setParameter("t",  t);
      } catch (NoSuchFieldException e) {
         // Ignore exception - expression just might not have a parameter
         //e.printStackTrace();
      }
   }


   public Double evaluate(double value) {
      return expr.evaluate(value);
   }

   @Override
   public Point2DOperationIterator operationIterator(double from, double to, double delta) {
       return new FunctionGraphIterator(this, from, to, delta);
   }
   
   void setIndex(int idx) {
      this.index = idx;
   }

   int getIndex() {
      return this.index;
   }

   public String getFormula() {
      return fn.getExpression();
   }

   public String toString() {
      return "Graph[" + this.index + ", \"" + fn.getExpression() + "\", " + this.isError + "]";
   }

   public void setFormula(String newFormula) {
      fn = new FunctionDefinition(newFormula);
      expr = fn.compiled();
   }

   public void setIsError(boolean b) {
      this.isError = b;
   }

   public boolean isError() {
      return this.isError;
   }

}
