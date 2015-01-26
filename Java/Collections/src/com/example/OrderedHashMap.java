package com.example;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;

public class OrderedHashMap<K, V> extends LinkedHashMap<K, V> {
    private static final long serialVersionUID = 6354582314971513369L;

    private List<V> items = new ArrayList<>();

    public V get(int index) {
        return items.get(index);
    }

    @Override
    public V put(K key, V value) {
        items.add(value);
        return super.put(key, value);
    }
}
