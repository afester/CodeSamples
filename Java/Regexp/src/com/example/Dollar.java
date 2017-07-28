package com.example;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Dollar {

    public static void main(String[] args) {
        //Pattern pattern = Pattern.compile("(.*?)\\s*(?: (.*))?$");   // Works
        //Pattern pattern = Pattern.compile("(.*?)\\s*(?: (.*))?");    // Does not work
        //Pattern pattern = Pattern.compile("(\\S+)(?: (.*))?");   // Works
        //Pattern pattern = Pattern.compile("(\\S+)(?: (.*))?$");   // Works
        Pattern pattern = Pattern.compile("(\\S+)(?: (.*))?$");   // Works
        
        
        Matcher matcher = pattern.matcher("first");
        matcher.find();
        System.out.println("first : " + matcher.group(1));
        System.out.println("second: " + matcher.group(2));
    }
}
