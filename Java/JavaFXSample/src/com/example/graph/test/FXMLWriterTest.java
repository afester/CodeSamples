package com.example.graph.test;

import org.junit.Test;

import com.example.javafx.tools.FXMLWriter;

public class FXMLWriterTest {

    @Test
    public void testEmpty() {
/*       
        Path path = new Path();
        MoveTo moveTo = new MoveTo();
        moveTo.setX(5.179016f);
        moveTo.setY(10.826772f);
        path.getElements().add(moveTo);

        LineTo lineTo0 = new LineTo();
        lineTo0.setX(4.269803f);
        lineTo0.setY(10.826772f);
        path.getElements().add(lineTo0);

        LineTo lineTo1 = new LineTo();
        lineTo1.setX(3.815196f);
        lineTo1.setY(10.039370f);
        path.getElements().add(lineTo1);

        LineTo lineTo2 = new LineTo();
        lineTo2.setX(4.269803f);
        lineTo2.setY(9.251969f);
        path.getElements().add(lineTo2);

        LineTo lineTo3 = new LineTo();
        lineTo3.setX(5.179016f);
        lineTo3.setY(9.251969f);
        path.getElements().add(lineTo3);

        LineTo lineTo4 = new LineTo();
        lineTo4.setX(5.633622f);
        lineTo4.setY(10.039370f);
        path.getElements().add(lineTo4);

        LineTo lineTo5 = new LineTo();
        lineTo5.setX(5.179016f);
        lineTo5.setY(10.826772f);
        path.getElements().add(lineTo5);
*/

        FXMLWriter writer = new FXMLWriter("test1.fxml");
        writer.write();
    }
}
