package com.example;

import java.lang.reflect.Field;

public abstract class AbstractExpression implements Expression {

   @Override
   public void setParameter(String name, double value) throws NoSuchFieldException {
      try {
         Field f = this.getClass().getField(name);
         f.setDouble(this, value);
      } catch (SecurityException se) {
         se.printStackTrace();
      } catch (IllegalArgumentException e) {
         e.printStackTrace();
      } catch (IllegalAccessException e) {
         e.printStackTrace();
      }
   }

}
