package com.example.tests;

import static org.junit.Assert.*;
import org.junit.Test;

import com.example.Expression;
import com.example.FunctionDefinition;

public class InterpreterTest {

   private final static double EPSILON = 0.0000001;

   private Double evaluate(String formula) {
      FunctionDefinition fn = new FunctionDefinition(formula);
      Expression e = fn.interpreted();
      return e.evaluate(0.0);
   }

   
   @Test
   public void floatLiteralTest() {
      Double result = evaluate("4.2");
      assertEquals(4.2, result, EPSILON);
   }

   
   @Test
   public void basicTests() {
      Double result = evaluate("4 + 5");
      assertEquals(9, result, EPSILON);

      result = evaluate("4 * 5");
      assertEquals(20, result, EPSILON);

      result = evaluate("20 / 4");
      assertEquals(5, result, EPSILON);

      result = evaluate("20 - 4");
      assertEquals(16, result, EPSILON);

      result = evaluate("4^2");
      assertEquals(16, result, EPSILON);
      
      result = evaluate("4^3^2");
      assertEquals(262144, result, EPSILON);
   }


   @Test
   public void precedenceTests() {
      Double result = evaluate("4 + 5 * 2");
      assertEquals(14, result, EPSILON);

      result = evaluate("4 * 5 + 2");
      assertEquals(22, result, EPSILON);

      result = evaluate("4 * 5 / 2");
      assertEquals(10, result, EPSILON);

      result = evaluate("4 / 2 - 2");
      assertEquals(0, result, EPSILON);

      result = evaluate("4 - 2 / 2");
      assertEquals(3, result, EPSILON);
   }

   
   @Test
   public void paranthesesTests() {
      Double result = evaluate("(4 + 5) * 2");
      assertEquals(18, result, EPSILON);

      result = evaluate("4 * (5 + 2)");
      assertEquals(28, result, EPSILON);

      result = evaluate("4 * (5 / 2)");
      assertEquals(10, result, EPSILON);

      result = evaluate("8 / (4 - 2)");
      assertEquals(4, result, EPSILON);

      result = evaluate("(4 - 2) / 2");
      assertEquals(1, result, EPSILON);
   }

   
   @Test
   public void functionTests() {
      Double result = evaluate("sin(1)");
      assertEquals(0.8414709848078965, result, EPSILON);

      result = evaluate("3 * cos(PI)");
      assertEquals(-3, result, EPSILON);
      
   }
   
   @Test
   public void constantTests() {
      Double result = evaluate("PI");
      assertEquals(3.141592653589793, result, EPSILON);
      
      result = evaluate("e");
      assertEquals(2.718281828459045, result, EPSILON);
   }


   @Test
   public void combinedTests() throws NoSuchFieldException {
      FunctionDefinition fn = new FunctionDefinition("2*sin(x+2*t)");
      Expression e = fn.interpreted();
      e.setParameter("t", 1.0);
      Double result = e.evaluate(2.0);
      assertEquals(-1.51360499, result, EPSILON);

      fn = new FunctionDefinition("log(x)+t");
      e = fn.interpreted();
      e.setParameter("t", 1.0);
      result = e.evaluate(0.0);
      assertEquals(Double.NEGATIVE_INFINITY, result, EPSILON);

      fn = new FunctionDefinition("e^(x+t)");
      e = fn.interpreted();
      e.setParameter("t", 0.0);
      result = e.evaluate(0.0);
      assertEquals(1, result, EPSILON);

      fn = new FunctionDefinition("x + 3*(x-t)^2");
      e = fn.interpreted();
      e.setParameter("t", 1.0);
      result = e.evaluate(2.0);
      assertEquals(5, result, EPSILON);
   }


   @Test
   public void seriesTests() {
      double[] expectedValues = 
         {0.0,      0.099833, 0.198669, 0.295520, 0.389418, 0.479426, 0.564642,
          0.644218, 0.717356, 0.783327, 0.841471, 0.891207, 0.932039, 0.963558,
          0.985450, 0.997495, 0.999574, 0.991665, 0.973848, 0.946300};
      
      FunctionDefinition fn = new FunctionDefinition("sin(x)");
      Expression e = fn.interpreted();

      for (int run = 0;  run < 1000;  run++) {
         int i = 0;
         for (double x = 0; x < 2;  x += 0.1, i++) {
            Double result = e.evaluate(x);
            assertEquals(expectedValues[i], result, 0.000001);
         }
      }
   }

}
