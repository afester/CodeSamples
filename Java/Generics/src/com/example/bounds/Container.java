package com.example.bounds;

import java.util.List;

public class Container<T extends Data> {

    private T theData;

    public Container(T data) {
        theData = data;
    }

    public void performOperation() {
        System.err.printf("Type: %s\n",  theData.getType());
    }


    public void setData(T data, List<? extends T> otherData) {
        theData = data;
    }
}
