package com.example.adf.view.bean;

import com.example.adf.CustomDynamicBean;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class GenericDynamicBean implements CustomDynamicBean {
    public GenericDynamicBean() {
        super();
    }

    @Override
    public Object getProperty(String name) {
        System.err.printf("GenericDynamicBean.getProperty(%s)\n", name);

        return "Value for " + name;
    }

    @Override
    public void setProperty(String name, Object value) {
        System.err.printf("GenericDynamicBean.setProperty(%s, %s)\n", name, value);
    }
    
    @Override
    public Class<?> getPropertyType(String name) {
        System.err.printf("GenericDynamicBean.getPropertyType(%s)\n", name);

        return Object.class;
    }

    @Override
    public boolean isReadOnly(String name) {
        System.err.printf("GenericDynamicBean.isReadOnly(%s)\n", name);

        return false;
    }
}
