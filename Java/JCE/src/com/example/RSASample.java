package com.example;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.KeyFactory;
import java.security.PublicKey;
import java.security.spec.X509EncodedKeySpec;

import javax.crypto.Cipher;

public class RSASample {

   
    private byte[] readFile(String fileName) {
       byte[] result = null;
       try {
          File srcFile = new File(fileName);
          result = new byte[(int) srcFile.length()];
          FileInputStream fis = new FileInputStream(srcFile);
          fis.read(result);
          fis.close();
      } catch (FileNotFoundException e) {
         e.printStackTrace();
      } catch (IOException e) {
         e.printStackTrace();
      }
       
       return result;
    }


    public void run() {
      try {
         byte[] encryptedData = readFile("data/encrypted.rsa");
         byte[] rsaKey = readFile("data/rsakey.prv");

         System.out.println(encryptedData.length);
         System.out.println(rsaKey.length);

         // Create key and cipher
         
         PublicKey publicKey = 
               KeyFactory.getInstance("RSA").generatePublic(new X509EncodedKeySpec(rsaKey));
//         Key key = new SecretKeySpec(rsaKey, "RSA");
         Cipher cipher = Cipher.getInstance("RSA");
/*
         // encrypt the text
         cipher.init(Cipher.ENCRYPT_MODE, aesKey);
         byte[] encrypted = cipher.doFinal(text.getBytes());
         System.err.println(new String(encrypted));
*/
         // decrypt the text
         cipher.init(Cipher.DECRYPT_MODE, publicKey);
         String decrypted = new String(cipher.doFinal(encryptedData));
         System.err.println(decrypted);
      }catch(Exception e) {
         e.printStackTrace();
      }
    }

    public static void main(String[] args) {
       RSASample app = new RSASample();
       app.run();
    }
}
