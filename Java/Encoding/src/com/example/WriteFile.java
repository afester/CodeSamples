package com.example;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

public class WriteFile {
    
    public void write(OutputStream os) {
        String data = "Hello World, this is some\ndata we want to write (including €).\n";
        
        try {
            os.write(data.getBytes());
            os.write(data.getBytes(StandardCharsets.UTF_8));
            os.write(data.getBytes(StandardCharsets.UTF_16));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static void main(String[] args) throws FileNotFoundException {
        WriteFile model = new WriteFile();
        model.write(System.err);
        model.write(new FileOutputStream("output.dat"));
    }
}
