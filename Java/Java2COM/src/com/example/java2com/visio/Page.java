package com.example.java2com.visio;

import com.example.java2com.COMInterface;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;


public class Page extends COMInterface {

    protected Page(IDispatch ci) {
        super(ci);
    }

    public String getName() {
        Variant result = ci.getProperty("Name");
        return result.strValue;
    }

    public Shapes getShapes() {
        Variant result = ci.getProperty("Shapes");
        return new Shapes(result.dispatch);
    }

}
