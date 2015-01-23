package com.example.java2com;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;


public abstract class IDispatch extends IUnknown {
    protected COMProxy ci;

    protected IDispatch(COMProxy ci) {
        this.ci = ci;
    }
    
    
    public final void release() {
        ci.release();
    }
    

    public final COMProxy getDispatch() {
        return ci;
    }

    public String toString() {
        return String.format("%s[dispatchHandle=0x%s]", 
                             getClass().getSimpleName(), ci.toString());
    }


    public static <T> T create(String className, Class<T> clazz) {
        
        COMProxy disp = new COMProxy(className);

        try {
            Constructor<?> cons = clazz.getConstructor(COMProxy.class);
            T result = (T) cons.newInstance(disp);
            return result;
        } catch (NoSuchMethodException | SecurityException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }

        return null;
    }
    
    
    public static <T> T get(String objectName, Class<T> clazz) {
        COMProxy disp = COMProxy.get(objectName);

        try {
            Constructor<?> cons = clazz.getConstructor(COMProxy.class);
            T result = (T) cons.newInstance(disp);
            return result;
        } catch (NoSuchMethodException | SecurityException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }

        return null;
    }

}
