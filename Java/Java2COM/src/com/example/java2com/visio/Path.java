package com.example.java2com.visio;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;

public class Path extends COMCollection<Curve> {

    protected Path(IDispatch dispatch) {
        super(dispatch);
    }

    public void getPoints() {
        ci.invoke("Points");
    }

    @Override
    protected Curve newInstance(IDispatch dispatch) {
        return new Curve(dispatch);
    }
}
