package com.example.java2com.msword;

import com.example.java2com.COMCollection;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Documents extends COMCollection<Document>  {

    protected Documents(IDispatch ci) {
        super(ci);
    }


    public Document open(String fileName) {
        Variant result = ci.invoke("Open", new Variant(fileName));
        return new Document(result.dispatch);
    }

    @Override
    protected Document newInstance(IDispatch dispatch) {
        return new Document(dispatch);
    }
    
}
