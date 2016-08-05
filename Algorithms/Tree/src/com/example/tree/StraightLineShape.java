package com.example.tree;

import javafx.geometry.Point2D;
import javafx.scene.shape.Circle;
import javafx.scene.shape.LineTo;
import javafx.scene.shape.MoveTo;
import javafx.scene.shape.Path;

public class StraightLineShape extends Path {

    private LineTo lineTo;

    public StraightLineShape(double x1, double y1, double x2, double y2) {

        Point2D dir = new Point2D(150, 50);
        Point2D dir2 = new Point2D(-dir.getY(), dir.getX());
//        dir.multiply(0.5);
        Point2D x = dir2.add(dir.midpoint(0, 0));

//        MoveTo moveTo = new MoveTo(50, 50);
//        LineTo a1 = new LineTo(60, 55);
//        LineTo a2 = new LineTo(60, 45);
//        LineTo a3 = new LineTo(50, 50);

        MoveTo moveTo2 = new MoveTo(50, 50);
        lineTo = new LineTo(200, 100);
        MoveTo moveTo3 = new MoveTo(50 + dir.getX(), 50 + dir.getY());
        Circle c = new Circle(50 + dir.getX(), 50 + dir.getY(), 3);
        // lineTo = new LineTo(200, 100);

        getElements().addAll(/*moveTo, a1, a2, a3, */moveTo2, lineTo, c);
    }
}
