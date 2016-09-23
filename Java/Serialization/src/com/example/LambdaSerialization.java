package com.example;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

import com.example.hexdump.HexDump;

public class LambdaSerialization {
    
    public void run() throws IOException, ClassNotFoundException {
        String c = "Hello"; // new Container(42, "World", 1.42F, 3210, null);
        System.err.println(c);

        ByteArrayOutputStream memory = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(memory);
        oos.writeObject(c);
        
        Runnable r = (Runnable & Serializable)() -> System.err.println("Hello World");
        oos.writeObject(r);
        System.err.println(r.getClass().getName());

        HexDump.dumpHex("", memory.toByteArray());

        ByteArrayInputStream input = new ByteArrayInputStream(memory.toByteArray());
        ObjectInputStream ois = new ObjectInputStream(input);

        String c1 = (String) ois.readObject(); 
        System.err.println(c1);
        
        System.err.println("DESERIALIZE\n=============");
        Runnable r2 = (Runnable) ois.readObject();
        r2.run();
    }


    public static void main(String[] args) {
        try {
            new LambdaSerialization().run();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
