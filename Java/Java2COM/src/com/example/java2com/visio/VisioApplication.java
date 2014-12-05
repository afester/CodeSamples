package com.example.java2com.visio;

import com.example.java2com.COMObject;
import com.example.java2com.IDispatch;

public class VisioApplication extends COMObject {

    public VisioApplication() {
        super(new IDispatch("Visio.Application"));        
    }

    private VisioApplication(IDispatch disp) {
        super(disp);        
    }

    public static VisioApplication get(String objectName) {
        return new VisioApplication(IDispatch.get(objectName));
    }

    public String getName() {
        return ci.getProperty("Name").strValue;
    }

    public String getVersion() {
        return ci.getProperty("Version").strValue;
    }

    public Documents getDocuments() {
        return new Documents(ci.getProperty("Documents").dispatch);
    }

    public Page getActivePage() {
        return new Page(ci.getProperty("ActivePage").dispatch);
    }

    public Document getActiveDocument() {
        return new Document(ci.getProperty("ActiveDocument").dispatch);
    }

    public void quit() {
        ci.invoke("Quit");
    }
}
