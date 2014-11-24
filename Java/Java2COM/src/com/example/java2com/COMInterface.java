package com.example.java2com;

public abstract class COMInterface {
    protected IDispatch ci;

    protected COMInterface(IDispatch ci) {
        this.ci = ci;
    }
    
    
    public void release() {
        ci.release();
    }
}
