package com.example;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.security.Key;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Iterator;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.spec.SecretKeySpec;

import codec.asn1.ASN1;
import codec.asn1.ASN1Exception;
import codec.asn1.ASN1Type;
import codec.asn1.DERDecoder;
import codec.asn1.ASN1Sequence;

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

    
    private String prefix = "";
    
    private void dumpASN1Sequence(ASN1Sequence seq) {
        prefix = prefix + "   ";
        Iterator i = seq.iterator();
        while(i.hasNext()) {
            ASN1Type type = (ASN1Type) i.next();
            dumpASN1Type(type);
        }
        prefix = prefix.substring(3);
    }

    private void dumpASN1Octet(byte[] octett) {
        prefix = prefix + "   ";
        dumpASN1(octett);
        prefix = prefix.substring(3);
    }

    private byte[] bitsToBytes(boolean[] val) {
        int idx = 0;
        byte[] b = new byte[val.length / 8];
        for (int bi = 0;  bi < b.length;  bi++) {
            int mask = 0x80;
            int bVal = 0;
            for (int bi2 = 0;  bi2 < 8;  bi2++) {
                if (val[idx]) {
                    bVal |= mask;
                }
                mask = mask >> 1;
                idx++;
            }
            b[bi] = (byte) bVal;
        }
        return b;
    }

    private void dumpASN1Type(ASN1Type type) {
        
        switch(type.getTag()) {
            case ASN1.TAG_NULL : {
                    System.err.println(prefix + "NULL (" + type.getTag() + ")");
                }
                break;

            case ASN1.TAG_SEQUENCE : {
                    System.err.println(prefix + "SEQUENCE (" + type.getTag() + ") " +  type.getValue().getClass() + " {");
                    ASN1Sequence val = (ASN1Sequence) type.getValue();
                    
                    dumpASN1Sequence(val);        
                    System.err.println(prefix + "}");
                }
                break;

            case ASN1.TAG_OCTETSTRING : {
                    System.err.println(prefix + "OCTETSTRING (" + type.getTag() + ") " +  type.getValue().getClass() + " {");
                    byte[] val = (byte[]) type.getValue();
                    
                    dumpASN1Octet(val);
                    System.err.println(prefix + "}");
                }
                break;

            case ASN1.TAG_BITSTRING : {
                    System.err.println(prefix + "BITSTRING (" + type.getTag() + ") " +  type.getValue().getClass() + " {");
                    boolean[] val = (boolean[]) type.getValue();

                    byte[] data = bitsToBytes(val);
                    dumpASN1Octet(data);
                    System.err.println(prefix + "}");
                }
                break;

            case ASN1.TAG_OID : {
                    System.err.println(prefix + "OID (" + type.getTag() + ") " +  type.getValue().getClass());
                    int[] val = (int[]) type.getValue();
                    
                    System.err.print(prefix + "    object identifier:");
                    for (int i = 0;  i < val.length;  i++) {
                        System.err.print(" " + val[i]);
                    }
                    System.err.println();
                }
                break;

            case ASN1.TAG_INTEGER : { 
                    System.err.println(prefix + "INTEGER (" + type.getTag() + ") " +  type.getValue().getClass());
                    BigInteger val = (BigInteger) type.getValue();
    
                    HexDump hd = new HexDump(val.toByteArray());
                    hd.setPrefix("   " + prefix);
                    hd.dumpAll(System.err);
                }
                break;
                
            default:
                System.err.println(prefix + "(" + type.getTag() + ") " +  type.getValue().getClass());
        }
    }


    public void dumpASN1(byte[] data) {
        ByteArrayInputStream in =
                new ByteArrayInputStream(data);
        
        try (DERDecoder d = new DERDecoder(in)) {
            ASN1Type t = d.readType();
            dumpASN1Type(t);
        } catch (ASN1Exception e) {
            e.printStackTrace();
        } catch (IOException e) {
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

            byte[] prvData = prv.getEncoded();
            byte[] pubData = pub.getEncoded();

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
            
            
            dumpASN1(prv.getEncoded());
            dumpASN1(pub.getEncoded());
            
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
