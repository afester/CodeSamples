package com.example.graph;


class DiscreteGraphIterator implements Point2DOperationIterator {

    private boolean first = true;
    private int index = 0;
    private DiscreteGraph graph;
    private double x;
    private double to;

    public DiscreteGraphIterator(DiscreteGraph graph, double from, double to) {
        this.index = 0;
        this.graph = graph;
        this.to = to;
        this.x = graph.start;
    }

    @Override
    public boolean hasNext() {
        return (index < graph.values.length && x < to);
    }

    @Override
    public Point2DOperation next() {
        // the first call needs to return a MOVETO operation, all following calls
        // will return a LINETO operation by default, unless y evaluates to NaN
        // (see below)
        Point2DOperation.Operation retOper = Point2DOperation.Operation.LINETO;
        if (first) {
            retOper = Point2DOperation.Operation.MOVETO;
            first = false;
        }

        Point2DOperation result = new Point2DOperation(x, graph.values[index], retOper);
        x += graph.delta;
        index++;
        return result;
    }

}
