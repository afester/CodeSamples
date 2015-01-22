package com.example.adf;

import java.beans.FeatureDescriptor;

import java.util.Iterator;

import javax.el.ELContext;
import javax.el.ELResolver;

public class DynamicBeanELResolver extends ELResolver {
    public DynamicBeanELResolver() {
    }

    @Override
    public Object getValue(ELContext elContext, Object base, Object property) {
        
        // base == null => variable, base != null => property of base
        if (base != null && base instanceof DynamicBean) {
            DynamicBean bean = (DynamicBean) base;
            elContext.setPropertyResolved(true);
            return bean.getProperty(property.toString());
        }

        return null;
    }

    @Override
    public Class<?> getType(ELContext elContext, Object base, Object property) {
        // base == null => variable, base != null => property of base
        if (base != null && base instanceof DynamicBean) {
            DynamicBean bean = (DynamicBean) base;
            elContext.setPropertyResolved(true);
            return bean.getPropertyType(property.toString());
        }

        return null;
    }

    @Override
    public void setValue(ELContext elContext, Object base, Object property, Object value) {
        // base == null => variable, base != null => property of base
        if (base != null && base instanceof DynamicBean) {
            DynamicBean bean = (DynamicBean) base;
            elContext.setPropertyResolved(true);
            bean.setProperty(property.toString(), value);
        }
    }

    @Override
    public boolean isReadOnly(ELContext elContext, Object base, Object property) {
        // base == null => variable, base != null => property of base
        if (base != null && base instanceof DynamicBean) {
            DynamicBean bean = (DynamicBean) base;
            elContext.setPropertyResolved(true);
            return bean.isReadOnly(property.toString());
        }

        return false;
    }

// Design time support methods
    @Override
    public Iterator<FeatureDescriptor> getFeatureDescriptors(ELContext eLContext, Object object) {
        throw new UnsupportedOperationException();
    }

    @Override
    public Class<?> getCommonPropertyType(ELContext eLContext, Object object) {
        throw new UnsupportedOperationException();
    }
}
