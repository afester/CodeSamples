package com.example;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

class Key {
}

public class MapSample {
    
    public void run() {
        Map<String, Long> map = createMap();

        // most simply output
        System.err.println("Simple output:");
        System.err.println("   " + map);

        // output in a loop
        System.err.println("Output in a loop:");
        Set<Entry<String, Long>> entries = map.entrySet();
        for (Entry<String, Long> entry : entries) {
            Object key = entry.getKey();
            Object value = entry.getValue();
            System.err.printf("   %s=%s\n", key, value);
        }

        // convert to sorted map
        Map<String, Long> sortedMap = new TreeMap<>();
        sortedMap.putAll(map);

        System.err.println("Output sorted by key:");
        Set<Entry<String, Long>> sortedEntries = sortedMap.entrySet();
        for (Entry<String, Long> entry : sortedEntries) {
            System.err.printf("   %s=%s\n", entry.getKey(), entry.getValue());
        }
    }

    // @SuppressWarnings({ "unchecked", "rawtypes" })
    private Map<String, Long> createMap() {
        Map result = new HashMap();
        result.put("one",  1L);
        result.put(new Key(),  3L);
        //result.put("three",  3L);
        result.put("two",  2L);
        result.put("five",  5L);
        result.put("four",  4L);
        return result;
    }

    public static void main(String[] args) {
        new MapSample().run();
    }
}
