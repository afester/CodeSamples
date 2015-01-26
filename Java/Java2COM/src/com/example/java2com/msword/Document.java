package com.example.java2com.msword;

import com.example.java2com.IDispatch;
import com.example.java2com.COMProxy;
import com.example.java2com.Variant;

public class Document extends IDispatch {

    protected Document(COMProxy dispatch) {
        super(dispatch);
    }

    public String getName() {
        Variant result = ci.getProperty("Name");
        return result.strValue;
    }

}
