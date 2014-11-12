package com.example.graph;

import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;

public class PointGraph implements Graph {

    private double[][] values = null;

    public PointGraph(double[][] values) {
        this.values = values;
    }

    @Override
    public Paint getColor() {
        return Color.RED;
    }

    @Override
    public void setIsError(boolean b) {
    }

    @Override
    public boolean isError() {
        return false;
    }

    @Override
    public Point2DOperationIterator operationIterator(double ignored, double ignored1,
                                                      double ignored2) {
        return new PointGraphIterator(values);
    }
}
