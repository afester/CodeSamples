package com.example.java2com.midl;

import java.util.List;

public class AnnotationDesc {

    private String annotationName;
    private List<String> annotationParams;

    public AnnotationDesc(String annotationName2, List<String> annotationParams2) {
        this.annotationName = annotationName2;
        this.annotationParams = annotationParams2;
    }

    public String getName() {
        return annotationName;
    }

    public List<String> getParams() {
        return annotationParams;
    }

    public String toString() {
        return annotationParams.toString();
    }
}
