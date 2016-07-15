package com.example.cipher;

import java.nio.charset.StandardCharsets;
import java.security.Key;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

import com.example.hexdump.HexDump;

public class AESSample {

    public void run() {
      try {
         String text = "Sample String which we want to encrypt";
         String key = "Bar12345Bar12345";                 // 128 bit key

         // Get the cipher (a cipher is a cryptographic algorithm)
         Cipher cipher = Cipher.getInstance("AES");
         // Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding"); // same as above
         // Cipher cipher = Cipher.getInstance("AES/ECB/NoPadding");    // NoPadding requires the input to be a multiple of 16 bytes

         // Create Key 
         Key aesKey = new SecretKeySpec(key.getBytes(StandardCharsets.ISO_8859_1), "AES");

         // print a hexdump of the key
         HexDump.dumpHex("Key:", aesKey.getEncoded());

/** Encryption and decryption - essentially same code as in AESSample! ********/
         // cryptographic algorithms work on binary data (byte arrays).
         // To encrypt a text string, we first need to convert the string into 
         // a byte array, using an appropriate encoding
         byte[] plaintext = text.getBytes(StandardCharsets.ISO_8859_1);
         HexDump.dumpHex("\nPlaintext:", plaintext);

         // encrypt the text
         cipher.init(Cipher.ENCRYPT_MODE, aesKey);
         byte[] ciphertext = cipher.doFinal(plaintext);

         // print a hexdump of the ciphertext
         HexDump.dumpHex("\nCiphertext:", ciphertext);

         // decrypt the text
         cipher.init(Cipher.DECRYPT_MODE, aesKey);
         byte[] decrypted = cipher.doFinal(ciphertext);

         // print a hexdump of the decrypted data
         HexDump.dumpHex("\nDecrypted text:", decrypted);

         String decryptedText = new String(decrypted, StandardCharsets.US_ASCII);
         System.err.println("   " + decryptedText);
/******************************************************************************/
      }catch(Exception e) {
         e.printStackTrace();
      }
    }

    public static void main(String[] args) {
       AESSample app = new AESSample();
       app.run();
    }
}
