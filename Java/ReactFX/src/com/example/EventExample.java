package com.example;

import org.reactfx.EventSource;
import org.reactfx.EventStream;

public class EventExample {

    public static void main(String[] args) {
        new EventExample().run();
    }

    private void run() {
        
        EventSource<Integer> stream = new EventSource<>();
        stream.addObserver(e -> System.err.printf("Observed: %s%n", e));
        
        stream.push(23);
        stream.push(42);
        stream.push(2);
        stream.emit(123);   // what is the difference between push and emit??
    }
}
