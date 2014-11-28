package com.example.java2com.visio;

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


    public Pages getPages() {
        Variant result = ci.getProperty("Pages");
        return new Pages(result.dispatch);
    }

    public String getCompany() {
        Variant result = ci.getProperty("Company");
        return result.strValue;
    }

    
    public String getCreator() {
        Variant result = ci.getProperty("Creator");
        return result.strValue;
    }

}
