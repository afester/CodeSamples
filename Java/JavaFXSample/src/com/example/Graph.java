package com.example;

import javafx.scene.paint.Color;

class Graph {
   private int index;
   private String text;
   private String formula;
   private Color color;
   private boolean isError;

   public Graph(String formula, Color color) {
      this.text = formula;
      this.formula = formula;
      this.color = color;
      this.isError = false;
   }

   void setIndex(int idx) {
      this.index = idx;
   }

   int getIndex() {
      return this.index;
   }

   public String getFormula() {
      return formula;
   }

   public Color getColor() {
      return color;
   }

   public String toString() {
      return "Graph[" + this.index + ", \"" + this.text + "\", " + this.color
            + ", " + this.isError + "]";
   }

   public void setFormula(String newFormula) {
      this.text = newFormula;
      this.formula = newFormula;
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
