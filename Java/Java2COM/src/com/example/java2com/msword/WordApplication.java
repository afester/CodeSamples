package com.example.java2com.msword;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;


public class WordApplication extends COMObject {

    public WordApplication() {
        super(new IDispatch("Word.Application"));        
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