package com.example.test;

import static org.junit.Assert.*;

import org.junit.Test;

import com.example.agent.Signature;


public class SignatureTest {

   @Test
   public void test() {
      String method = "com.example.HelloWorld.printGreeting()";
      Signature s = Signature.getSignature(method);
      assertEquals("com.example.HelloWorld.printGreeting()", s.toString());

      method = "HelloWorld.printGreeting()";
      s = Signature.getSignature(method);
      assertEquals("HelloWorld.printGreeting()", s.toString());

      method = "HelloWorld.printGreeting(String, float, int)";
      s = Signature.getSignature(method);
      assertEquals("HelloWorld.printGreeting(String, float, int)", s.toString());
   }
}

