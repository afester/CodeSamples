package com.example;

import java.util.ArrayList;
import java.util.List;

import org.reactfx.util.Either;

public class ReactFXSample {
    public static void main(String[] args) {
        new ReactFXSample().run();
    }


    public void normalerweise() {
        List<Object> objects = new ArrayList<>();
        objects.add("Hello");
        objects.add(42);
        objects.add("World");
        objects.add("Moon");
        objects.add(99);
        objects.add(44.5);    // NO compile time error! Stores a Double!!!
        objects.stream().forEach(e -> System.err.printf("%s%n", e));    // no (direct) type information!
    }


    public void reactFXApproach() {
        List<Either<String, Integer>> objects = new ArrayList<>();
        objects.add(Either.left("Hello"));
        objects.add(Either.right(42));
        objects.add(Either.left("World"));
        objects.add(Either.left("Moon"));
        objects.add(Either.right(99));
        // objects.add(Either.right(44.5));    // compile time error!

        // unify() returns T as a unified type (Boolean, in the example below)
        System.err.printf("unify()%n"); 
        objects.stream().forEach(e -> System.err.printf("  String? %s%n", e.unify(l -> true, 
                                                                                  r -> false)));

        // map() returns Either<L1, L2> where L1 and L2 are two new types to which the original types are mapped
        System.err.printf("map()%n");
        objects.stream().forEach(e -> System.err.printf("  %s%n", e.map(l -> l,                     // l is of Type String and remains a String
                                                                        r -> Double.valueOf(r))));  // r is of type Integer and is converted to Double

        // The previous example can be simplified by using mapRight(), since
        // the left element remains the same type
        System.err.printf("mapRight()%n");
        objects.stream().forEach(e -> System.err.printf("  %s%n", e.mapRight(r -> Double.valueOf(r))));  // r is of type Integer and is converted to Double

        // exec() just executes one of two Functions, depending on the type of the Either
        System.err.printf("exec()%n");
        objects.stream().forEach(e -> e.exec(l -> System.err.printf("  String: %s%n", l),
                                             r -> System.err.printf("  Integer: %s%n",  r)));
    }

    public void run() {
        normalerweise();
        reactFXApproach();
    }
}
