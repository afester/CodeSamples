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
        objects.stream().forEach(e -> e.unify(l -> System.err.printf("String: %s%n", l), 
                                              r -> System.err.printf("Integer: %s%n",  r)));  
    }

    public void run() {
        normalerweise();
        reactFXApproach();
    }
}
