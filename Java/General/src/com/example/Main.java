package com.example;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

public class Main {
    public static final Date fromBigEndian(String dateBigEndian) {
        DateFormat bigEndianFormat = new SimpleDateFormat("yyyy-MM-dd");
       // bigEndianFormat.setLenient(false);
        try {
            return bigEndianFormat.parse(dateBigEndian);
        } catch (ParseException e) {
            return null;
        }
    }

    public static void main(String[] args) {
        List<String> ls = Arrays.asList("Fred", "Jim", "Sheila", "Fred");
        Set<String> s1 = new HashSet<>(ls);
        Set<String> s2 = new TreeSet<>(ls);
        System.out.println(s1.equals(s2));
        System.out.println(System.identityHashCode(s1));
        System.out.println(System.identityHashCode(s2));
        
        String nonBigEndianDate = "01-06-2016";
        Date parsedDate = fromBigEndian(nonBigEndianDate);    
        System.err.println(parsedDate);
    }

    public void testFor() {
    	double number = 4.0;
    	for (int x = 5, number = 10; x < 10;  x++) {
    		
    	}
    }
}
