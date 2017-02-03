package com.example.functional;

import java.util.HashMap;
import java.util.Map;

/**
 * Functional interface which takes no parameters and returns nothing.
 * Could also use Runnable, but its semantics do not fit well.
 */
@FunctionalInterface
interface Worker {
   void execute(); 
}

/**
 * A switch on a particular value.
 *
 * @param <T> The type of the value to switch on.
 */
class Switch<T> {

    private final Map<T, Worker> switchMap = new HashMap<>();
    private final Worker defaultMethod;

    /**
     * Creates a new Switch.
     * A Switch always has a default method which is executed if no match
     * is found for a particular value.
     *
     * @param defaultMethod The default method to execute if no match
     *                      is found for a particular value. 
     */
    public Switch(Worker defaultMethod) {
        this.defaultMethod = defaultMethod; 
    }

    /**
     * Adds a new alternative ("case") to this switch.
     *
     * @param alternative The value for this alternative. 
     * @param method The method to execute for this alternative.
     */
    public void put(T alternative, Worker method) {
        switchMap.put(alternative, method);
    }

    public void execute(T alternative) {
        Worker method = switchMap.get(alternative);
        if (method == null) {
            method = defaultMethod;
        }
        method.execute();
    }
}


public class FunctionalSwitch {

    public static void main(String[] args) {
        new FunctionalSwitch().run();
    }

    private final Switch<String> alternative = new Switch<>(() -> System.err.println("No match."));

    public FunctionalSwitch() {
        // initialize the switch (drawback is that the map is instance based,
        // which might result in a lot of redundant entries...)
        alternative.put("a", this::case1);
        alternative.put("b", this::case2);
        alternative.put("c", this::case3);
    }

    private void run() {
        alternative.execute("c");
        alternative.execute("a");
        alternative.execute("x");
    }

    public void case1() {
        System.err.println("case1");
    }
    
    public void case2() {
        System.err.println("case2");
    }
    
    public void case3() {
        System.err.println("case3");
    }
}
