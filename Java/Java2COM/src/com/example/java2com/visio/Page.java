package com.example.java2com.visio;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;

public class Page extends COMObject {

    protected Page(IDispatch ci) {
        super(ci);
    }

    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public Shapes getShapes() {
        return new Shapes(ci.getProperty("Shapes").dispatch);
    }

    public Shape getPageSheet() {
        return new Shape(ci.getProperty("PageSheet").dispatch);
    }
}
