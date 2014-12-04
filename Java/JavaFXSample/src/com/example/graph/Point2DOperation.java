package com.example.graph;

import javafx.geometry.Point2D;

public class Point2DOperation {

    public enum Operation {MOVETO, LINETO}

    private double x;
    private double y;
    private Operation oper;
    
    public Point2DOperation(double x, double y, Operation oper) {
        this.x = x;
        this.y = y;
        this.oper = oper;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }
    
    public Operation getOperation() {
        return oper;
    }
 
    @Override
    public String toString() {
        return String.format("Point2DOperation[x=%f, y=%f, oper=%s]", x, y, oper); 
    }

    public Point2D getPoint() {
        return new Point2D(x, y);
    }
}
