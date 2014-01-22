package com.example;


/**
 */
public interface Expression {
   
   /**
    * Evaluates the expression.
    *
    * @param x The x value to use for the evaluation.
    * @return The value of the function.
    */
   double evaluate(double x);


   /**
    * Sets a parameter of the expresion.
    * Given the function "sin(x) + offset", the parameter "offset" can be
    * set to 1.0 by calling <code>setParameter("offset", 1.0);</code>.
    * 
    * @param name   The name of the parameter
    * @param value  The value to set for the parameter
    */
   void setParameter(String name, double value) throws NoSuchFieldException;
}
