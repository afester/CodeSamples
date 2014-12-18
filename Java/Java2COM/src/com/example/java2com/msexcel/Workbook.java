package com.example.java2com.msexcel;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;


public class Workbook extends COMObject {

    protected Workbook(IDispatch ci) {
        super(ci);
    }

    public Sheets getSheets() {
        Variant result = ci.getProperty("Sheets");
        return new Sheets(result.dispatch);
    }

}
