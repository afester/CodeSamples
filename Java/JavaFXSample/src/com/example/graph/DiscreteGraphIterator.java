package com.example.graph;


class DiscreteGraphIterator implements Point2DOperationIterator {

    private boolean first = true;
    private int index = 0;
    private double[] values;
    private double x;
    private double delta;
    private double to;

    public DiscreteGraphIterator(double[] values, double from, double to,
            double delta) {
        this.index = 0;
        this.values = values;
        this.to = to;
        this.delta = delta;
        this.x = from;
    }

    @Override
    public boolean hasNext() {
        return (index < values.length && x < to);
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

        Point2DOperation result = new Point2DOperation(x, values[index], retOper);
        x += delta;
        index++;
        return result;
    }

}
