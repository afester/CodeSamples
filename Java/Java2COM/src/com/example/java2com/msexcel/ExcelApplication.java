package com.example.java2com.msexcel;

import com.example.java2com.COMInterface;
import com.example.java2com.IDispatch;

public class ExcelApplication extends COMInterface {

    public ExcelApplication() {
        super(new IDispatch("Excel.Application"));        
    }

    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public Workbooks getWorkbooks() {
        return new Workbooks(ci.getProperty("Workbooks").dispatch);
    }
    
    public void quit() {
        ci.invoke("Quit");
    }
}
