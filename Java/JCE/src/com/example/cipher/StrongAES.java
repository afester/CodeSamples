package com.example.cipher;

import java.nio.charset.StandardCharsets;
import java.security.Key;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

import com.example.hexdump.HexDump;

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
         //String key = "Bar12345Bar12345Bar12345";         // 192 bit key
         //String key = "Bar12345Bar12345Bar12345Bar12345"; // 256 bit key

         // Create key and cipher
         Key aesKey = new SecretKeySpec(key.getBytes(StandardCharsets.ISO_8859_1), "AES");

         byte[] encodedKey = aesKey.getEncoded();
         HexDump.dumpHex("Encoded key:", encodedKey);
         
         Cipher cipher = Cipher.getInstance("AES");
         cipher.init(Cipher.ENCRYPT_MODE, aesKey);

         // encrypt the text
         // cryptographic algorithms work on binary data (byte arrays).
         // To encrypt a text string, we first need to convert the string into 
         // a byte array, using an appropriate encoding
         byte[] inputData = text.getBytes(StandardCharsets.US_ASCII);
         HexDump.dumpHex("\nInput message:", inputData);

         byte[] encrypted = cipher.doFinal(inputData);
         HexDump.dumpHex("\nEncrypted message:", encrypted);

         // decrypt the text
         cipher.init(Cipher.DECRYPT_MODE, aesKey);
         byte[] decryptedData = cipher.doFinal(encrypted);
         HexDump.dumpHex("\nDecrypted message:", decryptedData);

         String decrypted = new String(decryptedData, StandardCharsets.US_ASCII);
         System.err.println("   " + decrypted);
      }catch(Exception e) {
         e.printStackTrace();
      }
    }

    public static void main(String[] args) {
       StrongAES app = new StrongAES();
       app.run();
    }
}
