package com.example.java2com.test;

import static org.junit.Assert.assertEquals;

import java.io.File;

import javafx.geometry.Point2D;

import org.junit.Before;
import org.junit.Test;

import com.example.java2com.IDispatch;
import com.example.java2com.COMProxy;
import com.example.java2com.Variant;
import com.example.java2com.VariantOut;

import com.example.java2com.visio.IVApplication;
import com.example.java2com.visio.IVCurve;
import com.example.java2com.visio.IVDocument;
import com.example.java2com.visio.IVDocuments;
import com.example.java2com.visio.IVPage;
import com.example.java2com.visio.IVPages;
import com.example.java2com.visio.IVPath;
import com.example.java2com.visio.IVPaths;
import com.example.java2com.visio.IVShape;
import com.example.java2com.visio.IVShapes;

public class VisioAPITest {

    private String sampleFile;
    
    @Before
    public void setupFilenames() {
        String projDir = System.getProperty("user.dir");
        sampleFile = projDir + File.separator + "Sample.vsd";
         COMProxy.setDebugEnabled(false);
    }


    @Test
    public void testVisioAPI() {
        IVApplication appl = IDispatch.create("Visio.Application", IVApplication.class);
        assertEquals("Microsoft Visio", appl.getName());
        assertEquals("12,0", appl.getVersion());

        IVDocuments docs = appl.getDocuments();
        assertEquals(0, docs.getCount());

        IVDocument doc = docs.open(sampleFile);
        System.err.println(doc.getName());
        System.err.println(doc.getCompany());
        System.err.println(doc.getCreator());

        IVPages pages = doc.getPages();
        assertEquals(4, pages.getCount());

        for (IVPage p : pages) {
            System.err.println("# " + p.getName());
            p.release();
        }

        //Page ap = appl.getActivePage();
        //System.err.println(ap.getName());

        IVPage p = pages.getItem(new Variant("MYPAGE"));

        IVShapes shapes = p.getShapes();
        //assertEquals(2, shapes.getCount());

        for (IVShape s : shapes) {
            System.err.println("# Shape: " + s.getType() +"/"+ s.getID() + "/" + s.getUniqueID(1)+
                    "/"+s.getName());

            IVPaths paths = s.getPaths();
            System.err.println("   # " + paths.getCount());
            for (IVPath p1 : paths) {
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

        appl.quit();

        appl.release();
    }

    @Test
    public void testPaths() {
        IVApplication appl = IDispatch.create("Visio.Application", IVApplication.class);
        IVDocuments docs = appl.getDocuments();
        IVDocument doc = docs.open(sampleFile);
        IVPages pages = doc.getPages();
        IVPage p = pages.getItem(new Variant("MYPAGE"));
        IVShapes shapes = p.getShapes();
        IVShape s = shapes.getItem(new Variant(5));
        assertEquals("Hexagon", s.getName());

        // A Shape consists of Paths
        IVPaths paths = s.getPaths();
        IVPath path = paths.getItem(1);

        System.err.println("Path path = new Path();");

        // Each Path consists of Curves
        int idx = 0;
        boolean first = true;
        for (IVCurve c : path) {
            double start = c.getStart();
            double end = c.getEnd();

            if (first) {
                first = false;

                VariantOut x = new VariantOut(VariantOut.VT_R8);
                VariantOut y = new VariantOut(VariantOut.VT_R8);
                c.point(start, x, y);
                Point2D p1 = new Point2D(x.doubleValue, y.doubleValue);

                System.err.printf("MoveTo moveTo = new MoveTo();\n" +
                                   "moveTo.setX(%ff);\n" + 
                                   "moveTo.setY(%ff);\n" +
                                   "path.getElements().add(moveTo);\n\n", p1.getX(), p1.getY());
            }

            VariantOut x = new VariantOut(VariantOut.VT_R8);
            VariantOut y = new VariantOut(VariantOut.VT_R8);
            c.point(start, x, y);
            Point2D p2 = new Point2D(x.doubleValue, y.doubleValue);

            System.err.printf("LineTo lineTo%d = new LineTo();\n" + 
                               "lineTo%d.setX(%ff);\n" +
                               "lineTo%d.setY(%ff);\n" +
                               "path.getElements().add(lineTo%d);\n\n", idx, idx, p2.getX(), idx, p2.getY(), idx);

            idx++;
            c.release();
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
