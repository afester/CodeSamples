package com.example;

public class SampleFun extends AbstractExpression {
   public double t = 1.0;
   
   public double evaluate(double x) {
      return Math.log(x) + t;
   }
}
