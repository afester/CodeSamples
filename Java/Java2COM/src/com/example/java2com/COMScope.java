package com.example.java2com;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

public class COMScope {

    private List<COMProxy> proxyList = new ArrayList<>();
    private static Stack<COMScope> scopes = new Stack<>();

    static void add(COMProxy obj) {
        if (scopes.size() > 0) {
            COMScope scope = scopes.peek();
            scope.proxyList.add(obj);
        }
    }

    private void releaseAll() {
        for (COMProxy c : proxyList) {
            c.release();
        }
        proxyList = new ArrayList<>();
    }
    
    /**
     * Starts a new scope for COM object creation.
     * All COM objects which are created after this method has been called
     * can be released at once with a call to the end() method. 
     */
    public static void begin() {
        scopes.push(new COMScope());
    }


    /**
     * Ends the current scope for COM object creation.
     * All COM objects which have been created since the last call to begin()
     * will be released.
     */
    public static void end() {
        if (scopes.size() > 0) {
            COMScope scope = scopes.pop();
            scope.releaseAll();
        }
    }
}
