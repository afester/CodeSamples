package com.example.java2com;

public class COMProxy {
    
    static {
        System.loadLibrary("COMProxy");
    }

    
    private int dispatchHandle = 0; // TODO: architecture specific! (32 bit <=> 64 bit)

    /**
     * Creates a new IDispatch interface.
     * 
     * @param className The name of the class for which to create an IDispatch 
     *                  interface. Example: "Word.Application"
     */
    private native void Create(String className);

    /**
     * Gets an IDispatch interface for an existing object.
     * 
     * @param className The name of the class for which to get an IDispatch 
     *                  interface. Example: "Word.Application"
     */
    private native void Get(String className);

    /**
     * Invokes a member of this IDispatch interface.
     * 
     * @param member    The name of the member to invoke, like "Visible". Can be 
     *                  either a property or a method.
     * @param dispatchOrProperty Flag to determine whether to get a property,
     *                           to set a property or to invoke a member.
     * @param params    The parameters to pass to a method invocation.
     *
     * @return  The result of the method invocation.
     */
    private native Variant Invoke(String member, int dispatchOrProperty, Variant[] params);

    /**
     * Releases the reference count of this IDispatch interface. 
     */
    private native void Destroy();

    /**
     * Enables or disabled debug output from the native library.
     *
     * @param flag <code>true</code> to enable debug output, <code>false</code> 
     *             to disable debug output
     */
    private static native void SetDebugEnabled(boolean flag);

    private final static int DISPATCH_METHOD = 1;
    private final static int DISPATCH_PROPERTYGET = 2;
    private final static int DISPATCH_PROPERTYPUT = 4;
    private final static int DISPATCH_PROPERTYPUTREF = 8;

    /**
     *  Creates a new COMProxy object to delegate to a particular native
     *  COM object.
     */
    private COMProxy() {
        COMScope.add(this);
    }

/*** Public API below ********************************************************/
    
    /**
     * Creates a new IDispatch object to delegate to a particular instance of a class
     * 
     * @param className The name of the class to instanciate, like "Excel.Application"
     *                  or "Word.Application"
     */
    public COMProxy(String className) {
        COMScope.add(this);
        Create(className);
    }

    /**
     * @param className
     * @return An IDispatch object for an existing automation server.
     */
    public static COMProxy get(String className) {
        COMProxy result = new COMProxy();
        result.Get(className);
        return result;
    }

    public static void setDebugEnabled(boolean flag) {
        SetDebugEnabled(flag);
    }


    public Variant invoke(String member, Variant ... params) {
        return Invoke(member, DISPATCH_METHOD, params);
    }

    public void setProperty(String propertyName, Variant ... parameters) {
        Invoke(propertyName, DISPATCH_PROPERTYPUT, parameters);
    }

    public Variant getProperty(String propertyName, Variant ... parameters) {
        return Invoke(propertyName, DISPATCH_PROPERTYGET, parameters);
    }

    public void release() {
        Destroy();
        dispatchHandle = 0;
    }
    
    public String toString() {
        return String.format("%08x", dispatchHandle);
    }
}
