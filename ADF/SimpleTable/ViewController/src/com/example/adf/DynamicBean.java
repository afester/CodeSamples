package com.example.adf;

public interface DynamicBean {
    Object getProperty(String name);

    void setProperty(String name, Object value);

    Class<?> getPropertyType(String name);
    
    boolean isReadOnly(String name);
}
