package com.example.java2com.msexcel;

import com.example.java2com.COMCollection;
import com.example.java2com.COMProxy;


public class Sheets extends COMCollection<Worksheet> {

    public Sheets(COMProxy dispatch) {
        super(dispatch);
    }

    @Override
    protected Worksheet newInstance(COMProxy dispatch) {
        return new Worksheet(dispatch);
    }
}
