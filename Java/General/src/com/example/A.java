package com.example;

public class A {

    public void foo(){
        System.out.println("A");
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        A objA = new B();
        objA.foo();
    }

}