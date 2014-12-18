package com.example.java2com.visio;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;

public class Document extends COMObject {

    protected Document(IDispatch dispatch) {
        super(dispatch);
    }

    public static Document get(String objectName) {
        return new Document(IDispatch.get(objectName));
    }

    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public Pages getPages() {
        return new Pages(ci.getProperty("Pages").dispatch);
    }

    public String getCompany() {
        return ci.getProperty("Company").strValue;
    }

    public String getCreator() {
        return ci.getProperty("Creator").strValue;
    }
}
