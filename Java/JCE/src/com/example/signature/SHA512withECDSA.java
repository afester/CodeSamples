package com.example.signature;

import java.nio.charset.StandardCharsets;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.SignatureException;

import com.example.hexdump.HexDump;


public class SHA512withECDSA {

    private void run() throws NoSuchAlgorithmException, SignatureException, InvalidKeyException {
        String text = "Sample String which we want to sign";

        // create a key pair
        KeyPairGenerator kpg = KeyPairGenerator.getInstance("EC");
        KeyPair keyPair = kpg.generateKeyPair();
        PrivateKey prv = keyPair.getPrivate();
        PublicKey pub = keyPair.getPublic();

        // print a hexdump of the keys
        HexDump.dumpHex("Public Key:", pub.getEncoded());
        HexDump.dumpHex("\nPrivate Key:", prv.getEncoded());

        Signature sig = Signature.getInstance("SHA512withECDSA");

        sig.initSign(prv);
        sig.update(text.getBytes(StandardCharsets.ISO_8859_1));
        byte[] signature = sig.sign();
        HexDump.dumpHex("\nSignature (SHA512 hash, encrypted with ECDSA):", signature);

        //String text2 = "Sample String which we want to verify";
        String text2 = "Sample String which we want to sign";

        sig.initVerify(pub);
        sig.update(text2.getBytes(StandardCharsets.ISO_8859_1));
        boolean verified = sig.verify(signature);
        System.err.println("\nVerified: " + verified);
    }

    
    public static void main(String[] args) throws NoSuchAlgorithmException, SignatureException, InvalidKeyException {
       SHA512withECDSA app = new SHA512withECDSA();
       app.run();
    }

}
