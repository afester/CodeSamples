package com.example.graph;

public interface Graph {
    void setIsError(boolean b);

    boolean isError();

    Point2DOperationIterator operationIterator(double from, double to, double delta);
}
