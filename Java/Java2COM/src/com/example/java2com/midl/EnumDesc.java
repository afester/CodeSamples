package com.example.java2com.midl;

import java.util.ArrayList;
import java.util.List;

public class EnumDesc {

    private String name;
    private List<EnumElement> elements  = new ArrayList<>();

    public EnumDesc(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void addElement(EnumElement enumElement) {
        elements.add(enumElement);
    }

    public String getJavaFileName() {
        return name + ".java";
    }
    
    public List<EnumElement> getElements() {
        return elements;
    }
    

}
