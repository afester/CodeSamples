package com.example;

import java.io.Serializable;

public class Container implements Serializable {

    private static final long serialVersionUID = -8612894342923952818L;

    public int intData;
    public String stringData;
    public Float floatData;
    public transient int volatileData;
    private Object genericData = null;

    public Container(int i, String s, Float f, int v, Object o) {
        intData = i;
        stringData = s;
        floatData = f;
        volatileData = v;
        genericData = o;
    }
    
    
    @Override
    public String toString() {
        return "Container[" + intData + ", " + stringData + ", " +floatData + ", " + volatileData + ", " + genericData + "]";
    }
}
