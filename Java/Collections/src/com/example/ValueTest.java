package com.example;

import java.util.ArrayList;
import java.util.List;

public class ValueTest {

    
    private void run() {
        List<String> strList = new ArrayList<>();

        strList.add("A");
        strList.add("B");
        strList.add("C");
        System.err.println(strList.contains("A") + " " + strList.indexOf("A"));
        System.err.println(strList.contains("B") + " " + strList.indexOf("B"));
        System.err.println(strList.contains("C") + " " + strList.indexOf("C"));

        // Map<String, Long> strLongMap = new HashMap<>();
    }


    public static void main(String[] args) {
        new ValueTest().run();
    }

}
