package com.example;

public class Ternary1 {

    private void run() {
        Long condition = 1L;
        Long result = 0L;

        result = (condition == 0L) ? 1L : calculateResult();

        System.err.println("Result: " + result);
    }

    private Long calculateResult() {
        return null;
    }

    public static void main(String[] args) {
        new Ternary1().run();
    }
}
