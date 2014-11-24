package com.example.java2com.msword;

import com.example.java2com.COMInterface;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Document extends COMInterface {

    protected Document(IDispatch dispatch) {
        super(dispatch);
    }

    public String getName() {
        Variant result = ci.getProperty("Name");
        return result.strValue;
    }

}
