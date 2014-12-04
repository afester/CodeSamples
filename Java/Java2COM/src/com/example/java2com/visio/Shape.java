package com.example.java2com.visio;

import com.example.java2com.COMInterface;
import com.example.java2com.IDispatch;
import com.example.java2com.Variant;

public class Shape extends COMInterface {

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
        Variant result = ci.getProperty("GeometryCount");
        return result.intValue;
    }

    public int getType() {
        Variant result = ci.getProperty("Type");
        return result.intValue;
    }

    public Connects getConnects() {
        Variant result = ci.getProperty("Connects");
        return new Connects(result.dispatch);
    }

    public String getUniqueID() {
        Variant result = ci.getProperty("UniqueID", new Variant(1));
        return result.strValue;
    }

    public long getID() {
        Variant result = ci.getProperty("ID");
        return result.intValue;
    }

    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public Paths getPaths() {
        return new Paths(ci.getProperty("Paths").dispatch);
    }

/*    public String getBoundingBox() {
        Variant[] params = new Variant[5];
        params[0] = new Variant(0);
        ci.invoke("BoundingBox", params);

        return "";
    }*/
    
}
