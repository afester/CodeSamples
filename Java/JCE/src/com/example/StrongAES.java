package com.example;

import java.nio.charset.StandardCharsets;
import java.security.Key;
import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

/**
 * Sample application which uses strong encryption (256 bit AES)
 * This requires the Java Cryptography Extension (JCE) Unlimited Strength 
 * Jurisdiction Policy Files to be installed.
 * 
 * See also http://www.software-architect.net/articles/using-strong-encryption-in-java/introduction.html 
 */
public class StrongAES {

    public void run() {
      try {
         String text = "Long string with more than 16 characters";
         String key = "Bar12345Bar12345";                 // 128 bit key
         // String key = "Bar12345Bar12345Bar12345Bar12345"; // 256 bit key

         // Create key and cipher
         Key aesKey = new SecretKeySpec(key.getBytes(StandardCharsets.ISO_8859_1), "AES");
         Cipher cipher = Cipher.getInstance("AES");
         cipher.init(Cipher.ENCRYPT_MODE, aesKey);

         // encrypt the text
         byte[] encrypted = cipher.doFinal(text.getBytes(StandardCharsets.UTF_16));

         System.err.println(new String(encrypted, StandardCharsets.US_ASCII));

         // decrypt the text
         cipher.init(Cipher.DECRYPT_MODE, aesKey);
         String decrypted = new String(cipher.doFinal(encrypted), StandardCharsets.UTF_16);
         System.err.println(decrypted);
      }catch(Exception e) {
         e.printStackTrace();
      }
    }

    public static void main(String[] args) {
       StrongAES app = new StrongAES();
       app.run();
    }
}
