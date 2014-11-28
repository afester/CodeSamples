package com.example.java2com.visio;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

import com.example.java2com.IDispatch;

public class FXMLExport {

    private String fileName;

    public FXMLExport(String theFileName) {
        fileName = theFileName;
    }

    public static void main(String[] args) {
        new FXMLExport("sample.fxml").export();
    }

    private void export() {
        try (PrintStream file = new PrintStream(new FileOutputStream(fileName))) {
            file.println("XML");

            IDispatch.setDebugEnabled(true);
            VisioApplication appl = 
                    VisioApplication.get("Visio.Application");
//            System.err.println(appl);
            System.out.printf("Connected to %s %s\n", appl.getName(), appl.getVersion());
/*
            Page page = appl.getActivePage();

            Shapes shapes = page.getShapes();
            for (Shape s : shapes) {
                System.err.println(s);
            }
*/
            appl.release();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
