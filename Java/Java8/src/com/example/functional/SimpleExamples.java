package com.example.functional;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SimpleExamples {

    public static void main(String[] args) {

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
    }
}
