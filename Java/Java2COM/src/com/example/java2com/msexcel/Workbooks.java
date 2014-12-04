package com.example.java2com.msexcel;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;


public class Workbooks extends  COMCollection<Worksheet> {

    public Workbooks(IDispatch dispatch) {
        super(dispatch);
    }

    public Workbook open(String fileName) {
        Variant result = ci.invoke("Open", new Variant(fileName));
        return new Workbook(result.dispatch);
    }

    @Override
    protected Worksheet newInstance(IDispatch dispatch) {
        return new Worksheet(dispatch);
    }
}
