package com.example.graph;


public class DiscreteGraph implements Graph {

    protected double[] values = null;
    protected double start;
    protected double delta;

    public DiscreteGraph(double start, double delta, double[] is) {
        values = is;
        this.delta = delta;
        this.start = start;
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
        return new DiscreteGraphIterator(this, from, to);
    }
}
