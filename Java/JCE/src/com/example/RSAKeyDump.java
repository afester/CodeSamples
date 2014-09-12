package com.example;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.Key;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.spec.PKCS8EncodedKeySpec;

import javax.crypto.Cipher;

import com.example.hexdump.HexDump;

public class RSAKeyDump {

   
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
         PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(rsaKey);
         PublicKey publicKey = (PublicKey) KeyFactory.getInstance("RSA").generatePrivate(spec);
      //   PublicKey publicKey = 
               //KeyFactory.getInstance("RSA").generatePublic(new X509EncodedKeySpec(rsaKey));
//         Key publicKey = new SecretKeySpec(rsaKey, "RSA");

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

   
    public void generateKey() {
        try {
            // Create a single key - does not make sense for key pair algorithms!!! (DSA, RSA)
            //KeyGenerator kg = KeyGenerator.getInstance("DES");
            //byte[] key = kg.generateKey().getEncoded();
            
            KeyPairGenerator kpg = KeyPairGenerator.getInstance("RSA");
            KeyPair kp = kpg.generateKeyPair();

            Key prv = kp.getPrivate();
            Key pub = kp.getPublic();

//            byte[] prvData = prv.getEncoded();
//            byte[] pubData = pub.getEncoded();
//            System.err.println("Private key:"+ prv.getFormat());
//            HexDump hd = new HexDump(prv.getEncoded());
//            hd.setPrefix("   ");
//            hd.dumpAll(System.err);

            System.err.println("Public key:" + pub.getFormat());
            HexDump hd = new HexDump(pub.getEncoded());
            hd.setPrefix("   ");
            hd.dumpAll(System.err);

//            kp = kpg.generateKeyPair();
//            pub = kp.getPrivate();

            System.err.println("Private key:" + prv.getFormat());
            hd = new HexDump(prv.getEncoded());
            hd.setPrefix("   ");
            hd.dumpAll(System.err);
            
            
            try {
                byte[] data = prv.getEncoded();
                FileOutputStream fos = new FileOutputStream("private.pkcs8");
                fos.write(data);
                fos.close();
                
                data = pub.getEncoded();
                fos = new FileOutputStream("public.x509");
                fos.write(data);
                fos.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            
            ASN1Dump.dump(prv.getEncoded());
            ASN1Dump.dump(pub.getEncoded());

        } catch (NoSuchAlgorithmException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        
    }

    public static void main(String[] args) {
       RSAKeyDump app = new RSAKeyDump();
       app.generateKey();
       // app.run();
    }
}
