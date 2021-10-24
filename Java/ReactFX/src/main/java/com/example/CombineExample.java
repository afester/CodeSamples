package com.example;

import java.util.ArrayList;
import java.util.List;

import org.reactfx.value.Val;
import org.reactfx.value.Var;

public class CombineExample {

    public static void main(String[] args) {
        new CombineExample().run();
    }

    
    private Var<List<String>> stringList = Var.newSimpleVar(null);
    private Var<Integer> integer = Var.newSimpleVar(42);

    
    private void run() {

        Val<String> res = Val.combine(stringList,
                                      integer,
                                      (a, b) -> { System.err.println("EVAL!"); return String.format("%s / %s",  a, b); } );

        // When a listener is attached to the combined variable, the listener is called whenever 
        // either of the two combined variables changes!!!! (which is obvious, since
        // the listener gets passed the combined value!!!! and hence needs to call the getValue() method on res
        res.addListener((v, oldV, newV) -> System.err.printf("LISTENER: %s -> %s%n", oldV, newV));

        List<String> newList = new ArrayList<>(); 
        newList.add("First");
        newList.add("Second");

        System.err.println("Res: " + res.getValue());
        integer.setValue(22);
        stringList.setValue(newList);
        integer.setValue(44);

        System.err.println("-----Result:");
        System.err.println("Res: " + res.getValue());     // If no listener is attached, the lambda is only called here!!!!
        
        System.err.println("-----Test:");
        integer.setValue(44);                             // Note that only CHANGED values trigger a recalculation!
                                                          // if the identical object is (re)set as the value, it does not trigger
                                                          // a recalculation even if the INTERNAL state might have changed!
        System.err.println("-----Test2:");
        stringList.setValue(newList);                  // NO recalculation!!

        System.err.println("-----Test3:");
        stringList.setValue(new ArrayList<>());        // recalculation performed!!!
    }
}
