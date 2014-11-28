package com.example.java2com;

public abstract class COMInterface {
    protected IDispatch ci;

    protected COMInterface(IDispatch ci) {
        this.ci = ci;
    }
    
    
    public final void release() {
        ci.release();
    }
    

    public String toString() {
        return String.format("%s[dispatchHandle=0x%s]", 
                             getClass().getSimpleName(), ci.toString());
    }
}
