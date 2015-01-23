package com.example.java2com.msexcel;

import com.example.java2com.COMCollection;
import com.example.java2com.COMProxy;
import com.example.java2com.Variant;


public class Workbooks extends  COMCollection<Worksheet> {

    public Workbooks(COMProxy dispatch) {
        super(dispatch);
    }

    public Workbook open(String fileName) {
        Variant result = ci.invoke("Open", new Variant(fileName));
        return new Workbook(result.dispatch);
    }

    @Override
    protected Worksheet newInstance(COMProxy dispatch) {
        return new Worksheet(dispatch);
    }
}
