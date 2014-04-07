package com.example;

import org.apache.log4j.PropertyConfigurator;
import com.google.gson.Gson;

public class Sample {

   public static void main(String[] args) {
      PropertyConfigurator.configure("log4j.properties");
      Gson gson = new Gson();

      // ...
   }
}

