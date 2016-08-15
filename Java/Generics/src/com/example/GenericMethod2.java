package com.example;

import java.util.ArrayList;
import java.util.List;

public class GenericMethod2 {

    private List<Object> list = new ArrayList<>();

    public <T> void processData(T data) {
        list.add(data);
    }
}
