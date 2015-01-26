package com.example.java2com.msword;

import com.example.java2com.IDispatch;
import com.example.java2com.COMProxy;


public class WordApplication extends IDispatch {

    public WordApplication() {
        super(new COMProxy("Word.Application"));        
    }
    
    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public Documents getDocuments() {
        return new Documents(ci.getProperty("Documents").dispatch);
    }
    
    public void quit() {
        ci.invoke("Quit");
    }
}
