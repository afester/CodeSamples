package com.example.java2com.msword;

import com.example.java2com.COMCollection;
import com.example.java2com.COMProxy;
import com.example.java2com.Variant;

public class Documents extends COMCollection<Document>  {

    protected Documents(COMProxy ci) {
        super(ci);
    }


    public Document open(String fileName) {
        Variant result = ci.invoke("Open", new Variant(fileName));
        return new Document(result.dispatch);
    }

    @Override
    protected Document newInstance(COMProxy dispatch) {
        return new Document(dispatch);
    }
    
}
