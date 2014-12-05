package com.example.java2com.visio;

import javafx.geometry.Point2D;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;
import com.example.java2com.VariantOut;

public class Curve extends COMObject {

    protected Curve(IDispatch dispatch) {
        super(dispatch);
    }

    public double getStart() {
        return ci.getProperty("Start").doubleValue;
    }

    public double getEnd() {
        return ci.getProperty("End").doubleValue;
    }

    public Point2D point(double t) {
        Variant x = new VariantOut(VariantOut.VT_R8);
        Variant y = new VariantOut(VariantOut.VT_R8);
        Variant[] params = new Variant[] { new Variant(t), x, y };
        ci.invoke("Point", params);
        return new Point2D(x.doubleValue, y.doubleValue);
    }

    public int getObjectType() {
        return ci.getProperty("ObjectType").intValue;
    }
}
