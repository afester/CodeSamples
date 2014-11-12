package com.example;

import com.example.graph.Point2DOperation;
import com.example.graph.Point2DOperationIterator;

class FunctionGraphIterator implements Point2DOperationIterator {

    private FunctionGraph theGraph;
    private double to;
    private double delta;

    private double x;
    private boolean first = true;

    public FunctionGraphIterator(FunctionGraph functionGraph, double from,
                                 double to, double delta) {
        this.theGraph = functionGraph;
        this.to = to;
        this.delta = delta; 

        x = from;
    }

    @Override
    public boolean hasNext() {
        if (x + delta < to) {
            return true;
        }
        return false;
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

        // evaluate the current x value
        double y = theGraph.evaluate(x);

        // if evaluate returned a NaN, look up the next value which returns a valid
        // value, and return a MOVE operation (need to restart painting)
        if (Double.isNaN(y)) {
            while(Double.isNaN(y) && hasNext()) {
                x += delta;
                y = theGraph.evaluate(x);
            }
            retOper = Point2DOperation.Operation.MOVETO; 
        }

        
        Point2DOperation result = new Point2DOperation(x, y, retOper);
        x += delta;
        return result;
    }
}
