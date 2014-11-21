package com.example.java2com;

public class IDispatch {
    
    static {
        System.loadLibrary("Native");
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

    private final static int DISPATCH_METHOD = 1;
    private final static int DISPATCH_PROPERTYGET = 2;
    private final static int DISPATCH_PROPERTYPUT = 4;
    private final static int DISPATCH_PROPERTYPUTREF = 8;

    /**
     *  Creates a new IDispatch object to delegate to a particular IDispatch 
     *  COM instance.
     */
    private IDispatch() {
    }

/*** Public API below ********************************************************/
    
    /**
     * Creates a new IDispatch object to delegate to a particular instance of a class
     * 
     * @param className The name of the class to instanciate, like "Excel.Application"
     *                  or "Word.Application"
     */
    public IDispatch(String className) {
        Create(className);
    }
    

    public Variant invoke(String member) {
        return Invoke(member, DISPATCH_METHOD, null);
    }
    

    public Variant invoke(String member, Variant param) {
        Variant[] parameters = new Variant[1];
        parameters[0] = param;
        return Invoke(member, DISPATCH_METHOD, parameters);
    }


    public Variant invoke(String member, Variant[] params) {
        return Invoke(member, DISPATCH_METHOD, params);
    }

    
    public void setProperty(String propertyName, Variant value) {
        Variant[] parameters = new Variant[1];
        parameters[0] = value;
        Invoke(propertyName, DISPATCH_PROPERTYPUT, parameters);
    }

    // indexed properties
    public Variant getProperty(String propertyName, Variant index) {
        Variant[] parameters = new Variant[1];
        parameters[0] = index;
        return Invoke(propertyName, DISPATCH_PROPERTYGET, parameters);
    }


    public Variant getProperty(String propertyName) {
        return Invoke(propertyName, DISPATCH_PROPERTYGET, null);
    }

    
    public void release() {
        Destroy();
    }
}
