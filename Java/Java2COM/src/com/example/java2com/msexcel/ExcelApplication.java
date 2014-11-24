package com.example.java2com.msexcel;

import com.example.java2com.COMInterface;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;


public class ExcelApplication extends COMInterface {

    public ExcelApplication() {
        super(new IDispatch("Excel.Application"));        
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


    public Workbooks getWorkbooks() {
        Variant result = ci.getProperty("Workbooks");
        return new Workbooks(result.dispatch);
    }
}
