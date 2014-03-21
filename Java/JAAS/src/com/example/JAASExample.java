package com.example;

import javax.security.auth.login.Configuration;

public class JAASExample {
   public void run() {
      Configuration c = Configuration.getConfiguration();
   }
   

   public static void main(String[] args) {
      new JAASExample().run();
   }
}
