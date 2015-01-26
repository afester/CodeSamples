package com.example.pathlist;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

public class Path implements Iterable<String> {

    private List<String> elements = new ArrayList<>();
    
    public Path() {
    }

    public Path(String... elems) {
        Collections.addAll(elements, elems);
    }


    public Path append(String... elems) {
        Collections.addAll(elements, elems);
        return this;
    }

    public Iterator<String> iterator() {
        return elements.iterator();
    }

    @Override
    public String toString() {
        StringBuffer result = new StringBuffer("Path[");

        boolean first = true;
        for (String e : elements) {
            if (!first) {
                result.append(",");
            }
            result.append(e);
            first = false;
        }
        
        result.append("]");
        
        return result.toString();
    }

    public String asString(String delimiter) {
        StringBuffer result = new StringBuffer();

        boolean first = true;
        for (String e : elements) {
            if (!first) {
                result.append(delimiter);
            }
            result.append(e);
            first = false;
        }
        
        return result.toString();
    }
}
