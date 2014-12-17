package com.example.test;


import static org.junit.Assert.*;

import org.junit.Test;

import com.example.OrderedHashMap;


public class OrderedHashMapTest {


    @Test
    public void testPutGet() {
        OrderedHashMap<String, String> ohm = new OrderedHashMap<>();
        ohm.put("Z", "LetterZ");    // 0
        ohm.put("A", "LetterA");    // 1
        ohm.put("D", "LetterD");    // 2
        ohm.put("K", "LetterK");    // 3

        assertEquals(ohm.get(0), "LetterZ");
        assertEquals(ohm.get(3), "LetterK");
    }

    @Test
    public void testPutAll() {
        OrderedHashMap<String, String> ohm = new OrderedHashMap<>();
        ohm.put("Z", "LetterZ");    // 0
        ohm.put("A", "LetterA");    // 1
        ohm.put("D", "LetterD");    // 2
        ohm.put("K", "LetterK");    // 3

        OrderedHashMap<String, String> ohm2 = new OrderedHashMap<>();
        ohm2.putAll(ohm);

        assertEquals(ohm2.get(0), "LetterZ");
        assertEquals(ohm2.get(3), "LetterK");
    }

}
