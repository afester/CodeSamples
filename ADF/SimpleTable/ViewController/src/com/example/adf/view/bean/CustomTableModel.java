package com.example.adf.view.bean;

import java.util.ArrayList;
import java.util.List;

import oracle.adf.model.BindingContext;

import oracle.binding.BindingContainer;


/**
 * A simple table model which returns a two dimensional array.
 */
public class CustomTableModel {

    public CustomTableModel() {
        super();
    }
    
    
    public List getTableModel() {
        
        System.err.println("Dumping binding2 container:");
        BindingContainer bindings = BindingContext.getCurrent().getCurrentBindingsEntry();
        for (Object o : bindings.entrySet()) {
            System.err.println("BINDING:" + o + "/" + o.getClass().getName());
        }
        List row1 = new ArrayList();

        row1.add("Andreas");
        row1.add("Fester");

        List row2 = new ArrayList();
        row2.add("Willi");
        row2.add("Meier");

        List result = new ArrayList();
        result.add(row1);
        result.add(row2);

        return result;
    }
}
