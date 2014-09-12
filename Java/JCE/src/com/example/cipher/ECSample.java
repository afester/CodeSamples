package com.example.cipher;

import java.nio.charset.StandardCharsets;
import java.security.Key;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.Security;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

import org.bouncycastle.jce.provider.BouncyCastleProvider;

import com.example.ASN1Dump;
import com.example.hexdump.HexDump;

public class ECSample {

    public void run() {
      try {
         String text = "Sample String which we want to encrypt";
         String key = "Bar12345Bar12345";                 // 128 bit key

         // Get the cipher (a cipher is a cryptographic algorithm)
         // ECIES is not supported out-of-the-box, so we need a provider which supports it
         Security.addProvider(new BouncyCastleProvider());
         Cipher cipher = Cipher.getInstance("ECIES");

         
         // Create key pair - in a real application, we need to store the
         // key somewhere so that they can later be used to decrypt the message!
         KeyPairGenerator kpg = KeyPairGenerator.getInstance("ECIES"); // , "BC");
         KeyPair keyPair = kpg.generateKeyPair();
         Key prv = keyPair.getPrivate();
         Key pub = keyPair.getPublic();

         // print a hexdump of the keys
         HexDump.dumpHex("Public Key:", pub.getEncoded());
         HexDump.dumpHex("\nPrivate Key:", prv.getEncoded());
         
         // print ASN.1 dump f the keys
         ASN1Dump.dump("\nPublic Key:", pub.getEncoded());
         ASN1Dump.dump("\nPrivate Key:", prv.getEncoded());

/** Encryption and decryption - same code as in RSASample! ********/
         // cryptographic algorithms work on binary data (byte arrays).
         // To encrypt a text string, we first need to convert the string into 
         // a byte array, using an appropriate encoding
         byte[] plaintext = text.getBytes(StandardCharsets.ISO_8859_1);
         HexDump.dumpHex("\nPlaintext:", plaintext);

         // encrypt the text - we use the public key for encryption
         cipher.init(Cipher.ENCRYPT_MODE, pub);
         byte[] ciphertext = cipher.doFinal(plaintext);

         // print a hexdump of the ciphertext
         HexDump.dumpHex("\nCiphertext:", ciphertext);

         // decrypt the text - we use the private key for decryption
         cipher.init(Cipher.DECRYPT_MODE, prv);
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
       ECSample app = new ECSample();
       app.run();
    }
}
