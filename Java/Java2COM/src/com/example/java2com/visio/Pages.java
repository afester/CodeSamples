package com.example.java2com.visio;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;

public class Pages  extends COMCollection<Page>  {

    protected Pages(IDispatch ci) {
        super(ci);
    }

    @Override
    protected Page newInstance(IDispatch dispatch) {
        return new Page(dispatch);
    }
}
