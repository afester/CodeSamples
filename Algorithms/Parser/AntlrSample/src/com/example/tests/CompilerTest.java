package com.example.tests;

import static org.junit.Assert.*;

import org.junit.Test;

import com.example.Expression;
import com.example.FunctionDefinition;


public class CompilerTest {

   private final static double EPSILON = 0.0000001;
   
   @Test
   public void basicTests() {
      TestData[] testData = {
            new TestData("4 + 5",  9.0),
            new TestData("4 * 5",  20.0),
            new TestData("20 / 4", 5.0),
            new TestData("20 - 4", 16.0),
            new TestData("4^2",    16.0),
            new TestData("4^3^2",  262144.0)
      };

      for (TestData td : testData) {
         FunctionDefinition fn = new FunctionDefinition(td.formula);
         Expression e = fn.compiled();
         double result = e.evaluate(0);
         assertEquals(td.expected, result, EPSILON);
      }
   }


   @Test
   public void precedenceTests() {
      TestData[] testData = {
            new TestData("4 + 5 * 2", 14.0),
            new TestData("4 * 5 + 2", 22.0),
            new TestData("4 * 5 / 2", 10.0),
            new TestData("4 / 2 - 2", 0.0),
            new TestData("4 - 2 / 2", 3.0)
      };

      for (TestData td : testData) {
         FunctionDefinition fn = new FunctionDefinition(td.formula);
         Expression e = fn.compiled();
         double result = e.evaluate(0);
         assertEquals(td.expected, result, EPSILON);
      }
   }


   @Test
   public void paranthesesTests() {
      TestData[] testData = {
            new TestData("(4 + 5) * 2", 18.0),
            new TestData("4 * (5 + 2)", 28.0),
            new TestData("4 * (5 / 2)", 10.0),
            new TestData("8 / (4 - 2)", 4.0),
            new TestData("(4 - 2) / 2", 1.0)
      };

      for (TestData td : testData) {
         FunctionDefinition fn = new FunctionDefinition(td.formula);
         Expression e = fn.compiled();
         double result = e.evaluate(0);
         assertEquals(td.expected, result, EPSILON);
      }
   }

   
   @Test
   public void constantTests() {
      TestData[] testData = {
            new TestData("PI", 3.141592653589793),
            new TestData("e", 2.718281828459045)
      };

      for (TestData td : testData) {
         FunctionDefinition fn = new FunctionDefinition(td.formula);
         Expression e = fn.compiled();
         double result = e.evaluate(0);
         assertEquals(td.expected, result, EPSILON);
      }
   }


   @Test
   public void functionTests() {
      TestData[] testData = {
            new TestData("sin(1)", 0.8414709848078965),
            new TestData("3 * cos(PI)", -3.0)
      };

      for (TestData td : testData) {
         FunctionDefinition fn = new FunctionDefinition(td.formula);
         Expression e = fn.compiled();
         double result = e.evaluate(0);
         assertEquals(td.expected, result, EPSILON);
      }
   }


   @Test
   public void combinedTests() {
      TestData[] testData = {
            new TestData("2*sin(x+2*t)", -1.51360499,              2.0, 1.0),
            new TestData("3 * cos(PI)",  -3.0),
            new TestData("log(x) + t",   Double.NEGATIVE_INFINITY, 0.0, 1.0),
            new TestData("e^(x+t)",      1.0,                      0.0, 0.0),
            new TestData("x + 3*(x-t)^2",5.0,                      2.0, 1.0)
      };

      for (TestData td : testData) {
         FunctionDefinition fn = new FunctionDefinition(td.formula);
         Expression e = fn.compiled();

         if (td.t != null) {
            try {
               e.setParameter("t",  td.t);
            } catch (NoSuchFieldException e1) {
               e1.printStackTrace();
            }
         }

         double result = e.evaluate(td.x);
         assertEquals(td.formula + ": ", td.expected, result, EPSILON);
      }
   }

   
   @Test
   public void seriesTests() {
      double[] expectedValues = 
         {0.0,      0.099833, 0.198669, 0.295520, 0.389418, 0.479426, 0.564642,
          0.644218, 0.717356, 0.783327, 0.841471, 0.891207, 0.932039, 0.963558,
          0.985450, 0.997495, 0.999574, 0.991665, 0.973848, 0.946300};
      
      FunctionDefinition fn = new FunctionDefinition("sin(x)");
      Expression e = fn.compiled();

      for (int run = 0;  run < 1000;  run++) {
         int i = 0;
         for (double x = 0; x < 2;  x += 0.1, i++) {
            Double result = e.evaluate(x);
            assertEquals(expectedValues[i], result, 0.000001);
         }
      }
   }

}
