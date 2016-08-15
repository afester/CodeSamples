package com.example;

import java.util.ArrayList;
import java.util.List;
//
//interface CollectorIF<T> {
//    @SafeVarargs
//    void addAll(T... values);   // @SafeVarargs annotation cannot be applied to non-final instance method addAll
//}
//
//class Collector<T> implements CollectorIF<T> {
//
//
//    @SafeVarargs
//    public final void addAll(T... values) {
//    }
//}
//
//class Component<T> {
//
//    public void compute(T value) {
//        CollectorIF<T> values= new Collector<>();
//        values.addAll(value);   // Type safety: A generic array of T is created for a varargs parameter
//    }
//}
//
//
//
//public class VarArgExample {
//
//    private <N extends Number> void method1(N... values) {
//        for (N val : values) {
//            System.err.printf("%d%n", val);
//        }
//    }
//
//    private void run() {
//        method1(1, 2, 3, 4, 5);
//        method1(1.1, 2.5, 3.3, 4.9, 5.4);
//    }
//
//    public static void main(String[] args) {
//        new VarArgExample().run();
//    }
//}
