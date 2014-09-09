package com.example;

import java.security.Key;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;

public class RSAKeys {
    public static void main(String[] args) throws NoSuchAlgorithmException {
        KeyPairGenerator kpg = KeyPairGenerator.getInstance("RSA");
        KeyPair kp = kpg.generateKeyPair();
    
        Key prv = kp.getPrivate();
        Key pub = kp.getPublic();
    
        byte[] prvData = prv.getEncoded();
        byte[] pubData = pub.getEncoded();
    
    }
}
