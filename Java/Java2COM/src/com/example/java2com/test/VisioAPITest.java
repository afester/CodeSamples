package com.example.java2com.test;

import static org.junit.Assert.assertEquals;

import java.io.File;

import javafx.geometry.Point2D;

import org.junit.Before;
import org.junit.Test;

import com.example.java2com.IDispatch;
import com.example.java2com.visio.Document;
import com.example.java2com.visio.Documents;
import com.example.java2com.visio.Page;
import com.example.java2com.visio.Pages;
import com.example.java2com.visio.Path;
import com.example.java2com.visio.Paths;
import com.example.java2com.visio.Curve;
import com.example.java2com.visio.Shape;
import com.example.java2com.visio.Shapes;
import com.example.java2com.visio.VisioApplication;


public class VisioAPITest {

    private String sampleFile;
    
    @Before
    public void setupFilenames() {
        String projDir = System.getProperty("user.dir");
        sampleFile = projDir + File.separator + "Sample.vsd";
         IDispatch.setDebugEnabled(false);
    }


    @Test
    public void testVisioAPI() {
        VisioApplication appl = new VisioApplication();
        assertEquals("Microsoft Visio", appl.getName());
        assertEquals("12,0", appl.getVersion());

        Documents docs = appl.getDocuments();
        assertEquals(0, docs.getCount());

        Document doc = docs.open(sampleFile);
        System.err.println(doc.getName());
        System.err.println(doc.getCompany());
        System.err.println(doc.getCreator());

        Pages pages = doc.getPages();
        assertEquals(2, pages.getCount());

        for (Page p : pages) {
            System.err.println("# " + p.getName());
            p.release();
        }

        //Page ap = appl.getActivePage();
        //System.err.println(ap.getName());

        Page p = pages.getItem("MYPAGE");

        Shapes shapes = p.getShapes();
        //assertEquals(2, shapes.getCount());

        for (Shape s : shapes) {
            System.err.println("# Shape: " + s.getType() +"/"+ s.getID() + "/" + s.getUniqueID()+
                    "/"+s.getName());

            Paths paths = s.getPaths();
            System.err.println("   # " + paths.getCount());
            for (Path p1 : paths) {
                System.err.println("   " + p1);
                p1.release();
            }
            paths.release();

/*            Connects connects = s.getConnects();
            System.err.println("   # " + connects.getCount());
            for (Connect c : connects) {
                System.err.println("   " + c);
                c.release();
            }
            connects.release();
*/
            s.release();
        }
        shapes.release();

        p.release();
        pages.release();
        // System.err.println(docs.getCount());
        // assertEquals(1, docs.getCount());

        doc.release();

        docs.release();
        appl.release();
    }

    @Test
    public void testPaths() {
        VisioApplication appl = new VisioApplication();
        Documents docs = appl.getDocuments();
        Document doc = docs.open(sampleFile);
        Pages pages = doc.getPages();
        Page p = pages.getItem("MYPAGE");
        Shapes shapes = p.getShapes();
        Shape s = shapes.getItem(5);
        assertEquals("Hexagon", s.getName());

        // A Shape consists of Paths
        Paths paths = s.getPaths();
        Path path = paths.getItem(1);

        System.err.println("Path path = new Path();");

        // Each Path consists of Curves
        int idx = 0;
        boolean first = true;
        for (Curve c : path) {
            double start = c.getStart();
            double end = c.getEnd();

            if (first) {
                first = false;
                Point2D p1 = c.point(start);

                System.err.printf("MoveTo moveTo = new MoveTo();\n" +
                                   "moveTo.setX(%ff);\n" + 
                                   "moveTo.setY(%ff);\n" +
                                   "path.getElements().add(moveTo);\n\n", p1.getX(), p1.getY());
            }

            Point2D p2 = c.point(end);
            System.err.printf("LineTo lineTo%d = new LineTo();\n" + 
                               "lineTo%d.setX(%ff);\n" +
                               "lineTo%d.setY(%ff);\n" +
                               "path.getElements().add(lineTo%d);\n\n", idx, idx, p2.getX(), idx, p2.getY(), idx);
            idx++;
        }

        appl.quit();

        path.release();
        paths.release();
        s.release();
        shapes.release();
        p.release();
        pages.release();
        doc.release();
        docs.release();
        appl.release();
    }
}
