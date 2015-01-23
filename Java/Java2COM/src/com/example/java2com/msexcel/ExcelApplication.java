package com.example.java2com.msexcel;

import com.example.java2com.IDispatch;
import com.example.java2com.COMProxy;

public class ExcelApplication extends IDispatch {

    public ExcelApplication() {
        super(new COMProxy("Excel.Application"));        
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
