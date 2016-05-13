package com.example.functional;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.BiFunction;
import java.util.function.DoubleFunction;
import java.util.function.Function;
import java.util.function.IntSupplier;

public class SimpleExamples {

    
    public void supplierTest(IntSupplier val) {
        System.err.println(val.getAsInt());
    }

    public void functionTest(DoubleFunction<Double> val) {
        System.err.println(val.apply(12));
    }

    public void biFunctionTest(BiFunction<Double, Double, Double> val) {
        System.err.println(val.apply(5.0, 8.0));
    }

    
    public void varargTest(Function<List<Double>, Double> val) {
        System.err.println(val.apply(5.0, 8.0));
    }



    public void run() {
        // create a list of integers
        List<Integer> listOfIntegers = new ArrayList<>();
        listOfIntegers.addAll(Arrays.asList(1,4,2,7,5));
        
        // Sum up all array elements
        int sumResult = listOfIntegers.stream().mapToInt(Integer::valueOf).sum();
        System.err.printf("Sum of %s: %d\n",  listOfIntegers, sumResult);

        int sumResult2 = listOfIntegers.stream().reduce(0, (a, b) -> a + b);
        System.err.printf("Sum of %s: %d\n",  listOfIntegers, sumResult2);

        // Multiply all array elements
        int multResult = listOfIntegers.stream().reduce(1, (a, b) -> a * b);
        System.err.printf("Product of %s: %d\n",  listOfIntegers, multResult);

        // use lambda to provide a value to a function
        supplierTest(() -> 42);
        functionTest(x -> 5+2*x);
        biFunctionTest((a, b) -> a+2*b);
        varargTest((a, b, c) -> a + 2*b + c);
    }


    public static void main(String[] args) {
        new SimpleExamples().run();
    }
}
