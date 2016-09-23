package com.example;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class DataStream {

    public static void main(String[] args) {
        new DataStream().run();
    }

    private void run() {
        System.err.println("Writing ...");
        try {
            DataOutputStream dos = new DataOutputStream(new FileOutputStream("Test.dat"));
            dos.writeLong(1234567890L);
            dos.writeLong(9876543210L);
            //dos.writeUTF("Hello");
            //dos.writeUTF("World");
            dos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.err.println("Reading ...");
        try {
            DataInputStream dis = new DataInputStream(new FileInputStream("Test.dat"));

            //String val = dis.readUTF();
            //long val = dis.readLong();
            int val = dis.readInt();
            System.err.println("#1:" + val);
            //val = dis.readUTF();
            //val = dis.readLong();
            val = dis.readInt();
            System.err.println("#2:" + val);
            //val = dis.readUTF();
            //val = dis.readLong();
            val = dis.readInt();
            System.err.println("#3:" + val);

            dis.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

    }
}
