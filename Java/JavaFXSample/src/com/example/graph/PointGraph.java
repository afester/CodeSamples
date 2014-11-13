package com.example.graph;


public class PointGraph implements Graph {

    private double[][] values = null;

    public PointGraph(double[][] values) {
        this.values = values;
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
