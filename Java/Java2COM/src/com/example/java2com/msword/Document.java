package com.example.java2com.msword;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Document extends COMObject {

    protected Document(IDispatch dispatch) {
        super(dispatch);
    }

    public String getName() {
        Variant result = ci.getProperty("Name");
        return result.strValue;
    }

}
