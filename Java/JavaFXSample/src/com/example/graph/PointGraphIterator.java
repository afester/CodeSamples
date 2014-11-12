package com.example.graph;

public class PointGraphIterator implements Point2DOperationIterator {

    private double[][] values;
    private int index;
    private boolean first = true;

    public PointGraphIterator(double[][] values) {
        this.values = values;
        index = 0;
    }

    @Override
    public boolean hasNext() {
        return (index < values.length);
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
        
        Point2DOperation result = new Point2DOperation(values[index][0], values[index][1], retOper);
        index++;
        return result;
    }

}
