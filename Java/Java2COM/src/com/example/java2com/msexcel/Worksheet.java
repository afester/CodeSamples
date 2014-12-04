package com.example.java2com.msexcel;

import com.example.java2com.COMInterface;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Worksheet extends COMInterface {

    protected Worksheet(IDispatch ci) {
        super(ci);
    }

    public String getName() {
        Variant result = ci.getProperty("Name");
        return result.strValue;
    }

}
