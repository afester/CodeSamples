package com.example.java2com.visio;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;

public class Shapes extends COMCollection<Shape>  {

    protected Shapes(IDispatch ci) {
        super(ci);
    }


    @Override
    protected Shape newInstance(IDispatch dispatch) {
        return new Shape(dispatch);
    }
    
}

