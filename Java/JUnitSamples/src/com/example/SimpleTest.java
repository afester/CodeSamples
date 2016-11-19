package com.example;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.hamcrest.CoreMatchers;
import static org.hamcrest.core.Is.*;
import org.junit.Assert;
import org.junit.Test;

import static org.junit.matchers.JUnitMatchers.*; 


public class SimpleTest {

    private final static String EXPECTED_JUNIT_VERSION = "4.11";

    @Test
    public void basicTests() {
        assertEquals(EXPECTED_JUNIT_VERSION, junit.runner.Version.id());
        
        double val = Math.PI;
        assertEquals(3.1415, val, 0.0001);

        byte[] arr1 = new byte[] {3, 5, 7};
        assertArrayEquals( new byte[] {3, 5, 7}, arr1); // checks for same length, same elements, same order!

        SimpleTest st = new SimpleTest();
        SimpleTest st2 = st;
        assertSame(st, st2);        // checks for same object
    }

    /**
     * Simple Matcher tests using the (deprecated) JUnitMatchers methods
     */
    @Test
    public void matcherTests() {
        List<String> list = new ArrayList<String>() {{ 
                add("Hello");
                add("World");
                add("Moon");
        }};

        // check if collection contains specific item
        assertThat(list, hasItem("World")); 

        // check if collection contains all specified items, or more, in any order
        assertThat(list, hasItems("Moon", "Hello"));

        // combining matchers with "both" - same condition as before
        assertThat(list, both(hasItem("World")).and(hasItem("Hello")));

        // combining matchers with "either" - any one of the conditions must match 
        assertThat(list, either(hasItem("Pluto")).or(hasItem("World")));
    }
    
    
    /**
     * Same Matcher tests as above, using the hamcrest CoreMatchers matchers
     */
    @Test
    public void hamcrestTests() {
        List<String> list = new ArrayList<String>() {{ 
                add("Hello");
                add("World");
                add("Moon");
        }};
    
        // check if collection contains specific item
        assertThat(list, CoreMatchers.hasItem("World")); 
    
        // check if collection contains all specified items, or more, in any order
        assertThat(list, CoreMatchers.hasItems("Moon", "Hello"));

        // combining matchers with "both" - same condition as before
        assertThat(list, CoreMatchers.both(CoreMatchers.hasItem("World")).and(CoreMatchers.hasItem("Hello")));
    
        // combining matchers with "either" - any one of the conditions must match 
        assertThat(list, CoreMatchers.either(CoreMatchers.hasItem("Pluto")).or(CoreMatchers.hasItem("World")));
        
        // check if two collections contain the same elements, in any order
        String[] expected = { 
                "Mercury",
                "Venus",
                "Earth",
                "Mars",
                "Jupiter",
                "Saturn",
                "Uranus",
                "Neptun",
        };
        List<String> planets = new ArrayList<String>() {{ 
                add("Earth");
                add("Jupiter");
                add("Mars");
                add("Mercury");
                add("Neptun");
                add("Saturn");
                add("Uranus");
                add("Venus");
        }};

        // check if collection contains all specified items, or more, in any order
        assertThat(planets, CoreMatchers.hasItems(expected));
        // also check that the collection has the same size as the expected array
        assertEquals(expected.length, planets.size());
    }
    
    
    @Test
    public void testSetEquals() {

        Set<Number> actualSet = new HashSet<Number>();
        actualSet.add( 1L );

        Set<Number> expected = new HashSet<Number>();
        expected.add( 1 );

        Assert.assertThat(actualSet, is(expected));
    }
}
