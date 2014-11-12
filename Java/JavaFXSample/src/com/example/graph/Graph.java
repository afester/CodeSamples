package com.example.graph;

import javafx.scene.paint.Paint;

public interface Graph {
    /**
     * @param x The x value for which to return the value of this graph
     *
     * @return The y value of this graph for a given x value
     */
//    Double evaluate(double x);

    /**
     * @return The color in which to render this graph.
     */
    Paint getColor();

    void setIsError(boolean b);

    boolean isError();

    Point2DOperationIterator operationIterator(double from, double to, double delta);
}
