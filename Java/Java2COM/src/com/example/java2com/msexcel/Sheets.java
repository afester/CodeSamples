package com.example.java2com.msexcel;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;


public class Sheets extends COMCollection<Worksheet> {

    public Sheets(IDispatch dispatch) {
        super(dispatch);
    }

    @Override
    protected Worksheet newInstance(IDispatch dispatch) {
        return new Worksheet(dispatch);
    }
}
