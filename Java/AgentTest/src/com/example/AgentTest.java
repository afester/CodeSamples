package com.example;

import java.util.Scanner;

public class AgentTest {

    public static void main(String[] args) {
        System.out.println("Hello World");
        Scanner s = new Scanner(System.in);
        String command = null;
        while(!"quit".equals(command)) {
            System.out.println("> ");
            command = s.next();
        }
        System.out.println("Done");
    }

}
