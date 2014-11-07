package com.example;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import com.example.hexdump.HexDump;

public class Serialization {
    
    public void run() throws IOException, ClassNotFoundException {
        Container c = new Container(42, "World", 1.42F, 3210, 
                                    new Container(43, "Moon",  3.14F, 9876,  null));
        System.err.println(c);

        ByteArrayOutputStream memory = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(memory);
        oos.writeObject(c);
        HexDump.dumpHex("", memory.toByteArray());

        ByteArrayInputStream input = new ByteArrayInputStream(memory.toByteArray());
        ObjectInputStream ois = new ObjectInputStream(input);

        Container c1 = (Container) ois.readObject(); 
        System.err.println(c1);
    }


    public static void main(String[] args) {
        try {
            new Serialization().run();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
