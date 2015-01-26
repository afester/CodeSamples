package com.example.java2com.midl;

import java.util.HashMap;
import java.util.Map;

public class ParameterDesc {
    private String name;
    private TypeDesc type;
    public Map<String, AnnotationDesc> annotations = new HashMap<>();   // TODO

    public ParameterDesc(String pName, TypeDesc pType) {
        this.name = pName;
        this.type = pType; 
    }

    public String getName() {
        return name;
    }

    public String getTypeName() {
        return type.getName();
    }

    public Map<String, AnnotationDesc> getAnnotations() {
        return annotations;
    }

/*Convenience methods */    

    public boolean isReturn() {
        return annotations.containsKey("retval");
    }

    public TypeDesc getType() {
        return type;
    }

    public boolean isOut() {
        return annotations.containsKey("out");
    }
}
