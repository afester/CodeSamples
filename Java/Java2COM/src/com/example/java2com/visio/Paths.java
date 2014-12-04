package com.example.java2com.visio;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;

public class Paths extends COMCollection<Path>  {

    protected Paths(IDispatch ci) {
        super(ci);
    }

    @Override
    protected Path newInstance(IDispatch dispatch) {
        return new Path(dispatch);
    }
}
