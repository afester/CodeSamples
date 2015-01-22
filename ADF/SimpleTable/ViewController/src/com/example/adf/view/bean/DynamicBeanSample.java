package com.example.adf.view.bean;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class DynamicBeanSample implements Map<String, Object> {
    public DynamicBeanSample() {
        super();
        put("value1", 42L);
        put("value2", "Hello");
    }


    private Map<String, Object> theMap = new HashMap<>();

    @Override
    public int size() {
        return theMap.size();
    }

    @Override
    public boolean isEmpty() {
        return theMap.isEmpty();
    }

    @Override
    public boolean containsKey(Object object) {
        return theMap.containsKey(object);
    }

    @Override
    public boolean containsValue(Object object) {
        return theMap.containsValue(object);
    }

    @Override
    public Object get(Object object) {
        return theMap.get(object);
    }

    @Override
    public Object put(String object, Object object2) {
        return theMap.put(object, object2);
    }

    @Override
    public Object remove(Object object) {
        return theMap.remove(object);
    }

    @Override
    public void putAll(Map map) {
        theMap.putAll(map);
    }

    @Override
    public void clear() {
        theMap.clear();
    }

    @Override
    public Set<String> keySet() {
        return theMap.keySet();
    }

    @Override
    public Collection<Object> values() {
        return theMap.values();
    }

    @Override
    public Set entrySet() {
        return theMap.entrySet();
    }
}