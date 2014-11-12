package com.example;

import com.example.graph.Graph;
import com.example.graph.Point2DOperationIterator;

import javafx.scene.paint.Color;

public class FunctionGraph implements Graph {

   private FunctionDefinition fn;
   private Expression expr;

   private int index;
   private Color color;
   private boolean isError;

   public FunctionGraph(String formula, Color color) {
      fn = new FunctionDefinition(formula);
      expr = fn.compiled();

      //this.formula = formula;
      this.color = color;
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

   public Color getColor() {
      return color;
   }

   public String toString() {
      return "Graph[" + this.index + ", \"" + fn.getExpression() + "\", " + this.color
            + ", " + this.isError + "]";
   }

   public void setFormula(String newFormula) {
      fn = new FunctionDefinition(newFormula);
      expr = fn.compiled();
   }

   public void setColor(Color newColor) {
      this.color = newColor;
   }

   public void setIsError(boolean b) {
      this.isError = b;
   }

   public boolean isError() {
      return this.isError;
   }

}
