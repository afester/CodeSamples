package com.example;

import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.security.auth.callback.Callback;
import javax.security.auth.callback.CallbackHandler;
import javax.security.auth.callback.NameCallback;
import javax.security.auth.callback.PasswordCallback;
import javax.security.auth.callback.UnsupportedCallbackException;

class TextCallbackHandler implements CallbackHandler {

   private BufferedReader input = null;
   private Console cons = null;

   public TextCallbackHandler() {
      // set either cons or input
      cons = System.console();
      if (cons == null) {
         input = new BufferedReader(new InputStreamReader(System.in));
      }
   }

   private char[] readPassword() {
      
      String pass = System.getProperty("com.example.UserPass");
      if (pass != null) {
         return pass.toCharArray();
      } else {
   
         if (cons != null) {
            return cons.readPassword();
         } else {
            try {
               return input.readLine().toCharArray();
            } catch (IOException e) {
               e.printStackTrace();
            }
         }
      }

      return new char[0];
   }

   private String readLine() {
      String user = System.getProperty("com.example.UserName");
      if (user != null) {
         return user;
      } else { 
         if (cons != null) {
            return cons.readLine();
         } else {
            try {
               return input.readLine();
            } catch (IOException e) {
               e.printStackTrace();
            }
         }
      }

      return "";
   }

   @Override
   public void handle(Callback[] arg0) throws IOException,
         UnsupportedCallbackException {
      
      for (Callback callback : arg0) {
         if (callback instanceof NameCallback) {
            NameCallback nc = (NameCallback) callback;
            System.out.print(nc.getPrompt());
            String name = readLine();
            if (name.length() > 0) {
               nc.setName(name);
            }
         } else if (callback instanceof PasswordCallback) {
            PasswordCallback pc = (PasswordCallback) callback;
            System.out.print(pc.getPrompt());
            char[] pwd = readPassword();
            pc.setPassword(pwd);
         } else {
            System.out.println(callback);
            throw new UnsupportedCallbackException(callback, "Handler");
         }
      }
   }
}