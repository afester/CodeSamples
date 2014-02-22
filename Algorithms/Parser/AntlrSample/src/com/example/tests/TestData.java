package com.example.tests;

public class TestData {
   public String formula;
   public final double expected;
   public final double x;
   public final Double t;

   public TestData(String formula, double expected, double x, Double t) {
      this.formula = formula;
      this.expected = expected;
      this.x = x;
      this.t = t;
   }

   public TestData(String formula, double expected, double x) {
      this(formula, expected, x, null);
   }

   public TestData(String formula, double expected) {
      this(formula, expected, 0.0);
   }
}