package com.example.java2com.msexcel;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Worksheet extends COMObject {

    protected Worksheet(IDispatch ci) {
        super(ci);
    }

    public String getName() {
        Variant result = ci.getProperty("Name");
        return result.strValue;
    }

}
