package com.example;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.Key;
import java.security.KeyFactory;
import java.security.PublicKey;
import java.security.spec.X509EncodedKeySpec;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

import com.example.hexdump.HexDump;

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

         System.err.println("Encrypted data (" + encryptedData.length + " bytes):");
         HexDump hd = new HexDump(encryptedData);
         hd.setPrefix("   ");
         hd.dumpAll(System.err);

         byte[] rsaKey = readFile("data/rsakey.prv");
         System.err.println("\nRSA key (" + rsaKey.length + " bytes):");
         hd = new HexDump(rsaKey);
         hd.setPrefix("   ");
         hd.dumpAll(System.err);

         // Create key and cipher
         
        // PublicKey publicKey = 
          //     KeyFactory.getInstance("RSA").generatePublic(new X509EncodedKeySpec(rsaKey));
//         Key key = new SecretKeySpec(rsaKey, "RSA");
         Key publicKey = new SecretKeySpec(rsaKey, "RSA");

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
