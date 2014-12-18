package com.example.java2com.visio;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

import javafx.geometry.Point2D;

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

            IDispatch.setDebugEnabled(false);
            
            String projDir = System.getProperty("user.dir");
            String sampleFile = projDir + "/Sample.vsd";
            
            Document document = Document.get(sampleFile);

            Pages pages = document.getPages();
            Page page = pages.getItem("ONLINE");

            Shape properties = page.getPageSheet();
            System.err.println(properties.toString());

            // http://msdn.microsoft.com/en-us/library/office/ff765983(v=office.15).aspx
//            for (int i = 0;  i < 255;  i++) {
//                int rowCount = properties.getRowCount(i);
//                System.err.printf("%d: %d\n", i, rowCount);
//            }

/**************************/
            Cell w = properties.getCells("PageWidth");
            System.err.println(w);
            Cell h = properties.getCells("PageHeight");
            System.err.println(h);
/*
            Cell  cd = properties.getCellsSRC(1, 10, 0);
            System.err.println(cd);

            int section = 1;    // visSectionObject
            int rowCount = properties.getRowCount(section);
            System.err.printf("%d\n", rowCount);

            // section, row, column
            // VisSectionIndices: http://msdn.microsoft.com/en-us/library/office/ff765983(v=office.15).aspx
            // VisRowIndices    : http://msdn.microsoft.com/en-us/library/office/ff765539(v=office.15).aspx
            // VisCellIndices   : http://msdn.microsoft.com/en-us/library/office/ff767991(v=office.15).aspx
            for (int row = 0;  row < rowCount;  row++) {
                Cell result = properties.getCellsSRC(section, row, 0);
                System.err.println(result);
            }
/**************************/

            Shapes shapes = page.getShapes();
            for (Shape s : shapes) {
                System.out.println(s);
                Paths paths = s.getPaths();
                Path path = paths.getItem(1);

                for (Curve c : path) {
                    double start = c.getStart();
                    double end = c.getEnd();

                    Point2D p1 = c.point(start);
                    p1 = p1.multiply(2.54 * 10);

                    Point2D p2 = c.point(end);
                    p2 = p2.multiply(2.54 * 10);

                    System.out.println("   " + p1 + "" + p2);
                }

                s.release();
            }

            shapes.release();
            page.release();
            pages.release();
            document.release();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
