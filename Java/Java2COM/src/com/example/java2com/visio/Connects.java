package com.example.java2com.visio;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;

public class Connects extends COMCollection<Connect>  {

    protected Connects(IDispatch ci) {
        super(ci);
    }

    @Override
    protected Connect newInstance(IDispatch dispatch) {
        return new Connect(dispatch);
    }
}
