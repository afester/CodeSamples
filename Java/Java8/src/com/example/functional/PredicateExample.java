package com.example.functional;

import java.util.Arrays;
import java.util.List;

@FunctionalInterface
interface SimplePredicate<T> {
    boolean test(T value);
}



public class PredicateExample {

    private final static List<String> data = Arrays.asList("Hello", "World", "Some", "Sample", "Data");

    private void run() {
        List<String> result = data.stream().filter(arg0);
    }

    public static void main(String[] args) {
        new PredicateExample().run();
    }

}
