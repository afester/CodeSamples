package com.example;

interface CollectorIF<T> {
    // @SafeVarargs         // Error: @SafeVarargs annotation cannot be applied to non-final instance method addAll
    void addAll(T... values);   
}

class Collector<T> implements CollectorIF<T> {

    @SafeVarargs
    public final void addAll(T... values) {
    }
}

class Component<T> {

    public void compute(T value) {
        Collector<T> col1 = new Collector<>();
        col1.addAll(value);   // No warning

        CollectorIF<T> col2 = new Collector<>();
        col2.addAll(value);   // Type safety: A generic array of T is created for a varargs parameter
    }
}
