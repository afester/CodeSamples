package com.example.java8;

import java.util.function.Function;

interface ThreeParamsWithReturn<T, U, V, R> {
   R apply(T t, U u, V v);
}

public class LambdaExpression {
   public static void main(String[] args) {
      System.err.println("\nLambda expression sample\n=====================================");
      
      ThreeParamsWithReturn<Integer, Integer, Integer, Integer> fun2 = (a, b, c) -> a + b + c;
      System.err.println(fun2.apply(1, 2, 3));
     
      ThreeParamsWithReturn<Float, Float, Float, Float> func = (p1, p2, p3) -> { Float sum = p1 + p2 + p3;  return sum / 3.0F; };
      System.err.println(func.apply(1.0F, 5.0F, 7.0F));    // 4.3333
      

      Function<Double, Double> sinsquare = x -> Math.sin(Math.pow(x, 2.0));
      System.err.println(sinsquare.apply(3.4));            //  -0.844895
   }
}
