package com.example.java2com;

public abstract class COMObject {
    protected IDispatch ci;

    protected COMObject(IDispatch ci) {
        this.ci = ci;
    }
    
    
    public final void release() {
        ci.release();
    }
    

    public final IDispatch getDispatch() {
        return ci;
    }

    public String toString() {
        return String.format("%s[dispatchHandle=0x%s]", 
                             getClass().getSimpleName(), ci.toString());
    }
}
