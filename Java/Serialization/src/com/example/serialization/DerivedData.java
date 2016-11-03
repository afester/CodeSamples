package com.example.serialization;

import com.example.serialization.model.DerivedDataIF;

public class DerivedData implements DerivedDataIF {
    private final String payload;
    
    public DerivedData(String str) {
        payload = str;
    }


    @Override
    public String toString() {
        return "DerivedData[" + payload + "]";
    }
}
