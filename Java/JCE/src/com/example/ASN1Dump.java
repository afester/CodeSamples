package com.example;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.util.Iterator;

import codec.asn1.ASN1;
import codec.asn1.ASN1Exception;
import codec.asn1.ASN1Sequence;
import codec.asn1.ASN1Type;
import codec.asn1.DERDecoder;

import com.example.hexdump.HexDump;

public class ASN1Dump {
    
    private String prefix = "    ";
    
    private void dumpASN1Sequence(ASN1Sequence seq) {
        prefix = prefix + "   ";
        Iterator<?> i = seq.iterator();
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
                    
                    System.err.println("LENGTH:" + data.length);
                    
                    
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
           // System.err.println("Type:" + t.toString());
            dumpASN1Type(t);
        } catch (ASN1Exception e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    
    public static void dump(byte[] data) {
        ASN1Dump dump = new ASN1Dump();
        dump.dumpASN1(data);;
    }


    public static void dump(String title, byte[] data) {
        System.err.println(title);
        dump(data);
    }

}
