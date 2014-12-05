package com.example.java2com.visio;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Cell extends COMObject {

    protected Cell(IDispatch dispatch) {
        super(dispatch);
    }

    public double getResult(String unit) {
        return ci.getProperty("Result", new Variant(unit)).doubleValue;
    }

    public String getResultStr(String unit) {
        return ci.getProperty("ResultStr", new Variant(unit)).strValue;
    }

    public int getResultInt(String unit, int fRound) {
        return ci.getProperty("ResultInt", new Variant(unit), new Variant(fRound)).intValue;
    }

    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public String toString() {
        return String.format("%s[dispatchHandle=0x%s, name=%s, result=%f]", 
                             getClass().getSimpleName(), ci.toString(),
                             getName(), getResult("mm"));
    }


}
