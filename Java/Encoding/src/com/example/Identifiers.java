package com.example;

public class Identifiers {

    public static void main(String[] args) {
        Identifiers ids = new Identifiers();
        ids.$("Hello");
        ids.€("World");
        ids.µ("Test");
        ids.λ("Test");
    }

    private void λ(String x) {
        System.err.println("I am a lambda: " + x);
    }

    private void µ(String x) {
        System.err.println(x);
    }

    public void $(String x) {
        System.err.println(x);
    }

    public void €(String x) {
        System.err.println(x);
    }
}
