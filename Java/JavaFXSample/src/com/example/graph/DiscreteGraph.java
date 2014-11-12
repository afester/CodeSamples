package com.example.graph;

import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;

public class DiscreteGraph implements Graph {

    private double[] values = null;
    private double delta;

    public DiscreteGraph(double delta, double[] is) {
        values = is;
        this.delta = delta;
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
    public Point2DOperationIterator operationIterator(double from, double to,
                                                      double ignored) {
        return new DiscreteGraphIterator(values, from, to, this.delta);
    }
}
