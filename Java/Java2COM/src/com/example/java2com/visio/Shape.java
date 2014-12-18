package com.example.java2com.visio;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Shape extends COMObject {

    public final static int visTypeInval=0;
    public final static int visTypePage=1;
    public final static int visTypeGroup=2;
    public final static int visTypeShape=3;
    public final static int visTypeForeignObject=4;
    public final static int visTypeGuide=5;
    public final static int visTypeDoc=6;
    public final static int visTypeMetafile=16;
    public final static int visTypeBitmap=32;
    public final static int visTypeInk=64;
    public final static int visTypeIsLinked=256;
    public final static int visTypeIsEmbedded=512;
    public final static int visTypeIsControl=1024;
    public final static int visTypeIsOLE2=32768;


    protected Shape(IDispatch dispatch) {
        super(dispatch);
    }

    public int getGeometryCount() {
        return ci.getProperty("GeometryCount").intValue;
    }

    public int getType() {
        return ci.getProperty("Type").intValue;
    }

    public Connects getConnects() {
        return new Connects(ci.getProperty("Connects").dispatch);
    }

    public String getUniqueID() {
        return ci.getProperty("UniqueID", new Variant(1)).strValue;
    }

    public long getID() {
        return ci.getProperty("ID").intValue;
    }

    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public Paths getPaths() {
        return new Paths(ci.getProperty("Paths").dispatch);
    }

    public int getRowCount(int i) {
        return ci.getProperty("RowCount", new Variant(i)).intValue;
    }

    public Cell getCellsSRC(int i, int j, int k) {
        return new Cell(ci.getProperty("CellsSRC", new Variant(i), new Variant(j), new Variant(k)).dispatch);
    }

    public Cell getCells(String s) {
        return new Cell(ci.getProperty("Cells", new Variant(s)).dispatch);
    }

    public String getLineStyle() {
        return ci.getProperty("LineStyle").strValue;
    }

    public String toString() {
        return String.format("%s[dispatchHandle=0x%s, name=%s, type=%d]", 
                             getClass().getSimpleName(), ci.toString(),
                             getName(), getType());
    }
}
