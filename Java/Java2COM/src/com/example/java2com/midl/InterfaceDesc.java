package com.example.java2com.midl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class InterfaceDesc {

    private String name;
    private String extendsName; 
    private List<MethodDesc> methods = new ArrayList<>();
    private Map<String, MethodDesc> methodMap = new HashMap<>();
    public Map<String, AnnotationDesc> annotations = new HashMap<>();

    public InterfaceDesc(String name, String superName) {
        this.name = name;
        this.extendsName = superName;
    }

    public String getName() {
        return name;
    }

    public String getSuperName() {
        return extendsName;
    }

    public List<MethodDesc> getMethods() {
        return methods;
    }

    public void addMethod(MethodDesc currentMethod) {
        methods.add(currentMethod);
        methodMap.put(currentMethod.getName(),  currentMethod);
    }

    public Map<String, AnnotationDesc> getAnnotations() {
        return annotations;
    }

/*Convenience methods */    
    
    public String getJavaFileName() {
        return name + ".java";
    }

    public String getUUID() {
        String result = null;
        AnnotationDesc ad = annotations.get("uuid");
        if (ad != null) {
            if(ad.getParams().size() > 0) {
                return ad.getParams().get(0);
            }
        }
        return result;
    }

    public String getHelpstring() {
        String result = null;
        AnnotationDesc ad = annotations.get("helpstring");
        if (ad != null) {
            if(ad.getParams().size() > 0) {
                result = ad.getParams().get(0);
                if (result.startsWith("\"")) {
                    result = result.substring(1);
                }
                if (result.endsWith("\"")) {
                    result = result.substring(0, result.length() - 1);
                }
            }
        }
        return result;
    }

    public MethodDesc getMethod(String name) {
        return methodMap.get(name);
    }

    public boolean isIterable() {
        return (getMethod("Count") != null && 
                getMethod("Item") != null);
    }

}
