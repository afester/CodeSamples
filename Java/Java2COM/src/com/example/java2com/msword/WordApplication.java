package com.example.java2com.msword;

import com.example.java2com.COMInterface;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;


public class WordApplication extends COMInterface {

    public WordApplication() {
        super(new IDispatch("Word.Application"));        
    }

    
    public String getName() {
        Variant result = ci.getProperty("Name");
        return result.strValue;
        
    }


    public void release() {
        // Required for word to be terminated! Excel does not require this, though ...
        ci.invoke("Quit");
        ci.release();
    }


    public Documents getDocuments() {
        Variant result = ci.getProperty("Documents");
        return new Documents(result.dispatch);
    }
}
